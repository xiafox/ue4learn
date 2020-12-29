#-*-coding:utf-8-*-

import KBEngine
import Const


class BaseAppEntity( KBEngine.Entity ):
	def __init__( self ):
		KBEngine.Entity.__init__( self )
		self._allBaseAppEntityMBs = []
		self._allPlayers = {}
	
	
	def doFirstStartBaseAppEntityRegister( self, baseAppEntityMB ):
		"""
		"""
		self._allBaseAppEntityMBs.append( baseAppEntityMB )
		
		for iBaseApp in self._allBaseAppEntityMBs:
			if iBaseApp.id == self.id:
				continue
			iBaseApp.doRegisterBaseAppEntity( self._allBaseAppEntityMBs )
		
	def doRegisterBaseAppEntity( self, baseAppEntityMBs ):
		"""
		"""	
		self._allBaseAppEntityMBs = baseAppEntityMBs
	
	def registerPlayer( self, playerName, playerBaseMB ):
		"""
		"""
		self._allPlayers[playerName] = playerBaseMB

	def unregisterPlayer( self, playerName ):
		"""
		"""
		self._allPlayers.pop( playerName )


