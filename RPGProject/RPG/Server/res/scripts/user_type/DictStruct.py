# -*- coding: utf-8 -*-


class DictBase( dict ):
	#FIXED_DICT dict
	def __init__( self ):
		dict.__init__( self )
	
	@classmethod
	def getDictFromObj( self, obj ):
		datalist = []
		for k, v in obj.items():
			tdict = {}
			tdict[ "key" ] = k
			tdict[ "value" ] = v
			datalist.append( tdict )
			
		return { "dictData" : datalist }
		
#	@classmethod
	def createObjFromDict( self, dict ):
		obj = self.__class__()
		for r in dict[ "dictData" ]:
			obj[ r[ "key" ] ] = r[ "value" ]
			
		return obj
	
	#@classmethod
	def isSameType( self, obj ):
		return isinstance( obj, dict )

class DictStrStr( DictBase ):
	def __init__( self ):
		super( DictStrStr, self ).__init__()
	
class DictIntInt( DictBase ):
	def __init__( self ):
		super( DictIntInt, self ).__init__()

class DictStrInt( DictBase ):
	def __init__( self ):
		super( DictStrInt, self ).__init__()
		
class DictIntStr( DictBase ):
	def __init__( self ):
		super( DictIntStr, self ).__init__()

class DictStrIntList( DictBase ):
	def __init__( self ):
		super( DictStrIntList, self ).__init__()
	
	def addValue( self, key, value ):
		if not self.has_key( key ):
			self[ key ] = []
			
		self[ key ].append( value )
	
	def delValue( self, key, value ):
		if self.has_key( key ):
			if value in self[ key ]:
				self[ key ].remove( value )
	
	def delValues( self, key, remvoeList ):
		"""
		删除列表元素
		"""
		if self.has_key( key ):
			for v in remvoeList:
				if v in self[ key ]:
					self[ key ].remove( v )

					
	
g_dictStrStr = DictStrStr()
g_dictIntInt = DictIntInt()
g_dictStrInt = DictStrInt()
g_dictIntStr = DictIntStr()
g_dictStrIntList = DictStrIntList()
