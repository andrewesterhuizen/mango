#pragma once

#include <vector>
#include <exception>
#include <cstdlib>

#include "token.h"
#include "ast.h"

// TODO: we just crash for now, but we should have
//  a way of returning a helpful error in the future
#define UNEXPECTED_TOKEN(t) \
std::cerr << "unexpected token \"" << t.value << "\"\n"; \
assert(false);

namespace mango {

class Parser {
    int index = 0;
    std::vector<Token> tokens;

    Token current_token();
    Token next_token();
    Token peek_next_token();
    void backup();
    Token expect(TokenType type);
    void expect_optional(TokenType type);
    Operator get_operator();
    Statement* get_block_statement();
    Statement* get_declaration_statement();
    Statement* get_return_statement();
    Statement* get_if_statement();
    Statement* get_while_statement();
    Statement* get_expression_statement();
    Expression* get_assignment_expression();
    Expression* get_member_expression();
    Expression* get_object_expression();
    Expression* get_array_expression();
    Expression* get_function_expression();
    Expression* get_function_call_expression();
    Expression* get_expression();
    Statement* get_statement();
    std::vector<Statement*> get_statements();

public:
    Program parse(std::vector<Token> tokens);
};

}

