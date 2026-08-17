//
// Created by Nizam Mirza on 14/08/2026.
//


#include <catch2/catch_test_macros.hpp>
#include "../../rarescript/lexer/lexer.h"

TEST_CASE(
    "Lexer basic tokenization",
    "[lexer]"
) {
    const std::string source = "[foo 123]";
    auto lexer = Lexer(source);

    const auto tokens = lexer.tokenize();

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

TEST_CASE(
    "Lexer handles multiple identifiers",
    "[lexer]"
) {
    const std::string source = "[foo bar baz]";
    auto lexer = Lexer(source);

    const auto tokens = lexer.tokenize();

    REQUIRE(tokens.size() == 5);

    CHECK(tokens[0].kind == TokenKind::LeftBrace);

    CHECK(tokens[1].kind == TokenKind::Identifier);
    CHECK(tokens[1].lexeme == "foo");

    CHECK(tokens[2].kind == TokenKind::Identifier);
    CHECK(tokens[2].lexeme == "bar");

    CHECK(tokens[3].kind == TokenKind::Identifier);
    CHECK(tokens[3].lexeme == "baz");

    CHECK(tokens[4].kind == TokenKind::RightBrace);
}

TEST_CASE(
    "Lexer handles multiple integers",
    "[lexer]"
) {
    const std::string source = "[0 1 42 123456789]";
    auto lexer = Lexer(source);

    const auto tokens = lexer.tokenize();

    REQUIRE(tokens.size() == 6);

    CHECK(tokens[0].kind == TokenKind::LeftBrace);

    CHECK(tokens[1].kind == TokenKind::Integer);
    CHECK(tokens[1].lexeme == "0");

    CHECK(tokens[2].kind == TokenKind::Integer);
    CHECK(tokens[2].lexeme == "1");

    CHECK(tokens[3].kind == TokenKind::Integer);
    CHECK(tokens[3].lexeme == "42");

    CHECK(tokens[4].kind == TokenKind::Integer);
    CHECK(tokens[4].lexeme == "123456789");

    CHECK(tokens[5].kind == TokenKind::RightBrace);
}

TEST_CASE(
    "Lexer ignores whitespace",
    "[lexer]"
) {
    const std::string source = "  [   foo\t123\n]  ";
    auto lexer = Lexer(source);

    const auto tokens = lexer.tokenize();

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
    "Lexer handles identifiers with underscores",
    "[lexer]"
) {
    const std::string source = "[foo_bar _foo foo_]";
    auto lexer = Lexer(source);

    const auto tokens = lexer.tokenize();

    REQUIRE(tokens.size() == 5);

    CHECK(tokens[0].kind == TokenKind::LeftBrace);

    CHECK(tokens[1].kind == TokenKind::Identifier);
    CHECK(tokens[1].lexeme == "foo_bar");

    CHECK(tokens[2].kind == TokenKind::Identifier);
    CHECK(tokens[2].lexeme == "_foo");

    CHECK(tokens[3].kind == TokenKind::Identifier);
    CHECK(tokens[3].lexeme == "foo_");

    CHECK(tokens[4].kind == TokenKind::RightBrace);
}

TEST_CASE(
    "Lexer handles identifiers containing digits",
    "[lexer]"
) {
    const std::string source = "[foo123 test42 abc1]";
    auto lexer = Lexer(source);

    const auto tokens = lexer.tokenize();

    REQUIRE(tokens.size() == 5);

    CHECK(tokens[0].kind == TokenKind::LeftBrace);

    CHECK(tokens[1].kind == TokenKind::Identifier);
    CHECK(tokens[1].lexeme == "foo123");

    CHECK(tokens[2].kind == TokenKind::Identifier);
    CHECK(tokens[2].lexeme == "test42");

    CHECK(tokens[3].kind == TokenKind::Identifier);
    CHECK(tokens[3].lexeme == "abc1");

    CHECK(tokens[4].kind == TokenKind::RightBrace);
}

TEST_CASE(
    "Lexer handles nested braces",
    "[lexer]"
) {
    const std::string source = "[foo [bar 123]]";
    auto lexer = Lexer(source);

    const auto tokens = lexer.tokenize();

    REQUIRE(tokens.size() == 7);

    CHECK(tokens[0].kind == TokenKind::LeftBrace);

    CHECK(tokens[1].kind == TokenKind::Identifier);
    CHECK(tokens[1].lexeme == "foo");

    CHECK(tokens[2].kind == TokenKind::LeftBrace);

    CHECK(tokens[3].kind == TokenKind::Identifier);
    CHECK(tokens[3].lexeme == "bar");

    CHECK(tokens[4].kind == TokenKind::Integer);
    CHECK(tokens[4].lexeme == "123");

    CHECK(tokens[5].kind == TokenKind::RightBrace);
    CHECK(tokens[5].lexeme == "]");

    CHECK(tokens[6].kind == TokenKind::RightBrace);
    CHECK(tokens[6].lexeme == "]");
}
