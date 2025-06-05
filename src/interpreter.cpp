#include "interpreter.h"
#include "MiniscriptInterpreter.h"
#undef Error // Avoid conflict with Godot's Error enum
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>



using namespace godot;

void Interpreter::_bind_methods(){
    ClassDB::bind_method(D_METHOD("set_source_code", "source_code"), &Interpreter::set_source_code);
    ClassDB::bind_method(D_METHOD("get_source_code"), &Interpreter::get_source_code);
    ClassDB::bind_method(D_METHOD("run_until_done", "time_limit"), &Interpreter::run_until_done);
    ClassDB::bind_method(D_METHOD("repl", "code", "time_limit"), &Interpreter::repl);
    ClassDB::bind_method(D_METHOD("get_global_value", "name"), &Interpreter::get_global_value);
    ClassDB::bind_method(D_METHOD("set_global_value", "name", "value"), &Interpreter::set_global_value);
    //ClassDB::bind_static_method("Interpreter", D_METHOD("bind_callable", "name", "callable"), &Interpreter::bind_callable);
    
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "source_code"), "set_source_code", "get_source_code"); 
}

Interpreter::Interpreter() : source_code(), interpreter() {
    MiniScript::Intrinsic* f = MiniScript::Intrinsic::Create("call");
    f->AddParam("path", "");
    f->AddParam("name", "");
    f->AddParam("arguments", MiniScript::Value(MiniScript::ValueList()));
    
    f->code = [](MiniScript::Context* context, MiniScript::IntrinsicResult partialResult) -> MiniScript::IntrinsicResult {

        MiniScript::ValueList ms_list = context->GetVar("arguments").GetList();
        Array result = Array();
        for (int i = 0; i < ms_list.Count(); ++i) {
        MiniScript::Value item = ms_list.Item(i);

        // Dumb conversion logic, feel free to extend it if you’ve got fancier types
        switch (item.type) {
            case MiniScript::ValueType::String:
                result.append(String(item.ToString().c_str()));
                break;
            case MiniScript::ValueType::Number:
                result.append(item.DoubleValue());
                break;
            default:
                result.append(Variant()); // or maybe throw a tantrum here
                break;
            }
        }
        SceneTree *tree = dynamic_cast<SceneTree *>(Engine::get_singleton()->get_main_loop());
        Callable(tree->get_current_scene()->get_node<Node>(NodePath(context->GetVar("path").ToString().c_str())), context->GetVar("name").ToString().c_str()).callv(result);
        return MiniScript::IntrinsicResult(MiniScript::Value(), true);
    };
    
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

void Interpreter::run_until_done(int timelimit) {interpreter.RunUntilDone(timelimit);}

void Interpreter::repl(const String &code, int timelimit) {interpreter.REPL(MiniScript::String(code.utf8().get_data()), timelimit);}

Variant Interpreter::get_global_value(const String &name) {
    MiniScript::Value value = interpreter.GetGlobalValue(name.utf8().get_data());
    Variant variant;
    if (value.type == MiniScript::ValueType::String){variant = Variant(String(value.ToString().c_str()));}
    else if (value.type == MiniScript::ValueType::Number) {variant = Variant(value.DoubleValue());}
    // FYI MiniScript has no bools
    else variant = Variant(); 

    return variant;
}

void Interpreter::set_global_value(const String &name, const Variant &value){} //TBD


//void Interpreter::bind_callable(const String &name, const Callable &callable) {
    
//    bound_callables[name.utf8().get_data()] = callable;

///}
// this is uselesss now but im keeping it in case i mess up and need to recero

static void match_type(Variant value) {} //todo
