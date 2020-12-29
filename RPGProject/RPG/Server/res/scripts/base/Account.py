# -*- coding: utf-8 -*-
import KBEngine
import KBEDebug
import Const
import RPGConst
import json
import Functor
from RoleBornConfig import g_roleBornCfg
import time

# --------------------------------------------------------------------
# inner global methods
# --------------------------------------------------------------------
def queryRoles( parentID, callback ):
	"""
	query all roles which parent account is  parentID
	"""
	query = """select id, sm_playerName,sm_camp,sm_gender,sm_profession\
				from tbl_Role where sm_parentDBID = %i and sm_roleState != %i""" % (parentID, RPGConst.ROLE_STATE_DELETE)
	KBEDebug.INFO_MSG(query)
	KBEngine.executeRawDatabaseCommand( query, callback )
	
def deleteRole( roleID, callback ):
	"""
	删除角色
	"""
	deleteTime = int(time.time())
	sql_sentence = "UPDATE tbl_Role SET sm_roleState = %i,sm_deleteTime = %s where id = %i" %(RPGConst.ROLE_STATE_DELETE ,deleteTime,roleID)
	KBEDebug.INFO_MSG(sql_sentence)
	KBEngine.executeRawDatabaseCommand( sql_sentence, callback )
	
	
class Account(KBEngine.Proxy):
	def __init__(self):
		KBEngine.Proxy.__init__(self)
		self.playerName = self.__ACCOUNT_NAME__
		self.avatar = None
		self._playerList = []			# 记录账号当前的角色DBID列表
		self.loginState = Const.ACCOUNT_INITIAL_STATE	# 登录初始状态
		self.deleteDBID = None			# 用于避免同一时间处理多条删除角色的请求，从而引发不必要的问题
		
	def _logoff(self):
		"""
		<Define method>
		玩家下线
		"""
		#防止还在创建角色，角色添加数据库还没完成的时候就下线了
		if self.avatar and not self.avatar.isDestroyed and self.avatar.databaseID:
			self.avatar.kickFromAccount()
			return		
		self.changeLoginState(Const.ACCOUNT_INITIAL_STATE)
		self.destroy()
		
	def onTimer(self, id, userArg):
		"""
		KBEngine method.
		使用addTimer后， 当时间到达则该接口被调用
		@param id		: addTimer 的返回值ID
		@param userArg	: addTimer 最后一个参数所给入的数据
		"""
		KBEDebug.DEBUG_MSG(id, userArg)
		
	def onClientEnabled(self):
		"""
		KBEngine method.
		该entity被正式激活为可使用， 此时entity已经建立了client对应实体， 可以在此创建它的
		cell部分。
		"""
		KBEDebug.INFO_MSG("account[%i] entities enable. entityCall:%s" % (self.id, self.client))
		self.client.CLIENT_OnAccountCreateSuccessed()
		self.changeLoginState(Const.ACCOUNT_LOGIN_SUCCESSED)
			
	def onLogOnAttempt(self, ip, port, password):
		"""
		KBEngine method.
		客户端登陆失败时会回调到这里
		"""
		KBEDebug.INFO_MSG(ip, port, password)
		self.changeLoginState(Const.ACCOUNT_INITIAL_STATE)
		return KBEngine.LOG_ON_ACCEPT
		
	def onClientDeath(self):
		"""
		KBEngine method.
		客户端对应实体已经销毁
		"""
		KBEDebug.DEBUG_MSG("Account[%i].onClientDeath:" % self.id)
		self._logoff()
		
	def BASE_createRole( self, playerName, profession, camp, gender ):
		"""
		<Exposed method>
		创建角色
		"""
		if not self.isLoginState(Const.ACCOUNT_LOGIN_SUCCESSED):
			KBEDebug.ERROR_MSG("Account[%d:%d] create role: state[%d] error" %(self.databaseID, self.id, self.loginState))
			return
		if (self.avatar is not None) and (not self.avatar.isDestroyed):
			KBEDebug.ERROR_MSG("Account[%d:%d] create role: avatar[%d] is not None" %(self.databaseID, self.id, self.avatar.databaseID))
			return
		if len( self._playerList ) >= RPGConst.LOGIN_ROLE_UPPER_LIMIT:#创建角色数量已经达到上限
			KBEDebug.ERROR_MSG("Account[%d:%d] create role: role number upper limit[%d]" %(self.databaseID, self.id, RPGConst.LOGIN_ROLE_UPPER_LIMIT))
			return
		if playerName == "" :#角色名为空
			KBEDebug.ERROR_MSG("Account[%d:%d] create role: playerName is None" %(self.databaseID, self.id))
			return
		if len( playerName.encode("gbk") ) > RPGConst.ROLE_NAME_LENGTH_UPPER_LIMIT:#角色名字长度超出上限
			KBEDebug.ERROR_MSG("Account[%d:%d] create role: playerName length upper limit[%d]" %(self.databaseID, self.id, RPGConst.ROLE_NAME_LENGTH_UPPER_LIMIT))
			return
		paramDict = { 
					"playerName":playerName,
					"parentDBID":self.databaseID,
					"modelParts":g_roleBornCfg.getRoleBornModel(profession),
					"camp":camp,
					"gender"	:gender,
					"profession":profession,
					"spaceScriptID":"L_TestMap",
					"createTime":	int(time.time())
					}
		#引擎方法def createBaseLocally( entityType, params ):创建一个新的Base实体（函数参数需要提供创建的实体的类型，还有一个Python字典作为参数来初始化实体的值。这个Python字典不需要用户提供所有的属性，没有提供的属性默认为实体定义文件".def"提供的默认值。），
		avatar = KBEngine.createEntityLocally( "Role", paramDict )
		self.avatar = avatar
		avatar.accountEntity = self
		avatar.writeToDB( Functor.Functor(self.onWriteRoleToDBCollback, self.databaseID, self.__ACCOUNT_NAME__) )
	
	def onWriteRoleToDBCollback( self, databaseID, name, success, avatar ):	
		"""
		<define method> 写入数据库回调
		@type  databaseID:数据库ID
		@type  name:
		@type  success:标志成功或失败
		@type  avatar:base实体
		"""
		if success:
			self._playerList.append( avatar.databaseID )
			loginPlayer = {				
				"dbid"			: avatar.databaseID,
				"playerName"	: avatar.cellData["playerName"],#如果这个键是一个Cell属性，它会被添加到Base实体的'cellData'属性,
				"camp"			: avatar.cellData["camp"],
				"gender"		: avatar.cellData["gender"],
				"profession"	: avatar.cellData["profession"]
			}
			if self.client:
				self.client.CLIENT_OnAddRole( loginPlayer )
		else:
			#提示客户端账号名字已经存在
			KBEDebug.ERROR_MSG("Create Role Failed")
		# destroy entity
		avatar.destroy( writeToDB = False )
		
	def BASE_deleteRole( self, databaseID ):
		"""
		<Exposed method>
		删除一个角色	
		@type  databaseID: INT64
		"""
		if not self.isLoginState(Const.ACCOUNT_LOGIN_SUCCESSED):
			KBEDebug.ERROR_MSG("Account[%d:%d] delete role: state[%d] error" %(self.databaseID, self.id, self.loginState))
			return
		if self.deleteDBID is not None:
			KBEDebug.ERROR_MSG("Account[%d:%d] delete role: have a role[%d] in deleting" %(self.databaseID, self.id, self.deleteDBID))
			return
		if databaseID not in self._playerList:
			KBEDebug.ERROR_MSG("Account[%d:%d] delete role: role[%d] not in account" %(self.databaseID, self.id, databaseID))
			return
		self.deleteDBID = databaseID
		deleteRole( databaseID, self.deleteResult )
		
	def deleteResult( self, result, rows, insertid, errstr ):
		"""
		<define method>
		从数据库删除账号某个角色信息回调
		@type  errstr: 错误信息：当命令执行有错误时，这个参数是一个描述错误的字符串。命令执行没有发生错误时这个参数为None。
		"""
		if errstr is None :
			self.client.CLIENT_OnDeleteRole( self.deleteDBID )
			self._playerList.remove(self.deleteDBID)
		else:
			KBEDebug.ERROR_MSG( "--------->deleteRole:delete role fail!" )
		self.deleteDBID = None
			
	def BASE_queryRoles(self):
		"""
		<Exposed method>
		客户端请求角色列表
		"""
		queryRoles( self.databaseID, self.onQueryRoles )
		
	def onQueryRoles( self, resultSet, rows, insertid, errstr ):
		"""
		<define method>
		查询数据库账号角色信息回调
		@type  resultSet: 结果集合：这个结果集合参数是一个行列表。 每一行是一个包含字段值的字符串列表。
		@type  rows: 影响的行数：它是一个整数，表示命令执行受影响的行数。这个参数只和不返回结果结合的命令（如DELETE）相关。
		@type  rows: 自増长值：当成功的向一张带有自増长类型字段的表中插入数据时，它返回该数据在插入时自増长字段所被赋于的值。
		@type  errstr: 错误信息：当命令执行有错误时，这个参数是一个描述错误的字符串。命令执行没有发生错误时这个参数为None。
		"""
		if errstr is not None:
			KBEDebug.ERROR_MSG(errstr)
			return

		roles = resultSet			# [ [ id, playerName, level, raceclass, lifetime, hairNumber, ..], ... ]
		self._playerList = [int( e[0] ) for e in roles]		# 汇总所有角色用于登录、删除操作时认证
		#for e in roles:
		#	KBEDebug.ERROR_MSG("onQueryRoles[%d]" %(int( e[0]))
		#	self._playerList.append(int( e[0] ))
		tmpPlayers = []
		for role in roles:
			loginPlayer = {				
				"dbid"			: int( role[0] ),
				"playerName"	: role[1].decode( "utf-8" ),
				"camp"			: int(role[2]),
				"gender"		: int(role[3]),
				"profession"	: int(role[4]),
			}
			tmpPlayers.append( loginPlayer )
		if hasattr(self, "client") and  self.client:
			self.client.CLIENT_OnQueryRoles( {"infos":tmpPlayers} )	
		
	def BASE_enterWorld(self, databaseID):
		"""
		<Exposed method>
		玩家请求登陆
		"""
		if (self.avatar is not None) and (not self.avatar.isDestroyed):#已经登录了一个角色
			KBEDebug.ERROR_MSG("Account[%d:%d] login role[%d] error: have logined role[%d]" %(self.databaseID, self.id, databaseID, self.avatar.databaseID))
			return
		if databaseID not in self._playerList:
			KBEDebug.ERROR_MSG("Account[%d:%d] login role[%d] error: role[%d] not in account" %(self.databaseID, self.id, databaseID))
			return
		if not self.isLoginState(Const.ACCOUNT_LOGIN_SUCCESSED):
			KBEDebug.ERROR_MSG("Account[%d:%d] login role[%d] error: state[%d] error" %(self.databaseID, self.id, self.loginState))
			return
		#引擎方法，从数据库里加载数据创建一个Base实体
		KBEngine.createEntityFromDBID( "Role", databaseID, self.onLoadedAvatar )
		
	def onLoadedAvatar( self, baseRef, databaseID, wasActive ):
		"""
		<define method>
		Role的BaseEntity创建完成回调
		@type  baseRef: 如果操作成功，baseRef会是一个mailbox或者是新创建的Base实体的直接引用。否则为None
		@type  databaseID: databaseID会是实体的数据库ID，无论该实体是否已经激活
		@type  wasActive: 如果wasActive是True则baseRef是已经存在的实体的引用(已经从数据库检出)。如果操作失败这三个参数的值，baseRef将会是None，databaseID将会是0，wasActive将会是False。
		"""
		assert not wasActive, "(%i): the target entity was not active, I can't do it." % self.id
		if self.isDestroyed:
			if baseRef is not None:
				baseRef.destroySelf()
				return
		if baseRef is None:#创建角色Entity失败
			KBEDebug.INFO_MSG( "Load Avatar Failed.")
			return
		self.avatar = baseRef
		#创建角色，写入数据库的时候只写了playerName与parentDBID，所以其他属性需手动赋值
		baseRef.playerDBID = databaseID
		baseRef.accountEntity = self
		self.giveClientTo( baseRef ) #引擎方法：自动在客户端创建一个对应的Entity即RoleEntity
		
	def BASE_logoff(self):
		"""
		<Exposed method>
		玩家下线
		"""
		self._logoff()
		
	def onAvatarDeath( self ):
		"""
		<Define method>
		玩家BaseEntity销毁回调通知Account
		"""
		self.avatar = None
		self.changeLoginState(Const.ACCOUNT_INITIAL_STATE)	
		if not self.hasClient:#Account绑定了一个客户端连接，即返回选择的时候则不用销毁，返回登录才销毁
			self.destroy()
		
	def changeLoginState(self, newLoginState):
		"""
		define method
		改变登录状态
		"""
		if self.loginState == newLoginState: return
		self.loginState = newLoginState
		
	def isLoginState( self, state ):
		"""
		是否在state状态
		"""
		return self.loginState == state