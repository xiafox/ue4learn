# -*- coding: utf-8 -*-

import ObjectScript.ScriptFactory as ScriptFactory
from SmartImport import smartImport
import LoadModule

class MonsterFactory( ScriptFactory.ScriptFactory ):
	def __init__( self ):
		ScriptFactory.ScriptFactory.__init__( self )
		
	def loadData( self, config ):
		ScriptFactory.ScriptFactory.loadData( self,config )


g_MonsterFactory = MonsterFactory()