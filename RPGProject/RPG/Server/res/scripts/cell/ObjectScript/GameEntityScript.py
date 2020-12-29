# -*- coding: utf-8 -*-

import Math
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
		"""
		虚函数 可重写
		"""
		entityDict = {}
		entityDict.update( params )
		entityDict[ "scriptID" ] = self.scriptID
		return entityDict
		
	def createEntity( self, spaceEntity, position, direction, params ) :
		"""
		在当前cellapp创建，在当前进程指定space中创建一个新的实体。
		"""
		return KBEngine.createEntity( self._entityType, spaceEntity.spaceID, Math.Vector3(position), direction, self.getEntityProperties( params ) )
		