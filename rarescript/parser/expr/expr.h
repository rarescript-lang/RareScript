//
// Created by Nizam Mirza on 16/08/2026.
//

#ifndef RPGEN_EXPR_H
#define RPGEN_EXPR_H

#include <cstdint>
#include <memory>
#include <utility>
#include <variant>

#include "token.h"

struct Expr;

struct BinaryExpr;
struct UnaryExpr;
struct IdentifierExpr;
struct IntegerExpr;


struct IdentifierExpr {
    Token ident;

    explicit IdentifierExpr(const Token &ident);
};

struct IntegerExpr {
    Token integer;

    explicit IntegerExpr(const Token &integer);
};


enum class UnaryKind : std::uint8_t {
    Negate,
    Not
};

struct UnaryExpr {
    SourceLocation location;
    std::unique_ptr<Expr> operand;
    UnaryKind kind;

    UnaryExpr(
        std::unique_ptr<Expr> operand,
        UnaryKind kind,
        const Token &op
    );

    ~UnaryExpr();

    UnaryExpr(const UnaryExpr &) = delete;

    UnaryExpr &operator=(const UnaryExpr &) = delete;

    UnaryExpr(UnaryExpr &&) noexcept;

    UnaryExpr &operator=(UnaryExpr &&) noexcept;
};

enum class OpKind : std::uint8_t {
    Add,
    Subtract,
    Multiply,
    Divide
};

struct BinaryExpr {
    SourceLocation location;
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
    OpKind op;

    BinaryExpr(
        std::unique_ptr<Expr> left,
        OpKind op,
        std::unique_ptr<Expr> right
    );

    ~BinaryExpr();

    BinaryExpr(const BinaryExpr &) = delete;

    BinaryExpr &operator=(const BinaryExpr &) = delete;

    BinaryExpr(BinaryExpr &&) noexcept;

    BinaryExpr &operator=(BinaryExpr &&) noexcept;
};


struct Expr {
    using Node = std::variant<
        std::unique_ptr<BinaryExpr>,
        std::unique_ptr<UnaryExpr>,
        std::unique_ptr<IdentifierExpr>,
        std::unique_ptr<IntegerExpr>
    >;

    Node node;

    template<typename T, typename... Args>
    explicit Expr(
        std::in_place_type_t<T>,
        Args &&... args
    )
        : node(
            std::make_unique<T>(
                std::forward<Args>(args)...
            )
        ) {
    }

    explicit Expr(Node node)
        : node(std::move(node)) {
    }

    ~Expr() = default;

    Expr(const Expr &) = delete;

    Expr &operator=(const Expr &) = delete;

    Expr(Expr &&) noexcept = default;

    Expr &operator=(Expr &&) noexcept = default;

    SourceLocation &source_location() noexcept;
};


// IdentifierExpr

inline IdentifierExpr::IdentifierExpr(const Token &ident)
    : ident(ident) {
}

// IntegerExpr

inline IntegerExpr::IntegerExpr(const Token &integer)
    : integer(integer) {
}

// UnaryExpr

inline UnaryExpr::UnaryExpr(
    std::unique_ptr<Expr> operand,
    const UnaryKind kind,
    const Token &op
)
    : location(op.location.extend(operand->source_location())),
      operand(std::move(operand)),
      kind(kind) {
}

inline UnaryExpr::~UnaryExpr() = default;

inline UnaryExpr::UnaryExpr(UnaryExpr &&) noexcept = default;

inline UnaryExpr &UnaryExpr::operator=(UnaryExpr &&) noexcept = default;

// BinaryExpr

inline BinaryExpr::BinaryExpr(
    std::unique_ptr<Expr> left,
    const OpKind op,
    std::unique_ptr<Expr> right
)
    : location(left->source_location().extend(right->source_location())),
      left(std::move(left)),
      right(std::move(right)),
      op(op) {
}

inline BinaryExpr::~BinaryExpr() = default;

inline BinaryExpr::BinaryExpr(BinaryExpr &&) noexcept = default;

inline BinaryExpr &BinaryExpr::operator=(BinaryExpr &&) noexcept = default;

template<typename T, typename... Args>
std::unique_ptr<Expr> make_expr(Args &&... args) {
    return std::make_unique<Expr>(
        std::in_place_type<T>,
        std::forward<Args>(args)...
    );
}

#endif // RPGEN_EXPR_H
