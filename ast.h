#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

#include "string_builder.h"

namespace mango {

enum class DataType {
    Undefined,
    Integer,
//    Float,
    String,
    Bool,
    Function,
    Object,
    Array
};

std::string data_type_to_string(const DataType dt);

std::ostream &operator<<(std::ostream &os, const DataType &dt);

enum class Operator {
    Plus = 1,
    Minus,
    Multiply,
    Divide,
    LessThan,
    LessThanOrEqualTo,
    GreaterThan,
    GreaterThanOrEqualTo,
    EqualTo,
    NotEqualTo,
    Not
};

std::ostream &operator<<(std::ostream &os, const Operator &op);

std::string operator_to_string(Operator op);

enum class StatementType {
    Block,
    Declaration,
    Return,
    If,
    While,
    Expression
};

struct Statement {
    StatementType m_type;
    Statement() = delete;
    Statement(StatementType type) {
        m_type = type;
    };
    virtual ~Statement() = default;
    StatementType type() {
        return m_type;
    }
};

struct Expression {
    virtual ~Expression() = default;
};

struct UndefinedExpression : public Expression {
};

struct IdentifierExpression : public Expression {
    std::string value;
};

struct IntegerLiteralExpression : public Expression {
    int value;
};

struct StringLiteralExpression : public Expression {
    std::string value;
};

struct FunctionExpression : public Expression {
    DataType return_type;
    std::vector<std::string> parameters;
    Statement* body;
};

struct ObjectExpression : public Expression {
    std::unordered_map<std::string, Expression*> properties;
};

struct ArrayExpression : public Expression {
    std::vector<Expression*> elements;
};

struct MemberExpression : public Expression {
    std::string identifier;
    Expression* property;
};

struct FunctionCallExpression : public Expression {
    std::string value;
    std::vector<Expression*> arguments;
};

struct BinaryExpression : public Expression {
    Operator op;
    Expression* left;
    Expression* right;
};

struct AssignmentExpression : public Expression {
    Expression* left;
    Expression* right;
};

struct BlockStatement : public Statement {
    BlockStatement() : Statement(StatementType::Block) {};
    std::vector<Statement*> statements;
};

struct DeclarationStatement : public Statement {
    DeclarationStatement() : Statement(StatementType::Declaration) {};
    DataType data_type;
    std::string identifier;
    Expression* value;
};

struct ReturnStatement : public Statement {
    ReturnStatement() : Statement(StatementType::Return) {};
    Expression* value;
};

struct IfStatement : public Statement {
    IfStatement() : Statement(StatementType::If) {};
    Expression* condition;
    Statement* if_block;
    Statement* else_block;
};

struct WhileStatement : public Statement {
    WhileStatement() : Statement(StatementType::While) {};
    Expression* condition;
    Statement* body;
};

struct ExpressionStatement : public Statement {
    ExpressionStatement() : Statement(StatementType::Expression) {};
    Expression* value;
};

class Program {
public:
    std::vector<Statement*> statements;
};

void statement_to_string(string_builder::StringBuilder* sb, Statement* statement);

void expression_to_string(string_builder::StringBuilder* sb, Expression* expression);

std::string ast_to_string(Program ast);

}