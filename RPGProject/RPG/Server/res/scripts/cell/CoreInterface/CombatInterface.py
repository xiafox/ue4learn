# -*- coding: utf-8 -*-



class CombatInterface:
	""""""
	def __init__( self ):
		""""""
		pass
		
	def setHP( self, HP ):
		"""设置HP"""
		oldHP = self.HP
		newHP = min(self.HP + HP, self.HP_max)
		if oldHP != newHP:
			self.HP = newHP
			self.onHPChange( oldHP, newHP )
		
	def onHPChange( self, oldHP, newHP ):
		"""HP改变"""
		pass
		
	def setMP( self, MP ):
		"""设置MP"""
		oldMP = self.MP
		newMP = min(self.MP + MP, self.MP_max)
		if oldMP != newMP:
			self.MP = newMP
			self.onMPChange( oldMP, newMP )
		
	def onMPChange( self, oldMP, newMP ):
		"""MP改变"""
		pass
