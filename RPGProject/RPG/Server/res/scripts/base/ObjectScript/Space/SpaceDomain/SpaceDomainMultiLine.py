# -*- coding: utf-8 -*-
import KBEngine
import KBEDebug
from ObjectScript.Space.SpaceDomain.SpaceDomainBase import SpaceDomainBase
import csdefine

class SpaceDomainMultiLine( SpaceDomainBase ):
	"""
	多线地图空间领域
	"""
	def __init__( self, configData ):
		SpaceDomainBase.__init__( self, configData )
		self.currInitLine = 1	#当前初始分线
		self.maxLine = 1		#最大分线
		self.lineSpaceNumbers = {}	#Key为分线，Value为space编号
		
	def initSpaceDomain( self ):
		"""
		virtual method 子类重写
		"""
		self.__createMultiLineSpaceItem( self.currInitLine )
		
	def __createMultiLineSpaceItem( self, lineNumber  ):
		dict = { "spaceKey": str(lineNumber), "belongType": csdefine.SPACE_BELONG_NORMAL}
		spaceItem = self.createSpaceItem( dict )
		self.lineSpaceNumbers[lineNumber] = spaceItem.spaceNumber
		
	def findSpaceItem( self, pickArgs ):
		"""
		virtual method 子类重写
		查找一个空间
		"""
		spaceNumber = self.lineSpaceNumbers[1] #暂时只做一个分线
		spaceItem = self.getSpaceItem( spaceNumber )
		KBEDebug.INFO_MSG( "++++++SpaceDomainMultiLine:findSpaceItem spaceNumber=%i "% spaceNumber)
		return spaceItem
#------------------------------外部调用接口-----------------------------------------------------
