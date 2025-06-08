#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/array.hpp>
#include "MiniscriptTypes.h"
#include "converter.h"


MiniScript::Value Converter::variant_to_value(const godot::Variant &var) {
    switch (var.get_type()) {
        case godot::Variant::Type::STRING:
            return MiniScript::Value(var.stringify().utf8().get_data());
        case godot::Variant::Type::FLOAT:
        case godot::Variant::Type::INT:
            return MiniScript::Value(double(var));
        case godot::Variant::Type::ARRAY: {
            MiniScript::ValueList ms_list;
            godot::Array arr = var;
            for (int i = 0; i < arr.size(); ++i) {
                ms_list.Add(variant_to_value(arr[i]));
            }
            return MiniScript::Value(ms_list);
        }
        default:
            return MiniScript::Value();
    }
}

godot::Variant Converter::value_to_variant(MiniScript::Value val) {
    switch (val.type) {
        case MiniScript::ValueType::String:
            return godot::String(val.ToString().c_str());
        case MiniScript::ValueType::Number:
            return val.DoubleValue();
        case MiniScript::ValueType::List: {
            godot::Array arr;
            const MiniScript::ValueList &ms_list = val.GetList();
            for (int i = 0; i < ms_list.Count(); ++i) {
                arr.append(value_to_variant(ms_list.Item(i)));
            }
            return arr;
        }
        default:
            return godot::Variant();
    }
}