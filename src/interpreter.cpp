#include "interpreter.h"
#include "converter.h"
#include "MiniscriptInterpreter.h"
#undef Error // Avoid conflict with Godot's Error enum
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>



using namespace godot;

Callable* Interpreter::standard_output = nullptr;
Callable* Interpreter::implicit_output = nullptr;
Callable* Interpreter::error_output = nullptr;

void Interpreter::_bind_methods(){
    ClassDB::bind_method(D_METHOD("set_source_code", "source_code"), &Interpreter::set_source_code);
    ClassDB::bind_method(D_METHOD("get_source_code"), &Interpreter::get_source_code);

    ClassDB::bind_method(D_METHOD("run_until_done", "time_limit"), &Interpreter::run_until_done);
    ClassDB::bind_method(D_METHOD("repl", "code", "time_limit"), &Interpreter::repl);

    ClassDB::bind_method(D_METHOD("get_global_value", "name"), &Interpreter::get_global_value);
    ClassDB::bind_method(D_METHOD("set_global_value", "name", "value"), &Interpreter::set_global_value);

    ClassDB::bind_static_method("Interpreter", D_METHOD("get_standard_output"), &Interpreter::get_standard_output);
    ClassDB::bind_static_method("Interpreter", D_METHOD("set_standard_output", "callable"), &Interpreter::set_standard_output);

    ClassDB::bind_static_method("Interpreter", D_METHOD("get_implicit_output"), &Interpreter::get_implicit_output);
    ClassDB::bind_static_method("Interpreter", D_METHOD("set_implicit_output", "callable"), &Interpreter::set_implicit_output);

    ClassDB::bind_static_method("Interpreter", D_METHOD("get_error_output"), &Interpreter::get_error_output);
    ClassDB::bind_static_method("Interpreter", D_METHOD("set_error_output", "callable"), &Interpreter::set_error_output);



    
    
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "source_code"), "set_source_code", "get_source_code"); 
}

Interpreter::Interpreter() : source_code(), interpreter() {

    MiniScript::Intrinsic* f = MiniScript::Intrinsic::Create("call");
    f->AddParam("path", "");
    f->AddParam("name", "");
    f->AddParam("arguments", MiniScript::Value(MiniScript::ValueList()));
    
    f->code = [](MiniScript::Context* context, MiniScript::IntrinsicResult partialResult) -> MiniScript::IntrinsicResult {

        godot::Array args = Converter::value_to_variant(context->GetVar("arguments"));
        SceneTree *tree = dynamic_cast<SceneTree *>(Engine::get_singleton()->get_main_loop());
        godot::Variant result = Callable(tree->get_current_scene()->get_node<Node>(
            NodePath(Converter::value_to_variant(context->GetVar("path")))),
            Converter::value_to_variant(context->GetVar("name"))).callv(args);
        return MiniScript::IntrinsicResult(Converter::variant_to_value(result), true);
    };
    
    interpreter.standardOutput = [](MiniScript::String output, bool addLineBreak) {
		Interpreter::output(OutputType::STANDARD, String(output.c_str()), addLineBreak);
    };
    interpreter.implicitOutput = [](MiniScript::String output, bool addLineBreak) {
		Interpreter::output(OutputType::IMPLICIT, String(output.c_str()), addLineBreak);
    };
    interpreter.errorOutput = [](MiniScript::String error, bool addLineBreak) {
        Interpreter::output(OutputType::ERROR, String(error.c_str()), addLineBreak);
    };
}


Interpreter::~Interpreter() {}


void Interpreter::set_source_code(const String &sourcecode) {

    source_code = sourcecode;
    interpreter.Reset(MiniScript::String(Converter::variant_to_value(source_code).ToString()));

    interpreter.Compile();

}

String Interpreter::get_source_code() const {return source_code;}

void Interpreter::run_until_done(int timelimit) {interpreter.RunUntilDone(timelimit);}

void Interpreter::repl(const String &code, int timelimit) {interpreter.REPL(Converter::variant_to_value(code).ToString(), timelimit);}

Variant Interpreter::get_global_value(const String &name) {
    return Converter::value_to_variant(interpreter.GetGlobalValue(Converter::variant_to_value(name).ToString()));
}

void Interpreter::set_global_value(const String &name, const Variant &value){
    interpreter.SetGlobalValue(Converter::variant_to_value(name).ToString(), Converter::variant_to_value(value));
}

void Interpreter::set_standard_output(const Callable &callable) {
    delete standard_output;
    standard_output = new Callable(callable);
}

Callable Interpreter::get_standard_output(){
    return *standard_output;
}

void Interpreter::set_implicit_output(const Callable &callable) {
    delete implicit_output;
    implicit_output = new Callable(callable);
}

Callable Interpreter::get_implicit_output(){
    return *implicit_output;
}

void Interpreter::set_error_output(const Callable &callable) {
    delete error_output;
    error_output = new Callable(callable);
}

Callable Interpreter::get_error_output(){
    return *error_output;
}

void Interpreter::output(OutputType type, const String &output, bool addLineBreak) {
    const String result = output + String(addLineBreak ? "\n" : "");

    switch (type) {
        case OutputType::STANDARD:
            if (standard_output != nullptr) {
                standard_output->call(result);
            } else {
                UtilityFunctions::print("Interpreter output: " + result);
            }
            break;
        case OutputType::IMPLICIT:
            if (implicit_output != nullptr) {
                implicit_output->call(result);
            } else {
                UtilityFunctions::print("Interpreter implicit output: " + result);
            }
            break;
        case OutputType::ERROR:
            if (error_output != nullptr) {
                error_output->call(result);
            } else {
                UtilityFunctions::push_error("Interpreter error: " + result);
            }
            break;
        default:
            UtilityFunctions::push_warning("Interpreter unknown output " + result);
            break;
    }
}