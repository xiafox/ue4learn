# -*- coding: utf-8 -*-


import RPGConst

class StateInterface:
	""""""
	def __init__( self ):
		""""""
		self.effectState = 0
		self.effectCounter = [0] * len( RPGConst.EFFECT_STATE_LIST )
		
		self.actForbid = 0
		self.actCounter = [0] * len( RPGConst.ACTION_FLAG_LIST )
		self.actCounterInc( RPGConst.STATE_TO_ACTION_FLAGS[self.state] )		# 恢复存盘过的状态计数
		
	def changeState( self, newState = RPGConst.ENTITY_STATE_FREE ):
		"""
		改变状态
		"""
		if self.state != newState:
			oldState = self.state
			self.changeStateBefore( newState, oldState )
			self.state = newState
			self.changeStateOver( newState, oldState )
			
	def changeStateBefore( self, newState, oldState ):
		""""""
		self.actCounterDec( RPGConst.STATE_TO_ACTION_FLAGS[oldState] )
		
	def changeStateOver( self, newState, oldState ):
		""""""
		self.actCounterInc( RPGConst.STATE_TO_ACTION_FLAGS[newState] )
	
	#---------------------------------------------------------------------------------------------------------------------
	# 效果状态
	#---------------------------------------------------------------------------------------------------------------------
	def effectStateInc( self, estate ):
		"""
		<Define method>
		添加一个效果状态到记数器
		"""
		for i, es in enumerate( RPGConst.EFFECT_STATE_LIST ):
			if estate & es:
				self.effectCounter[i] += 1		# Counter不得大于255
				if self.effectCounter[i] == 1:
					self.effectState |=  estate
					self.actCounterInc( RPGConst.EFFECT_STATE_TO_ACTION_FLAGS[estate] )
					self.onAddEffectState( estate )
				
	def effectStateDec( self, estate ):
		"""
		<Define method>
		删除一个效果状态到记数器
		"""
		for i, es in enumerate( RPGConst.EFFECT_STATE_LIST ):
			if estate & es:
				self.effectCounter[i] -= 1
				if self.effectCounter[i] == 0:
					self.effectState &= ~estate
					self.actCounterDec( RPGConst.EFFECT_STATE_TO_ACTION_FLAGS[estate] )
					self.onRemoveEffectState( estate )
					
	def onAddEffectState( self, estate ):
		"""
		增加效果状态回调
		@param estate		:	效果标识(非组合)
		@type estate		:	integer
		@param disabled		:	效果是否生效
		@param disabled		:	bool
		"""
		pass
		
	def onRemoveEffectState( self, estate ):
		"""
		删除效果状态回调
		@param estate		:	效果标识(非组合)
		@type estate		:	integer
		@param disabled		:	效果是否生效
		@param disabled		:	bool
		"""
		pass
	
	def hasEffectState( self, effectState ):
		"""
		是否存在效果状态。
			@return	:	True or False
			@rtype	:	bool
		"""
		return self.effectState & effectState != 0
	
	#---------------------------------------------------------------------------------------------------------------------
	# 行为状态
	#---------------------------------------------------------------------------------------------------------------------
	def actCounterInc( self, stateWord ):
		"""
		动作计数器加一，并维护动作限制。
		
		@param stateWord	:	动作状态字
		@type stateWord		:	integer
		"""
		actCounter = [0] * len( RPGConst.ACTION_FLAG_LIST )
		for i, act in enumerate( RPGConst.ACTION_FLAG_LIST ):
			if stateWord & act:
				self.actCounter[i] += 1		# Counter不得大于255
				if self.actCounter[i] == 1:
					self.actForbid |= act
					actCounter[i] += 1
		self.onAddActCounter( actCounter )

	def actCounterDec( self, stateWord ):
		"""
		动作计数器减一，并维护动作限制。
		
		@param stateWord	:	动作状态字
		@type stateWord		:	integer
		"""
		actCounter = [0] * len( RPGConst.ACTION_FLAG_LIST )
		for i, act in enumerate( RPGConst.ACTION_FLAG_LIST ):
			if stateWord & act:
				if self.actCounter[i] - 1 >= 0:
					self.actCounter[i] -= 1
					if self.actCounter[i] == 0:
						self.actForbid &= ~act
						actCounter[i] += 1
		self.onRemoveActCounter( actCounter )

	def actionForbidSign( self, signWord ):
		"""
		是否存在标记。
			@return	:	标记字
			@rtype	:	bool
		"""
		return self.actForbid & signWord != 0

	def onAddActCounter( self, stateWord ):
		"""添加动作状态计数回调"""
		pass
	
	def onRemoveActCounter( self, stateWord ):
		"""删除动作状态计数回调"""
		pass