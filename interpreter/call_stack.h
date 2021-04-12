#pragma once

#include <unordered_map>
#include <iostream>

#include "types.h"

namespace mango {

struct Frame {
    std::unordered_map<std::string, Object*> variables;
    std::string function_name;
    Frame(std::string fn) : function_name(fn) {}
};

class CallStack {
    std::vector<Frame> frames;
    int current = 0;

public:
    CallStack();
    void new_frame(std::string function_name);
    void pop_frame();
    Object* set_variable(std::string id, Object* v);
    Object* lookup_variable(std::string id);
};

}