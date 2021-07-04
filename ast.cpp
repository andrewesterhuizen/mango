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

void statement_to_string(string_builder::StringBuilder* sb, Statement* statement) {
    auto type = statement->type();

    switch (type) {
        case StatementType::Declaration: {
            auto s = static_cast<DeclarationStatement*>(statement);
            sb->append_line("DeclarationStatement {");
            sb->increase_indent();
            sb->append("type: ");
            sb->append_line_no_indent(data_type_to_string(s->data_type));
            sb->append("identifier: ");
            sb->append_line_no_indent(s->identifier);
            sb->append("value: ");
            s->value->print(sb);
            sb->append_line_no_indent("");
            sb->decrease_indent();
            sb->append_line("}");
            break;
        }
        case StatementType::Expression: {
            auto s = static_cast<ExpressionStatement*>(statement);
            sb->append_line("ExpressionStatement {");
            sb->increase_indent();
            sb->append("value: ");
            s->value->print(sb);
            sb->append_line("");
            sb->decrease_indent();
            sb->append_line("}");
            break;
        }
        case StatementType::Block: {
            auto s = static_cast<BlockStatement*>(statement);
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
            break;
        }
        case StatementType::Return: {
            auto s = static_cast<ReturnStatement*>(statement);
            sb->append_line("ReturnStatement {");
            sb->increase_indent();
            sb->append("value: ");
            s->value->print(sb);
            sb->append_line("");
            sb->decrease_indent();
            sb->append_line("}");
            break;
        }
        case StatementType::If: {
            auto s = static_cast<IfStatement*>(statement);
            sb->append_line("IfStatement {");
            sb->increase_indent();
            sb->append("condition: ");
            s->condition->print(sb);
            sb->append_line("");
            sb->append("if_block: ");
            statement_to_string(sb, s->if_block);
            sb->append_line("");
            sb->append("else_block: ");
            statement_to_string(sb, s->if_block);
            sb->append_line("");
            sb->decrease_indent();
            sb->append_line("}");
            break;
        }
        case StatementType::While: {
            auto s = static_cast<WhileStatement*>(statement);
            sb->append_line("WhileStatement {");
            sb->increase_indent();
            sb->append("condition: ");
            s->condition->print(sb);
            sb->append_line("");
            sb->append("body: ");
            statement_to_string(sb, s->body);
            sb->append_line("");
            sb->decrease_indent();
            sb->append_line("}");
            break;
        }
    }
    std::cerr << "unknown statement\n";
    assert(false);
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