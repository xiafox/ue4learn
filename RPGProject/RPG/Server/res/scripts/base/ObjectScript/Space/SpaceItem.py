# -*- coding: utf-8 -*-
import KBEngine
import KBEDebug
import kbemain
import Functor

class SpaceItem:
	def __init__( self, scriptID, spaceNumber, params ):
		self.scriptID = scriptID			# 空间脚本ID
		self.spaceNumber = spaceNumber		# 空间编号
		self.belongType = params.get("belongType", 0 )	# 空间归属类型
		self.spaceKey = params.get( "spaceKey", "" )	# 空间key
		self.__createArgs = params
		self.spaceMailBox = None			# 空间的BASE mailbox
		self.isBaseCreating = False			# 当前space base是否在创建中
		self.hasBase = False				# 是否已创建了空间的base实体
		self.hasCell = False				# 是否存在cell标志
		self.__waitEnterPlayers = []			# 要传送进入空间的玩家数据: [(),...]
		self.__waitLoginPlayers = []			# 要上线进入空间的玩家数据: [Base,...]
		
	def createSpaceBase( self, callBack ):
		"""
		创建空间实体，由SpaceDomain调用
		"""
		if self.isBaseCreating or self.hasBase:
			return
		self.isBaseCreating = True
		dict = { "spaceNumber": self.spaceNumber, "belongType": self.belongType, "spaceKey": self.spaceKey, "createArgs": self.__createArgs }
		#通过空间脚本对象创建空间类baseEntity实体（选择任何的Baseapp来创建)
		kbemain.g_objScriptFactory.getSpaceObject( self.scriptID ).createAnywhereEntity( dict, Functor.Functor( self.__onSpaceCreateCallBack, callBack ) )
		
	def __onSpaceCreateCallBack( self, callBack, base ):
		"""
		space的baseEntity创建完成回调
		@param 	base	:		space entity base
		@type 	base	:		mailbox
		"""
		if not base:
			return
		self.spaceMailBox = base
		self.isBaseCreating = False
		self.hasBase = True
		if callBack:
			callBack( self, base )
		
	def __addEnterSpacePlayers( self, playerBase, position, direction, pickData ):
		"""
		添加进入空间的玩家记录
		"""
		self.__waitEnterPlayers.append( ( playerBase, position, direction, pickData ) )
			
	def __addLoginSpacePlayers( self, playerBase ):
		"""
		添加在空间上线的玩家记录
		@param 	playerBase	:		玩家的base
		@type 	playerBase	:		mailbox
		"""
		self.__waitLoginPlayers.append( playerBase )
		
	def onLoseCell( self ):
		"""
		cell关闭，SpaceDomain通知过来的
		"""
		self.hasCell = False

	def onGetCell( self ):
		"""
		space获得了cell部份，SpaceDomain通知过来的，执行状态改变，并把需要进入space的玩家传到space中
		"""
		self.hasCell = True
		self.cellCreateing = False
		
		for playerBase, position, direction, pickData in self.__waitEnterPlayers:
			self.spaceMailBox.teleportEntity( playerBase, position, direction, pickData )
		self.__waitEnterPlayers = []
		
		for playerBase in self.__waitLoginPlayers:
			self.spaceMailBox.loginPlayer( playerBase )
		self.__waitLoginPlayers = []
		
#----------------------------------------------------------------------------
	def gotoSpace( self, playerBase, position, direction, pickData ):
		"""
		玩家进入空间
		@param 	playerBase	:		玩家的base
		@type 	playerBase	:		mailbox
		@param 	position	:		玩家的位置
		@type 	position	:		vector3
		@param 	direction	:		玩家的朝向
		@type 	direction	:		vector3
		@param 	pickData	:		打包的登陆数据
		@type 	pickData	:		字典
		@return: None
		"""
		if self.hasCell:
			self.spaceMailBox.teleportEntity( playerBase, position, direction, pickData)
		else:
			self.__addEnterSpacePlayers( playerBase, position, direction, pickData )

	def roleLogin( self, playerBase, pickData ):
		"""
		玩家上线
		@param 	playerBase	:		玩家的base
		@type 	playerBase	:		mailbox
		"""
		if self.hasCell:
			self.spaceMailBox.loginPlayer( playerBase )
		else:
			self.__addLoginSpacePlayers( playerBase )		# 加入等待列表中