//
// Created by Nizam Mirza on 16/08/2026.
//

#include "expr.h"

SourceLocation &Expr::source_location() noexcept {
    return std::visit([]<typename T0>(T0 &ptr) -> SourceLocation & {
        // ptr is automatically deduced as the active std::unique_ptr here
        using T = std::decay_t<T0>::element_type;

        if constexpr (std::same_as<T, BinaryExpr> || std::same_as<T, UnaryExpr>) {
            return ptr->location;
        } else if constexpr (std::same_as<T, IdentifierExpr>) {
            return ptr->ident.location;
        } else if constexpr (std::same_as<T, IntegerExpr>) {
            return ptr->integer.location;
        } else {
            throw std::runtime_error("Unhandled AST element node path intercepted!");
        }
    }, node);
}
