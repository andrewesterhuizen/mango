#pragma once

#include <unordered_map>
#include <iostream>

#include "value.h"

namespace mango {

using Scope = std::unordered_map<std::string, Value>;

class ScopeStack {
    std::vector<Scope> scopes;
    int current = 0;

public:
    ScopeStack();
    void new_frame();
    void pop_frame();
    Value set_variable(std::string id, Value v);
    Value lookup_variable(std::string id);
};

}