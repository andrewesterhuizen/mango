#include <iostream>
#include <string>

#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "interpreter/interpreter.h"

mango::Program compile(std::string src) {
    mango::Lexer lexer;
    auto tokens = lexer.get_tokens(src);
    mango::Parser parser;
    auto ast = parser.parse(tokens);
    return ast;
}

class Repl {
public:
    int start() {
        std::string line;
        mango::interpreter::Interpreter interpreter;

        while (1) {
            std::cout << ">";
            std::getline(std::cin, line);
            if (line == "exit") {
                return 0;
            }

            auto ast = compile(line);
            auto out = ast.execute(interpreter);
            std::cout << *out << "\n";
        };
    }
};

int main() {
    std::string src = "var x = 1;"
                      "x = x + 123 + 12;"
                      "if(x > 50) {"
                      "x = x + 34;"
                      "}";

//    Repl repl;
//    return repl.start();


    auto ast = compile(src);
    std::cout << ast.print() << "\n";
    auto out = ast.generate();
    std::cout << out;
    return 0;
}
