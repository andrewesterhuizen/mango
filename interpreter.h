#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>
#include <variant>

#include "ast.h"

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

using ValueVariant = std::variant<int, std::string, bool, Function, Object>;

struct Value {
    DataType type = DataType::Undefined;
    ValueVariant value;
};

using Scope = std::unordered_map<std::string, Value>;

class CallStack {
    std::vector<Scope> scopes;
    int current = 0;

public:
    CallStack() {
        // add initial scope
        scopes.push_back(Scope{});
    };

    void new_frame() {
        scopes.push_back(Scope{});
        current++;
    }

    void pop_frame() {
        scopes.pop_back();
        current--;
    }

    Value set_variable(std::string id, Value v) {
        scopes.at(current)[id] = v;
        return v;
    }

    Value lookup_variable(std::string id) {
        for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
            if (auto var = it->find(id); var != it->end()) {
                return var->second;
            }
        }

        return Value{};
    }
};

std::ostream &operator<<(std::ostream &os, const Value &t);

class Interpreter {
    CallStack call_stack;

    Value execute_binary_expression(BinaryExpression *e);

    Value execute_identifier_expression(IdentifierExpression *e);

    Value execute_integer_literal_expression(IntegerLiteralExpression *e);

    Value execute_string_literal_expression(StringLiteralExpression *e);

    Value execute_object_expression(ObjectExpression *e);

    Value execute_function_expression(FunctionExpression *e);

    Value execute_function_call_expression(FunctionCallExpression *e);

    Value execute_member_expression(MemberExpression *expression);

    Value execute_assignment_expression(AssignmentExpression *e);

    Value execute_expression(Expression *expression);

    Value execute_declaration_statement(DeclarationStatement *s);


    Value execute_expression_statement(ExpressionStatement *s);

    Value execute_block_statement(BlockStatement *bs);

    Value execute_return_statement(ReturnStatement *s);

    Value execute_if_statement(IfStatement *s);

    Value execute_while_statement(WhileStatement *s);

    Value execute_statement(Statement *statement);

public:
    Interpreter();

    Value run(Program ast);
};

}