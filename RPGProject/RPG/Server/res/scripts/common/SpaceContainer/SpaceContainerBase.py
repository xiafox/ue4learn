# -*- coding: utf-8 -*-

class SpaceContainerBase( dict ):
	def __init__( self ):
		dict.__init__( self )
	
#------------------------------for cell---------------------------------------------
	@classmethod
	def packDataToFindSpace( SELF, spaceScriptID, roleEntity, args ):
		"""
		打包数据用于查找space
		"""
		return args

#------------------------------for base---------------------------------------------
	@classmethod
	def packDataToLogin( SELF, spaceScriptID, roleEntity, args ):
		"""
		打包数据，用于登陆
		"""
		return args
		