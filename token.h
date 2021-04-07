#pragma once

#include <string>
#include <iostream>

namespace mango {
    
enum class TokenType {
    Keyword = 1,
    Identifier,
    Equals,
    Number,
    SemiColon,
    Plus,
    Minus,
    Asterisk,
    Slash,
    EndOfFile
};

std::ostream& operator <<(std::ostream& os, const TokenType& t);

struct Token {
    TokenType type;
    std::string value;
};

std::ostream& operator <<(std::ostream& os, const Token& t);

}
