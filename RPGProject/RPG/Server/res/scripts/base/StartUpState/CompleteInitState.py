# -*- coding: utf-8 -*-
import KBEngine
import Const
from StartUpState.StartUpStateBase import StartUpStateBase

class CompleteInitState(StartUpStateBase):
	"""
	服务器初始化完毕
	"""
	def __init__(self):
		"""
		"""
		StartUpStateBase.__init__(self)
		
	def onEnter( self ):
		"""
		重写父类方法，进入某个状态
		"""
		
		StartUpStateBase.onEnter( self )
		
	def doComplete( self ):
		"""
		重写父类方法,状态完成
		"""
		srvMgrBaseCall = KBEngine.globalData["ServerGameStartUpMgr"] #都在第一个BaseApp进程上
		srvMgr = KBEngine.entities[ srvMgrBaseCall.id ] #获得实例
		srvMgr.onStartUpCompleted()