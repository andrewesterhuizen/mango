#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "token.h"

namespace mango {

class Lexer {
    std::string source;
    int index = 0;
    std::vector<Token> tokens;
    std::vector<std::string> keywords{"var"};

    bool is_keyword(std::string text);

    char current_char();

    char next_char();

    std::string get_text();

    std::string get_number();

public:
    std::vector<Token> get_tokens(std::string src);
};

}

