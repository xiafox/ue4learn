# -*- coding: utf-8 -*-

"""
封装引擎功能，不包含游戏层功能
"""

import KBEngine

class ServerEntity( KBEngine.Entity ):
	def __init__( self ):
		KBEngine.Entity.__init__( self )
		self.isDestroy = False
		
	def getClient( self ):
		"""
		获取client
		"""
		if hasattr(self, "client") and self.client:
			return self.client
		return None
		
	def getAllClients( self ):
		"""
		获取allClients
		"""
		if hasattr(self, "allClients") and self.allClients:
			return self.allClients
		return None