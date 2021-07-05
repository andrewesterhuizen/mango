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
        case Operator::Not:
            std::cerr << "operator \"!\" is not a binary operator\n";
            assert(false);
    }

    std::cerr << "unhandled operator " << op << "\n";
    assert(false);
}


}