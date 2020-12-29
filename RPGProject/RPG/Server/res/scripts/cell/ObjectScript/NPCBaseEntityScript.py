# -*- coding: utf-8 -*-

import ObjectScript.GameEntityScript as GameEntityScript

class NPCBaseEntityScript( GameEntityScript.GameEntityScript ):
	def __init__( self ):
		GameEntityScript.GameEntityScript.__init__( self )
		self._entityProperty = {}
		
	def setEntityProperties( self, propertyKey, propertyValue ):
		"""设置创建entity的属性"""
		self._entityProperty[propertyKey] = propertyValue
		
	def getEntityProperties( self, params = {} ):
		""""""
		params.update( self._entityProperty )
		return params
		