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
        case TokenType::Colon:
            return "Colon";
        case TokenType::SemiColon:
            return "SemiColon";
        case TokenType::Dot:
            return "Dot";
        case TokenType::Comma:
            return "Comma";
        case TokenType::Exclamation:
            return "Exclamation";
        case TokenType::Ampersand:
            return "Ampersand";
        case TokenType::Pipe:
            return "Pipe";
        case TokenType::LeftParen:
            return "LeftParen";
        case TokenType::RightParen:
            return "RightParen";
        case TokenType::LeftBrace:
            return "LeftBrace";
        case TokenType::RightBrace:
            return "RightBrace";
        case TokenType::LeftAngleBracket:
            return "LeftAngleBracket";
        case TokenType::RightAngleBracket:
            return "RightAngleBracket";
        case TokenType::LeftBracket:
            return "LeftBracket";
        case TokenType::RightBracket:
            return "RightBracket";
        case TokenType::Plus:
            return "Plus";
        case TokenType::Minus:
            return "Minus";
        case TokenType::Asterisk:
            return "Asterisk";
        case TokenType::Slash:
            return "Slash";
        case TokenType::NewLine:
            return "NewLine";
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
    std::string value = t.value;
    if (t.type == TokenType::NewLine) {
        value = "\\n";
    }

    os << "{ "
       << "Type: " << token_type_to_string(t.type) << ", "
       << "Value: " << "\"" << value << "\"" << ", "
       << "File: " << "\"" << t.file << "\"" << ", "
       << "Line: " << t.line << ", "
       << "Column: " << t.column << " "
       << "}";
    return os;
}

}