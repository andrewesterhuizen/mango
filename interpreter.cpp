#include "interpreter.h"

namespace mango {

std::ostream &operator<<(std::ostream &os, const Value &v) {
    switch (v.type) {
        case DataType::Undefined:
            os << "undefined";
            return os;
        case DataType::Integer:
            os << v.value_int;
            return os;
        case DataType::String:
            os << "\"" << v.value_string << "\"";
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
            return v.value_int != 0;
        case DataType::String:
            return true;
        case DataType::Function:
            return true;
    }

    std::cerr << "\nunknown value to check truthyness\n";
    assert(false);
}

Value Interpreter::execute_binary_expression(BinaryExpression *e) {
    auto l = execute_expression(e->left);
    auto r = execute_expression(e->right);

    // TODO: handle non integer types
    // TODO: operator precedence

    switch (e->op) {
        case Operator::Plus:
            return Value{DataType::Integer, l.value_int + r.value_int};
        case Operator::Minus:
            return Value{DataType::Integer, l.value_int - r.value_int};
        case Operator::Multiply:
            return Value{DataType::Integer, l.value_int * r.value_int};
        case Operator::Divide:
            return Value{DataType::Integer, l.value_int / r.value_int};
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
    v.value_string = e->value;
    return v;
}

Value Interpreter::execute_function_expression(FunctionExpression *e) {
    Value v;
    v.type = DataType::Function;
    v.value_function = Function{e->parameters, e->body};
    return v;
}

Value Interpreter::execute_function_call_expression(FunctionCallExpression *e) {
    auto fn = call_stack.lookup_variable(e->value);
    if (fn.type == DataType::Undefined) {
        std::cerr << "reference error: no definition found for identifier \"" << e->value << "\"\n";
        assert(false);
    }

    if (fn.value_function.is_builtin) {
        std::vector<Value> args;

        for (int i = 0; i < e->arguments.size(); i++) {
            args.push_back(execute_expression(e->arguments[i]));
        }

        return fn.value_function.builtin_fn(args);
    } else {
        call_stack.new_frame();

        for (int i = 0; i < e->arguments.size(); i++) {


            auto name = fn.value_function.parameters[i];
            call_stack.set_variable(name, execute_expression(e->arguments[i]));
        }

        auto return_value = execute_statement(fn.value_function.body);

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
    } else if(s->else_block != nullptr) {
        execute_statement(s->else_block);
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
    } else {
        std::cerr << "unknown statement type\n";
        assert(false);
    }
}

Interpreter::Interpreter() {
    // TODO: builtin function setup needs to be generalised
    Value print;
    print.type = DataType::Function;
    print.value_function.is_builtin = true;
    auto fn = [&](std::vector<Value> args) -> Value {
        std::cout << args[0] << "\n";
        return Value{};
    };
    print.value_function.builtin_fn = fn;

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
