#pragma once

#include <iostream>
#include <unordered_map>

#include "ast.h"

namespace mango {

struct Value {
    DataType type;
    int value_int;
};

std::ostream &operator<<(std::ostream &os, const Value &t);

class Interpreter {
    std::unordered_map<std::string, Value> vars;

    Value execute_binary_expression(BinaryExpression *e);

    Value execute_identifier_expression(IdentifierExpression *e);

    Value execute_integer_literal_expression(IntegerLiteralExpression *e);

    Value execute_expression(Expression *expression);

    void execute_declaration_statement(DeclarationStatement *s);

    void execute_assignment_statement(AssignmentStatement *s);

    void execute_expression_statement(ExpressionStatement *s);

    void execute_statement(Statement *statement);

public:
    void run(Program ast);
};

}