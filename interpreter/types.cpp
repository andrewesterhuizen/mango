#include "types.h"

namespace mango {

std::ostream &operator<<(std::ostream &os, Object &o) {
    os << o.to_string();
    return os;
}

}