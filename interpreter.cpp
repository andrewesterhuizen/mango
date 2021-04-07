#include "interpreter.h"

namespace mango
{

std::ostream &operator<<(std::ostream &os, const Value &v)
{
    switch (v.type)
    {
    case DataType::Integer:
        os << v.value_int;
        return os;
    }

    os << "unknown";
    return os;
}

Value Interpreter::execute_binary_expression(BinaryExpression *e)
{
    auto l = execute_expression(e->left);
    auto r = execute_expression(e->right);

    // TODO: handle non integer types
    // TODO: operator precedence

    switch (e->op)
    {
    case Operator::Plus:
        return Value{DataType::Integer, l.value_int + r.value_int};
    case Operator::Minus:
        return Value{DataType::Integer, l.value_int - r.value_int};
    case Operator::Multiply:
        return Value{DataType::Integer, l.value_int * r.value_int};
    case Operator::Divide:
        return Value{DataType::Integer, l.value_int / r.value_int};
    }

    std::cerr << "unhandled operator" << (int)e->op << "\n";
    assert(false);
}

Value Interpreter::execute_identifier_expression(IdentifierExpression *e)
{
    return vars[e->value];
}

Value Interpreter::execute_integer_literal_expression(IntegerLiteralExpression *e)
{
    return Value{DataType::Integer, e->value};
}

Value Interpreter::execute_expression(Expression *expression)
{
    std::cout << "execute_expression\n";
    if (auto e = dynamic_cast<IdentifierExpression *>(expression))
    {
        auto value = execute_identifier_expression(e);
        std::cout << "IdentifierExpression => " << value << "\n";
        return value;
    }
    else if (auto e = dynamic_cast<BinaryExpression *>(expression))
    {
        auto value = execute_binary_expression(e);
        std::cout << "BinaryExpression => " << value << "\n";
        return value;
    }
    else if (auto e = dynamic_cast<IntegerLiteralExpression *>(expression))
    {
        auto value = execute_integer_literal_expression(e);
        std::cout << "BinaryExpression => " << value << "\n";
        return value;
    }
    else
    {
        std::cout << "unknown expression type" << typeid(expression).name() << "\n";
        assert(false);
    }
}

void Interpreter::execute_declaration_statement(DeclarationStatement *s)
{
    vars[s->identifier] = execute_expression(s->value);
}

void Interpreter::execute_assignment_statement(AssignmentStatement *s)
{
    vars[s->identifier] = execute_expression(s->value);
}

void Interpreter::execute_expression_statement(ExpressionStatement *s)
{
    execute_expression(s->value);
}

void Interpreter::execute_statement(Statement *statement)
{
    if (auto s = dynamic_cast<DeclarationStatement *>(statement))
    {
        execute_declaration_statement(s);
    }
    else if (auto s = dynamic_cast<ExpressionStatement *>(statement))
    {
        execute_expression_statement(s);
    }
    else if (auto s = dynamic_cast<AssignmentStatement *>(statement))
    {
        execute_assignment_statement(s);
    }
    else
    {
        std::cerr << "unknown statement type\n";
        assert(false);
    }
}

void Interpreter::run(Program ast)
{
    for (auto s : ast.statements)
    {
        execute_statement(s);
    }
}

}
