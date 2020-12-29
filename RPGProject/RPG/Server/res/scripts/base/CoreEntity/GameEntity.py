# -*- coding: utf-8 -*-

"""
游戏层公共方法、属性
"""

import CoreEntity.ServerEntity as ServerEntity

class GameEntity( ServerEntity.ServerEntity ):
	def __init__( self ):
		ServerEntity.ServerEntity.__init__( self )
	
	def destroySelf( self ):
		"""
		<Define method>
		virtual method
		销毁entity唯一入口
		"""
		if self.isDestroyed or self.isDestroy:
			return
		self.isDestroy = True
		self.beforeDestroy()
		self.destroy()
		
	def beforeDestroy( self ):
		"""
		vitural method
		做销毁前的清理
		"""
		pass