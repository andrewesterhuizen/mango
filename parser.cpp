#include "parser.h"

namespace mango {

Token Parser::current_token() {
    return tokens.at(index);
}

Token Parser::next_token() {
    return tokens.at(++index);
}

Token Parser::peek_next_token() {
    return tokens.at(index + 1);
}

void Parser::backup() {
    index--;
}

Token Parser::expect(TokenType type) {
    auto t = next_token();
    if (t.type != type) {
        std::cerr << "expected token type \"" << type << "\" and got " << t << "\"\n";
        assert(false);
    }
    return t;
}

Statement *Parser::get_declaration_statement() {
    auto s = new DeclarationStatement;
    s->type = DataType::Integer;

    auto id_token = expect(TokenType::Identifier);
    s->identifier = id_token.value;

    expect(TokenType::Equals);

    auto next = next_token();
    if (next.type == TokenType::SemiColon) {
        s->value = new UndefinedExpression();
        return s;
    }

    s->value = get_expression();

    return s;
}

Statement *Parser::get_assignment_statement() {
    auto s = new AssignmentStatement;

    auto id_token = expect(TokenType::Identifier);
    s->identifier = id_token.value;
    expect(TokenType::Equals);
    next_token();
    s->value = get_expression();

    return s;
}

Expression *Parser::get_expression() {
    auto t = current_token();

    if (t.type != TokenType::Identifier && t.type != TokenType::Number) {
        UNEXPECTED_TOKEN(t);
    }

    Expression *left = nullptr;

    switch (t.type) {
        case TokenType::Identifier:
            left = new IdentifierExpression{t.value};
            break;
        case TokenType::Number:
            left = new IntegerLiteralExpression{atoi(t.value.data())};
            break;
    }

    auto next = next_token();

    if (next.type == TokenType::SemiColon) {
        return left;
    }

    auto op = get_operator(next.value.data()[0]);
    next_token();
    auto right = get_expression();

    auto b = new BinaryExpression;
    b->op = op;
    b->left = left;
    b->right = right;

    return b;
}

std::vector<Statement *> Parser::get_statements() {
    std::vector<Statement *> statements;

    auto t = current_token();
    while (t.type != TokenType::EndOfFile) {
        switch (t.type) {
            case TokenType::Keyword: {
                if (t.value == "var") {
                    statements.push_back(get_declaration_statement());
                } else {
                    UNEXPECTED_TOKEN(t);
                }
                break;
            }
            case TokenType::Identifier: {
                if (peek_next_token().type == TokenType::Equals) {
                    backup();
                    statements.push_back(get_assignment_statement());
                } else {
                    statements.push_back(new ExpressionStatement(get_expression()));
                }

                break;
            }
            case TokenType::Number: {
                statements.push_back(new ExpressionStatement(get_expression()));
                break;
            }
            default:
            UNEXPECTED_TOKEN(t);
        }

        t = next_token();
    }

    return statements;
}

Program Parser::parse(std::vector<Token> tokens) {
    this->tokens = tokens;

    Program program;
    program.statements = get_statements();

    return program;
}
}