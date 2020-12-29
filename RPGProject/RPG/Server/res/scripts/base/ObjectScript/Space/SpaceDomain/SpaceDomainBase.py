# -*- coding: utf-8 -*-

import KBEngine
from ObjectScript.ScriptBase import ScriptBase
from ObjectScript.Space.SpaceItem import SpaceItem
import KBEDebug

class SpaceDomainBase( ScriptBase ):
	"""
	空间领域
	"""
	def __init__( self, configData ):
		ScriptBase.__init__( self, configData )
		self.spaceItems = {} #Key是创建的space编号，Value是SpaceItem类实例
		
	def initSpaceDomain( self ):
		"""
		virtual method
		"""
		pass
		
	def createSpaceItem(self, params):
		"""
		创建空间spaceItem并返回
		"""
		spaceMgrBaseCall = KBEngine.globalData["SpaceManager"] #SpaceManger和Domain都在第一个BaseApp上
		spaceMgr = KBEngine.entities[ spaceMgrBaseCall.id ]
		spaceNumber = spaceMgr.getNewSpaceNumber()
		spaceItem = SpaceItem( self.scriptID, spaceNumber, params )
		spaceItem.createSpaceBase( self.onSpaceCreateCallBack )
		self.spaceItems[ spaceNumber ] = spaceItem
		return spaceItem
		
	def onSpaceCreateCallBack( self, spaceItem, spaceBase ):
		"""
		virtual method
		space base创建完成
		"""
		pass
		
	def getSpaceItem( self, spaceNumber ):
		"""
		根据space编号获取一个SpaceItem
		"""
		return self.spaceItems.get( spaceNumber, None )
		
	def findSpaceItem( self, pickArgs ):
		"""
		virtual method
		查找一个空间
		"""
		spaceKey = pickArgs["spaceKey"]
		belongType = pickArgs["belongType"]
		for spaceItem in self.spaceItems.values():
			if spaceItem.spaceKey == spaceKey and spaceItem.belongType == belongType:
				return spaceItem
		return None
		
	def onSpaceGetCell( self, spaceNumber ):
		"""
		space cell创建完成（由空间管理器通知）
		"""
		self.spaceItems[ spaceNumber ].onGetCell()
	
	def onSpaceLoseCell( self, spaceNumber ):
		"""
		space cell销毁通知（由空间管理器通知）
		"""
		self.spaceItems[ spaceNumber ].onLoseCell()
		self.spaceItems.pop( spaceNumber ) #一个spaceItem对应一个spaceEntity,spaceEntity销毁了spaceItem也需删除
		
#------------------------------外部调用接口-----------------------------------------------------
	def roleLogin( self, roleMB, pickArgs  ):
		"""
		virtual method
		玩家登陆
		"""
		spaceItem = self.findSpaceItem( pickArgs )
		if spaceItem:
			spaceItem.roleLogin( roleMB, pickArgs )
	
	def gotoSpace( self, roleMB, position, direction, pickArgs ):
		"""
		virtual method
		玩家传送
		"""
		spaceKey = pickArgs["spaceKey"]
		belongType = pickArgs["belongType"]
		spaceItem = self.findSpaceItem( pickArgs )
		if spaceItem:
			spaceItem.gotoSpace(  roleMB, position, direction, pickArgs )