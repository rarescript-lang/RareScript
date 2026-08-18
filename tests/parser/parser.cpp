
//
// Created by Nizam Mirza on 17/08/2026.
//

#include <catch2/catch_test_macros.hpp>

#include "../../rarescript/parser/parser.h"
#include "../../rarescript/lexer/lexer.h"
#include "../../rarescript/parser/ast/ast.h"

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
    "Parser parses empty game segment",
    "[parser]"
) {
    const std::string source =
        "[game]";

    Lexer lexer(source);
    const auto tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto ast = parser.parse();

    REQUIRE(ast.size() == 1);

    auto *segment = std::get<std::unique_ptr<SegmentBlock>>(ast[0].node).get();

    REQUIRE(segment != nullptr);
    CHECK(segment->name == "game");
    CHECK(segment->children.empty());
}

TEST_CASE(
    "Parser parses variable declaration inside segment",
    "[parser]"
) {
    const std::string source =
        "[game]\n"
        "new exp nat(0)";

    Lexer lexer(source);
    const auto tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto ast = parser.parse();

    REQUIRE(ast.size() == 1);

    auto *segment = std::get<std::unique_ptr<SegmentBlock>>(ast[0].node).get();

    REQUIRE(segment != nullptr);
    CHECK(segment->name == "game");

    REQUIRE(segment->children.size() == 1);

    auto *declaration =
        std::get<std::unique_ptr<NewDeclaration>>(
            segment->children[0].node
        ).get();

    REQUIRE(declaration != nullptr);

    CHECK(declaration->ident.lexeme == "exp");
    CHECK(declaration->type.lexeme == "nat");
    REQUIRE(declaration->value != nullptr);
}

TEST_CASE(
    "Parser parses declaration and assignment inside segment",
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

    REQUIRE(ast.size() == 1);

    auto *segment = std::get<std::unique_ptr<SegmentBlock>>(ast[0].node).get();

    REQUIRE(segment != nullptr);
    CHECK(segment->name == "game");

    REQUIRE(segment->children.size() == 2);

    auto *new_decl =
        std::get<std::unique_ptr<NewDeclaration>>(
            segment->children[0].node
        ).get();

    REQUIRE(new_decl != nullptr);

    CHECK(new_decl->ident.lexeme == "exp");
    CHECK(new_decl->type.lexeme == "nat");
    REQUIRE(new_decl->value != nullptr);

    auto *set_decl =
        std::get<std::unique_ptr<SetDeclaration>>(
            segment->children[1].node
        ).get();

    REQUIRE(set_decl != nullptr);

    CHECK(set_decl->ident.lexeme == "exp");
    REQUIRE(set_decl->value != nullptr);
}

TEST_CASE(
    "Parser parses multiple segments",
    "[parser]"
) {
    const std::string source =
        "[game]\n"
        "new exp nat(0)\n"
        "set exp 100\n"
        "[player]\n"
        "new hp nat(100)";

    Lexer lexer(source);
    const auto tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto ast = parser.parse();

    REQUIRE(ast.size() == 2);

    auto *game =
        std::get<std::unique_ptr<SegmentBlock>>(ast[0].node).get();

    REQUIRE(game != nullptr);
    CHECK(game->name == "game");

    REQUIRE(game->children.size() == 2);
    CHECK(
        std::holds_alternative<std::unique_ptr<NewDeclaration>>(
            game->children[0].node
        )
    );
    CHECK(
        std::holds_alternative<std::unique_ptr<SetDeclaration>>(
            game->children[1].node
        )
    );

    auto *player =
        std::get<std::unique_ptr<SegmentBlock>>(ast[1].node).get();

    REQUIRE(player != nullptr);
    CHECK(player->name == "player");

    REQUIRE(player->children.size() == 1);
    CHECK(
        std::holds_alternative<std::unique_ptr<NewDeclaration>>(
            player->children[0].node
        )
    );
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
    "Parser throws on malformed variable declaration",
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
