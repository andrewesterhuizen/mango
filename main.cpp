#include <iostream>
#include <string>

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
    void start() {
        std::string line;
        mango::Interpreter interpreter;

        while (1) {
            std::getline(std::cin, line);
            auto ast = compile(line);
            auto out = interpreter.run(ast);
            std::cout << *out << "\n";
        };
    }
};

int main() {
    std::string src = "var x = { prop: 123 }; print(x.prop);";

//    Repl repl;
//    repl.start();

    return 0;
}
