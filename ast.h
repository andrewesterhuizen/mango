#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

#include "data_type.h"
#include "types.h"
#include "string_builder.h"
#include "interpreter.h"

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
    virtual void print(string_builder::StringBuilder* sb) = 0;
    virtual interpreter::Object* execute(interpreter::Interpreter &interpreter) = 0;
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
    virtual interpreter::Object* execute(interpreter::Interpreter &interpreter) = 0;
};

struct UndefinedExpression : public Expression {
    UndefinedExpression() : Expression(ExpressionType::Undefined) {};

    void print(string_builder::StringBuilder* sb) override {
        sb->append_line_no_indent("UndefinedExpression {}");
    }

    interpreter::Object* execute(interpreter::Interpreter &interpreter) override {
        return new interpreter::Undefined();
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

    interpreter::Object* execute(interpreter::Interpreter &interpreter) override {
        return interpreter.lookup_variable(value);
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

    interpreter::Object* execute(interpreter::Interpreter &interpreter) override {
        return new interpreter::Integer(value);
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

    interpreter::Object* execute(interpreter::Interpreter &interpreter) override {
        return new interpreter::String(value);
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
        body->print(sb);
        sb->decrease_indent();
        sb->append_line("}");
    }

    interpreter::Object* execute(interpreter::Interpreter &interpreter) override {
        auto obj = new interpreter::Function();
        obj->parameters = parameters;
        obj->body = body;
        return obj;
    }
};

struct ObjectExpression : public Expression {
    ObjectExpression() : Expression(ExpressionType::Object) {};
    std::unordered_map<std::string, Expression*> properties;

    void print(string_builder::StringBuilder* sb) override {
        sb->append_line_no_indent("ObjectExpression {");
        sb->append_line("}");
    }

    interpreter::Object* execute(interpreter::Interpreter &interpreter) override {
        auto obj = new interpreter::Object();

        for (auto prop : properties) {
            obj->properties[prop.first] = prop.second->execute(interpreter);
        }

        return obj;
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

    interpreter::Object* execute(interpreter::Interpreter &interpreter) override {
        auto obj = new interpreter::Array();

        for (auto el : elements) {
            obj->elements.push_back(el->execute(interpreter));
        }

        return obj;
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

    interpreter::Object* execute(interpreter::Interpreter &interpreter) override {
        auto variable = interpreter.lookup_variable(identifier);
        if (variable->type() == DataType::Undefined) {
            std::cerr << "reference error: no definition found for identifier \"" << identifier << "\"\n";
            assert(false);
        }

        if (auto array = dynamic_cast<interpreter::Array*>(variable)) {
            auto property_value = property->execute(interpreter);
            auto property_integer = dynamic_cast<interpreter::Integer*>(property_value);

            // TODO: handle built in properties like .size for arrays
            if (property_integer == nullptr) {
                std::cerr << "unsupported data type for property lookup\n";
                assert(false);
            }

            // element lookup
            auto index = property_integer->value;
            if (index >= array->elements.size()) {
                return new interpreter::Undefined();
            }

            return array->elements.at(index);
        } else {
            auto property_value = property->execute(interpreter);

            std::string property_string;
            if (auto str = dynamic_cast<interpreter::String*>(property)) {
                property_string = str->value;
            } else {
                property_string = property_value->to_string();
            }

            auto v = variable->properties.find(property_string);
            if (v == variable->properties.end()) {
                return new interpreter::Undefined();
            }

            return v->second;
        }

        std::cerr << "unsupported data type for property lookup\n";
        assert(false);
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

    interpreter::Object* execute(interpreter::Interpreter &interpreter) override {
        auto obj = interpreter.lookup_variable(value);
        if (obj->type() == DataType::Undefined) {
            std::cerr << "reference error: no definition found for identifier \"" << value << "\"\n";
            assert(false);
        }

        auto function = dynamic_cast<interpreter::Function*>(obj);
        if (function == nullptr) {
            std::cerr << "identifier " << value << " is not a function\n";
            assert(false);
        }

        if (function->is_builtin) {
            std::vector<interpreter::Object*> args;

            for (int i = 0; i < arguments.size(); i++) {
                args.push_back(arguments[i]->execute(interpreter));
            }

            return function->builtin_fn(args);
        } else {
            interpreter.new_frame(value);

            for (int i = 0; i < arguments.size(); i++) {
                auto identifier = function->parameters[i];
                interpreter.set_variable(identifier, arguments[i]->execute(interpreter));
            }

            auto return_value = function->body->execute(interpreter);

            interpreter.pop_frame();

            return return_value;
        }
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

    interpreter::Object* execute(interpreter::Interpreter &interpreter) override;
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

    interpreter::Object* execute(interpreter::Interpreter &interpreter) override {
        auto right_value = right->execute(interpreter);

        if (auto e = dynamic_cast<IdentifierExpression*>(left)) {
            interpreter.set_variable(e->value, right_value);
            return right_value;
        } else if (auto e = dynamic_cast<MemberExpression*>(left)) {
            auto variable = interpreter.lookup_variable(e->identifier);
            if (variable->type() == DataType::Undefined) {
                std::cerr << "reference error: no definition found for identifier \"" << e->identifier << "\"\n";
                assert(false);
            }

            auto name = e->property->execute(interpreter);
            auto name_string = dynamic_cast<interpreter::String*>(name);
            assert(name_string != nullptr);

            auto v = variable->properties.find(name_string->value);
            variable->properties[name_string->value] = right_value;
            return right_value;
        }

        assert(false);
    }
};

struct BlockStatement : public Statement {
    BlockStatement() : Statement(StatementType::Block) {};
    std::vector<Statement*> statements;

    void print(string_builder::StringBuilder* sb) override {
        sb->append_line("BlockStatement {");
        sb->increase_indent();
        sb->append_line("value: [");
        sb->increase_indent();
        if (statements.size() > 0) {
            for (auto st : statements) {
                st->print(sb);
            }
        } else {
            sb->append_line("<empty>");
        }
        sb->decrease_indent();
        sb->append_line("]");
        sb->decrease_indent();
        sb->append_line("}");
    }

    interpreter::Object* execute(interpreter::Interpreter &interpreter) override {
        interpreter::Object* v;

        for (auto s: statements) {
            v = s->execute(interpreter);
        }

        return v;
    }
};

struct DeclarationStatement : public Statement {
    DeclarationStatement() : Statement(StatementType::Declaration) {};
    DataType data_type;
    std::string identifier;
    Expression* value;

    void print(string_builder::StringBuilder* sb) override {
        sb->append_line("DeclarationStatement {");
        sb->increase_indent();
        sb->append("type: ");
        sb->append_line_no_indent(data_type_to_string(data_type));
        sb->append("identifier: ");
        sb->append_line_no_indent(identifier);
        sb->append("value: ");
        value->print(sb);
        sb->append_line_no_indent("");
        sb->decrease_indent();
        sb->append_line("}");
    }

    interpreter::Object* execute(interpreter::Interpreter &interpreter) override {
        return interpreter.set_variable(identifier, value->execute(interpreter));
    }
};

struct ReturnStatement : public Statement {
    ReturnStatement() : Statement(StatementType::Return) {};
    Expression* value;

    void print(string_builder::StringBuilder* sb) override {
        sb->append_line("ReturnStatement {");
        sb->increase_indent();
        sb->append("value: ");
        value->print(sb);
        sb->append_line("");
        sb->decrease_indent();
        sb->append_line("}");
    }

    interpreter::Object* execute(interpreter::Interpreter &interpreter) override {
        return value->execute(interpreter);
    }
};

struct IfStatement : public Statement {
    IfStatement() : Statement(StatementType::If) {};
    Expression* condition;
    Statement* if_block;
    Statement* else_block;

    void print(string_builder::StringBuilder* sb) override {
        sb->append_line("IfStatement {");
        sb->increase_indent();
        sb->append("condition: ");
        condition->print(sb);
        sb->append_line("");
        sb->append("if_block: ");
        if_block->print(sb);
        sb->append_line("");
        sb->append("else_block: ");
        else_block->print(sb);
        sb->append_line("");
        sb->decrease_indent();
        sb->append_line("}");
    }

    interpreter::Object* execute(interpreter::Interpreter &interpreter) override {
        auto condition_value = condition->execute(interpreter);
        if (condition_value->is_truthy()) {
            if_block->execute(interpreter);
        } else if (else_block != nullptr) {
            else_block->execute(interpreter);
        }

        return new interpreter::Undefined();
    }
};

struct WhileStatement : public Statement {
    WhileStatement() : Statement(StatementType::While) {};
    Expression* condition;
    Statement* body;

    void print(string_builder::StringBuilder* sb) override {
        sb->append_line("WhileStatement {");
        sb->increase_indent();
        sb->append("condition: ");
        condition->print(sb);
        sb->append_line("");
        sb->append("body: ");
        body->print(sb);
        sb->append_line("");
        sb->decrease_indent();
        sb->append_line("}");
    }

    interpreter::Object* execute(interpreter::Interpreter &interpreter) override {
        while (condition->execute(interpreter)->is_truthy()) {
            body->execute(interpreter);
        }

        return new interpreter::Undefined();
    }
};

struct ExpressionStatement : public Statement {
    ExpressionStatement() : Statement(StatementType::Expression) {};
    Expression* value;

    void print(string_builder::StringBuilder* sb) override {
        sb->append_line("ExpressionStatement {");
        sb->increase_indent();
        sb->append("value: ");
        value->print(sb);
        sb->append_line("");
        sb->decrease_indent();
        sb->append_line("}");
    }

    interpreter::Object* execute(interpreter::Interpreter &interpreter) override {
        return value->execute(interpreter);
    }
};

class Program {
public:
    std::vector<Statement*> statements;
    std::string print() {
        string_builder::StringBuilder sb;

        sb.append_line("Program {");
        sb.increase_indent();

        sb.append_line("statements: [");
        sb.increase_indent();

        for (auto s : statements) {
            s->print(&sb);
        }

        sb.decrease_indent();
        sb.append_line("]");

        sb.decrease_indent();
        sb.append_line("}");

        return sb.get_string();
    }

    interpreter::Object* execute(interpreter::Interpreter& interpreter) {
        interpreter::Object* v;

        for (auto s : statements) {
            v = s->execute(interpreter);
        }

        return v;
    }
};

}
