# -*- coding: utf-8 -*-

from ObjectScript.GameEntityScript import GameEntityScript

class SpaceBase( GameEntityScript ):
	"""
	空间脚本基类
	"""
	def __init__( self, configData ):
		GameEntityScript.__init__( self, configData )
		self._belongTypes = [ int(i) for i in configData[ "belongType" ].split("|") ]		#同一个副本可以有多种归属
		self._spaceName = configData.get("SpaceName","")
	
	def getSpaceName( self ):
		"""
		获得SpaceName
		"""
		return self._spaceName