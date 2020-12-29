# -*- coding: utf-8 -*-

"""
"""
import KBEngine
import CoreEntity.HasCellEntity as HasCellEntity
import kbemain
import KBEDebug

PROCESS_ENTER_TIME = 0.1	#玩家检测进入地图的时间
PROCESS_ENTER_TIMER_ARG = 30	#玩家检测Timer标识符

class SpaceEntity( HasCellEntity.HasCellEntity, KBEngine.Entity ):
	def __init__( self ):
		HasCellEntity.HasCellEntity.__init__( self )
		KBEngine.Entity.__init__( self )
		self.scriptID = self.cellData[ "scriptID" ]
		self.spaceNumber = self.cellData[ "spaceNumber" ] #空间编号，用于查找空间
		self._enters = []
		self._logons = []
		self.__createSpaceCell()
		
	def getScript( self ):
		return kbemain.g_objScriptFactory.getSpaceObject( self.scriptID )
		
	def __createSpaceCell( self ):
		"""
		创建空间及空间的CellEntity
		"""
		self.createCellEntityInNewSpace( 0 ) #引擎方法：在cellapp上创建一个空间(space)并且将该实体的cell创建到这个新的空间中
		
	def __createLoginRoleCell( self, roleBase ):
		"""
		创建登陆角色的CellEntity（玩家登陆用到）
		"""
		roleBase.createCellFromSpace( self.cell )
		
	def __teleportEntity( self, roleBase, position, direction, packArgs ):
		"""
		传送玩家
		"""
		self.cell.teleportEntity( roleBase, position, direction, packArgs )
		
	def onProcessEnterTimer( self ):
		"""
		检查进入
		"""
		if not self.hasCell:
			return

		if len( self._logons ) > 0:
			self.__createLoginRoleCell( self._logons.pop( 0 ) )
		elif len( self._enters ) > 0:
			playerbase, position, direction, packArgs = self._enters.pop( 0 )
			self.__teleportEntity( playerbase, position, direction, packArgs )
			
		if len( self._logons ) > 0 or len( self._enters ) > 0:#防止一下子过多人进入导致服务器出问题，这个进行了一个排除机制
			self.addTimer( PROCESS_ENTER_TIME, 0, PROCESS_ENTER_TIMER_ARG )
		
	def onTimer( self, timerHandle, userData ):
		if userArg == PROCESS_ENTER_TIMER_ARG:
			self.onProcessEnterTimer()
		
#-----------------remote call------------------------------------------

	def teleportEntity( self, roleBase, position, direction, packArgs ):
		"""
		<define method>
		传送到这个space的玩家，由SpaceItem通知
		"""
		self._enters.append( ( roleBase, position, direction, packArgs ) )
		self.onProcessEnterTimer()
		
	def loginPlayer( self, playerbase ):
		"""
		<Define method>
		登陆到这个space的玩家，由SpaceItem通知
		"""
		self._logons.append( playerbase )
		self.onProcessEnterTimer()
		
#-----------------引擎回调------------------------------------------
	
	def onGetCell( self ):
		"""
		引擎回调，cell 实体创建完成。
		"""
		HasCellEntity.HasCellEntity.onGetCell( self )
		spaceMgrBaseCall = KBEngine.globalData["SpaceManager"]
		if spaceMgrBaseCall:
			spaceMgrBaseCall.onSpaceGetCell( self.scriptID, self.spaceNumber )
			spaceMgrBaseCall.onLoadOver( self.scriptID )	#暂时没有寻路数据，space加载完成就先写这
		self.onProcessEnterTimer()
		
	def onLoseCell( self ):
		"""
		引擎回调，这个函数在它关联的cell实体销毁之后被调用。这个函数没有参数
		"""
		HasCellEntity.HasCellEntity.onLoseCell( self )
		KBEngine.globalData["SpaceManager"].onSpaceLoseCell( self.scriptID, self.spaceNumber )