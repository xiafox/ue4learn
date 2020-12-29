# -*- coding: utf-8 -*-
import KBEngine
import KBEDebug
import Const
import kbemain
import CoreEntity.MgrEntity as MgrEntity

class SpaceManager( MgrEntity.MgrEntity ):
	"""
	空间管理器
	"""
	def __init__( self  ):
		MgrEntity.MgrEntity.__init__( self )
		self.__lastSpaceNumber = 0	#记录上一次创建的space的编号
		self.spaceDomains = {}	#存取配置表中所有space配置，字典样式 {scriptID:SpaceDomainBase类实例,.....}
		#self.__spaceGlobalRecord = {} #副本记录,字典样式{spaceClassName:{belongType:[spaceKey,....]},......}
		self.__needInitCompletedSpace = Const.NEED_INIT_COMPLETED_SPACE #需要初始化创建好的空间
		self.initSpaces = set() #已经初始化完成的space
		self.__hasInitCompleted = False
		self.__spaceStartProcessCheck()
		self.initSpaceDomains()
		
	def __spaceStartProcessCheck( self ):
		"""
		空间启动检查
		"""
		if self.__hasInitCompleted:
			return
		if len( self.__needInitCompletedSpace ) == 0:
			self.__hasInitCompleted = True
			self.onCompleteInit()	
		
	def initSpaceDomains( self ):
		"""
		初始化空间组
		"""
		self.spaceDomains =kbemain.g_objScriptFactory.getAllSpaceDomian()
		for spaceDomain in self.spaceDomains.values():
			spaceDomain.initSpaceDomain()
		
	def getNewSpaceNumber( self ):
		"""
		获取一个唯一的space编号
		@return: INT32
		"""
		number = self.__lastSpaceNumber
		number = ( number + 1 ) % 0x7FFFFFFF
		self._lastSpaceNumber = number
		return number

#-----------------remote call------------------------------------------

	def onLoadOver( self, spaceScriptID ):
		"""
		define method
		Space加载完
		"""	
		KBEDebug.INFO_MSG( "Space:%s Load Completed."%(spaceScriptID))
		if spaceScriptID in self.initSpaces:
			return
		self.initSpaces.add( spaceScriptID )
		if spaceScriptID in self.__needInitCompletedSpace:
			self.__needInitCompletedSpace.remove( spaceScriptID )
		#检测需初始化启动的地图是否加载完成
		self.__spaceStartProcessCheck()
		
	def roleLogin( self, spaceClassName, roleMB, pickArgs ):
		"""
		<define method>
		玩家登陆进入地图
		玩家登录向SpaceDomain请求登录 -> SpaceDomain通知SpaceItem玩家要登录 -> SpaceItem找到对应的Space的baseEntity通知登陆 ->space的baseEntity通知玩家在SpaceEntity空间创建Rolecell在这个位置roleBase.createCellEntity(spaceEntity.cell)
		"""
		self.spaceDomains[ spaceClassName ].roleLogin( roleMB, pickArgs )
	
	def gotoSpace( self, spaceClassName, roleMB, position, direction, pickArgs ):
		"""
		<define method>
		传送进地图
		玩家向SpaceDomain请求传送 -> SpaceDomain通知SpaceItem玩家传送 -> SpaceItem找到对应的space的baseEntity通知传送 -> space的baseEntity通知自己cellEntity传送玩家 -> space的cellEntity通知玩家的roleCell传送，并把自己的sell传过去
		（注意：这里不能直接在space的baseEntity传参数self.cell通知玩家的roleCell，详见KBE的API里面 createCellEntity介绍）
		"""
		self.spaceDomains[ spaceClassName ].gotoSpace( roleMB, position, direction, pickArgs )
		
	def onSpaceGetCell( self, spaceScriptID, spaceNumber ):
		"""
		<define method>
		空间创建完成通知
		"""
		self.spaceDomains[ spaceScriptID ].onSpaceGetCell( spaceNumber )
	
	def onSpaceLoseCell( self, spaceScriptID, spaceNumber ):
		"""
		<define method>
		空间cell销毁通知
		"""
		self.spaceDomains[ spaceScriptID ].onSpaceLoseCell( spaceNumber )