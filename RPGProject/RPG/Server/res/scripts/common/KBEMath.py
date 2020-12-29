#UE4和KBE之间坐标及方向转换函数

import math
import Math
"""
1、UE4中，距离的单位是厘米；而在KBE中，距离的单位是米
2、在虚幻 4 中，X 的正方向是“前”，Y 的正方向是“右”，Z 的正方向是“上”
"""
def Unreal2KBEnginePosition(unrealPoint):
	"""
	"""
	return ( unrealPoint[1] * 0.01, unrealPoint[2] * 0.01, unrealPoint[0] * 0.01 )
	
def KBEngine2UnrealPosition(kbenginePoint):
	"""
	"""
	return ( kbenginePoint[2] * 100, kbenginePoint[0] * 100, kbenginePoint[1] * 100 )


"""
1、kbe和UE4中，描述方向的X,Y,Z两者的对应关系是一致的（数值表述是不一致的，kbe用弧度，UE4用角度；后面所讲的θ，在kbe中为弧度，在UE4中为角度，后面不在重复）
2、在游戏的过程中，我们实际所需要同步的只是Z轴（朝向）
"""	
def Unreal2KBEngineDirection(unrealDir):
	"""
	"""
	x = (unrealDir[0] / 360) * (2 * math.pi)
	y = (unrealDir[1] / 360) * (2 * math.pi)
	z = (unrealDir[2] / 360) * (2 * math.pi)
	return Math.Vector3( x, y, z )
	
def KBEngine2UnrealDirection(kbengineDir):
	"""
	"""
	x = (kbengineDir[0] / (2 * math.pi)) * 360
	y = (kbengineDir[1] / (2 * math.pi)) * 360
	z = (kbengineDir[2] / (2 * math.pi)) * 360
	return ( x, y, z )