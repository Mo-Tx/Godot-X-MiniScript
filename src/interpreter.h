#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "MiniscriptInterpreter.h"
#include <godot_cpp/classes/object.hpp>

namespace godot{

class Interpreter : public Object{
    GDCLASS(Interpreter, Object)

private:
    MiniScript::Interpreter interpreter;

    String source_code;

protected:
	static void _bind_methods();

public:
    Interpreter();
    ~Interpreter();

    void set_source_code(const String &sourcecode = String());
    String get_source_code() const;
    void run(); //runs current source code
    void repl(const String &code); //REPL
};
    

}

#endif