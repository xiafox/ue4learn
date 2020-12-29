# -*- coding: utf-8 -*-

import KBEDebug

class TestInterface:
	""""""
	def Client_testInt(self):
		int8_num = 8
		int16_num = 16
		int32_num = 32
		int64_num = 64
		self.client.CLIENT_testInt(int8_num, int16_num, int32_num, int64_num)
		
	def Client_testUInt(self):
		uint8_num = 8
		uint16_num = 16
		uint32_num = 32
		uint64_num = 64
		self.client.CLIENT_testUInt(uint8_num, uint16_num, uint32_num, uint64_num)
		
	def Client_testFloat(self):
		self.client.CLIENT_testFloat(0.2, 1.3, 2.0)
		
	def Client_testStr(self):
		self.client.CLIENT_testStr("abc", "测试" )
		
	def Client_testListInt(self):
		self.client.CLIENT_testListInt([4,5,6,7,8,9])
		
	def Client_testListUInt(self):
		self.client.CLIENT_testListUInt([16,16,16,16,16,16])
		
	def Client_testListStr(self):
		self.client.Client_testListStr(["abc","123","测试"])
		
	def Client_testDict(self):
		self.client.Client_testDict({ "infos" : [{"dbid":12345, "playerName":"哈哈"}, {"dbid":6789, "playerName":"嘻嘻"}]})
		
	# ------------------------------------模型测试--------------------------------------------------------------------------------
	def CELL_SetModelPart( self, srcEntityID, modelPart, modelID ):
		"""
		exposed method
		设置模型部位
		"""
		self.setModelPart( modelPart, modelID )
		
	def CELL_SetModel( self, srcEntityID ):
		"""
		exposed method
		设置模型部位
		"""
		self.setModel( {17:"SM_Long_003", 4:"SK_M_F002", 5:"SK_M_H101", 6:"SK_M_T101", 8:"SK_M_C101" } )
		
	def CELL_SetModelExt( self, srcEntityID, isAdd ):
		"""
		exposed method
		设置模型部位
		"""
		if isAdd:
			self.setModelExt( {4:"test"} )
		else:
			self.setModelExt({})
	# ------------------------------------GM指令测试--------------------------------------------------------------------------------
	def CELL_SetIntAttr( self, srcEntityID, attrName, attrValue ):
		"""
		exposed method
		GM指令设置整型属性
		"""
		if hasattr( self, attrName ):
			setattr( self, attrName, attrValue )
		else:
			KBEDebug.ERROR_MSG("GM set attr err!!can not find attr(%s)"%attrName)
			
	def CELL_SetStrAttr( self, srcEntityID, attrName, attrValue ):
		"""
		exposed method
		GM指令设置字符串属性
		"""
		if hasattr( self, attrName ):
			setattr( self, attrName, attrValue )
		else:
			KBEDebug.ERROR_MSG("GM set attr err!!can not find attr(%s)"%attrName)