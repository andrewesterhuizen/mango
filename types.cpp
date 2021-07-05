#include "types.h"

#include <unordered_map>
#include <iostream>

namespace mango {

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

std::ostream &operator<<(std::ostream &os, Object &o) {
    os << o.to_string();
    return os;
}

}