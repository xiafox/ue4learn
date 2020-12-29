# -*- coding: utf-8 -*-

import ObjectScript.ScriptFactory as ScriptFactory
from SmartImport import smartImport
import LoadModule

class SpaceFactory( ScriptFactory.ScriptFactory ):
	def __init__( self ):
		ScriptFactory.ScriptFactory.__init__( self )
	
	def loadData( self, config ):
		ScriptFactory.ScriptFactory.loadData( self,config )
		objConfigs = LoadModule.openJsonPathCfg( config ) #打开的是一个路径
		for config in objConfigs:
			scriptID = config[ "scriptID" ]
			#加载SpaceEntity脚本类
			entityScriptName = config[ "EntityScript" ]
			spaceObjClass = smartImport( "ObjectScript.Space.SpaceScript."+entityScriptName + ":" + entityScriptName )( config )
			self._datas[ scriptID ] = spaceObjClass

g_SpaceFactory = SpaceFactory()