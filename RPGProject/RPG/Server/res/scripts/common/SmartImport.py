# -*- coding: utf-8 -*-


from KBEDebug import *

def smartImport( mname ) :
	"""
	导入一个模块。
	@type			mname		: STRING
	@param			mname		: 模块名称。具体格式为"mod1.mod2.modN:className"
								  其中":className"是可选的，表示要导入哪个类，如果存在则只允许有一个，类似于 "from mod import className"
								  也就是说可以直接导入一个模块里指定的类。
	@rtype				 		: object of module or class
	@return				 		: 返回指定的模块
	@raise 			ImportError : 如果指定的模块不存在则产生此异常
	"""
	compons = mname.split( ":" )							# 拆分路径和模块
	assert len( compons ) > 0, "wrong module name!"			# 排除空路径
	mod = __import__( compons[0], fromlist=["*"] )			# import 最后一层模块(fromlist只要非空，就会返回最后一层模块)
															# 一个直观的理解是，调用__import__( 'A.B', fromlist=['c'] )
															# 应该是如同 from A.B import c，在调用的局部作用域里会设置c这个变量，或者__import__
															# 会返回c所指向的目标，但实际上并不是如此。根据python帮助文档，__import__( 'A.B', fromlist=['c'] )
															# 不会返回c所指向的目标，而只会得到A.B这个模块，python文档是这样说的：
															# the __import__() function does not set the local variable named eggs; this is done by
															# subsequent code that is generated for the import statement.
															# 也就是说，设置局部变量c不是在__import__里做的，而是在import方法里才做的，因此想要得到
															# c所指向的目标，只能在获取到A.B模块之后，再从模块中获取c，而只要fromlist参数不为空，
															# __import__就会返回A.B模块（否则返回A模块），所以传入一个非空的fromlist参数就不需要再使用迭代
															# 去逐级获取所需的c目标。
															# (注意：使用默认fromlist的话，只会 import 最顶层，但它会对全路径进行检查，路径不存在会 import 失败)

	if len( compons ) > 1 :									# 需要import模块的类
		try :
			mod = getattr( mod, compons[-1] )
		except AttributeError as err :						# 这里将AttrbuteError转变为了ImportError，如果外部捕捉ImportError，当这里出现了AttrbuteError，外部也会捕捉到异常，但是会失去异常的真实信息
			EXCEHOOK_MSG( err )
			raise ImportError( "module '%s' has no class or attribute '%s'!" % ( compons[0], compons[-1] ) )
	return mod
