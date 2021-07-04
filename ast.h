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
    virtual void print(string_builder::StringBuilder* sb) = 0;
};

void statement_to_string(string_builder::StringBuilder* sb, Statement* statement);

struct UndefinedExpression : public Expression {
    UndefinedExpression() : Expression(ExpressionType::Undefined) {};
    
    void print(string_builder::StringBuilder* sb) override {
        sb->append_line_no_indent("UndefinedExpression {}");
    }
};

struct IdentifierExpression : public Expression {
    IdentifierExpression() : Expression(ExpressionType::Identifier) {};
    std::string value;

    void print(string_builder::StringBuilder* sb) override {
        sb->append_no_indent("IdentifierExpression { value: ");
        sb->append_no_indent(value);
        sb->append_no_indent(" }");
    }
};

struct IntegerLiteralExpression : public Expression {
    IntegerLiteralExpression() : Expression(ExpressionType::IntegerLiteral) {};
    int value;

    void print(string_builder::StringBuilder* sb) override {
        sb->append_no_indent("IntegerLiteralExpression { value: ");
        sb->append_no_indent(std::to_string(value));
        sb->append_no_indent(" }");
    }
};

struct StringLiteralExpression : public Expression {
    StringLiteralExpression() : Expression(ExpressionType::StringLiteral) {};
    std::string value;

    void print(string_builder::StringBuilder* sb) override {
        sb->append_no_indent("StringLiteralExpression { value: ");
        sb->append_no_indent(value);
        sb->append_no_indent(" }");
    }
};

struct FunctionExpression : public Expression {
    FunctionExpression() : Expression(ExpressionType::Function) {};
    DataType return_type;
    std::vector<std::string> parameters;
    Statement* body;

    void print(string_builder::StringBuilder* sb) override {
        sb->append_line_no_indent("FunctionExpression {");
        // params
        sb->increase_indent();
        sb->append_line("parameters: [");
        sb->increase_indent();
        for (auto p : parameters) {
            sb->append_line(p);
        }
        sb->decrease_indent();
        sb->append_line("]");
        // body
        sb->append("value: ");
        statement_to_string(sb, body);
        sb->decrease_indent();
        sb->append_line("}");
    }
};

struct ObjectExpression : public Expression {
    ObjectExpression() : Expression(ExpressionType::Object) {};
    std::unordered_map<std::string, Expression*> properties;

    void print(string_builder::StringBuilder* sb) override {
        sb->append_line_no_indent("ObjectExpression {");
        sb->append_line("}");
    }
};

struct ArrayExpression : public Expression {
    ArrayExpression() : Expression(ExpressionType::Array) {};
    std::vector<Expression*> elements;

    void print(string_builder::StringBuilder* sb) override {
        sb->append_line_no_indent("ArrayExpression {");
        sb->append_line("elements: [");
        sb->increase_indent();
        for (auto e : elements) {
            sb->append("");
            e->print(sb);
            sb->append_line("");
        }
        sb->decrease_indent();
        sb->append_line("");
        sb->append_line("]");
        sb->decrease_indent();
        sb->append_line("}");
    }
};

struct MemberExpression : public Expression {
    MemberExpression() : Expression(ExpressionType::Member) {};
    std::string identifier;
    Expression* property;

    void print(string_builder::StringBuilder* sb) override {
        sb->append_line_no_indent("MemberExpression {");
        sb->increase_indent();
        sb->append("object: ");
        sb->append_line_no_indent(identifier);
        sb->append("property: ");
        property->print(sb);
        sb->decrease_indent();
        sb->append_no_indent(" }");
    }
};

struct FunctionCallExpression : public Expression {
    FunctionCallExpression() : Expression(ExpressionType::FunctionCall) {};
    std::string value;
    std::vector<Expression*> arguments;

    void print(string_builder::StringBuilder* sb) override {
        sb->append_line_no_indent("FunctionCallExpression {");
        sb->increase_indent();
        sb->append("value: ");
        sb->append_line_no_indent(value);
        sb->append_line("arguments: [");
        sb->increase_indent();
        for (auto e : arguments) {
            sb->append("");
            e->print(sb);
            sb->append_line("");
        }
        sb->decrease_indent();
        sb->append_line("");
        sb->append_line("]");
        sb->decrease_indent();
        sb->append_line("}");
    }
};

struct BinaryExpression : public Expression {
    BinaryExpression() : Expression(ExpressionType::Binary) {};
    Operator op;
    Expression* left;
    Expression* right;

    void print(string_builder::StringBuilder* sb) override {
        sb->append_line_no_indent("BinaryExpression {");
        sb->increase_indent();
        sb->append("operator: ");
        sb->append_line_no_indent(operator_to_string(op));
        sb->append("left: ");
        left->print(sb);
        sb->append_line("");
        sb->append("right: ");
        right->print(sb);
        sb->append_line("");
        sb->decrease_indent();
        sb->append("}");
    }
};

struct AssignmentExpression : public Expression {
    AssignmentExpression() : Expression(ExpressionType::Assignment) {};
    Expression* left;
    Expression* right;

    void print(string_builder::StringBuilder* sb) override {
        sb->append_line_no_indent("AssignmentExpression {");
        sb->increase_indent();
        sb->append("left: ");
        left->print(sb);
        sb->append_line("");
        sb->append("right: ");
        right->print(sb);
        sb->append_line("");
        sb->decrease_indent();
        sb->append("}");
    }
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

std::string ast_to_string(Program ast);

}