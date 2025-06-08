#ifndef CONVERTER_H
#define CONVERTER_H

#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/array.hpp>
#include "MiniscriptTypes.h"

namespace Converter{
// Standalone functions to convert between godot::Variant and MiniScript::Value
MiniScript::Value variant_to_value(const godot::Variant &var);
godot::Variant value_to_variant(MiniScript::Value val);
} // namespace Converter


#endif // CONVERTER_H