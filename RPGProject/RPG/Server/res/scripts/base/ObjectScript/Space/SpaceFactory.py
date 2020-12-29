# -*- coding: utf-8 -*-

import ObjectScript.ScriptFactory as ScriptFactory
from SmartImport import smartImport
import LoadModule

class SpaceFactory( ScriptFactory.ScriptFactory ):
	def __init__( self ):
		ScriptFactory.ScriptFactory.__init__( self )
		self._domainDatas = {}
		
	def loadData( self, config ):
		ScriptFactory.ScriptFactory.loadData( self,config )
		objConfigs = LoadModule.openJsonPathCfg( config ) #打开的是一个路径
		for config in objConfigs:
			scriptID = config[ "scriptID" ]
			#加载SpaceEntity脚本类
			entityScriptName = config[ "EntityScript" ]
			spaceObjClass = smartImport( "ObjectScript.Space.SpaceScript."+entityScriptName + ":" + entityScriptName )( config )
			self._datas[ scriptID ] = spaceObjClass
			#加载SpaceDomain脚本类
			domainTypeName = config[ "DomainType" ]
			spaceDomainObjClass = smartImport( "ObjectScript.Space.SpaceDomain."+domainTypeName + ":" + domainTypeName )( config )
			self._domainDatas[ scriptID ] = spaceDomainObjClass
	
	def getSpaceDomainObject( self, scriptID ):
		#获取SpaceDomain 对象
		return self._domainDatas.get( scriptID, None )

	def getAllSpaceDomian( self ):
		#获取所有SpaceDomain
		return self._domainDatas
		

g_SpaceFactory = SpaceFactory()