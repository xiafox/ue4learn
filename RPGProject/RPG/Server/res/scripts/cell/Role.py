# -*- coding: utf-8 -*-


#engine
import KBEngine
import CoreEntity.GameEntity as GameEntity
import CoreInterface.Role.RoleSpaceInterface as RoleSpaceInterface
import CoreInterface.Role.TestInterface as TestInterface
import CoreInterface.StateInterface as StateInterface
import CoreInterface.CombatInterface as CombatInterface
import KBEDebug

class Role(	GameEntity.GameEntity,
			RoleSpaceInterface.RoleSpaceInterface,
			TestInterface.TestInterface,
			StateInterface.StateInterface,
			CombatInterface.CombatInterface,
			):
	def __init__( self ):
		GameEntity.GameEntity.__init__( self )
		RoleSpaceInterface.RoleSpaceInterface.__init__( self )
		StateInterface.StateInterface.__init__( self )
		CombatInterface.CombatInterface.__init__( self )
	
	def beforeDestroy( self ):
		"""
		entity摧毁前处理事情
		"""
		self.onLeaveSpaceNotify( "", {} )
		
#-----------------remote call------------------------------------------
	def CELL_changeRoleJumpState(self, srcEntityID, JumpState, InVelocity, InPos):
		"""
		<Exposed method>
		改变玩家跳跃状态状态
		"""
		if srcEntityID == self.id:
			self.allClients.CLIENT_OnRepJumpState(JumpState, InVelocity, InPos)