#include "interpreter.h"
#include "MiniscriptInterpreter.h"
#undef Error // Avoid conflict with Godot's Error enum
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void Interpreter::_bind_methods(){
    ClassDB::bind_method(D_METHOD("set_source_code"), &Interpreter::set_source_code);
    ClassDB::bind_method(D_METHOD("get_source_code"), &Interpreter::get_source_code);
    ClassDB::bind_method(D_METHOD("run"), &Interpreter::run);
    ClassDB::bind_method(D_METHOD("repl", "code"), &Interpreter::repl);
    
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "source_code"), "set_source_code", "get_source_code"); 
}

Interpreter::Interpreter() : source_code(), interpreter() {
    interpreter.standardOutput = [](MiniScript::String output, bool addLineBreak) {
		UtilityFunctions::print("Interpreter output: " + String(output.c_str()));
    };
    interpreter.errorOutput = [](MiniScript::String error, bool addLineBreak) {
        UtilityFunctions::printerr("interpreter error: " + String(error.c_str()));
    };
}


Interpreter::~Interpreter() {}


void Interpreter::set_source_code(const String &sourcecode) {

    source_code = sourcecode;
    interpreter.Reset(MiniScript::String(source_code.utf8().get_data()));

    interpreter.Compile();

}

String Interpreter::get_source_code() const {return source_code;}

void Interpreter::run() {interpreter.RunUntilDone();}

void Interpreter::repl(const String &code) {interpreter.REPL(MiniScript::String(code.utf8().get_data()));}
