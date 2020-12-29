# -*- coding: utf-8 -*-
import KBEngine

class ScriptBase( object ):
	"""
	script层基类
	"""
	def __init__( self, configData ):
		self.scriptID = configData[ "scriptID" ]