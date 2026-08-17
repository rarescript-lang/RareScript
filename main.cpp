#include <print>

#include "rarescript/lexer/lexer.h"
#include "rarescript/parser/parser.h"

int main() {
    const std::string source = "[game]\nnew age nat(12 + 2 * -3)";
    auto lexer = Lexer(source);
    auto tokens = lexer.tokenize();
    for (auto &t: tokens) {
        std::println("{}", t);
    }
    auto parser = Parser(tokens);
    auto asts = parser.parse();
    for (auto &ast: asts) {
        std::println("{}", ast);
    }
    return 0;
}
