# -*- coding: utf-8 -*-
import KBEngine
import KBEDebug
import KBEMath
import Math
import kbemain
import Const

class RoleSpaceInterface:
	def __init__( self ):
		self.isTeleporting = False
		
	def CELL_GoToSpace( self, srcEntityID, spaceScriptID, position, direction ):
		"""
		<exposed method>
		客户端请求传送到某地图
		"""
		if srcEntityID == self.id:
			self.__gotoSpace(spaceScriptID, Math.Vector3(KBEMath.Unreal2KBEnginePosition( position )), KBEMath.Unreal2KBEngineDirection( direction ), {})#虚幻坐标朝向和服务器有差别需转换
		
	def __gotoSpace( self, spaceScriptID, position, direction, pickArgs ):
		"""
		传送到指定地图
		"""
		if self.isDestroy:
			return
		spaceObj = kbemain.g_objScriptFactory.getSpaceObject( spaceScriptID )
		if not spaceObj:
			KBEDebug.INFO_MSG("Space(%s) config not exist!"%spaceScriptID)
			return
		gotoArgs = spaceObj.getPackDomainData( self, pickArgs ) #打包参数
		KBEngine.globalData[ "SpaceManager" ].gotoSpace( spaceScriptID, self.base, position, direction, gotoArgs )
		
	def onLoginSpaceSuccess( self ):
		"""
		<define method>
		登录成功回调通知（玩家上线的时候才是登陆）
		"""
		self.getCurrentSpaceEntity().onLoginSuccess( self )
		
	def gotoExitSpacePos( self ):
		"""
		<define method>
		传送到ExitSpace配置的地图
		"""
		pass
		
	def doGotoSpace( self, spaceScriptID, spaceCell, position, direction, packArgs ):
		"""
		<define method>
		执行传送到副本,由space的CellEntity通知
		"""
		self.__teleportPlayerToSpace( spaceScriptID, spaceCell, position, direction, packArgs )
		
	def __teleportPlayerToSpace( self, spaceScriptID, spaceCell, position, direction, packArgs ): 
		"""
		传送玩家
		"""
		self.isTeleporting = True
		isSameSpace = ( self.getCurrentSpace().id == spaceCell.id )	# 目标地图id与当前地图的id相同，是相同地图传送
		if isSameSpace:
			self.teleport( spaceCell, Math.Vector3(position), direction ) #引擎方法
		else:
			self.onLeaveSpaceNotify( spaceScriptID, packArgs )
			self.teleport( spaceCell, Math.Vector3(position), direction )
		
	def onTeleportSuccess( self, nearbyEntity ):
		"""
		引擎方法传送成功通知
		"""
		KBEDebug.INFO_MSG("onTeleportSuccess")
		self.isTeleporting = False
		self.onEnterSpaceNotify()
		
	def onTeleportFailure( self ):
		"""
		引擎方法传送成功通知
		"""
		KBEDebug.INFO_MSG("onTeleportFailure")
		self.isTeleporting = False
		
	def onEnterSpaceNotify( self ):
		"""
		传送进入某个地图回调通知
		"""
		self.spaceScriptID = self.getCurrentSpaceScriptID()
		self.spaceBelongType = int(self.getCurrentSpaceData( Const.SPACE_DATA_BELONG_TYPE ))
		self.getCurrentSpaceEntity().onEnter( self, {} )	#玩家和spaceEntity是在一个进程的
		self.client.CLIENT_OnEnterSpaceNotify( self.spaceScriptID, self.getCurrentSpaceObject().getSpaceType() )
		
	def onLeaveSpaceNotify( self, dstSpaceScriptID, packArgs ):
		"""
		离开某地图回调通知
		"""
		spaceEntity = self.getCurrentSpaceEntity()
		if spaceEntity:
			spaceEntity.onLeave( self, packArgs )
		if self.getClient():
			self.client.CLIENT_OnLeaveSpaceNotify()