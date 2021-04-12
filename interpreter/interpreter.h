#pragma once

#include "../ast.h"
#include "types.h"
#include "call_stack.h"

namespace mango {

class Interpreter {
    CallStack call_stack;

    Object* execute_binary_expression(BinaryExpression* e);
    Object* execute_identifier_expression(IdentifierExpression* e);
    Object* execute_integer_literal_expression(IntegerLiteralExpression* e);
    Object* execute_string_literal_expression(StringLiteralExpression* e);
    Object* execute_object_expression(ObjectExpression* e);
    Object* execute_array_expression(ArrayExpression* e);
    Object* execute_function_expression(FunctionExpression* e);
    Object* execute_function_call_expression(FunctionCallExpression* e);
    Object* execute_member_expression(MemberExpression* expression);
    Object* execute_assignment_expression(AssignmentExpression* e);
    Object* execute_expression(Expression* expression);
    Object* execute_declaration_statement(DeclarationStatement* s);
    Object* execute_expression_statement(ExpressionStatement* s);
    Object* execute_block_statement(BlockStatement* bs);
    Object* execute_return_statement(ReturnStatement* s);
    Object* execute_if_statement(IfStatement* s);
    Object* execute_while_statement(WhileStatement* s);
    Object* execute_statement(Statement* statement);

public:
    Object* run(Program ast);
};

}