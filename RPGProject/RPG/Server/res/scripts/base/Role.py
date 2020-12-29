# -*- coding: utf-8 -*-


import KBEngine
import KBEDebug
import CoreEntity.HasCellEntity as HasCellEntity
import CoreInterface.Role.RoleSpaceInterface as RoleSpaceInterface

class Role(
	KBEngine.Proxy,
	HasCellEntity.HasCellEntity,
	RoleSpaceInterface.RoleSpaceInterface
	):

	def __init__( self ):
		#基本模块
		KBEngine.Proxy.__init__( self )
		HasCellEntity.HasCellEntity.__init__( self )
		RoleSpaceInterface.RoleSpaceInterface.__init__( self )
		
	def __transferClientToAccount( self ):
		"""
		将客户端从Role移交给Account
		"""
		if self.accountEntity is not None:
			if self.hasClient and not self.accountEntity.hasClient:
				self.giveClientTo( self.accountEntity )		# 把控制权交到account中
		
	def __destroySelf(self):
		"""
		"""
		#注意 如果DBID还没有的话，也是无法销毁的
		if self.isDestroy or not self.databaseID: return
		self.destroySelf()
		
	def __initRoleInfo(self):
		"""
		对一个角色而言，仅调用一次，在创建玩家后，首次进入游戏		
		"""
		KBEDebug.INFO_MSG("base playerDBID[%d] first create!"%self.databaseID)
		
	def __logonSpace( self ):
		"""
		玩家上线时触发，请求登录到指定的地图
		"""
		RoleSpaceInterface.RoleSpaceInterface.loginSpace( self )
		
	def onClientGetCell( self ):
		"""
		引擎回调，当客户端能够调用实体的cell属性时，该回调被调用
		"""
		self.__initRoleInfo()
		RoleSpaceInterface.RoleSpaceInterface.onClientGetCell( self )
		
	def onLoseCell( self ):
		"""
		引擎回调，这个函数在它关联的cell实体销毁之后被调用。这个函数没有参数
		"""
		HasCellEntity.HasCellEntity.onLoseCell( self )
		
	def onClientEnabled( self ):
		"""
		引擎机制，详情请看引擎相关文档
		Client上RoleEntity创建完成之后自动调用
		"""
		if not self.hasCell:
			self.__logonSpace()
		
	def onGetCell( self ):
		"""
		引擎回调，cell 实体创建完成。
		"""
		HasCellEntity.HasCellEntity.onGetCell( self )
		RoleSpaceInterface.RoleSpaceInterface.onGetCell( self )
		
	def onCreateCellFailure( self ):
		"""
		引擎回调，cell创建失败
		"""
		RoleSpaceInterface.RoleSpaceInterface.onCreateCellFailure( self )
		
	def onClientDeath( self ):
		"""
		引擎回调，客户端断开连接时被调用，销毁自身即下线操作。
		"""
		if self.isDestroy: return
		self.__destroySelf()
		
	def kickFromAccount(self):
		"""
		account通知销毁
		"""
		if self.isDestroy: return
		KBEDebug.INFO_MSG("role[%d:%s] kickFromAccount..............."  %(self.databaseID, self.playerName))
		self.__destroySelf()
		
	def beforeDestroy( self ):
		"""
		entity摧毁前处理事情
		"""
		if self.accountEntity:
			self.accountEntity.onAvatarDeath()
		
	def BASE_logoff(self):
		"""
		<Exposed method>
		玩家下线
		"""
		if self.isDestroy: return
		KBEDebug.INFO_MSG("role[%d:%d]%s logoff..............."  %(self.databaseID, self.id, self.playerName))
		self.__destroySelf()
		
	def BASE_logout( self ):
		"""
		<Exposed method>
		注销到角色选择界面
		"""
		if self.isDestroy: return
		KBEDebug.INFO_MSG("role[%d:%d]%s logout..............."  %(self.databaseID, self.id, self.playerName))
		self.__destroySelf()
		self.__transferClientToAccount()