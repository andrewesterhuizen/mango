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

    Statement *get_declaration_statement();

    Statement *get_assignment_statement();

    Expression *get_expression();

    std::vector<Statement *> get_statements();

public:
    Program parse(std::vector<Token> tokens);
};

}
