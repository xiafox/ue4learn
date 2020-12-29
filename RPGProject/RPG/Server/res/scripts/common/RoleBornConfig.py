#-*-coding:utf-8-*-

import LoadModule
import Singleton
import RPGConst

# 玩家出生相关配置



class RoleBornCfg( Singleton.Singleton ):
	def __init__( self ):
		""""""
		self.initRoleModelCfg = {}
		
		self.loadCfg( )
		
	def loadCfg( self ):
		datas = LoadModule.openJsonCfg("InitModelTable.json")
		for professionStr, modelDatas in datas.items():
			self.initRoleModelCfg[professionStr] = {}
			for modelPartStr, modelValue in modelDatas.items():
				modelPart = getattr( RPGConst, modelPartStr, None )
				if modelPart and modelValue:
					self.initRoleModelCfg[professionStr][modelPart] = modelValue
			

	def getRoleBornModel( self, profession ):
		"""获取玩家初始的模型数据"""
		if profession ==1:
			return self.initRoleModelCfg["Warrior"]
		elif profession ==2:
			return self.initRoleModelCfg["Swordsman"]
		elif profession ==3:
			return self.initRoleModelCfg["Shooter"]
		elif profession ==4:
			return self.initRoleModelCfg["Master"]
		else:
			return self.initRoleModelCfg["Default"]
		
		
g_roleBornCfg = RoleBornCfg()