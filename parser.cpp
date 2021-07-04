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

void Parser::expect_optional(TokenType type) {
    if (peek_next_token().type == type) {
        next_token();
    }
}

Operator Parser::get_operator() {
    auto t = current_token();
    auto next = peek_next_token();

    switch (t.type) {
        case TokenType::Keyword:
        case TokenType::Identifier:
        case TokenType::Number:
        case TokenType::String:
        case TokenType::Colon:
        case TokenType::Dot:
        case TokenType::Comma:
        case TokenType::SemiColon:
        case TokenType::LeftParen:
        case TokenType::RightParen:
        case TokenType::LeftBrace:
        case TokenType::RightBrace:
        case TokenType::LeftBracket:
        case TokenType::RightBracket:
        case TokenType::NewLine:
        case TokenType::EndOfFile:
            std::cerr << "invalid operator \"" << t.value << "\"\n";
            assert(false);
        case TokenType::Plus:
            return Operator::Plus;
        case TokenType::Minus:
            return Operator::Minus;
        case TokenType::Asterisk:
            return Operator::Multiply;
        case TokenType::Slash:
            return Operator::Divide;
        case TokenType::Equals:
            if (next.type != TokenType::Equals) {
                break;
            }
            next_token();
            return Operator::EqualTo;
        case TokenType::Exclamation:
            if (next.type == TokenType::Equals) {
                next_token();
                return Operator::NotEqualTo;
            }

            return Operator::Not;
        case TokenType::LeftAngleBracket:
            if (next.type == TokenType::Equals) {
                next_token();
                return Operator::LessThanOrEqualTo;
            }
            return Operator::LessThan;
        case TokenType::RightAngleBracket:
            if (next.type == TokenType::Equals) {
                next_token();
                return Operator::GreaterThanOrEqualTo;
            }
            return Operator::GreaterThan;
    }

    std::cerr << "invalid operator \"" << t.value << "\"\n";
    assert(false);
}

Statement* Parser::get_declaration_statement() {
    auto type_token = expect(TokenType::Keyword);
    // declarations with var only for now
    assert(type_token.value == "var");

    auto id_token = expect(TokenType::Identifier);

    expect(TokenType::Equals);

    auto next = next_token();

    Expression* value;

    if (next.type == TokenType::SemiColon) {
        value = new UndefinedExpression();
    } else {
        backup();
        value = get_expression();
        if (peek_next_token().type == TokenType::SemiColon) {
            next_token();
        }
    }

    auto s = new DeclarationStatement;
    // TODO: check expressions for type
    s->data_type = DataType::Integer;
    s->identifier = id_token.value;
    s->value = value;

    return s;
}

Statement* Parser::get_return_statement() {
    auto keyword_token = expect(TokenType::Keyword);
    assert(keyword_token.value == "return");

    auto s = new ReturnStatement;

    // check for return without a value
    if (peek_next_token().type == TokenType::SemiColon) {
        s->value = new UndefinedExpression;
        return s;
    }

    s->value = get_expression();
    expect_optional(TokenType::SemiColon);

    return s;
}

Statement* Parser::get_if_statement() {
    auto keyword_token = expect(TokenType::Keyword);
    assert(keyword_token.value == "if");

    expect(TokenType::LeftParen);
    auto condition = get_expression();
    expect(TokenType::RightParen);

    auto if_block = get_statement();
    Statement* else_block = nullptr;

    auto next = peek_next_token();
    if (next.type == TokenType::Keyword && next.value == "else") {
        next_token();
        else_block = get_statement();
    }

    auto s = new IfStatement;
    s->condition = condition;
    s->if_block = if_block;
    s->else_block = else_block;

    return s;
}

Statement* Parser::get_while_statement() {
    auto keyword_token = expect(TokenType::Keyword);
    assert(keyword_token.value == "while");

    expect(TokenType::LeftParen);
    auto condition = get_expression();
    expect(TokenType::RightParen);

    auto body = get_statement();

    auto s = new WhileStatement;
    s->condition = condition;
    s->body = body;
    return s;
}

Statement* Parser::get_block_statement() {
    expect(TokenType::LeftBrace);

    auto s = new BlockStatement;

    // check for empty block
    if (peek_next_token().type != TokenType::RightBrace) {
        s->statements = get_statements();
    }

    expect(TokenType::RightBrace);
    expect_optional(TokenType::SemiColon);

    return s;
}

Expression* Parser::get_function_expression() {
    auto keyword_token = expect(TokenType::Keyword);
    assert(keyword_token.value == "func");

    expect(TokenType::LeftParen);

    auto params = std::vector<std::string>();

    while (peek_next_token().type == TokenType::Identifier) {
        params.push_back(next_token().value);
        if (peek_next_token().type == TokenType::Comma) {
            next_token();
        }
    }

    expect(TokenType::RightParen);

    auto body = get_statement();

    auto fe = new FunctionExpression();
    fe->parameters = params;
    fe->body = body;

    return fe;
}

Expression* Parser::get_function_call_expression() {
    auto id_token = expect(TokenType::Identifier);

    expect(TokenType::LeftParen);

    auto args = std::vector<Expression*>();

    while (peek_next_token().type != TokenType::RightParen) {
        args.push_back(get_expression());
        if (peek_next_token().type == TokenType::Comma) {
            next_token();
        }
    }

    expect(TokenType::RightParen);

    auto fce = new FunctionCallExpression();
    fce->value = id_token.value;
    fce->arguments = args;
    return fce;
};

Expression* Parser::get_assignment_expression() {
    auto id_token = expect(TokenType::Identifier);
    expect(TokenType::Equals);
    auto ae = new AssignmentExpression();
    auto ie = new IdentifierExpression();
    ie->value = id_token.value;
    ae->left = ie;
    ae->right = get_expression();
    return ae;
};

Expression* Parser::get_object_expression() {
    expect(TokenType::LeftBrace);

    std::unordered_map<std::string, Expression*> props;

    while (peek_next_token().type != TokenType::RightBrace) {
        auto id_token = expect(TokenType::Identifier);
        expect(TokenType::Colon);

        props[id_token.value] = get_expression();

        if (peek_next_token().type == TokenType::Comma) {
            next_token();
        }
    };

    expect(TokenType::RightBrace);

    auto oe = new ObjectExpression();
    oe->properties = props;
    return oe;
};

Expression* Parser::get_array_expression() {
    expect(TokenType::LeftBracket);

    std::vector<Expression*> elements;

    while (peek_next_token().type != TokenType::RightBracket) {
        elements.push_back(get_expression());
        if (peek_next_token().type == TokenType::Comma) {
            next_token();
        }
    };

    expect(TokenType::RightBracket);

    auto e = new ArrayExpression();
    e->elements = elements;
    return e;
};

Expression* Parser::get_member_expression() {
    auto object_id_token = expect(TokenType::Identifier);
    expect(TokenType::Dot);
    auto property_id_token = expect(TokenType::Identifier);

    auto me = new MemberExpression();
    me->identifier = object_id_token.value;
    auto prop = new IdentifierExpression();
    prop->value = property_id_token.value;
    me->property = prop;

    return me;
};

Expression* Parser::get_expression() {
    auto t = next_token();

    Expression* left;

    switch (t.type) {
        case TokenType::Identifier: {
            if (peek_next_token().type == TokenType::LeftParen) {
                backup();
                left = get_function_call_expression();
            } else if (peek_next_token().type == TokenType::Dot) {
                backup();
                auto me = get_member_expression();

                if (peek_next_token().type == TokenType::Equals) {
                    expect(TokenType::Equals);
                    auto ae = new AssignmentExpression();
                    ae->left = me;
                    ae->right = get_expression();
                    return ae;
                }

                left = me;
            } else if (peek_next_token().type == TokenType::LeftBracket) {
                expect(TokenType::LeftBracket);
                auto inner = get_expression();
                expect(TokenType::RightBracket);

                auto me = new MemberExpression();
                me->identifier = t.value;
                me->property = inner;
                return me;
            } else if (peek_next_token().type == TokenType::Equals) {
                backup();
                left = get_assignment_expression();
            } else {
                auto ie = new IdentifierExpression();
                ie->value = t.value;
                left = ie;
            }
            break;
        }

        case TokenType::Keyword: {
            backup();
            return get_function_expression();
        }
        case TokenType::LeftBrace: {
            backup();
            return get_object_expression();
        }
        case TokenType::LeftBracket: {
            backup();
            return get_array_expression();
        }
        case TokenType::Number: {
            auto ile = new IntegerLiteralExpression;
            ile->value = atoi(t.value.data());
            left = ile;
            break;
        }
        case TokenType::String: {
            auto sle = new StringLiteralExpression;
            sle->value = t.value;
            left = sle;
            break;
        }
        default:
        UNEXPECTED_TOKEN(t);
    }

    auto next = peek_next_token();
    // check for tokens that end an expression
    if (next.type == TokenType::SemiColon ||
        next.type == TokenType::RightParen ||
        next.type == TokenType::Comma ||
        next.type == TokenType::RightBrace ||
        next.type == TokenType::RightBracket ||
        next.type == TokenType::EndOfFile) {
        return left;
    }
    next_token();

    auto op = get_operator();
    auto right = get_expression();

    auto b = new BinaryExpression;
    b->op = op;
    b->left = left;
    b->right = right;
    return b;
}

Statement* Parser::get_statement() {
    auto t = next_token();

    switch (t.type) {
        case TokenType::Keyword: {
            if (t.value == "var") {
                backup();
                return get_declaration_statement();
            }

            if (t.value == "return") {
                backup();
                return get_return_statement();
            }

            if (t.value == "if") {
                backup();
                return get_if_statement();
            }

            if (t.value == "while") {
                backup();
                return get_while_statement();
            }

            UNEXPECTED_TOKEN(t);
        }

        case TokenType::Identifier:
        case TokenType::Number:
        case TokenType::String: {
            backup();
            auto s = new ExpressionStatement();
            s->value = get_expression();
            expect_optional(TokenType::SemiColon);
            return s;
        }
        case TokenType::LeftBrace: {
            backup();
            return get_block_statement();
        }
        default:
        UNEXPECTED_TOKEN(t);
    }
};

std::vector<Statement*> Parser::get_statements() {
    std::vector<Statement*> statements;

    auto next_token = peek_next_token();

    while (next_token.type != TokenType::EndOfFile && next_token.type != TokenType::RightBrace) {
        statements.push_back(get_statement());
        next_token = peek_next_token();
    }

    return statements;
}

Program Parser::parse(std::vector<Token> tokens) {
    this->tokens = tokens;

    Program program;
    backup();
    program.statements = get_statements();

    return program;
}

}