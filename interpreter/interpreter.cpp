#include "interpreter.h"

namespace mango {

#define INVALID_BINARY_OPERATION(LEFT, RIGHT, OPERATOR) \
     std::cerr << "operator " << OPERATOR << " cannot be applied to types " \
     << LEFT->type() << " and " << RIGHT->type() << "\n"; \
     assert(false);

#define BINARY_MATHS_OPERATION(TYPE, OPERATOR, LEFT, RIGHT) \
    if (LEFT->type() == DataType::TYPE) { \
        auto l = static_cast<TYPE*>(LEFT); \
        auto r = static_cast<TYPE*>(RIGHT); \
        return new TYPE(l->value OPERATOR r->value); \
    }

#define BINARY_LOGIC_OPERATION(TYPE, OPERATOR, LEFT, RIGHT) \
    if (LEFT->type() == DataType::TYPE) { \
        auto l = static_cast<TYPE*>(LEFT); \
        auto r = static_cast<TYPE*>(RIGHT); \
        return new Bool(l->value OPERATOR r->value); \
    }

Object* add(Object* left, Object* right) {
    BINARY_MATHS_OPERATION(Integer, +, left, right);
    BINARY_MATHS_OPERATION(String, +, left, right);
    INVALID_BINARY_OPERATION(left, right, "+")
}

Object* subract(Object* left, Object* right) {
    BINARY_MATHS_OPERATION(Integer, -, left, right);
    INVALID_BINARY_OPERATION(left, right, "-")
}

Object* multiply(Object* left, Object* right) {
    BINARY_MATHS_OPERATION(Integer, *, left, right);
    INVALID_BINARY_OPERATION(left, right, "*")
}

Object* divide(Object* left, Object* right) {
    BINARY_MATHS_OPERATION(Integer, /, left, right);
    INVALID_BINARY_OPERATION(left, right, "/")
}

Object* equal_to(Object* left, Object* right) {
    BINARY_LOGIC_OPERATION(Integer, ==, left, right);
    BINARY_LOGIC_OPERATION(String, ==, left, right);
    BINARY_LOGIC_OPERATION(Bool, ==, left, right);
    INVALID_BINARY_OPERATION(left, right, "==")
}

Object* not_equal_to(Object* left, Object* right) {
    BINARY_LOGIC_OPERATION(Integer, !=, left, right);
    BINARY_LOGIC_OPERATION(String, !=, left, right);
    BINARY_LOGIC_OPERATION(Bool, !=, left, right);
    INVALID_BINARY_OPERATION(left, right, "!=")
}

Object* greater_than(Object* left, Object* right) {
    BINARY_LOGIC_OPERATION(Integer, >, left, right);
    BINARY_LOGIC_OPERATION(Bool, >, left, right);
    INVALID_BINARY_OPERATION(left, right, ">")
}

Object* greater_than_or_equal_to(Object* left, Object* right) {
    BINARY_LOGIC_OPERATION(Integer, >=, left, right);
    BINARY_LOGIC_OPERATION(Bool, >=, left, right);
    INVALID_BINARY_OPERATION(left, right, ">=")
}

Object* less_than(Object* left, Object* right) {
    BINARY_LOGIC_OPERATION(Integer, <, left, right);
    BINARY_LOGIC_OPERATION(Bool, <, left, right);
    INVALID_BINARY_OPERATION(left, right, "<")
}

Object* less_than_or_equal_to(Object* left, Object* right) {
    BINARY_LOGIC_OPERATION(Integer, <=, left, right);
    BINARY_LOGIC_OPERATION(Bool, <=, left, right);
    INVALID_BINARY_OPERATION(left, right, "<=")
}

Object* Interpreter::execute_binary_expression(BinaryExpression* e) {
    auto l = execute_expression(e->left);
    auto r = execute_expression(e->right);

    // TODO: operator precedence

    // this is a simple solution for now but we can probably allow some
    // automatic type converting for operations on values of different types
    if (l->type() != r->type()) {
        std::cerr << "operator " << e->op << " cannot be applied to types " << l->type() << " and " << r->type()
                  << "\n";
        assert(false);
    }

    switch (e->op) {
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
        case Operator::Not:
            std::cerr << "operator \"!\" is not a binary operator\n";
            assert(false);
    }

    std::cerr << "unhandled operator " << e->op << "\n";
    assert(false);
}

Object* Interpreter::execute_identifier_expression(IdentifierExpression* e) {
    return call_stack.lookup_variable(e->value);
}

Object* Interpreter::execute_integer_literal_expression(IntegerLiteralExpression* e) {
    return new Integer(e->value);
}

Object* Interpreter::execute_string_literal_expression(StringLiteralExpression* e) {
    return new String(e->value);
}

Object* Interpreter::execute_member_expression(MemberExpression* e) {
    auto variable = call_stack.lookup_variable(e->identifier);
    if (variable->type() == DataType::Undefined) {
        std::cerr << "reference error: no definition found for identifier \"" << e->identifier << "\"\n";
        assert(false);
    }

    if (auto array = dynamic_cast<Array*>(variable)) {
        auto property = execute_expression(e->property);
        auto property_integer = dynamic_cast<Integer*>(property);

        // TODO: handle built in properties like .size for arrays
        if (property_integer == nullptr) {
            std::cerr << "unsupported data type for property lookup\n";
            assert(false);
        }

        // element lookup
        auto index = property_integer->value;
        if (index >= array->elements.size()) {
            return new Undefined();
        }

        return array->elements.at(index);
    } else {
        auto property = execute_expression(e->property);

        std::string property_string;
        if (auto str = dynamic_cast<String*>(property)) {
            property_string = str->value;
        } else {
            property_string = property->to_string();
        }

        auto v = variable->properties.find(property_string);
        if (v == variable->properties.end()) {
            return new Undefined();
        }

        return v->second;
    }

    std::cerr << "unsupported data type for property lookup\n";
    assert(false);
}

Object* Interpreter::execute_assignment_expression(AssignmentExpression* expression) {
    auto right_value = execute_expression(expression->right);

    if (auto e = dynamic_cast<IdentifierExpression*>(expression->left)) {
        call_stack.set_variable(e->value, right_value);
        return right_value;
    } else if (auto e = dynamic_cast<MemberExpression*>(expression->left)) {
        auto variable = call_stack.lookup_variable(e->identifier);
        if (variable->type() == DataType::Undefined) {
            std::cerr << "reference error: no definition found for identifier \"" << e->identifier << "\"\n";
            assert(false);
        }

        auto name = execute_expression(e->property);
        auto name_string = dynamic_cast<String*>(name);
        assert(name_string != nullptr);

        auto v = variable->properties.find(name_string->value);
        variable->properties[name_string->value] = right_value;
        return right_value;
    }

    assert(false);
}

Object* Interpreter::execute_object_expression(ObjectExpression* e) {
    auto obj = new Object();

    for (auto prop : e->properties) {
        obj->properties[prop.first] = execute_expression(prop.second);
    }

    return obj;
}

Object* Interpreter::execute_array_expression(ArrayExpression* e) {
    auto obj = new Array();

    for (auto el : e->elements) {
        obj->elements.push_back(execute_expression(el));
    }

    return obj;
}

Object* Interpreter::execute_function_expression(FunctionExpression* e) {
    auto obj = new Function();
    obj->parameters = e->parameters;
    obj->body = e->body;
    return obj;
}

Object* Interpreter::execute_function_call_expression(FunctionCallExpression* e) {
    auto obj = call_stack.lookup_variable(e->value);
    if (obj->type() == DataType::Undefined) {
        std::cerr << "reference error: no definition found for identifier \"" << e->value << "\"\n";
        assert(false);
    }

    auto function = dynamic_cast<Function*>(obj);
    if (function == nullptr) {
        std::cerr << "identifier " << e->value << " is not a function\n";
        assert(false);
    }

    if (function->is_builtin) {
        std::vector<Object*> args;

        for (int i = 0; i < e->arguments.size(); i++) {
            args.push_back(execute_expression(e->arguments[i]));
        }

        return function->builtin_fn(args);
    } else {
        call_stack.new_frame(e->value);

        for (int i = 0; i < e->arguments.size(); i++) {
            auto identifier = function->parameters[i];
            call_stack.set_variable(identifier, execute_expression(e->arguments[i]));
        }

        auto return_value = execute_statement(function->body);

        call_stack.pop_frame();

        return return_value;
    }
}

Object* Interpreter::execute_expression(Expression* expression) {
    if (auto e = dynamic_cast<IdentifierExpression*>(expression)) {
        return execute_identifier_expression(e);
    } else if (auto e = dynamic_cast<BinaryExpression*>(expression)) {
        return execute_binary_expression(e);
    } else if (auto e = dynamic_cast<IntegerLiteralExpression*>(expression)) {
        return execute_integer_literal_expression(e);
    } else if (auto e = dynamic_cast<StringLiteralExpression*>(expression)) {
        return execute_string_literal_expression(e);
    } else if (auto e = dynamic_cast<FunctionExpression*>(expression)) {
        return execute_function_expression(e);
    } else if (auto e = dynamic_cast<FunctionCallExpression*>(expression)) {
        return execute_function_call_expression(e);
    } else if (auto e = dynamic_cast<ObjectExpression*>(expression)) {
        return execute_object_expression(e);
    } else if (auto e = dynamic_cast<ArrayExpression*>(expression)) {
        return execute_array_expression(e);
    } else if (auto e = dynamic_cast<MemberExpression*>(expression)) {
        return execute_member_expression(e);
    } else if (auto e = dynamic_cast<AssignmentExpression*>(expression)) {
        return execute_assignment_expression(e);
    } else {
        std::cerr << "unknown expression type\n";
        assert(false);
    }
}

Object* Interpreter::execute_declaration_statement(DeclarationStatement* s) {
    return call_stack.set_variable(s->identifier, execute_expression(s->value));
}

Object* Interpreter::execute_expression_statement(ExpressionStatement* s) {
    return execute_expression(s->value);
}

Object* Interpreter::execute_block_statement(BlockStatement* bs) {
    Object* v;

    for (auto s: bs->statements) {
        v = execute_statement(s);
    }

    return v;
}

Object* Interpreter::execute_return_statement(ReturnStatement* s) {
    return execute_expression(s->value);
}

Object* Interpreter::execute_if_statement(IfStatement* s) {
    auto condition_value = execute_expression(s->condition);
    if (condition_value->is_truthy()) {
        execute_statement(s->if_block);
    } else if (s->else_block != nullptr) {
        execute_statement(s->else_block);
    }

    return new Undefined();
}

Object* Interpreter::execute_while_statement(WhileStatement* s) {
    while (execute_expression(s->condition)->is_truthy()) {
        execute_statement(s->body);
    }

    return new Undefined();
}

Object* Interpreter::execute_statement(Statement* statement) {
    switch (statement->type()) {
        case StatementType::Declaration:
            return execute_declaration_statement(static_cast<DeclarationStatement*>(statement));
        case StatementType::Expression:
            return execute_expression_statement(static_cast<ExpressionStatement*>(statement));
        case StatementType::Block:
            return execute_block_statement(static_cast<BlockStatement*>(statement));
        case StatementType::Return:
            return execute_return_statement(static_cast<ReturnStatement*>(statement));
        case StatementType::If:
            return execute_if_statement(static_cast<IfStatement*>(statement));
        case StatementType::While:
            return execute_while_statement(static_cast<WhileStatement*>(statement));

    }

    std::cerr << "unknown statement type\n";
    assert(false);
}

Object* Interpreter::run(Program ast) {
    Object* v;

    for (auto s : ast.statements) {
        v = execute_statement(s);
    }

    return v;
}

}
