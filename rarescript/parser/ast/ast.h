//
// Created by Nizam Mirza on 14/08/2026.
//

#ifndef RPGEN_AST_H
#define RPGEN_AST_H
#include <string_view>
#include <variant>

#include "token.h"
#include "../expr/expr.h"

class NewDeclaration {
    Token ident;
    Token type;
    std::unique_ptr<Expr> value;

public:
    explicit NewDeclaration(const Token &ident, const Token &type, std::unique_ptr<Expr> value) : ident(ident), type(type),
        value(std::move(value)) {
    }
};


class SegmentBlock;

using Ast = std::variant<
    SegmentBlock,
    NewDeclaration
>;

class SegmentBlock {
    std::string_view name;
    SourceLocation location;

public:
    explicit SegmentBlock(const std::string_view name,
                          const SourceLocation &location) noexcept : name(name), location(location) {
    }

    std::vector<Ast> children{};
};

#endif //RPGEN_AST_H
