extends Node2D

var interp := Interpreter.new()

func _ready() -> void:
	
	interp.source_code = """
	time_passed = 0.0
	_process = function(delta)
		globals.time_passed += delta
		print "time passed: " + globals.time_passed
	end function
	"""
	
	interp.run()

func _process(delta: float) -> void: interp.repl("_process("+str(delta)+")")
