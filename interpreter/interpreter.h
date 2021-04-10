#pragma once

#include "../ast.h"
#include "value.h"
#include "scope.h"

namespace mango {

class Interpreter {
  ScopeStack scope_stack;

  Value execute_binary_expression(BinaryExpression *e);
  Value execute_identifier_expression(IdentifierExpression *e);
  Value execute_integer_literal_expression(IntegerLiteralExpression *e);
  Value execute_string_literal_expression(StringLiteralExpression *e);
  Value execute_object_expression(ObjectExpression *e);
  Value execute_array_expression(ArrayExpression *e);
  Value execute_function_expression(FunctionExpression *e);
  Value execute_function_call_expression(FunctionCallExpression *e);
  Value execute_member_expression(MemberExpression *expression);
  Value execute_assignment_expression(AssignmentExpression *e);
  Value execute_expression(Expression *expression);
  Value execute_declaration_statement(DeclarationStatement *s);
  Value execute_expression_statement(ExpressionStatement *s);
  Value execute_block_statement(BlockStatement *bs);
  Value execute_return_statement(ReturnStatement *s);
  Value execute_if_statement(IfStatement *s);
  Value execute_while_statement(WhileStatement *s);
  Value execute_statement(Statement *statement);

 public:
  Value run(Program ast);
};

}