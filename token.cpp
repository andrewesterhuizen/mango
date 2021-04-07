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
        case TokenType::SemiColon:
            return "SemiColon";
        case TokenType::EndOfFile:
            return "EndOfFile";
    }

    return "Unknown";
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