#include "interpreter.h"

namespace mango::interpreter {

Object* Interpreter::lookup_variable(std::string id) {
    return frame_stack.lookup_variable(id);
}

Object* Interpreter::set_variable(std::string id, Object* v) {
    return frame_stack.set_variable(id, v);
}

void Interpreter::new_frame(std::string function_name) {
    return frame_stack.new_frame(function_name);
}

void Interpreter::pop_frame() {
    return frame_stack.pop_frame();
}

}
