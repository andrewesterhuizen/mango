#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>

#include "ast.h"

namespace mango {

struct Value;

struct Function {
    std::vector<std::string> parameters;
    Statement *body;
    bool is_builtin;
    std::function<Value(std::vector<Value>)> builtin_fn;
};

struct Value {
    DataType type = DataType::Undefined;
    int value_int;
    std::string value_string;
    Function value_function;
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

    Value execute_function_expression(FunctionExpression *e);

    Value execute_function_call_expression(FunctionCallExpression *e);

    Value execute_expression(Expression *expression);

    Value execute_declaration_statement(DeclarationStatement *s);

    Value execute_assignment_statement(AssignmentStatement *s);

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