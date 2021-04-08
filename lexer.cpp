#include "lexer.h"

#include <regex>

namespace mango {

bool Lexer::is_keyword(std::string text) {
    for (auto keyword : keywords) {
        if (text == keyword)
            return true;
    }

    return false;
}

char Lexer::current_char() {
    return source.at(index);
}

char Lexer::next_char() {
    index++;
    if (index >= source.size()) {
        return '\0';
    }
    return source.at(index);
}

std::string Lexer::get_string() {
    std::string text = "";
    auto c = current_char();

    while (c != '"') {
        text += c;
        c = next_char();
    }

    return text;
}

bool is_valid_identifier_character(char c) {
    return std::regex_match(std::string{c}, std::regex("\\w+"));
}

std::string Lexer::get_identifier() {
    std::string text = "";

    auto c = current_char();

    while (c != ' ' && is_valid_identifier_character(c) && c != '\n' && c != ';') {
        text += c;
        c = next_char();
    }

    index--;

    return text;
}

std::string Lexer::get_number() {
    std::string n = "";
    auto c = current_char();

    while (isnumber(c)) {
        n += c;
        c = next_char();
    }

    index--;

    return n;
}

std::vector<Token> Lexer::get_tokens(std::string src) {
    source = src;

    auto c = current_char();

    while (c) {
        if (isalpha(c)) {
            auto text = get_identifier();
            auto type = is_keyword(text) ? TokenType::Keyword : TokenType::Identifier;
            tokens.push_back(Token{type, text});
        } else if (isnumber(c)) {
            auto n = get_number();
            tokens.push_back(Token{TokenType::Number, n});
        } else if (c == '"') {
            next_char();
            auto s = get_string();
            tokens.push_back(Token{TokenType::String, s});
        } else {
            if (c == ' ') {
                // skip
            } else if (auto entry = single_char_tokens.find(c); entry != single_char_tokens.end()) {
                tokens.push_back(Token{entry->second, std::string{c}});
            } else {
                std::cerr << "unexpected token " << c << "\n";
                assert(false);
            }
        }

        c = next_char();
    }

    tokens.push_back(Token{TokenType::EndOfFile, ""});

    return tokens;
}

}