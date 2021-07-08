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
    std::vector<std::string> keywords{"var", "func", "return", "if", "else", "while", "true", "false"};
    std::unordered_map<char, TokenType> single_char_tokens = {
            {':',  TokenType::Colon},
            {';',  TokenType::SemiColon},
            {',',  TokenType::Comma},
            {'.',  TokenType::Dot},
            {'=',  TokenType::Equals},
            {'(',  TokenType::LeftParen},
            {')',  TokenType::RightParen},
            {'{',  TokenType::LeftBrace},
            {'}',  TokenType::RightBrace},
            {'<',  TokenType::LeftAngleBracket},
            {'>',  TokenType::RightAngleBracket},
            {'+',  TokenType::Plus},
            {'-',  TokenType::Minus},
            {'*',  TokenType::Asterisk},
            {'/',  TokenType::Slash},
            {'!',  TokenType::Exclamation},
            {'&',  TokenType::Ampersand},
            {'|',  TokenType::Pipe},
            {'<',  TokenType::LeftAngleBracket},
            {'>',  TokenType::RightAngleBracket},
            {'[',  TokenType::LeftBracket},
            {']',  TokenType::RightBracket},
            {'\n', TokenType::NewLine}
    };

    int line = 1;
    int column = 1;

    bool is_keyword(std::string text);
    char current_char();
    char next_char();
    void add_token(TokenType type, std::string value);
    std::string get_identifier();
    std::string get_string();
    std::string get_number();

public:
    std::vector<Token> get_tokens(std::string src);
};

}

