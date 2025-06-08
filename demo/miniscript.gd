extends Node2D

var interp := Interpreter.new()

func _ready() -> void:
	
	Interpreter.set_standard_output(test_output)
	Interpreter.set_implicit_output(test_output)
	
	interp.source_code = """
	//demo 1: variable integration!
	globals.time_passed = 0.0
	_process = function(delta)
		globals.time_passed += delta
		print "time passed: " + globals.time_passed
	end function
	
	//demo 2; function integration/custom functions!
	foo = function
		print call("Node2D", "compare_strings", ["foo", "bar"]) //call function: (node path relative to root[main in this case], method name, list of arguments); enjoy!
		print call("Node2D", "compare_strings", ["baz", "baz"]) //the scene root is called main and it has a child called Node2D
	end function
	"""
	
	interp.run_until_done(60) ## run source code
	
	interp.repl("1+1", 1) ##REPL and implicit output testing
	
	interp.repl("foo()", 1) #excecute the foo function
	
	##error handling test
	interp.source_code = """
	cal
	call()
	call("", "a", ["da"])
	"""
	interp.run_until_done(60)

func _process(delta: float) -> void: pass
		
	
func compare_strings(str1: String, str2: String): 
	if str1==str2: return("strings are identical!")
	else: return("nah...")
	
func test_output(message: String):
	print("Ipsum INTERPRETER: " + message)
