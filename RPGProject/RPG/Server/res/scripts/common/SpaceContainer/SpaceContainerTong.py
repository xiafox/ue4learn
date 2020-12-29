# -*- coding: utf-8 -*-

import csdefine
import KBEngine
from SpaceContainer import SpaceContainerBase

class SpaceContainerTong( SpaceContainerBase.SpaceContainerBase ):
	def __init__( self ):
		SpaceContainerBase.SpaceContainerBase.__init__( self )

	#------------------------------for cell---------------------------------------------
	@classmethod
	def packDataToFindSpace( SELF, spaceScriptID, roleEntity, args ):
		"""
		打包数据用于查找space
		"""
		if KBEngine.component == "cellapp":
			packDict = { "playerDBID": roleEntity.playerDBID, "spaceKey": str( args.get( "enterSpaceTongDBID", roleEntity.tongDBID ) ), "belongType": csdefine.SPACE_BELONG_TONG }
		else:
			packDict = { "playerDBID": roleEntity.databaseID, "spaceKey": str( args.get( "enterSpaceTongDBID", roleEntity.tongDBID ) ), "belongType": csdefine.SPACE_BELONG_TONG }
		args.update( packDict )
		return args
	
	#------------------------------for base---------------------------------------------
	@classmethod
	def packDataToLogin( SELF, spaceScriptID, roleEntity, args ):
		"""
		打包数据，用于登陆
		"""
		if KBEngine.component == "cellapp":
			packDict = { "playerDBID": roleEntity.playerDBID, "spaceKey": str( args.get( "enterSpaceTongDBID", roleEntity.tongDBID ) ), "belongType": csdefine.SPACE_BELONG_TONG }
		else:
			packDict = { "playerDBID": roleEntity.databaseID, "spaceKey": str( args.get( "enterSpaceTongDBID", roleEntity.tongDBID ) ), "belongType": csdefine.SPACE_BELONG_TONG }
		args.update( packDict )
		return args
