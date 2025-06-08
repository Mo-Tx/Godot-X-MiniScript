#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "MiniscriptInterpreter.h"
#include <godot_cpp/classes/object.hpp>
#include <map>

namespace godot{

class Interpreter : public Object{
    GDCLASS(Interpreter, Object)

private:
    MiniScript::Interpreter interpreter;

    String source_code;

    static Callable* standard_output;
    static Callable* implicit_output;
    static Callable* error_output;

    enum class OutputType{
        STANDARD,
        IMPLICIT,
        ERROR
    };

protected:
	static void _bind_methods();

public:
    Interpreter();
    ~Interpreter();

    //inline static std::map<std::string, Callable> bound_callables;

    void set_source_code(const String &sourcecode = String());
    String get_source_code() const;

    void run_until_done(int timelimit=60); //runs current source code
    void repl(const String &code, int timelimit=60); //REPL

    Variant get_global_value(const String &name);
    void set_global_value(const String &name, const Variant &value);

    static void set_standard_output(const Callable &callable);
    static Callable get_standard_output();

    static void set_implicit_output(const Callable &callable);
    static Callable get_implicit_output();

    static void set_error_output(const Callable &callable);
    static Callable get_error_output();

    static void output(OutputType type, const String &output, bool addLineBreak = true);
};
    

}

#endif