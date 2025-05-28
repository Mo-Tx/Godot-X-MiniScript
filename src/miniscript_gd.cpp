#include "miniscript_gd.h"
#include <godot_cpp/core/class_db.hpp>
#include "MiniscriptInterpreter.h"
#include <iostream>

using namespace godot;

void MiniScriptGD::_bind_methods() {
}

MiniScriptGD::MiniScriptGD() {
	// Initialize any variables here.
	interpreter = new MiniScript::Interpreter(); // This initializes the interpreter without any source code.
}

MiniScriptGD::~MiniScriptGD() {
	delete interpreter;
}

void MiniScriptGD::run(String code) {
	delete interpreter; // Clean up any previous interpreter instance.
	interpreter = new MiniScript::Interpreter(MiniScript::String(code.utf8().get_data()));
	interpreter->Compile();
	interpreter->RunUntilDone();
	print_line("Script executed successfully.");
}
