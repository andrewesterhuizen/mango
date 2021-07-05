#include "interpreter.h"

namespace mango {

Object* Interpreter::lookup_variable(std::string id) {
    return call_stack.lookup_variable(id);
}

Object* Interpreter::set_variable(std::string id, Object* v) {
    return call_stack.set_variable(id, v);
}

void Interpreter::new_frame(std::string function_name) {
    return call_stack.new_frame(function_name);
}

void Interpreter::pop_frame() {
    return call_stack.pop_frame();
}

}
