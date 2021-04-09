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

Statement *Parser::get_return_statement() {
    auto s = new ReturnStatement;

    auto next = next_token();
    if (next.type == TokenType::SemiColon) {
        s->value = new UndefinedExpression();
        return s;
    }

    s->value = get_expression();

    if (peek_next_token().type == TokenType::SemiColon) {
        next_token();
    }

    return s;
}

Statement *Parser::get_if_statement() {
    auto s = new IfStatement;

    expect(TokenType::LeftParen);
    next_token();
    s->condition = get_expression();
    expect(TokenType::RightParen);
    next_token();

    s->if_block = get_statement();

    auto next = next_token();
    if(next.type == TokenType::Keyword && next.value == "else") {
        next_token();
        s->else_block = get_statement();
    }

    return s;
}

Statement *Parser::get_while_statement() {
    auto s = new WhileStatement;

    expect(TokenType::LeftParen);
    next_token();
    s->condition = get_expression();
    expect(TokenType::RightParen);
    next_token();

    s->body = get_statement();

    return s;
}

Statement *Parser::get_assignment_statement() {
    auto s = new AssignmentStatement;
    s->type = DataType::Integer;
    auto id_token = expect(TokenType::Identifier);
    s->identifier = id_token.value;
    expect(TokenType::Equals);
    next_token();
    s->value = get_expression();

    return s;
}

Statement *Parser::get_block_statement() {
    auto s = new BlockStatement;
    expect(TokenType::LeftBrace);
    if (peek_next_token().type == TokenType::RightBrace) {
        return s;
    }

    auto next = next_token();
    s->statements = get_statements();

    if (peek_next_token().type == TokenType::RightBrace) {
        next_token();
    }

    if (peek_next_token().type == TokenType::SemiColon) {
        next_token();
    }

    return s;
}

Expression *Parser::get_expression() {
    auto t = current_token();

    Expression *left = nullptr;

    switch (t.type) {
        case TokenType::Identifier: {
            if (peek_next_token().type == TokenType::LeftParen) {
                auto fce = new FunctionCallExpression{t.value};
                expect(TokenType::LeftParen);
                next_token();

                while (current_token().type != TokenType::RightParen) {
                    fce->arguments.push_back(get_expression());
                    if (peek_next_token().type == TokenType::Comma) {
                        next_token();
                    }
                    next_token();
                }

                left = fce;
            } else {
                left = new IdentifierExpression{t.value};
            }
            break;
        }
        case TokenType::Keyword: {
            if (t.value == "func") {
                auto fe = new FunctionExpression();

                expect(TokenType::LeftParen);

                while (peek_next_token().type == TokenType::Identifier) {
                    fe->parameters.push_back(next_token().value);
                    if (peek_next_token().type == TokenType::Comma) {
                        next_token();
                    }
                }

                expect(TokenType::RightParen);

                next_token();
                fe->body = get_statement();
                return fe;
            }
            break;
        }
        case TokenType::Number:
            left = new IntegerLiteralExpression{atoi(t.value.data())};
            break;
        case TokenType::String:
            left = new StringLiteralExpression{t.value};
            break;

        default:
        UNEXPECTED_TOKEN(t);

    }

    auto next = next_token();

    if (next.type == TokenType::RightBrace || next.type == TokenType::RightParen || next.type == TokenType::Comma) {
        backup();
        return left;
    }

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

Statement *Parser::get_statement() {
    auto t = current_token();

    switch (t.type) {
        case TokenType::Keyword: {
            if (t.value == "var") {
                return get_declaration_statement();
            }

            if (t.value == "return") {
                return get_return_statement();
            }

            if (t.value == "if") {
                return get_if_statement();
            }

            if (t.value == "while") {
                return get_while_statement();
            }

            UNEXPECTED_TOKEN(t);
        }
        case TokenType::Identifier: {
            if (peek_next_token().type == TokenType::Equals) {
                backup();
                return get_assignment_statement();
            }

            return new ExpressionStatement(get_expression());
        }
        case TokenType::Number: {
            return new ExpressionStatement(get_expression());
        }
        case TokenType::String: {
            return new ExpressionStatement(get_expression());
        }
        case TokenType::LeftBrace: {
            backup();
            return get_block_statement();
        }
        default:
        UNEXPECTED_TOKEN(t);
    }
};

std::vector<Statement *> Parser::get_statements() {
    std::vector<Statement *> statements;

    auto t = current_token();
    while (t.type != TokenType::EndOfFile && t.type != TokenType::RightBrace) {
        statements.push_back(get_statement());
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