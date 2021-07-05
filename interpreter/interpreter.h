#pragma once

#include "types.h"
#include "call_stack.h"

namespace mango::interpreter {

class Interpreter {
    FrameStack frame_stack;

public:
    Object* lookup_variable(std::string id);
    Object* set_variable(std::string id, Object* v);
    void new_frame(std::string function_name);
    void pop_frame();
};

}