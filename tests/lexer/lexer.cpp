//
// Created by Nizam Mirza on 14/08/2026.
//

// tests/test_lexer.cpp
#include <catch2/catch_test_macros.hpp>
#include "../../rarescript/lexer/lexer.h"

TEST_CASE(
    "Lexer basic tokenization",
    "[lexer]"
) {
    std::string source = "[foo 123]";
    auto lexer = Lexer(source);

    auto tokens = lexer.tokenize();

    REQUIRE(tokens.size() == 4);

    CHECK(tokens[0].kind == TokenKind::LeftBrace);
    CHECK(tokens[0].lexeme == "[");

    CHECK(tokens[1].kind == TokenKind::Identifier);
    CHECK(tokens[1].lexeme == "foo");

    CHECK(tokens[2].kind == TokenKind::Integer);
    CHECK(tokens[2].lexeme == "123");

    CHECK(tokens[3].kind == TokenKind::RightBrace);
    CHECK(tokens[3].lexeme == "]");
}

TEST_CASE(
    "Lexer handles empty source",
    "[lexer]"
) {
    const std::string source;
    auto lexer = Lexer(source);
    const auto tokens = lexer.tokenize();
    REQUIRE(tokens.empty());
}
