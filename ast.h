#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "string_builder.h"

namespace mango {

enum class DataType {
    Integer = 1,
    Float,
    String
};

std::string data_type_to_string(const DataType dt);

std::ostream &operator<<(std::ostream &os, const DataType &dt);

enum class Operator {
    Plus = 1,
    Minus,
    Multiply,
    Divide
};

Operator get_operator(char c);

std::string operator_to_string(Operator op);

struct Expression {
    virtual ~Expression() = default;
};

struct UndefinedExpression : public Expression {
};

struct IdentifierExpression : public Expression {
    std::string value;

    IdentifierExpression(std::string v) : value(v) {}
};

struct IntegerLiteralExpression : public Expression {
    int value;

    IntegerLiteralExpression(int v) : value(v) {}
};

struct BinaryExpression : public Expression {
    Operator op;
    Expression *left;
    Expression *right;
};

struct Statement {
    virtual ~Statement() = default;
};

struct DeclarationStatement : public Statement {
    DataType type;
    std::string identifier;
    Expression *value;
};

struct AssignmentStatement : public Statement {
    std::string identifier;
    Expression *value;

};

struct ExpressionStatement : public Statement {
    Expression *value;

    ExpressionStatement(Expression *e) : value(e) {};

};


class Program {
public:
    std::vector<Statement *> statements;
};

void statement_to_string(string_builder::StringBuilder *sb, Statement *statement);

void expression_to_string(string_builder::StringBuilder *sb, Expression *expression);

std::string ast_to_string(Program ast);

}