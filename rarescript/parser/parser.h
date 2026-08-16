//
// Created by Nizam Mirza on 14/08/2026.
//

#ifndef RPGEN_PARSER_H
#define RPGEN_PARSER_H
#include <span>
#include <vector>

#include "ast/ast.h"
#include "token.h"
#include "expr/expr.h"


class Parser {
    std::span<const Token> tokens;
    uint32_t idx, len;

    [[nodiscard]] TokenKind peek() const noexcept;

    [[nodiscard]] Token advance() noexcept;

    [[nodiscard]] Token expect(TokenKind kind) noexcept;

    std::unique_ptr<Expr> operand() noexcept;

    std::unique_ptr<Expr> parse_expr(uint8_t min_precedence) noexcept;

public:
    explicit Parser(const std::span<const Token> tokens) : tokens(tokens), idx(0), len(tokens.size()) {
    }

    std::vector<Ast> parse() noexcept;
};


#endif //RPGEN_PARSER_H
