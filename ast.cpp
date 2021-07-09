#include "ast.h"

#include <unordered_map>

namespace mango {

auto operator_string_lookup = std::unordered_map<Operator, std::string>{
        {Operator::Plus,                 "+"},
        {Operator::Minus,                "-"},
        {Operator::Multiply,             "*"},
        {Operator::Divide,               "/"},
        {Operator::EqualTo,              "=="},
        {Operator::NotEqualTo,           "!="},
        {Operator::Not,                  "!"},
        {Operator::LessThan,             "<"},
        {Operator::LessThanOrEqualTo,    "<="},
        {Operator::GreaterThan,          ">"},
        {Operator::GreaterThanOrEqualTo, ">="},
        {Operator::And,                  "&&"},
        {Operator::Or,                   "||"},
};

std::string operator_to_string(Operator op) {
    if (operator_string_lookup.find(op) != operator_string_lookup.end()) {
        return operator_string_lookup[op];
    }

    std::cerr << "no string defined for operator\n";
    assert(false);
}

std::ostream &operator<<(std::ostream &os, const Operator &op) {
    os << operator_to_string(op);
    return os;
}

void BinaryExpression::print(string_builder::StringBuilder* sb) {
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

void BinaryExpression::generate(string_builder::StringBuilder* sb) {
    left->generate(sb);
    sb->append_no_indent(" ");
    sb->append_no_indent(operator_to_string(op));
    sb->append_no_indent(" ");
    right->generate(sb);
}

void UnaryExpression::print(string_builder::StringBuilder* sb) {
    sb->append_line_no_indent("UnaryExpression {");
    sb->increase_indent();
    sb->append("operator: ");
    sb->append_line_no_indent(operator_to_string(op));
    sb->append("argument: ");
    argument->print(sb);
    sb->append_line("");
    sb->decrease_indent();
    sb->append("}");
}

void UndefinedExpression::print(string_builder::StringBuilder* sb) {
    sb->append_line_no_indent("UndefinedExpression {}");
}

void IdentifierExpression::print(string_builder::StringBuilder* sb) {
    sb->append_no_indent("IdentifierExpression { value: ");
    sb->append_no_indent(value);
    sb->append_no_indent(" }");
}

void IdentifierExpression::generate(string_builder::StringBuilder* sb) {
    sb->append_no_indent(value);
}

void IntegerLiteralExpression::print(string_builder::StringBuilder* sb) {
    sb->append_no_indent("IntegerLiteralExpression { value: ");
    sb->append_no_indent(std::to_string(value));
    sb->append_no_indent(" }");
}

void IntegerLiteralExpression::generate(string_builder::StringBuilder* sb) {
    sb->append_no_indent(std::to_string(value));
}

void StringLiteralExpression::print(string_builder::StringBuilder* sb) {
    sb->append_no_indent("StringLiteralExpression { value: ");
    sb->append_no_indent(value);
    sb->append_no_indent(" }");
}

void StringLiteralExpression::generate(string_builder::StringBuilder* sb) {
    std::cerr << "TODO: strings\n";
    assert(false);
}

void BooleanLiteralExpression::print(string_builder::StringBuilder* sb) {
    sb->append_no_indent("BooleanLiteralExpression { value: ");
    sb->append_no_indent(value ? "true" : "false");
    sb->append_no_indent(" }");
}

void BooleanLiteralExpression::generate(string_builder::StringBuilder* sb) {
    sb->append_no_indent(value ? "1" : "0");
}

void FunctionExpression::print(string_builder::StringBuilder* sb) {
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

void ExpressionStatement::print(string_builder::StringBuilder* sb) {
    sb->append_line("ExpressionStatement {");
    sb->increase_indent();
    sb->append("value: ");
    value->print(sb);
    sb->append_line("");
    sb->decrease_indent();
    sb->append_line("}");
}

void ExpressionStatement::generate(string_builder::StringBuilder* sb) {
    sb->append("");
    value->generate(sb);
    sb->append_no_indent(";");
    sb->append_line("");
}

void WhileStatement::print(string_builder::StringBuilder* sb) {
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

void IfStatement::print(string_builder::StringBuilder* sb) {
    sb->append_line("IfStatement {");
    sb->increase_indent();
    sb->append("condition: ");
    condition->print(sb);
    sb->append_line("");
    sb->append("if_block: ");
    if_block->print(sb);
    sb->append_line("");
    sb->append("else_block: ");
    if (else_block) {
        else_block->print(sb);
    } else {
        sb->append_no_indent("<null>");
    }
    sb->append_line("");
    sb->decrease_indent();
    sb->append_line("}");
}

void IfStatement::generate(string_builder::StringBuilder* sb) {
    sb->append("if (");
    condition->generate(sb);
    sb->append_line_no_indent(")");
    if_block->generate(sb);
    if (else_block) {
        sb->append_line("else");
        else_block->generate(sb);
    }
}

void ReturnStatement::print(string_builder::StringBuilder* sb) {
    sb->append_line("ReturnStatement {");
    sb->increase_indent();
    sb->append("value: ");
    value->print(sb);
    sb->append_line("");
    sb->decrease_indent();
    sb->append_line("}");
}

void DeclarationStatement::print(string_builder::StringBuilder* sb) {
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

void DeclarationStatement::generate(string_builder::StringBuilder* sb) {
    assert(data_type == DataType::Integer);
    sb->append("int ");
    sb->append_no_indent(identifier);
    sb->append_no_indent(" = ");
    value->generate(sb);
    sb->append_no_indent(";");
    sb->append_line("");
}

void BlockStatement::print(string_builder::StringBuilder* sb) {
    sb->append_line_no_indent("BlockStatement {");
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

void BlockStatement::generate(string_builder::StringBuilder* sb) {
    sb->append_line("{");
    sb->increase_indent();

    for (auto s: statements) {
        s->generate(sb);
    }

    sb->decrease_indent();
    sb->append_line("}");
}

void AssignmentExpression::print(string_builder::StringBuilder* sb) {
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

void AssignmentExpression::generate(string_builder::StringBuilder* sb) {
    auto id = dynamic_cast<IdentifierExpression*>(left);
    assert(id != nullptr);

    sb->append_no_indent(id->value);
    sb->append_no_indent(" = ");
    right->generate(sb);
}

void FunctionCallExpression::print(string_builder::StringBuilder* sb) {
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

void MemberExpression::print(string_builder::StringBuilder* sb) {
    sb->append_line_no_indent("MemberExpression {");
    sb->increase_indent();
    sb->append("object: ");
    sb->append_line_no_indent(identifier);
    sb->append("property: ");
    property->print(sb);
    sb->decrease_indent();
    sb->append_no_indent(" }");
}

void ArrayExpression::print(string_builder::StringBuilder* sb) {
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

void ObjectExpression::print(string_builder::StringBuilder* sb) {
    sb->append_line_no_indent("ObjectExpression {");
    sb->append_line("}");
}

std::string Program::print() {
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

std::string Program::generate() {
    string_builder::StringBuilder sb;

    sb.append_line("int main() {");
    sb.increase_indent();

    for (auto s : statements) {
        s->generate(&sb);
    }

    sb.append_line("return 0;");

    sb.decrease_indent();
    sb.append_line("}");

    return sb.get_string();
}

}