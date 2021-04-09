#pragma once

#include <string>
#include <iostream>

namespace mango {
    
enum class TokenType {
    Keyword = 1,
    Identifier,
    Equals,
    Number,
    String,
    SemiColon,
    Comma,
    Exclamation,
    Plus,
    Minus,
    Asterisk,
    Slash,
    LeftParen,
    RightParen,
    LeftBrace,
    RightBrace,
    LeftAngleBracket,
    RightAngleBracket,
    EndOfFile
};

std::ostream& operator <<(std::ostream& os, const TokenType& t);

struct Token {
    TokenType type;
    std::string value;
};

std::ostream& operator <<(std::ostream& os, const Token& t);

}
