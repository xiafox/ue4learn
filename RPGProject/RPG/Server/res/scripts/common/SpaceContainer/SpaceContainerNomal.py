# -*- coding: utf-8 -*-

import csdefine
from SpaceContainer import SpaceContainerBase

class SpaceContainerNomal( SpaceContainerBase.SpaceContainerBase ):
	def __init__( self ):
		SpaceContainerBase.SpaceContainerBase.__init__( self )
	
	#------------------------------for cell---------------------------------------------
	@classmethod
	def packDataToFindSpace( SELF, spaceScriptID, roleEntity, args ):
		"""
		打包数据用于查找space
		"""
		packDict = { "spaceKey": spaceScriptID, "belongType": csdefine.SPACE_BELONG_NORMAL,"playerDBID": roleEntity.playerDBID }
		args.update( packDict )
		return args
	
	#------------------------------for base---------------------------------------------
	@classmethod
	def packDataToLogin( SELF, spaceScriptID, roleEntity, args ):
		"""
		打包数据，用于登陆
		"""
		packDict = { "spaceKey": spaceScriptID, "belongType": csdefine.SPACE_BELONG_NORMAL,"playerDBID": roleEntity.cellData["playerDBID"] }
		args.update( packDict )
		return args
