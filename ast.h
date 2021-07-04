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

enum class ExpressionType {
    Undefined,
    Identifier,
    IntegerLiteral,
    StringLiteral,
    Function,
    Object,
    Array,
    Member,
    FunctionCall,
    Binary,
    Assignment,
};

struct Expression {
    ExpressionType m_type;
    Expression() = delete;
    Expression(ExpressionType type) {
        m_type = type;
    }
    virtual ~Expression() = default;
    ExpressionType type() {
        return m_type;
    }
};

struct UndefinedExpression : public Expression {
    UndefinedExpression() : Expression(ExpressionType::Undefined) {};
};

struct IdentifierExpression : public Expression {
    IdentifierExpression() : Expression(ExpressionType::Identifier) {};
    std::string value;
};

struct IntegerLiteralExpression : public Expression {
    IntegerLiteralExpression() : Expression(ExpressionType::IntegerLiteral) {};
    int value;
};

struct StringLiteralExpression : public Expression {
    StringLiteralExpression() : Expression(ExpressionType::StringLiteral) {};
    std::string value;
};

struct FunctionExpression : public Expression {
    FunctionExpression() : Expression(ExpressionType::Function) {};
    DataType return_type;
    std::vector<std::string> parameters;
    Statement* body;
};

struct ObjectExpression : public Expression {
    ObjectExpression() : Expression(ExpressionType::Object) {};
    std::unordered_map<std::string, Expression*> properties;
};

struct ArrayExpression : public Expression {
    ArrayExpression() : Expression(ExpressionType::Array) {};
    std::vector<Expression*> elements;
};

struct MemberExpression : public Expression {
    MemberExpression() : Expression(ExpressionType::Member) {};
    std::string identifier;
    Expression* property;
};

struct FunctionCallExpression : public Expression {
    FunctionCallExpression() : Expression(ExpressionType::FunctionCall) {};
    std::string value;
    std::vector<Expression*> arguments;
};

struct BinaryExpression : public Expression {
    BinaryExpression() : Expression(ExpressionType::Binary) {};
    Operator op;
    Expression* left;
    Expression* right;
};

struct AssignmentExpression : public Expression {
    AssignmentExpression() : Expression(ExpressionType::Assignment) {};
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