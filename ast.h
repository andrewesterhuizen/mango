#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "string_builder.h"

namespace mango {

enum class DataType {
    Undefined,
    Integer,
//    Float,
    String,
    Function
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

struct Statement {
    virtual ~Statement() = default;
};

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

struct StringLiteralExpression : public Expression {
    std::string value;

    StringLiteralExpression(std::string v) : value(v) {}
};

struct BlockStatement : public Statement {
    std::vector<Statement *> statements;
};

struct FunctionExpression : public Expression {
    DataType return_type;
    std::vector<std::string> parameters;
    Statement *body;
};

struct FunctionCallExpression : public Expression {
    std::string value;
    std::vector<Expression *> arguments;

    FunctionCallExpression(std::string v) : value(v) {}
};


struct BinaryExpression : public Expression {
    Operator op;
    Expression *left;
    Expression *right;
};

struct DeclarationStatement : public Statement {
    DataType type;
    std::string identifier;
    Expression *value;
};

struct AssignmentStatement : public Statement {
    DataType type;
    std::string identifier;
    Expression *value;
};

struct ReturnStatement : public Statement {
    Expression *value;
};

struct IfStatement : public Statement {
    Expression *condition;
    Statement *if_block;
    Statement *else_block;
};

struct WhileStatement : public Statement {
    Expression *condition;
    Statement *body;
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