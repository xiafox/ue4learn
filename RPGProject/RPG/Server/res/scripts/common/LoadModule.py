# -*- coding: utf-8 -*-
import os
import sys
import json
import glob
import hashlib
import RPGConst
FILE_TYPE = ".json"

GET_GAME_CONFIG = lambda dir : RPGConst.CONFIG_PATCH + dir

def openFile( filePath ):
	fileObject = open( filePath, encoding="utf8" )
	fileContent = fileObject.read()
	if fileContent.startswith(u'\ufeff'):
		fileContent = fileContent.encode('utf8')[3:].decode('utf8')
		
	jsFileObj = json.loads( fileContent )
	fileObject.close()
	return jsFileObj

def openJsonPathCfg( configPath ):
	"""
	"""
	fileBbjList = []
	configPath = GET_GAME_CONFIG( configPath )
	if os.path.isdir( configPath ):
		for file in os.listdir( configPath ):
			filePath = configPath + "/" + file
			if os.path.isfile( filePath ):
				jsFileObj = openFile( filePath )
				fileBbjList.append( jsFileObj )
				
	return fileBbjList

def openJsonCfg( filePath ):
	filePath = GET_GAME_CONFIG( filePath )
	if os.path.isfile( filePath ):
		jsFileObj = openFile( filePath )
		
		return jsFileObj
				
	return None

def openJsonPathConfig( configPath ):
	"""
	加载文件夹下的所有json文件，返回{文件名：数据}
	"""
	fileBbjDict = {}
	configPath = GET_GAME_CONFIG( configPath )
	if os.path.isdir( configPath ):
		for file in os.listdir( configPath ):
			filePath = configPath + "/" + file
			if os.path.isfile( filePath ) and os.path.splitext(filePath)[1] == FILE_TYPE:
				jsFileObj = openFile( filePath )
				fileBbjDict[file.rsplit("/")[-1].rsplit(".")[0]] = jsFileObj

	return fileBbjDict
	
def utf16ToJsonCfg( path ):
	allPath = RPGConst.CONFIG_PATCH + path
	fileObject = open( allPath, encoding="utf16" )
	jsFileObj = json.loads( fileObject.read() )
	fileObject.close()
	return jsFileObj