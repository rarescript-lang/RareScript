#include <print>

#include "rarescript/lexer/lexer.h"
#include "rarescript/parser/parser.h"

int main() {
    const std::string source = "[description]";
    auto lexer = Lexer(source);
    auto tokens = lexer.tokenize();
    for (auto t: tokens) {
        std::println("{}", t);
    }
    auto parser = Parser(tokens);
    return 0;
}
