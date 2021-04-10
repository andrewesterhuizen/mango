#include "scope.h"

namespace mango {

ScopeStack::ScopeStack() {
  // add initial scope
  scopes.push_back(Scope{});

  // This is definitely not the right place to put this but for now we just stick
  // built in functions in the top level scope. What to do with built in objects
  // will become more clear when modules are implemented
  Value print;
  print.type = DataType::Function;
  Function fn;
  fn.is_builtin = true;
  fn.builtin_fn = [&](std::vector<Value> args) -> Value {
    std::cout << args[0] << "\n";
    return Value{};
  };
  print.value = fn;
  set_variable("print", print);
};

void ScopeStack::new_frame() {
  scopes.push_back(Scope{});
  current++;
}

void ScopeStack::pop_frame() {
  scopes.pop_back();
  current--;
}

Value ScopeStack::set_variable(std::string id, Value v) {
  scopes.at(current)[id] = v;
  return v;
}

Value ScopeStack::lookup_variable(std::string id) {
  for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
    if (auto var = it->find(id); var != it->end()) {
      return var->second;
    }
  }

  return Value{};
}

}