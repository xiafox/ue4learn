# -*- coding: utf-8 -*-

GLOBAL_BASEDATA_FIRST_START_BASEAPP = "GBAE_FIRST_START_BASE_APP"

#服务器启动状态
WaitInit = 0			#等待初始化
FirstStageMgrInit = 1	#第一阶段管理器初始化：即引擎、工具、登陆相关管理器化
SecondStageMgrInit = 2	#第二阶段管理器初始化：即需要被别的管理器依赖的管理器
ThirdStageMgrInit = 3	#第三阶段管理器初始化：即所有剩下的继承MgrEntity的管理器
CompleteInit = 4		#初始化完毕

#服务器启动需要创建好的地图
NEED_INIT_COMPLETED_SPACE = ["L_TestMap"]

#玩家account entity状态
ACCOUNT_INITIAL_STATE					= 0		# 初始状态
ACCOUNT_LOGIN_SUCCESSED					= 1		# 账号已登陆状态