#include <iostream>
#include <string>

#include "lexer.h"
#include "parser.h"
#include "interpreter/interpreter.h"

int main() {
    std::string src = "var a = [1,2,3,4,5]; "
                      "var i = 0;"
                      "while(i < 5) {"
                      "print(a[i]);"
                      "i = i+1;"
                      "};";

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
