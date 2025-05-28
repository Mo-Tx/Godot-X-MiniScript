#ifndef MINISCRIPTGD_H
#define MINISCRIPTGD_H

#include <godot_cpp/classes/Node.hpp>
#include "MiniscriptInterpreter.h"

namespace godot {

class MiniscriptGD : public Node {
	GDCLASS(MiniscriptGD, Node)

private:
	MiniScript::Interpreter* interpreter;

protected:
	static void _bind_methods();

public:
	MiniscriptGD();
	~MiniscriptGD();

	void run(String code);
};

}

#endif