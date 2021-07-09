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


#define INVALID_BINARY_OPERATION(LEFT, RIGHT, OPERATOR) \
     std::cerr << "operator " << OPERATOR << " cannot be applied to types " \
     << LEFT->type() << " and " << RIGHT->type() << "\n"; \
     assert(false);

#define BINARY_MATHS_OPERATION(TYPE, OPERATOR, LEFT, RIGHT) \
    if (LEFT->type() == DataType::TYPE) { \
        auto l = static_cast<interpreter::TYPE*>(LEFT); \
        auto r = static_cast<interpreter::TYPE*>(RIGHT); \
        return new interpreter::TYPE(l->value OPERATOR r->value); \
    }

#define BINARY_LOGIC_OPERATION(TYPE, OPERATOR, LEFT, RIGHT) \
    if (LEFT->type() == DataType::TYPE) { \
        auto l = static_cast<interpreter::TYPE*>(LEFT); \
        auto r = static_cast<interpreter::TYPE*>(RIGHT); \
        return new interpreter::Bool(l->value OPERATOR r->value); \
    }

interpreter::Object* add(interpreter::Object* left, interpreter::Object* right) {
    BINARY_MATHS_OPERATION(Integer, +, left, right);
    BINARY_MATHS_OPERATION(String, +, left, right);
    INVALID_BINARY_OPERATION(left, right, "+")
}

interpreter::Object* subract(interpreter::Object* left, interpreter::Object* right) {
    BINARY_MATHS_OPERATION(Integer, -, left, right);
    INVALID_BINARY_OPERATION(left, right, "-")
}

interpreter::Object* multiply(interpreter::Object* left, interpreter::Object* right) {
    BINARY_MATHS_OPERATION(Integer, *, left, right);
    INVALID_BINARY_OPERATION(left, right, "*")
}

interpreter::Object* divide(interpreter::Object* left, interpreter::Object* right) {
    BINARY_MATHS_OPERATION(Integer, /, left, right);
    INVALID_BINARY_OPERATION(left, right, "/")
}

interpreter::Object* equal_to(interpreter::Object* left, interpreter::Object* right) {
    BINARY_LOGIC_OPERATION(Integer, ==, left, right);
    BINARY_LOGIC_OPERATION(String, ==, left, right);
    BINARY_LOGIC_OPERATION(Bool, ==, left, right);
    INVALID_BINARY_OPERATION(left, right, "==")
}

interpreter::Object* not_equal_to(interpreter::Object* left, interpreter::Object* right) {
    BINARY_LOGIC_OPERATION(Integer, !=, left, right);
    BINARY_LOGIC_OPERATION(String, !=, left, right);
    BINARY_LOGIC_OPERATION(Bool, !=, left, right);
    INVALID_BINARY_OPERATION(left, right, "!=")
}

interpreter::Object* greater_than(interpreter::Object* left, interpreter::Object* right) {
    BINARY_LOGIC_OPERATION(Integer, >, left, right);
    BINARY_LOGIC_OPERATION(Bool, >, left, right);
    INVALID_BINARY_OPERATION(left, right, ">")
}

interpreter::Object* greater_than_or_equal_to(interpreter::Object* left, interpreter::Object* right) {
    BINARY_LOGIC_OPERATION(Integer, >=, left, right);
    BINARY_LOGIC_OPERATION(Bool, >=, left, right);
    INVALID_BINARY_OPERATION(left, right, ">=")
}

interpreter::Object* less_than(interpreter::Object* left, interpreter::Object* right) {
    BINARY_LOGIC_OPERATION(Integer, <, left, right);
    BINARY_LOGIC_OPERATION(Bool, <, left, right);
    INVALID_BINARY_OPERATION(left, right, "<")
}

interpreter::Object* less_than_or_equal_to(interpreter::Object* left, interpreter::Object* right) {
    BINARY_LOGIC_OPERATION(Integer, <=, left, right);
    BINARY_LOGIC_OPERATION(Bool, <=, left, right);
    INVALID_BINARY_OPERATION(left, right, "<=")
}

interpreter::Object* logical_and(interpreter::Object* left, interpreter::Object* right) {
    BINARY_LOGIC_OPERATION(Integer, &&, left, right);
    BINARY_LOGIC_OPERATION(Bool, &&, left, right);
    INVALID_BINARY_OPERATION(left, right, "&&")
}

interpreter::Object* logical_or(interpreter::Object* left, interpreter::Object* right) {
    BINARY_LOGIC_OPERATION(Integer, ||, left, right);
    BINARY_LOGIC_OPERATION(Bool, ||, left, right);
    INVALID_BINARY_OPERATION(left, right, "||")
}

interpreter::Object* BinaryExpression::execute(interpreter::Interpreter &interpreter) {
    auto l = left->execute(interpreter);
    auto r = right->execute(interpreter);

    // TODO: operator precedence

    // this is a simple solution for now but we can probably allow some
    // automatic type converting for operations on values of different types
    if (l->type() != r->type()) {
        std::cerr << "operator " << op << " cannot be applied to types " << l->type() << " and " << r->type()
                  << "\n";
        assert(false);
    }

    switch (op) {
        case Operator::Plus:
            return add(l, r);
        case Operator::Minus:
            return subract(l, r);
        case Operator::Multiply:
            return multiply(l, r);
        case Operator::Divide:
            return divide(l, r);
        case Operator::EqualTo:
            return equal_to(l, r);
        case Operator::NotEqualTo:
            return not_equal_to(l, r);
        case Operator::GreaterThan:
            return greater_than(l, r);
        case Operator::GreaterThanOrEqualTo:
            return greater_than_or_equal_to(l, r);
        case Operator::LessThan:
            return less_than(l, r);
        case Operator::LessThanOrEqualTo:
            return less_than_or_equal_to(l, r);
        case Operator::And:
            return logical_and(l, r);
        case Operator::Or:
            return logical_or(l, r);
        case Operator::Not:
            std::cerr << "operator \"!\" is not a binary operator\n";
            assert(false);
    }

    std::cerr << "unhandled operator " << op << "\n";
    assert(false);
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

interpreter::Object* UnaryExpression::execute(interpreter::Interpreter &interpreter) {
    auto arg = argument->execute(interpreter);

    switch (op) {
        case Operator::Not: {
            assert(arg->type() == DataType::Bool);
            auto bool_arg = static_cast<interpreter::Bool*>(arg);
            return new interpreter::Bool(!bool_arg->value);
        }
        case Operator::Plus:
        case Operator::Minus:
        case Operator::Multiply:
        case Operator::Divide:
        case Operator::EqualTo:
        case Operator::NotEqualTo:
        case Operator::GreaterThan:
        case Operator::GreaterThanOrEqualTo:
        case Operator::LessThan:
        case Operator::LessThanOrEqualTo:
        case Operator::And:
        case Operator::Or:
            std::cerr << "invalid operator " << op;
            assert(false);
    }

    std::cerr << "unhandled operator " << op << "\n";
    assert(false);
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

interpreter::Object* UndefinedExpression::execute(interpreter::Interpreter &interpreter) {
    return new interpreter::Undefined();
}

void IdentifierExpression::print(string_builder::StringBuilder* sb) {
    sb->append_no_indent("IdentifierExpression { value: ");
    sb->append_no_indent(value);
    sb->append_no_indent(" }");
}

interpreter::Object* IdentifierExpression::execute(interpreter::Interpreter &interpreter) {
    return interpreter.lookup_variable(value);
}

void IdentifierExpression::generate(string_builder::StringBuilder* sb) {
    sb->append_no_indent(value);
}

void IntegerLiteralExpression::print(string_builder::StringBuilder* sb) {
    sb->append_no_indent("IntegerLiteralExpression { value: ");
    sb->append_no_indent(std::to_string(value));
    sb->append_no_indent(" }");
}

interpreter::Object* IntegerLiteralExpression::execute(interpreter::Interpreter &interpreter) {
    return new interpreter::Integer(value);
}

void IntegerLiteralExpression::generate(string_builder::StringBuilder* sb) {
    sb->append_no_indent(std::to_string(value));
}

void StringLiteralExpression::print(string_builder::StringBuilder* sb) {
    sb->append_no_indent("StringLiteralExpression { value: ");
    sb->append_no_indent(value);
    sb->append_no_indent(" }");
}

interpreter::Object* StringLiteralExpression::execute(interpreter::Interpreter &interpreter) {
    return new interpreter::String(value);
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

interpreter::Object* BooleanLiteralExpression::execute(interpreter::Interpreter &interpreter) {
    return new interpreter::Bool(value);
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

interpreter::Object* FunctionExpression::execute(interpreter::Interpreter &interpreter) {
    auto obj = new interpreter::Function();
    obj->parameters = parameters;
    obj->body = body;
    return obj;
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

interpreter::Object* ExpressionStatement::execute(interpreter::Interpreter &interpreter) {
    return value->execute(interpreter);
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

interpreter::Object* WhileStatement::execute(interpreter::Interpreter &interpreter) {
    while (condition->execute(interpreter)->is_truthy()) {
        body->execute(interpreter);
    }

    return new interpreter::Undefined();
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

interpreter::Object* IfStatement::execute(interpreter::Interpreter &interpreter) {
    auto condition_value = condition->execute(interpreter);
    if (condition_value->is_truthy()) {
        if_block->execute(interpreter);
    } else if (else_block != nullptr) {
        else_block->execute(interpreter);
    }

    return new interpreter::Undefined();
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

interpreter::Object* ReturnStatement::execute(interpreter::Interpreter &interpreter) {
    return value->execute(interpreter);
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

interpreter::Object* DeclarationStatement::execute(interpreter::Interpreter &interpreter) {
    return interpreter.set_variable(identifier, value->execute(interpreter));
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

interpreter::Object* BlockStatement::execute(interpreter::Interpreter &interpreter) {
    interpreter::Object* v = new interpreter::Undefined();

    for (auto s: statements) {
        v = s->execute(interpreter);
    }

    return v;
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

interpreter::Object* AssignmentExpression::execute(interpreter::Interpreter &interpreter) {
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

interpreter::Object* FunctionCallExpression::execute(interpreter::Interpreter &interpreter) {
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

interpreter::Object* MemberExpression::execute(interpreter::Interpreter &interpreter) {
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

interpreter::Object* ArrayExpression::execute(interpreter::Interpreter &interpreter) {
    auto obj = new interpreter::Array();

    for (auto el : elements) {
        obj->elements.push_back(el->execute(interpreter));
    }

    return obj;
}

void ObjectExpression::print(string_builder::StringBuilder* sb) {
    sb->append_line_no_indent("ObjectExpression {");
    sb->append_line("}");
}

interpreter::Object* ObjectExpression::execute(interpreter::Interpreter &interpreter) {
    auto obj = new interpreter::Object();

    for (auto prop : properties) {
        obj->properties[prop.first] = prop.second->execute(interpreter);
    }

    return obj;
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

interpreter::Object* Program::execute(interpreter::Interpreter &interpreter) {
    interpreter::Object* v = new interpreter::Undefined();

    for (auto s : statements) {
        v = s->execute(interpreter);
    }

    return v;
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