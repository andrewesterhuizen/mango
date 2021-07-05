#include "types.h"

#include <unordered_map>
#include <iostream>

namespace mango {

std::ostream &operator<<(std::ostream &os, Object &o) {
    os << o.to_string();
    return os;
}

}