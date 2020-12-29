# -*- coding: utf-8 -*-
import KBEngine
import KBEDebug
import Functor

class StartUpStateBase:
	"""
	服务器游戏启动状态基类
	"""
	def __init__(self):
		"""
		"""
		self.waitList = []
		self.initMgr = {}
		
	def onEnter( self ):
		"""
		虚函数子类可重写，重写后需调用父类方法
		"""
		KBEDebug.INFO_MSG("Start-up State Enter %s" %(self.__class__.__name__))
		self.__createMgrEntity()
		self.__checkIsCompleted()
	
	def __createMgrEntity( self ):
		for key, value in self.initMgr.items():
			if key in self.waitList:
				KBEDebug.ERROR_MSG("%s has exists" %(key))
				continue
			else:
				self.waitList.append(key)
			
			if value[0]:
				self.createMgrFromDB(key)
			else:
				self.createLocally(key, value[1])
	
	def doComplete( self ):
		"""
		状态完成，虚函数，子类可重写
		"""
		pass
	
	def __checkIsCompleted( self ):
		"""
		检测状态是否完成
		"""
		if len(self.waitList) <= 0:
			self.doComplete()
			KBEDebug.INFO_MSG("%s state is init completed." %(self.__class__.__name__))
	
	def createLocally(self, mgrName, params):
		KBEngine.createEntityLocally(mgrName, params)
		
	def createMgrFromDB( mgrName ):
		"""
		从数据库创建管理器
		"""
		mgrTableName = "tbl_%s"%mgrName
		strSql = "SELECT `id` FROM %s WHERE `sm_mgrName`='%s'"%( mgrTableName, mgrName )
		KBEngine.executeRawDatabaseCommand( strSql, Functor.Functor( __onGetMgrDBID, mgrName ) )

	def __onGetMgrDBID( mgrName, result, rows, insertid, errstr ):
		if errstr != None:
			ERROR_MSG( "Create Manager:%s Error:%s"%( mgrName, errstr ) )
			return

		if result and len( result ):
			mgrDBID = int( result[ 0 ][ 0 ] )
			if mgrDBID > 0:
				KBEngine.createEntityFromDBID( mgrName, mgrDBID )
		else:
			mgr = KBEngine.createEntityLocally( mgrName, { "mgrName" : mgrName } )
			mgr.writeToDB()
	
#------------------------------外部调用接口-----------------------------------------------------
	def onMgrInitCompleted(self, mgrName):
		"""
		管理器初始化完成，由ServerGameStartUpMgr通知过来的
		"""
		if mgrName not in self.waitList:
			KBEDebug.ERROR_MSG("not found mgr(%s)" %(mgrName))
			return
		
		self.waitList.remove(mgrName)
		self.__checkIsCompleted()