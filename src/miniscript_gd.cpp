#include "miniscript_gd.h"
#include <godot_cpp/core/class_db.hpp>
#include "MiniScriptInterpreter.h"

using namespace godot;

void MiniScriptGD::_bind_methods() {
}

MiniScriptGD::MiniscriptGD() {
	// Initialize any variables here.
	interpreter = MiniScript::Interpreter(); // This initializes the interpreter without any source code.
}

MiniScriptGD::~MiniscriptGD() {
	delete interpreter;
}

MiniScriptGD

void run(String code) {
	interpreter = MiniScript::Interpreter(script);
	interpreter->Compile();
	interpreter->RunUntilDone();
	print_line("Script executed successfully.");
}
