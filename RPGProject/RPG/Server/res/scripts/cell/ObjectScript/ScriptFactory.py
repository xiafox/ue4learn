# -*- coding: utf-8 -*-

import Singleton

class ScriptFactory( Singleton.Singleton ):
	"""
	script工厂类基类
	"""
	def __init__( self ):
		Singleton.Singleton.__init__( self )
		self._datas = {}
		
	def loadData( self, config ):
		pass
	
	def getObject( self, scriptID ):
		return self._datas.get( scriptID, None )
		
	def hasObject( scriptID ):
		return self._datas.has_key( scriptID )
	
	def getAllObject( self ):
		return self._datas
