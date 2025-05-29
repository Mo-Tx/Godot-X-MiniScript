#ifndef MINISCRIPTGD_H
#define MINISCRIPTGD_H

#include <godot_cpp/classes/node.hpp>
#include "MiniscriptInterpreter.h"

namespace godot {

class MiniScriptGD : public Node {
	GDCLASS(MiniScriptGD, Node)

private:
	MiniScript::Interpreter* interpreter;

protected:
	static void _bind_methods();

public:
	MiniScriptGD();
	~MiniScriptGD();

	void run(const String code);
};

}

#endif