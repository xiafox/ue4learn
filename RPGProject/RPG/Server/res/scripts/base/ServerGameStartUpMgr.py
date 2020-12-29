# -*- coding: utf-8 -*-
import KBEngine
import KBEDebug
import Const
import CoreEntity.MgrEntity as MgrEntity
import StartUpState

class ServerGameStartUpMgr( MgrEntity.MgrEntity ):
	def __init__( self  ):
		MgrEntity.MgrEntity.__init__( self )
		self.__currState = -1 #当前启动状态
		self.__startUpStateMap = {} #状态实例字典
		self.__timeoutID = 0
		self.__registerAllStartUpStateObject()
	
	def init( self ):
		"""
		"""
		#self.timeoutID = self.addTimer( 5 * 60 ) #超时检测
		self.changeState(Const.WaitInit)
	
	def onTimer( self, timerID, userData ): 
		"""
		定时器回调
		"""
		pass
	
	def __registerAllStartUpStateObject( self ):
		"""
		注册所有的启动状态对象字典
		"""
		self.__startUpStateMap = {
			Const.WaitInit			: StartUpState.WaitInitState.WaitInitState(),
			Const.FirstStageMgrInit	: StartUpState.FirstStageMgrInitState.FirstStageMgrInitState(),
			Const.SecondStageMgrInit	: StartUpState.SecondStageMgrInitState.SecondStageMgrInitState(),
			Const.ThirdStageMgrInit	: StartUpState.ThirdStageMgrInitState.ThirdStageMgrInitState(),
			Const.CompleteInit		: StartUpState.CompleteInitState.CompleteInitState(),
			}
	
#------------------------------外部调用接口-----------------------------------------------------
	def changeState( self, state ):
		"""
		改变状态
		"""
		if self.__currState == state:
			return	
		if state not in self.__startUpStateMap:
			KBEDebug.ERROR_MSG("not found state(%d)" %(state))
			return
		self.__currState = state
		self.__startUpStateMap.get( state ).onEnter()
		
	def onMgrInitCompleted(self, mgrName):
		"""
		管理器初始化完成，由管理器Entity初始化完毕通知过来的
		"""
		stateObject = self.__startUpStateMap.get( self.__currState, None )
		if stateObject is None:
			KBEDebug.ERROR_MSG("stateObject is None")
			return
		stateObject.onMgrInitCompleted( mgrName )
	
	def onStartUpCompleted( self):
		"""
		服务器启动完成回调，由状态实例通知
		"""
		KBEDebug.INFO_MSG( "Server start-up completed" )