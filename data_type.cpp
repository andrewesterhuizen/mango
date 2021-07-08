#include "data_type.h"

#include <unordered_map>
#include <iostream>

auto data_type_string_lookup = std::unordered_map<DataType, std::string>{
        {DataType::Undefined, "undefined"},
        {DataType::String,    "string"},
        {DataType::Integer,   "integer"},
        {DataType::Function,  "function"},
        {DataType::Bool,      "bool"},
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