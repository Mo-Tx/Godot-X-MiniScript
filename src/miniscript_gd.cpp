#include "miniscript_gd.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "MiniscriptInterpreter.h"
#include <iostream>

using namespace godot;

void MiniScriptGD::_bind_methods() {
	ClassDB::bind_method(D_METHOD("run", "code"), &MiniScriptGD::run);
}

MiniScriptGD::MiniScriptGD() {
	// Initialize any variables here.
	interpreter = new MiniScript::Interpreter(); // This initializes the interpreter without any source code.
}

MiniScriptGD::~MiniScriptGD() {
	delete interpreter;
}

void MiniScriptGD::run(const String code) {
	delete interpreter; // Clean up any previous interpreter instance.
	interpreter = new MiniScript::Interpreter(MiniScript::String(code.utf8().get_data()));

	interpreter->standardOutput = [](MiniScript::String output, bool addLineBreak) {
		UtilityFunctions::print("MiniScipt output: " + String(output.c_str()));
    };
    interpreter->errorOutput = [](MiniScript::String error, bool addLineBreak) {
        UtilityFunctions::printerr("MiniScipt error: " + String(error.c_str()));
    };

	interpreter->Compile();
	interpreter->RunUntilDone();
}
