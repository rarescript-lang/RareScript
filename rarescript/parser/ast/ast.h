//
// Created by Nizam Mirza on 14/08/2026.
//

#ifndef RPGEN_AST_H
#define RPGEN_AST_H

#include <format>
#include <string_view>
#include <variant>
#include <vector>

#include "token.h"
#include "../expr/expr.h"

struct Ast;

struct SegmentBlock;
struct NewDeclaration;
struct SetDeclaration;


struct NewDeclaration {
    Token ident;
    Token type;
    std::unique_ptr<Expr> value;

    NewDeclaration(
        const Token &ident,
        const Token &type,
        std::unique_ptr<Expr> value
    )
        : ident(ident),
          type(type),
          value(std::move(value)) {
    }

    NewDeclaration(const NewDeclaration &) = delete;

    NewDeclaration &operator=(const NewDeclaration &) = delete;

    NewDeclaration(NewDeclaration &&) noexcept = default;

    NewDeclaration &operator=(NewDeclaration &&) noexcept = default;
};

struct SetDeclaration {
    Token ident;
    std::unique_ptr<Expr> value;

    SetDeclaration(
        const Token &ident,
        std::unique_ptr<Expr> value
    )
        : ident(ident),
          value(std::move(value)) {
    }

    SetDeclaration(const SetDeclaration &) = delete;

    SetDeclaration &operator=(const SetDeclaration &) = delete;

    SetDeclaration(SetDeclaration &&) noexcept = default;

    SetDeclaration &operator=(SetDeclaration &&) noexcept = default;
};


using AstNode = std::variant<
    std::unique_ptr<SegmentBlock>,
    std::unique_ptr<NewDeclaration>,
    std::unique_ptr<SetDeclaration>
>;

struct Ast {
    AstNode node;

    template<typename T, typename... Args>
    explicit Ast(
        std::in_place_type_t<T>,
        Args &&... args
    )
        : node(
            std::make_unique<T>(
                std::forward<Args>(args)...
            )
        ) {
    }

    explicit Ast(AstNode node)
        : node(std::move(node)) {
    }

    Ast(Ast &&) noexcept = default;

    Ast &operator=(Ast &&) noexcept = default;

    Ast(const Ast &) = delete;

    Ast &operator=(const Ast &) = delete;
};

struct SegmentBlock {
    std::string_view name;
    SourceLocation location;
    std::vector<Ast> children;

    SegmentBlock(
        const std::string_view name,
        const SourceLocation &location
    )
        : name(name),
          location(location) {
    }

    SegmentBlock(SegmentBlock &&) noexcept = default;

    SegmentBlock &operator=(SegmentBlock &&) noexcept = default;

    SegmentBlock(const SegmentBlock &) = delete;

    SegmentBlock &operator=(const SegmentBlock &) = delete;
};

template<>
struct std::formatter<Ast> : std::formatter<std::string_view> {
    static auto format(const Ast &ast, std::format_context &ctx) {
        return std::visit(
            [&]<typename T0>(const T0 &ptr) {
                using T = std::decay_t<T0>::element_type;

                if constexpr (std::is_same_v<T, SegmentBlock>) {
                    auto out = ctx.out();

                    std::format_to(out, "segment {}:", ptr->name);

                    for (const auto &child: ptr->children) {
                        std::format_to(out, "\n{}", child);
                    }

                    return out;
                } else if constexpr (std::is_same_v<T, NewDeclaration>) {
                    return std::format_to(
                        ctx.out(),
                        "new {} {}({})",
                        ptr->ident.lexeme,
                        ptr->type.lexeme,
                        *ptr->value
                    );
                } else if constexpr (std::is_same_v<T, SetDeclaration>) {
                    return std::format_to(
                        ctx.out(),
                        "set {} {}",
                        ptr->ident.lexeme,
                        *ptr->value
                    );
                }
            },
            ast.node
        );
    }
};

template<typename T, typename... Args>
Ast make_ast(Args &&... args) {
    return Ast(
        std::in_place_type<T>,
        std::forward<Args>(args)...
    );
}

#endif // RPGEN_AST_H
