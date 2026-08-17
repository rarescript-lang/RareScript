//
// Created by Nizam Mirza on 17/08/2026.
//

#include <catch2/catch_test_macros.hpp>
#include "../../rarescript/parser/parser.h"
#include "../../rarescript/lexer/lexer.h"

TEST_CASE(
    "Parser parses empty token stream",
    "[parser]"
) {
    constexpr std::vector<Token> tokens;

    Parser parser(tokens);
    const auto ast = parser.parse();

    REQUIRE(ast.empty());
}

TEST_CASE(
    "Parser parses game declaration",
    "[parser]"
) {
    const std::string source =
        "[game]";

    Lexer lexer(source);
    const auto tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto ast = parser.parse();

    REQUIRE(ast.size() == 1);
}

TEST_CASE(
    "Parser parses variable declaration",
    "[parser]"
) {
    const std::string source =
        "[game]\n"
        "new exp nat(0)";

    Lexer lexer(source);
    const auto tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto ast = parser.parse();

    REQUIRE(ast.size() == 2);
}

TEST_CASE(
    "Parser parses variable declaration and assignment",
    "[parser]"
) {
    const std::string source =
        "[game]\n"
        "new exp nat(0)\n"
        "set exp 100";

    Lexer lexer(source);
    const auto tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto ast = parser.parse();

    REQUIRE(ast.size() == 3);
}

TEST_CASE(
    "Parser parses complete game source",
    "[parser]"
) {
    const std::string source =
        "[game]\n"
        "new exp nat(0)\n"
        "set exp 100";

    Lexer lexer(source);
    const auto tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto ast = parser.parse();

    REQUIRE(ast.size() == 3);
}

TEST_CASE(
    "Parser throws on incomplete variable declaration",
    "[parser][error]"
) {
    const std::string source =
        "[game]\n"
        "new exp";

    Lexer lexer(source);
    const auto tokens = lexer.tokenize();

    Parser parser(tokens);

    REQUIRE_THROWS_AS(
        parser.parse(),
        std::runtime_error
    );
}

TEST_CASE(
    "Parser throws on incomplete assignment",
    "[parser][error]"
) {
    const std::string source =
        "[game]\n"
        "set exp";

    Lexer lexer(source);
    const auto tokens = lexer.tokenize();

    Parser parser(tokens);

    REQUIRE_THROWS_AS(
        parser.parse(),
        std::runtime_error
    );
}

TEST_CASE(
    "Parser throws on malformed declaration",
    "[parser][error]"
) {
    const std::string source =
        "[game]\n"
        "new exp nat";

    Lexer lexer(source);
    const auto tokens = lexer.tokenize();

    Parser parser(tokens);

    REQUIRE_THROWS_AS(
        parser.parse(),
        std::runtime_error
    );
}
