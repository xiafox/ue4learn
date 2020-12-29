# -*- coding: utf-8 -*-

import KBEngine
import ObjectScript.ScriptBase as ScriptBase

class GameEntityScript( ScriptBase.ScriptBase ):
	"""
	游戏对象Entity层对应的script基类
	"""
	def __init__( self, configData ):
		ScriptBase.ScriptBase.__init__( self, configData )
		self._entityType = configData[ "EntityType" ]
	
	def getEntityProperties( self, params ):
		entityDict = {}
		entityDict.update( params )
		entityDict[ "scriptID" ] = self.scriptID
		return entityDict
	
	def createLocalEntity( self, params = None ) :
		"""
		在当前baseapp创建
		"""
		return KBEngine.createEntityLocally( self._entityType, self.getEntityProperties( params ) )
	
	def createAnywhereEntity( self, params = None, callbackFunc = None ):
		"""
		找一个空闲的baseapp创建
		"""
		KBEngine.createEntityAnywhere( self._entityType, self.getEntityProperties( params ), callbackFunc )
