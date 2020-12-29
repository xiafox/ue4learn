# -*- coding: utf-8 -*-

import ObjectScript.ScriptFactory as ScriptFactory

class NPCFactory( ScriptFactory.ScriptFactory ):
	def __init__( self ):
		ScriptFactory.ScriptFactory.__init__( self )
		
	def loadData( self, config ):
		ScriptFactory.ScriptFactory.loadData( self,config )


g_NPCFactory = NPCFactory()