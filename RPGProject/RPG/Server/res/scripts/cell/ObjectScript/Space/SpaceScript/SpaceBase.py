# -*- coding: utf-8 -*-

from ObjectScript.GameEntityScript import GameEntityScript
import Const
import csdefine
import SpaceContainer

class SpaceBase( GameEntityScript ):
	"""
	空间脚本基类
	"""
	def __init__( self, configData ):
		GameEntityScript.__init__( self, configData )
		self._belongTypes = [ int(i) for i in configData[ "belongType" ].split("|") ]		#同一个副本可以有多种归属
		self._spaceName = configData.get("SpaceName","")
		self._spaceType = getattr( csdefine, configData[ "spaceType" ] )
		
	def getSpaceName( self ):
		"""
		获得SpaceName
		"""
		return self._spaceName
		
	def getSpaceType( self ):
		return self._spaceType
		
	def getPackDomainData( self, playerEntity, args ):
		"""
		virtual method
		"""
		return SpaceContainer.getPickContainerData( self._belongTypes[0], self.scriptID, playerEntity, args )
		
	def onEnter( self, selfEntity, playerCell, packArgs ):
		"""
		玩家进入空间通知，由space的CellEntity通知
		"""
		pass
	
	def onLeave( self, selfEntity, playerCell, packArgs ):
		"""
		玩家离开空间通知，由space的CellEntity通知
		"""
		pass
		
	def onLoginSuccess( self, selfEntity, playerCell ):
		"""
		玩家登陆空间通知，由space的CellEntity通知
		"""
		pass