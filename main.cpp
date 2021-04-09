#include <iostream>
#include <string>

#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

int main() {
    std::string src = "var x = 5;"
                      "while(x) { x = x - 1; print(x); }";

    mango::Lexer lexer;

    auto tokens = lexer.get_tokens(src);

    for (auto t : tokens) {
        std::cout << t << "\n";
    }

    mango::Parser parser;

    auto ast = parser.parse(tokens);

    std::cout << ast_to_string(ast);

    mango::Interpreter interpreter;
    auto out = interpreter.run(ast);
    std::cout << out << "\n";

    return 0;
}
