#include "lexer.h"

namespace mango
{

bool Lexer::is_keyword(std::string text)
{
    for (auto keyword : keywords)
    {
        if (text == keyword)
            return true;
    }

    return false;
}

char Lexer::current_char()
{
    return source.at(index);
}

char Lexer::next_char()
{
    index++;
    if (index >= source.size())
    {
        return '\0';
    }
    return source.at(index);
}

std::string Lexer::get_text()
{
    std::string text = "";
    auto c = current_char();

    while (c != ' ' && c != '\n' && c != ';')
    {
        text += c;
        c = next_char();
    }

    index--;

    return text;
}

std::string Lexer::get_number()
{
    std::string n = "";
    auto c = current_char();

    while (isnumber(c))
    {
        n += c;
        c = next_char();
    }

    index--;

    return n;
}

std::vector<Token> Lexer::get_tokens(std::string src)
{
    source = src;

    auto c = current_char();

    while (c)
    {
        if (isalpha(c))
        {
            auto text = get_text();
            auto type = is_keyword(text) ? TokenType::Keyword : TokenType::Identifier;
            tokens.push_back(Token{type, text});
        }
        else if (isnumber(c))
        {
            auto n = get_number();
            tokens.push_back(Token{TokenType::Number, n});
        }
        else
        {
            switch (c)
            {
            case ' ':
                // skip
                break;
            case '=':
                tokens.push_back(Token{TokenType::Equals, "="});
                break;
            case ';':
                tokens.push_back(Token{TokenType::SemiColon, ";"});
                break;
            case '+':
                tokens.push_back(Token{TokenType::Plus, "+"});
                break;
            case '-':
                tokens.push_back(Token{TokenType::Minus, "-"});
                break;
            case '*':
                tokens.push_back(Token{TokenType::Asterisk, "*"});
                break;
            case '/':
                tokens.push_back(Token{TokenType::Slash, "/"});
                break;
            default:
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