# -*- coding: utf-8 -*-
import KBEngine
import KBEDebug
import Const
import CoreEntity.SpaceEntity as SpaceEntity

class SpaceMultiLine( SpaceEntity.SpaceEntity ):
	def __init__( self  ):
		SpaceEntity.SpaceEntity.__init__( self )
		
	def initEntity( self ):
		"""
		virtual method 重写父类方法
		初始化space entity要做的事情
		"""
		SpaceEntity.SpaceEntity.initEntity( self )
		self.setCurrentSpaceData( Const.SPACE_DATA_LINE_NUMBER,self.spaceKey )