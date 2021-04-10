#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>
#include <variant>

#include "../ast.h"

namespace mango {

struct Value;

struct Function {
    std::vector<std::string> parameters;
    Statement *body;
    bool is_builtin;
    std::function<Value(std::vector<Value>)> builtin_fn;
};

struct Object {
    std::unordered_map<std::string, Value> properties;
};

struct Array {
    std::vector<Value> elements;
};

using ValueVariant = std::variant<int, std::string, bool, Function, Object, Array>;

struct Value {
    DataType type = DataType::Undefined;
    ValueVariant value;
    bool is_truthy();
    std::string to_string();
};

std::ostream &operator<<(std::ostream &os, Value &t);

template<class... Ts>
struct overload : Ts ... {
    using Ts::operator()...;
};
template<class... Ts> overload(Ts...) -> overload<Ts...>;

// this solution sucks and won't scale well to other types
#define INTEGER_OPERATIONS_VISITOR(OPERATOR) \
overload{ \
    [](int l, int r) -> Value { return Value{DataType::Integer, l OPERATOR r}; }, \
    [](auto, auto) -> Value { return Value{DataType::Undefined}; }, \
}

#define INTEGER_LOGICAL_OPERATIONS_VISITOR(OPERATOR) \
overload{ \
    [](int l, int r) -> Value { return Value{DataType::Bool, l OPERATOR r}; }, \
    [](auto, auto) -> Value { return Value{DataType::Undefined}; }, \
}

#define INTEGER_AND_STRING_OPERATIONS_VISITOR(OPERATOR) \
overload{ \
    [](int l, int r) -> Value { return Value{DataType::Integer, l OPERATOR r}; }, \
    [](std::string &l, std::string &r) -> Value { return Value{DataType::String, l OPERATOR r}; }, \
    [](auto, auto) -> Value { return Value{DataType::Undefined}; }, \
}

#define INTEGER_AND_STRING_LOGICAL_OPERATIONS_VISITOR(OPERATOR) \
overload{ \
    [](int l, int r) -> Value { return Value{DataType::Bool, l OPERATOR r}; }, \
    [](std::string &l, std::string &r) -> Value { return Value{DataType::Bool, l OPERATOR r}; }, \
    [](auto, auto) -> Value { return Value{DataType::Undefined}; }, \
}

auto add_visitor = INTEGER_AND_STRING_OPERATIONS_VISITOR(+);
auto subtract_visitor = INTEGER_OPERATIONS_VISITOR(-);
auto multiply_visitor = INTEGER_OPERATIONS_VISITOR(*);
auto divide_visitor = INTEGER_OPERATIONS_VISITOR(/);
auto equal_to_visitor = INTEGER_AND_STRING_LOGICAL_OPERATIONS_VISITOR(==);
auto not_equal_to_visitor = INTEGER_AND_STRING_LOGICAL_OPERATIONS_VISITOR(!=);
auto less_than_visitor = INTEGER_LOGICAL_OPERATIONS_VISITOR(<);
auto less_than_or_equal_to_visitor = INTEGER_LOGICAL_OPERATIONS_VISITOR(<=);
auto greater_than_visitor = INTEGER_LOGICAL_OPERATIONS_VISITOR(>);
auto greater_than_or_equal_to_visitor = INTEGER_LOGICAL_OPERATIONS_VISITOR(>=);

}