#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

#include "token.h"

namespace mango {

class Lexer {
    std::string source;
    int index = 0;
    std::vector<Token> tokens;
    std::vector<std::string> keywords{"var", "func", "return"};

    std::unordered_map<char, TokenType> single_char_tokens = {
            {';', TokenType::SemiColon},
            {',', TokenType::Comma},
            {'=', TokenType::Equals},
            {'(', TokenType::LeftParen},
            {')', TokenType::RightParen},
            {'{', TokenType::LeftBrace},
            {'}', TokenType::RightBrace},
            {'+', TokenType::Plus},
            {'-', TokenType::Minus},
            {'*', TokenType::Asterisk},
            {'/', TokenType::Slash},
    };

    bool is_keyword(std::string text);

    char current_char();

    char next_char();

    std::string get_identifier();

    std::string get_string();

    std::string get_number();

public:
    std::vector<Token> get_tokens(std::string src);
};

}

