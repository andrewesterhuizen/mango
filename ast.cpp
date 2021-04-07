#include "ast.h"

namespace mango
{

Operator get_operator(char c)
{
    switch (c)
    {
    case '+':
        return Operator::Plus;
    case '-':
        return Operator::Minus;
    case '*':
        return Operator::Multiply;
    case '/':
        return Operator::Divide;
    }

    std::cout << "invalid operator \"" << c << "\"\n";
    assert(false);
}

std::string operator_to_string(Operator op)
{
    switch (op)
    {
    case Operator::Plus:
        return "+";
    case Operator::Minus:
        return "-";
    case Operator::Multiply:
        return "*";
    case Operator::Divide:
        return "/";
    }

    return "unknown";
}

void expression_to_string(string_builder::StringBuilder *sb, Expression *expression)
{
    if (auto e = dynamic_cast<IntegerLiteralExpression *>(expression))
    {
        sb->append_no_indent("IntegerLiteralExpression { value: ");
        sb->append_no_indent(std::to_string(e->value));
        sb->append_no_indent(" }");
    }
    else if (auto e = dynamic_cast<IdentifierExpression *>(expression))
    {
        sb->append_no_indent("IdentifierExpression { value: ");
        sb->append_no_indent(e->value);
        sb->append_no_indent(" }");
    }
    else if (auto e = dynamic_cast<BinaryExpression *>(expression))
    {
        sb->append_line_no_indent("BinaryExpression {");

        sb->increase_indent();

        sb->append("operator: ");
        sb->append_line_no_indent(operator_to_string(e->op));

        sb->append("left: ");
        expression_to_string(sb, e->left);
        sb->append_line("");

        sb->append("right: ");
        expression_to_string(sb, e->right);
        sb->append_line("");

        sb->decrease_indent();

        sb->append("}");
    }
    else
    {
        assert(false);
    }
}

void statement_to_string(string_builder::StringBuilder *sb, Statement *statement)
{
    if (auto s = dynamic_cast<DeclarationStatement *>(statement))
    {
        sb->append_line("DeclarationStatement {");

        sb->increase_indent();

        sb->append("type: ");
        sb->append_line_no_indent(data_type_to_string(s->type));

        sb->append("identifier: ");
        sb->append_line_no_indent(s->identifier);

        sb->append("value: ");
        expression_to_string(sb, s->value);
        sb->append_line_no_indent("");

        sb->decrease_indent();

        sb->append_line("}");
    }
    else if (auto s = dynamic_cast<AssignmentStatement *>(statement))
    {
        sb->append_line("AssignmentStatement {");

        sb->increase_indent();

        sb->append("identifier: ");
        sb->append_line_no_indent(s->identifier);

        sb->append("value: ");
        expression_to_string(sb, s->value);
        sb->append_line_no_indent("");

        sb->decrease_indent();

        sb->append_line("}");
    }
    else if (auto s = dynamic_cast<ExpressionStatement *>(statement))
    {
        sb->append_line("ExpressionStatement {");

        sb->increase_indent();

        sb->append("value: ");
        expression_to_string(sb, s->value);
        sb->append_line("");

        sb->decrease_indent();

        sb->append_line("}");
    }
    else
    {
        assert(false);
    }
}

std::string ast_to_string(Program ast)
{
    string_builder::StringBuilder sb;

    sb.append_line("Program {");
    sb.increase_indent();

    sb.append_line("statements: [");
    sb.increase_indent();

    for (auto s : ast.statements)
    {
        statement_to_string(&sb, s);
    }

    sb.decrease_indent();
    sb.append_line("]");

    sb.decrease_indent();
    sb.append_line("}");

    return sb.get_string();
}

std::string data_type_to_string(const DataType dt)
{
    switch (dt)
    {
    case DataType::String:
        return "string";
    case DataType::Integer:
        return "int";
    case DataType::Float:
        return "float";
    }

    return "unknown";
}

std::ostream &operator<<(std::ostream &os, const DataType &dt)
{
    os << data_type_to_string(dt);
    return os;
}

}