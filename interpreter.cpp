#include "interpreter.h"

namespace mango {

std::ostream &operator<<(std::ostream &os, const Value &v) {
    switch (v.type) {
        case DataType::Undefined:
            os << "undefined";
            return os;
        case DataType::Integer:
            os << std::get<int>(v.value);
            return os;
        case DataType::String:
            os << "\"" << std::get<std::string>(v.value) << "\"";
            return os;
        case DataType::Bool:
            os << (std::get<bool>(v.value) ? "true" : "false");
            return os;
        case DataType::Function:
            os << "function";
            return os;
    }

    std::cerr << "\nunknown value to convert to string\n";
    assert(false);
}

bool value_is_truthy(Value v) {
    switch (v.type) {
        case DataType::Undefined:
            return false;
        case DataType::Integer:
            return std::get<int>(v.value) != 0;
        case DataType::String:
            return true;
        case DataType::Bool:
            return std::get<bool>(v.value);
        case DataType::Function:
            return true;
    }

    std::cerr << "\nunknown value to check truthyness\n";
    assert(false);
}

auto add_visitor = INTEGER_AND_STRING_OPERATIONS_VISITOR(+);
auto subtract_visitor = INTEGER_OPERATIONS_VISITOR(-);
auto multiply_visitor = INTEGER_OPERATIONS_VISITOR(-);
auto divide_visitor = INTEGER_OPERATIONS_VISITOR(/);
auto equal_to_visitor = INTEGER_AND_STRING_LOGICAL_OPERATIONS_VISITOR(==);
auto not_equal_to_visitor = INTEGER_AND_STRING_LOGICAL_OPERATIONS_VISITOR(!=);
auto less_than_visitor = INTEGER_LOGICAL_OPERATIONS_VISITOR(<);
auto less_than_or_equal_to_visitor = INTEGER_LOGICAL_OPERATIONS_VISITOR(<=);
auto greater_than_visitor = INTEGER_LOGICAL_OPERATIONS_VISITOR(>);
auto greater_than_or_equal_to_visitor = INTEGER_LOGICAL_OPERATIONS_VISITOR(>=);


Value Interpreter::execute_binary_expression(BinaryExpression *e) {
    auto l = execute_expression(e->left);
    auto r = execute_expression(e->right);

    // TODO: operator precedence

    // this is a simple solution for now but we can probably allow some
    // automatic type converting for operations on values of different types
    if (l.type != r.type) {
        std::cerr << "operator " << e->op << " cannot be applied to types " << l.type << " and " << r.type << "\n";
        assert(false);
    }

    switch (e->op) {
        case Operator::Plus:
            return std::visit(add_visitor, l.value, r.value);
        case Operator::Minus:
            return std::visit(subtract_visitor, l.value, r.value);
        case Operator::Multiply:
            return std::visit(multiply_visitor, l.value, r.value);
        case Operator::Divide:
            return std::visit(divide_visitor, l.value, r.value);
        case Operator::EqualTo:
            return std::visit(equal_to_visitor, l.value, r.value);
        case Operator::NotEqualTo:
            return std::visit(not_equal_to_visitor, l.value, r.value);
        case Operator::GreaterThan:
            return std::visit(greater_than_visitor, l.value, r.value);
        case Operator::GreaterThanOrEqualTo:
            return std::visit(greater_than_or_equal_to_visitor, l.value, r.value);
        case Operator::LessThan:
            return std::visit(less_than_visitor, l.value, r.value);
        case Operator::LessThanOrEqualTo:
            return std::visit(less_than_or_equal_to_visitor, l.value, r.value);
        case Operator::Not:
            std::cerr << "operator \"!\" is not a binary operator\n";
            assert(false);
    }

    std::cerr << "unhandled operator" << (int) e->op << "\n";
    assert(false);
}

Value Interpreter::execute_identifier_expression(IdentifierExpression *e) {
    return call_stack.lookup_variable(e->value);
}

Value Interpreter::execute_integer_literal_expression(IntegerLiteralExpression *e) {
    return Value{DataType::Integer, e->value};
}

Value Interpreter::execute_string_literal_expression(StringLiteralExpression *e) {
    Value v;
    v.type = DataType::String;
    v.value = e->value;
    return v;
}

Value Interpreter::execute_function_expression(FunctionExpression *e) {
    Value v;
    v.type = DataType::Function;
    v.value = Function{e->parameters, e->body};
    return v;
}

Value Interpreter::execute_function_call_expression(FunctionCallExpression *e) {
    auto function_value = call_stack.lookup_variable(e->value);
    if (function_value.type == DataType::Undefined) {
        std::cerr << "reference error: no definition found for identifier \"" << e->value << "\"\n";
        assert(false);
    }

    auto fn = std::get<Function>(function_value.value);

    if (fn.is_builtin) {
        std::vector<Value> args;

        for (int i = 0; i < e->arguments.size(); i++) {
            args.push_back(execute_expression(e->arguments[i]));
        }

        return fn.builtin_fn(args);
    } else {
        call_stack.new_frame();

        for (int i = 0; i < e->arguments.size(); i++) {


            auto name = fn.parameters[i];
            call_stack.set_variable(name, execute_expression(e->arguments[i]));
        }

        auto return_value = execute_statement(fn.body);

        call_stack.pop_frame();

        return return_value;
    }
}

Value Interpreter::execute_expression(Expression *expression) {
    if (auto e = dynamic_cast<IdentifierExpression *>(expression)) {
        return execute_identifier_expression(e);
    } else if (auto e = dynamic_cast<BinaryExpression *>(expression)) {
        return execute_binary_expression(e);
    } else if (auto e = dynamic_cast<IntegerLiteralExpression *>(expression)) {
        return execute_integer_literal_expression(e);
    } else if (auto e = dynamic_cast<StringLiteralExpression *>(expression)) {
        return execute_string_literal_expression(e);
    } else if (auto e = dynamic_cast<FunctionExpression *>(expression)) {
        return execute_function_expression(e);
    } else if (auto e = dynamic_cast<FunctionCallExpression *>(expression)) {
        return execute_function_call_expression(e);
    } else {
        std::cerr << "unknown expression type\n";
        assert(false);
    }
}

Value Interpreter::execute_declaration_statement(DeclarationStatement *s) {
    return call_stack.set_variable(s->identifier, execute_expression(s->value));
}

Value Interpreter::execute_assignment_statement(AssignmentStatement *s) {
    return call_stack.set_variable(s->identifier, execute_expression(s->value));
}

Value Interpreter::execute_expression_statement(ExpressionStatement *s) {
    return execute_expression(s->value);
}

Value Interpreter::execute_block_statement(BlockStatement *bs) {
    Value v;

    for (auto s: bs->statements) {
        v = execute_statement(s);
    }

    return v;
}

Value Interpreter::execute_return_statement(ReturnStatement *s) {
    return execute_expression(s->value);
}

Value Interpreter::execute_if_statement(IfStatement *s) {
    auto condition_value = execute_expression(s->condition);
    if (value_is_truthy(condition_value)) {
        execute_statement(s->if_block);
    } else if (s->else_block != nullptr) {
        execute_statement(s->else_block);
    }

    return Value{};
}

Value Interpreter::execute_while_statement(WhileStatement *s) {
    while (value_is_truthy(execute_expression(s->condition))) {
        execute_statement(s->body);
    }

    return Value{};
}

Value Interpreter::execute_statement(Statement *statement) {
    if (auto s = dynamic_cast<DeclarationStatement *>(statement)) {
        return execute_declaration_statement(s);
    } else if (auto s = dynamic_cast<ExpressionStatement *>(statement)) {
        return execute_expression_statement(s);
    } else if (auto s = dynamic_cast<AssignmentStatement *>(statement)) {
        return execute_assignment_statement(s);
    } else if (auto s = dynamic_cast<BlockStatement *>(statement)) {
        return execute_block_statement(s);
    } else if (auto s = dynamic_cast<ReturnStatement *>(statement)) {
        return execute_return_statement(s);
    } else if (auto s = dynamic_cast<IfStatement *>(statement)) {
        return execute_if_statement(s);
    } else if (auto s = dynamic_cast<WhileStatement *>(statement)) {
        return execute_while_statement(s);
    } else {
        std::cerr << "unknown statement type\n";
        assert(false);
    }
}

Interpreter::Interpreter() {
    // TODO: builtin function setup needs to be generalised
    Value print;
    print.type = DataType::Function;
    Function fn;
    fn.is_builtin = true;
    fn.builtin_fn = [&](std::vector<Value> args) -> Value {
        std::cout << args[0] << "\n";
        return Value{};
    };
    print.value = fn;

    call_stack.set_variable("print", print);
}

Value Interpreter::run(Program ast) {
    Value v;

    for (auto s : ast.statements) {
        v = execute_statement(s);
    }

    return v;
}

}
