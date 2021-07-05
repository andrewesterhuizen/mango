#include "call_stack.h"

namespace mango::interpreter {

FrameStack::FrameStack() {
    // add initial stack frame
    frames.push_back(Frame("main"));

    // This is definitely not the right place to put this but for now we just stick
    // built in functions in the top level scope. What to do with built in objects
    // will become more clear when modules are implemented
    auto print = new Function();
    print->is_builtin = true;
    print->builtin_fn = [](std::vector<Object*> args) -> Object* {
        std::cout << args[0]->to_string() << "\n";
        return new Undefined();
    };
    set_variable("print", print);
};

void FrameStack::new_frame(std::string function_name) {
    frames.push_back(Frame(function_name));
    current++;
}

void FrameStack::pop_frame() {
    frames.pop_back();
    current--;
}

Object* FrameStack::set_variable(std::string id, Object* v) {
    frames.at(current).variables[id] = v;
    return v;
}

Object* FrameStack::lookup_variable(std::string id) {
    for (auto it = frames.rbegin(); it != frames.rend(); ++it) {
        auto vars = it->variables;
        if (auto var = vars.find(id); var != vars.end()) {
            return var->second;
        }
    }

    return new Undefined();
}

}