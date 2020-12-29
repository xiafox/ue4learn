# -*- coding: utf-8 -*-

from ObjectScript.Space.SpaceScript.SpaceBase import SpaceBase

class SpaceMultiLine( SpaceBase ):
	def __init__( self, configData ):
		SpaceBase.__init__( self, configData )
		
	def getPackDomainData( self, playerEntity, args ):
		"""
		virtual method 子类重写
		"""
		gotoArgs = SpaceBase.getPackDomainData( self, playerEntity, args )
		gotoArgs["spaceKey"] = 1
		return gotoArgs