# -*- coding: utf-8 -*-

"""
"""
import KBEngine
import CoreEntity.GameEntity as GameEntity

class MgrEntity( GameEntity.GameEntity, KBEngine.Entity ):
	def __init__( self ):
		GameEntity.GameEntity.__init__( self )
		KBEngine.Entity.__init__( self )
		self.registerInGlobalData()
		
	def registerInGlobalData(self):
		"""
		将管理器名字注册到全局字典数据保存，以供在不同的baseAPP或CellAPP都可获取管理器
		"""
		KBEngine.globalData[ self.__class__.__name__ ] = self
	
	def onCompleteInit(self):
		"""
		管理器初始化完成回调
		"""
		KBEngine.globalData["ServerGameStartUpMgr"].onMgrInitCompleted( self.__class__.__name__ )
