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

void expression_to_string(string_builder::StringBuilder* sb, Expression* expression) {
    if (auto e = dynamic_cast<IntegerLiteralExpression*>(expression)) {
        sb->append_no_indent("IntegerLiteralExpression { value: ");
        sb->append_no_indent(std::to_string(e->value));
        sb->append_no_indent(" }");
    } else if (auto e = dynamic_cast<StringLiteralExpression*>(expression)) {
        sb->append_no_indent("StringLiteralExpression { value: ");
        sb->append_no_indent(e->value);
        sb->append_no_indent(" }");
    } else if (auto e = dynamic_cast<IdentifierExpression*>(expression)) {
        sb->append_no_indent("IdentifierExpression { value: ");
        sb->append_no_indent(e->value);
        sb->append_no_indent(" }");
    } else if (auto e = dynamic_cast<MemberExpression*>(expression)) {
        sb->append_line_no_indent("MemberExpression {");
        sb->increase_indent();
        sb->append("object: ");
        sb->append_line_no_indent(e->identifier);
        sb->append("property: ");
        expression_to_string(sb, e->property);
        sb->decrease_indent();
        sb->append_no_indent(" }");
    } else if (auto e = dynamic_cast<BinaryExpression*>(expression)) {
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
    } else if (auto e = dynamic_cast<AssignmentExpression*>(expression)) {
        sb->append_line_no_indent("AssignmentExpression {");

        sb->increase_indent();

        sb->append("left: ");
        expression_to_string(sb, e->left);
        sb->append_line("");

        sb->append("right: ");
        expression_to_string(sb, e->right);
        sb->append_line("");

        sb->decrease_indent();

        sb->append("}");
    } else if (auto e = dynamic_cast<FunctionExpression*>(expression)) {
        sb->append_line_no_indent("FunctionExpression {");

        // params
        sb->increase_indent();
        sb->append_line("parameters: [");
        sb->increase_indent();
        for (auto p : e->parameters) {
            sb->append_line(p);
        }
        sb->decrease_indent();
        sb->append_line("]");

        // body
        sb->append("value: ");
        statement_to_string(sb, e->body);

        sb->decrease_indent();
        sb->append_line("}");
    } else if (auto e = dynamic_cast<FunctionCallExpression*>(expression)) {
        sb->append_line_no_indent("FunctionCallExpression {");

        sb->increase_indent();

        sb->append("value: ");
        sb->append_line_no_indent(e->value);

        sb->append_line("arguments: [");
        sb->increase_indent();
        for (auto a : e->arguments) {
            sb->append("");
            expression_to_string(sb, a);
            sb->append_line("");
        }
        sb->decrease_indent();
        sb->append_line("");
        sb->append_line("]");

        sb->decrease_indent();
        sb->append_line("}");
    } else if (auto e = dynamic_cast<ObjectExpression*>(expression)) {
        sb->append_line_no_indent("ObjectExpression {");
        sb->append_line("}");
    } else if (auto e = dynamic_cast<ArrayExpression*>(expression)) {
        sb->append_line_no_indent("ArrayExpression {");

        sb->append_line("elements: [");
        sb->increase_indent();
        for (auto a : e->elements) {
            sb->append("");
            expression_to_string(sb, a);
            sb->append_line("");
        }
        sb->decrease_indent();
        sb->append_line("");
        sb->append_line("]");

        sb->decrease_indent();
        sb->append_line("}");
    } else {
        std::cerr << "unknown expression\n";
        assert(false);
    }
}

void statement_to_string(string_builder::StringBuilder* sb, Statement* statement) {
    if (auto s = dynamic_cast<DeclarationStatement*>(statement)) {
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
    } else if (auto s = dynamic_cast<ExpressionStatement*>(statement)) {
        sb->append_line("ExpressionStatement {");

        sb->increase_indent();

        sb->append("value: ");
        expression_to_string(sb, s->value);
        sb->append_line("");

        sb->decrease_indent();

        sb->append_line("}");
    } else if (auto s = dynamic_cast<BlockStatement*>(statement)) {
        sb->append_line("BlockStatement {");
        sb->increase_indent();

        sb->append_line("value: [");
        sb->increase_indent();

        if (s->statements.size() > 0) {
            for (auto st : s->statements) {
                statement_to_string(sb, st);
            }
        } else {
            sb->append_line("<empty>");
        }

        sb->decrease_indent();
        sb->append_line("]");

        sb->decrease_indent();

        sb->append_line("}");
    } else if (auto s = dynamic_cast<ReturnStatement*>(statement)) {
        sb->append_line("ReturnStatement {");

        sb->increase_indent();

        sb->append("value: ");
        expression_to_string(sb, s->value);
        sb->append_line("");

        sb->decrease_indent();

        sb->append_line("}");
    } else if (auto s = dynamic_cast<IfStatement*>(statement)) {
        sb->append_line("IfStatement {");

        sb->increase_indent();

        sb->append("condition: ");
        expression_to_string(sb, s->condition);
        sb->append_line("");

        sb->append("if_block: ");
        statement_to_string(sb, s->if_block);
        sb->append_line("");

        sb->append("else_block: ");
        statement_to_string(sb, s->if_block);
        sb->append_line("");

        sb->decrease_indent();

        sb->append_line("}");
    } else if (auto s = dynamic_cast<WhileStatement*>(statement)) {
        sb->append_line("WhileStatement {");

        sb->increase_indent();

        sb->append("condition: ");
        expression_to_string(sb, s->condition);
        sb->append_line("");

        sb->append("body: ");
        statement_to_string(sb, s->body);
        sb->append_line("");

        sb->decrease_indent();

        sb->append_line("}");
    } else {
        assert(false);
    }
}

std::string ast_to_string(Program ast) {
    string_builder::StringBuilder sb;

    sb.append_line("Program {");
    sb.increase_indent();

    sb.append_line("statements: [");
    sb.increase_indent();

    for (auto s : ast.statements) {
        statement_to_string(&sb, s);
    }

    sb.decrease_indent();
    sb.append_line("]");

    sb.decrease_indent();
    sb.append_line("}");

    return sb.get_string();
}

auto data_type_string_lookup = std::unordered_map<DataType, std::string>{
        {DataType::Undefined, "undefined"},
        {DataType::String,    "string"},
        {DataType::Integer,   "integer"},
        {DataType::Function,  "function"},
};

std::string data_type_to_string(const DataType dt) {
    if (data_type_string_lookup.find(dt) != data_type_string_lookup.end()) {
        return data_type_string_lookup[dt];
    }

    std::cerr << "no string defined for data type\n";
    assert(false);
}

std::ostream &operator<<(std::ostream &os, const DataType &dt) {
    os << data_type_to_string(dt);
    return os;
}

}