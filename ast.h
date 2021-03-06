#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

#include "data_type.h"
#include "string_builder.h"

namespace mango {

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
    Not,
    And,
    Or,
};

std::ostream &operator<<(std::ostream &os, const Operator &op);

std::string operator_to_string(Operator op);

struct Statement {
    virtual ~Statement() = default;
    virtual void print(string_builder::StringBuilder* sb) = 0;
    virtual void generate(string_builder::StringBuilder* sb) {
        assert(false);
    };
};

struct Expression {
    virtual ~Expression() = default;
    virtual void print(string_builder::StringBuilder* sb) = 0;
    virtual void generate(string_builder::StringBuilder* sb) {
        assert(false);
    };
};

struct UndefinedExpression : public Expression {
    void print(string_builder::StringBuilder* sb) override;
};

struct IdentifierExpression : public Expression {
    std::string value;
    void print(string_builder::StringBuilder* sb) override;
    void generate(string_builder::StringBuilder* sb) override;
};

struct IntegerLiteralExpression : public Expression {
    int value;
    void print(string_builder::StringBuilder* sb) override;
    void generate(string_builder::StringBuilder* sb) override;
};

struct StringLiteralExpression : public Expression {
    std::string value;
    void print(string_builder::StringBuilder* sb) override;
    void generate(string_builder::StringBuilder* sb) override;
};

struct BooleanLiteralExpression : public Expression {
    bool value;
    void print(string_builder::StringBuilder* sb) override;
    void generate(string_builder::StringBuilder* sb) override;
};

struct FunctionExpression : public Expression {
    DataType return_type;
    std::vector<std::string> parameters;
    Statement* body;
    void print(string_builder::StringBuilder* sb) override;
};

struct ObjectExpression : public Expression {
    std::unordered_map<std::string, Expression*> properties;
    void print(string_builder::StringBuilder* sb) override;
};

struct ArrayExpression : public Expression {
    std::vector<Expression*> elements;
    void print(string_builder::StringBuilder* sb) override;
};

struct MemberExpression : public Expression {
    std::string identifier;
    Expression* property;
    void print(string_builder::StringBuilder* sb) override;
};

struct FunctionCallExpression : public Expression {
    std::string value;
    std::vector<Expression*> arguments;
    void print(string_builder::StringBuilder* sb) override;
};

struct BinaryExpression : public Expression {
    Operator op;
    Expression* left;
    Expression* right;
    void print(string_builder::StringBuilder* sb) override;
    void generate(string_builder::StringBuilder* sb) override;
};

struct UnaryExpression : public Expression {
    Operator op;
    Expression* argument;
    void print(string_builder::StringBuilder* sb) override;
};

struct AssignmentExpression : public Expression {
    Expression* left;
    Expression* right;
    void print(string_builder::StringBuilder* sb) override;
    void generate(string_builder::StringBuilder* sb) override;
};

struct BlockStatement : public Statement {
    std::vector<Statement*> statements;
    void print(string_builder::StringBuilder* sb) override;
    void generate(string_builder::StringBuilder* sb) override;
};

struct DeclarationStatement : public Statement {
    DataType data_type;
    std::string identifier;
    Expression* value;
    void print(string_builder::StringBuilder* sb) override;
    void generate(string_builder::StringBuilder* sb) override;
};

struct ReturnStatement : public Statement {
    Expression* value;
    void print(string_builder::StringBuilder* sb) override;
};

struct IfStatement : public Statement {
    Expression* condition;
    Statement* if_block;
    Statement* else_block;
    void print(string_builder::StringBuilder* sb) override;
    void generate(string_builder::StringBuilder* sb) override;
};

struct WhileStatement : public Statement {
    Expression* condition;
    Statement* body;
    void print(string_builder::StringBuilder* sb) override;
};

struct ExpressionStatement : public Statement {
    Expression* value;
    void print(string_builder::StringBuilder* sb) override;
    void generate(string_builder::StringBuilder* sb) override;
};

class Program {
public:
    std::vector<Statement*> statements;
    std::string print();
    std::string generate();
};

}
