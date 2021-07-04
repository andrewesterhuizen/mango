#include "ast.h"

#include <unordered_map>

namespace mango {

auto operator_string_lookup = std::unordered_map<Operator, std::string>{
        {Operator::Plus,                 "+"},
        {Operator::Minus,                "-"},
        {Operator::Multiply,             "*"},
        {Operator::Divide,               "/"},
        {Operator::EqualTo,              "=="},
        {Operator::NotEqualTo,           "!="},
        {Operator::Not,                  "!"},
        {Operator::LessThan,             "<"},
        {Operator::LessThanOrEqualTo,    "<="},
        {Operator::GreaterThan,          ">"},
        {Operator::GreaterThanOrEqualTo, ">="},
};

std::string operator_to_string(Operator op) {
    if (operator_string_lookup.find(op) != operator_string_lookup.end()) {
        return operator_string_lookup[op];
    }

    std::cerr << "no string defined for operator\n";
    assert(false);
}

std::ostream &operator<<(std::ostream &os, const Operator &op) {
    os << operator_to_string(op);
    return os;
}

auto data_type_string_lookup = std::unordered_map<DataType, std::string>{
        {DataType::Undefined, "undefined"},
        {DataType::String,    "string"},
        {DataType::Integer,   "integer"},
        {DataType::Function,  "function"},
};

std::string data_type_to_string(const DataType dt) {
    if (data_type_string_lookup.find(dt) != data_type_string_lookup.end()) {
        return data_type_string_lookup[dt];
    }

    std::cerr << "no string defined for data type\n";
    assert(false);
}

std::ostream &operator<<(std::ostream &os, const DataType &dt) {
    os << data_type_to_string(dt);
    return os;
}

}