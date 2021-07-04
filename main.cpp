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
        mango::Interpreter interpreter;

        while (1) {
            std::getline(std::cin, line);
            if (line == "exit") {
                return 0;
            }

            auto ast = compile(line);
            auto out = interpreter.run(ast);
            std::cout << *out << "\n";
        };
    }
};

int main() {
    std::string src = "var x = 1; x = x + 2;";

    Repl repl;
    return repl.start();

    return 0;
}
