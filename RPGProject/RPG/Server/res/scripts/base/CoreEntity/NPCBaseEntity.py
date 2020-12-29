# -*- coding: utf-8 -*-

"""
"""
import KBEngine
import CoreEntity.GameEntity as GameEntity

class NPCBaseEntity( GameEntity.GameEntity, KBEngine.entity ):
	def __init__( self ):
		GameEntity.GameEntity.__init__( self )
		
