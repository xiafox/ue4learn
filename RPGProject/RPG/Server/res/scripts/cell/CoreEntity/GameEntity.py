# -*- coding: utf-8 -*-

"""
游戏层公共方法、属性
"""
import Const
import CoreEntity.ServerEntity as ServerEntity
import RPGConst
import kbemain
import KBEngine

class GameEntity( ServerEntity.ServerEntity ):
	def __init__( self ):
		ServerEntity.ServerEntity.__init__( self )
		
	def destroyCellByBase( self ):
		"""
		<define method>
		base请求销毁cell
		"""
		self.destroySelf()
		
	def destroySelf( self ):
		"""
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
		
	def onDestroy( self ):
		"""
		这个函数在调用Entity.destroy()后，在实际销毁之前被调用
		"""
		if not self.isDestroy:	#被动销毁（掉线）的情况下，不会走destroySelf流程，这里要另外调用一下
			self.isDestroy = True
			self.beforeDestroy()
		
	def getScript( self ):
		"""
		virtual method
		"""
		return None
		
	def onSpaceGone( self ):
		"""
		引擎回调
		副本销毁
		"""
		self.destroySelf()
		
#----------------------space--------------------------------------------------------------	
	def getCurrentSpaceData( self, key ):
		return KBEngine.getSpaceData( self.spaceID, key )
	
	def setCurrentSpaceData( self, key, value ):
		KBEngine.setSpaceData( self.spaceID, key, value )
		
	def getCurrentSpace( self ):
		"""
		获得当前空间实体的mailbox
		"""
		return KBEngine.cellAppData.get( Const.GET_SPACE_CELLAPP_KEY( self.spaceID ), None )
		
	def getCurrentSpaceEntity( self ):
		"""
		获得当前空间实体
		"""
		spaceBase = self.getCurrentSpace()
		if spaceBase:
			return KBEngine.entities.get( spaceBase.id, None )
		else:
			return None
		
	def getCurrentSpaceScriptID( self ):
		spaceEntity = self.getCurrentSpaceEntity()
		if spaceEntity:
			return spaceEntity.scriptID
		return None
		
	def getCurrentSpaceObject( self ):
		return kbemain.g_objScriptFactory.getSpaceObject( self.getCurrentSpaceScriptID() )
		
	#----------------------------------------model---------------------------------------------------------------
	def setModel( self, partDict ):
		"""设置模型"""
		for part in partDict.keys():
			assert part in RPGConst.MODEL_PARTS, "Model part(%d) must be define!!"%part
		self.modelParts = partDict
	
	def setModelPart( self, part, modelID ):
		"""设置模型部位"""
		assert part in RPGConst.MODEL_PARTS, "Model part(%d) must be define!!"%part
		if modelID:
			self.modelParts[part] = modelID
		else:
			self.modelParts.pop(part)
		self.allClients.CLIENT_OnSetModelPart( part, modelID )
		
	def setModelExt( self, partDict ):
		"""设置变身模型"""
		for part in partDict.keys():
			assert part in RPGConst.MODEL_PARTS, "Model part(%d) must be define!!"%part
		self.modelExtParts = partDict
		
	def setModelScale( self, scale ):
		"""设置模型大小"""
		if scale != self.modelScale:
			self.modelScale = scale
			
	
