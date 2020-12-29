# -*- coding: utf-8 -*-

import csdefine
from SpaceContainer import SpaceContainerBase

class SpaceContainerPerson( SpaceContainerBase.SpaceContainerBase ):
	def __init__( self ):
		SpaceContainerBase.SpaceContainerBase.__init__( self )
	
	#------------------------------for cell---------------------------------------------
	@classmethod
	def packDataToFindSpace( SELF, spaceScriptID, roleEntity, args ):
		"""
		打包数据用于查找space
		"""
		packDict = { "playerDBID": roleEntity.playerDBID, "spaceKey": str( roleEntity.playerDBID ), "belongType": csdefine.SPACE_BELONG_PERSON }
		args.update( packDict )
		if "packSpaceLevel" not in args:
			args["packSpaceLevel"] = roleEntity.getLevel()	#单人副本用创建者等级赋值副本等级（CST-2880）
		return args
	
	#------------------------------for base---------------------------------------------
	@classmethod
	def packDataToLogin( SELF, spaceScriptID, roleEntity, args ):
		"""
		打包数据，用于登陆
		"""
		packDict = { "playerDBID": roleEntity.cellData["playerDBID"], "spaceKey": str( roleEntity.cellData["playerDBID"] ), "belongType": csdefine.SPACE_BELONG_PERSON }
		args.update( packDict )
		return args
