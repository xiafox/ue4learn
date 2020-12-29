# -*- coding: utf-8 -*-
import KBEngine
import KBEDebug
from .GameEntity import GameEntity

class HasCellEntity( GameEntity ):
	def __init__( self  ):
		GameEntity.__init__( self )
		self.hasCell = False	#是否CellEntity创建完成
		
	def destroySelf( self ):
		if self.cell is not None:#如果有cellEntity，先摧毁CellEntity
			self.cell.destroyCellByBase()
		else:
			GameEntity.destroySelf( self )
	
#-----------------引擎回调------------------------------------------
	def onLoseCell( self ):
		"""
		CELL丢失
		"""
		if self.cell is not None:
			self.cell.destroyCellByBase()
		else:
			self.hasCell = False
			self.destroySelf()
	
	def onGetCell( self ):
		"""
		CELL创建完成
		"""
		self.hasCell = True