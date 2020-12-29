# -*- coding: utf-8 -*-

import KBEngine
import KBEDebug
import CoreEntity.GameEntity as GameEntity
import Const
import kbemain

class SpaceEntity( GameEntity.GameEntity ):
	def __init__( self ):
		GameEntity.GameEntity.__init__( self )
		KBEngine.cellAppData[ Const.GET_SPACE_CELLAPP_KEY( self.spaceID ) ] = self.base #注册之后这个字典对象会在所有的CellApps之间自动同步
		self.__readyEnterRoles = []	#准备进入该空间的玩家的baseMailbox
		self.__spaceRoles = [] #记录已经在这个空间上的所有玩家的Cell mailbox（退出副本有用到）
		self.__timeID = 0
		self.initEntity()
		
	def initEntity( self ):
		"""
		virtual method
		空间创建完成设置指定key的space数据
		"""
		self.setCurrentSpaceData( Const.SPACE_DATA_SCRIPT_ID, self.scriptID )
		self.setCurrentSpaceData( Const.SPACE_DATA_SPACE_ID, str( self.id ) )
		self.setCurrentSpaceData( Const.SPACE_DATA_BELONG_TYPE, str( self.belongType ) )
		self.setCurrentSpaceData( Const.SPACE_DATA_SPACE_TYPE, str( self.getScript().getSpaceType() ) )
		
	def __isInReadyEnterRoles( self, roleBase ):
		"""
		判断玩家是否在准备进入该空间的列表中
		"""
		for roleMB in self.__readyEnterRoles:
			if roleMB.id == roleBase.id:
				return True
		return False
		
	def __hasRoleInSpace( self, roleBase ):
		"""
		判断玩家是否已在该space中
		"""
		for roleCell in self.__spaceRoles:
			if roleCell.id == roleBase.id:
				return True
		return False
		
	def getScript( self ):
		return kbemain.g_objScriptFactory.getSpaceObject( self.scriptID )
		
#---------------空间玩家数据管理------------------------------------
	def __registerRole( self, roleCell ):
		"""
		记录空间玩家
		"""
		self.__spaceRoles.append( roleCell )
	
	def __deregisterRole( self, roleCell ):
		"""
		删除空间玩家
		"""
		for r in self.__spaceRoles:
			if r.id == roleCell.id:
				self.__spaceRoles.remove( r )
				break
	
	def hasRole( self, roleMB ):
		for roleCell in self.__spaceRoles:
			if roleCell.id == roleMB.id:
				return True
		return False
	
	def getSpaceRoleNum( self ):
		"""
		获取空间上玩家数量
		"""
		return len( self.__spaceRoles )

	def getSpaceRoles(self):
		return self.__spaceRoles
		
#----------------以下方法是由玩家的cell通知过来，玩家Cell和SpaceCell在一个进程上，不需要def定义-----------------
		
	def onEnter( self, playerCell, packArgs ):
		"""
		玩家进入空间通知（由玩家的cell通知过来）
		"""
		KBEDebug.INFO_MSG("Player(%s) enter space(%s)! scriptID:%s."%( playerCell.id, self.id, self.scriptID ))
		self.__registerRole( playerCell )
		for roleMB in self.__readyEnterRoles:
			if roleMB.id == playerCell.id:
				self.__readyEnterRoles.remove( roleMB )
		self.getScript().onEnter( self, playerCell, packArgs )
		
	def onLeave( self, playerCell, packArgs ):
		"""
		玩家离开空间通知（由玩家的cell通知过来）
		"""
		self.__deregisterRole( playerCell )
		self.getScript().onLeave( self, playerCell, packArgs )
		
	def onLoginSuccess( self, playerCell ):
		"""
		玩家登陆成功通知（由玩家的cell通知过来）
		"""
		KBEDebug.INFO_MSG("Player(%s) Login space(%s)! scriptID:%s."%( playerCell.id, self.id, self.scriptID ))
		self.__registerRole( playerCell )
		self.getScript().onLoginSuccess( self, playerCell )
		
#-------------------------------remote call-------------------------------
	def teleportEntity( self, roleBase, position, direction, packArgs ):
		"""
		<define method>
		传送一个entity,spaceItem通知过来的
		"""
		if not self.__hasRoleInSpace( roleBase ) and not self.__isInReadyEnterRoles( roleBase ):
			self.__readyEnterRoles.append( roleBase )
			roleBase.cell.doGotoSpace( self.scriptID, self, position, direction, packArgs )
		
	def onSpaceDataChanged( self, spaceID, key, value ):
		"""
		<define method>
		当spaceData改变通知
		"""
		pass
		
	def closeSpace():
		"""
		<define method>
		virtual method 子类可重写,	
		关闭副本唯一入口，所有关闭副本操作（base或cell）都应该走此接口
		"""
		KBEDebug.INFO_MSG("Close space(%s)! scriptID:%s."%( self.id, self.scriptID ))
		self.__readyEnterRoles = []
		if len( self.__spaceRoles ):
			for roleCell in self.__spaceRoles:
				roleCell.gotoExitSpacePos()
			#玩家先退出空间，5秒后再摧毁spaceEntity，摧毁空间是在entity摧毁后做
			self.__timeID = self.addTimer( 5, 0 )
		else:
			self.destroySelf()
		
	def onTimer( self, timerHandle, userData ):
		if self.__timeID == timerHandle:
			self.destroySelf()

#---------------------------------引擎方法---------------------------------
	def onDestroy( self ):
		"""
		这个函数在调用Entity.destroy()后，在实际销毁之前被调用
		"""
		self.destroySpace()		#引擎方法，销毁这个实体所在的空间。