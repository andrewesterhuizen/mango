#include <iostream>
#include <string>

#include "lexer.h"
#include "parser.h"

int main() {
    std::string src = "var x = 1;"
                      "x = x + 123 + 12;"
                      "if(x > 50) {"
                      "x = x + 34;"
                      "}";


    mango::Lexer lexer;
    auto tokens = lexer.get_tokens(src);
    mango::Parser parser;
    auto ast = parser.parse(tokens);
    std::cout << ast.print() << "\n";
    auto out = ast.generate();
    std::cout << out;
    return 0;
}
