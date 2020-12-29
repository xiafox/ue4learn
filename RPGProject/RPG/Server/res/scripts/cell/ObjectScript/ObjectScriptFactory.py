# -*- coding: utf-8 -*-

import KBEngine
import Singleton
from ObjectScript.Space import SpaceFactory


class ObjectScriptFactory( Singleton.Singleton ):
	"""
	游戏对象script工厂类入口
	"""
	def __init__( self ):
		Singleton.Singleton.__init__( self )
		self.__LoadConfig()
	
	def __LoadConfig( self ):
		SpaceFactory.g_SpaceFactory.loadData( "GameObject/SpaceObject" )

	def getSpaceObject( self, scriptID ):
		#获取SpaceObject 对象
		return SpaceFactory.g_SpaceFactory.getObject( scriptID )	


#global instance
g_objScriptFactory = ObjectScriptFactory()