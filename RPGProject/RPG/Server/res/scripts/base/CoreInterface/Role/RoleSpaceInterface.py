# -*- coding: utf-8 -*-
import KBEngine
import KBEDebug
import SpaceContainer
import csdefine 

class RoleSpaceInterface:
	def __init__( self ):
		self.isCellCreating = False#标识当前cell是否正在创建
		self.birthSpace = self.cellData["spaceScriptID"]
		self.birthSpaceBelong = csdefine.SPACE_BELONG_NORMAL
		
	def loginSpace(self):
		"""
		登陆(当前玩家还没有cell登陆地图时调此方法，所以肯定有cellData)
		"""
		params = SpaceContainer.getPickContainerLoginData( self.birthSpaceBelong, self.birthSpace, self, { "packBelongType":self.birthSpaceBelong } )
		KBEngine.globalData[ "SpaceManager" ].roleLogin( self.birthSpace, self, params )
		
#-------------------------------remote call-------------------------------
	def createCellFromSpace( self, spaceCellMB ):
		"""
		<define method>
		在spaceCell上创建roleCell
		@param spaceCell:	空间cell
		@type spaceCell:	mailbox
		"""
		KBEDebug.DEBUG_MSG("Role[%d:%d] CreateCellFromSpace, space[%d]" %(self.databaseID, self.id, spaceCellMB.id))
		#引擎方法：请求在一个cell里面创建一个关联的实体，这是在baseApp上创建cellEntity
		self.createCellEntity( spaceCellMB ) 
		self.isCellCreating = True
		
#---------------------------------引擎方法---------------------------------
	def onGetCell( self ):
		"""
		cell创建成功
		"""
		self.isCellCreating = False
		
	def onCreateCellFailure( self ):
		"""
		cell创建失败
		"""
		self.isCellCreating = False
		
	def onClientGetCell( self ):
		self.cell.onLoginSpaceSuccess()
		
