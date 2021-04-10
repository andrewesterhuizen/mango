#include "value.h"

namespace mango {

bool Value::is_truthy() {
    switch (type) {
        case DataType::Undefined:return false;
        case DataType::Integer:return std::get<int>(value) != 0;
        case DataType::Bool:return std::get<bool>(value);
        case DataType::String:
        case DataType::Function:
        case DataType::Object:
        case DataType::Array:return true;
    }

    std::cerr << "\nunknown value to check truthyness\n";
    assert(false);
}

std::string Value::to_string() {
    switch (type) {
        case DataType::Undefined:return "undefined";
        case DataType::Integer:return std::to_string(std::get<int>(value));
        case DataType::String:return "\"" + std::get<std::string>(value) + "\"";
        case DataType::Bool:return (std::get<bool>(value) ? "true" : "false");
        case DataType::Function:return "function";
        case DataType::Array: {
            std::string out = "[";

            auto obj = std::get<Array>(value);

            for (auto el : obj.elements) {
                out += el.to_string() + ", ";
            }

            out += "]";
            return out;
        }
        case DataType::Object: {
            std::string out = "{\n";

            auto obj = std::get<Object>(value);

            for (auto prop : obj.properties) {
                out += "  " + prop.first + ": " + prop.second.to_string() + ",\n";
            }

            out += "}";
            return out;
        }
    }

    std::cerr << "\nunknown value to convert to string\n";
    assert(false);
}

std::ostream &operator<<(std::ostream &os, Value &v) {
    os << v.to_string();
    return os;
}

}