#include <iostream>

#include "token.h"

namespace mango {

std::string token_type_to_string(TokenType t) {
    switch (t) {
        case TokenType::Keyword:
            return "Keyword";
        case TokenType::Identifier:
            return "Identifier";
        case TokenType::Equals:
            return "Equals";
        case TokenType::Number:
            return "Number";
        case TokenType::String:
            return "String";
        case TokenType::SemiColon:
            return "SemiColon";
        case TokenType::Comma:
            return "Comma";
        case TokenType::LeftParen:
            return "LeftParen";
        case TokenType::RightParen:
            return "RightParen";
        case TokenType::LeftBrace:
            return "LeftBrace";
        case TokenType::RightBrace:
            return "RightBrace";
        case TokenType::Plus:
            return "Plus";
        case TokenType::Minus:
            return "Minus";
        case TokenType::Asterisk:
            return "Asterisk";
        case TokenType::Slash:
            return "Slash";
        case TokenType::EndOfFile:
            return "EndOfFile";
    }

    std::cerr << "unknown token type\n";
    assert(false);
}

std::ostream &operator<<(std::ostream &os, const TokenType &t) {
    os << token_type_to_string(t);
    return os;
}

std::ostream &operator<<(std::ostream &os, const Token &t) {
    os << "{ " << "Type: " << token_type_to_string(t.type) << ", Value: \"" << t.value << "\" }";
    return os;
}

}