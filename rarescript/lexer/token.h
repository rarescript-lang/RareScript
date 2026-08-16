//
// Created by Nizam Mirza on 13/08/2026.
//

#ifndef RPGEN_TOKEN_H
#define RPGEN_TOKEN_H
#include <cstdint>
#include <format>
#include <string_view>

enum class TokenKind : uint32_t {
    Identifier,
    Integer,

    LeftBrace,
    RightBrace,
    LeftParenthesis,
    RightParenthesis,

    Plus,
    Minus,
    Star,
    Slash,

    Unknown,
    Eof
};

constexpr std::string_view to_string(const TokenKind kind) noexcept {
    switch (kind) {
        case TokenKind::LeftBrace: return "LeftBrace";
        case TokenKind::RightBrace: return "RightBrace";
        case TokenKind::LeftParenthesis: return "LeftParenthesis";
        case TokenKind::RightParenthesis: return "RightParenthesis";
        case TokenKind::Integer: return "Integer";
        case TokenKind::Identifier: return "Identifier";
        case TokenKind::Unknown: return "Unknown";
        case TokenKind::Eof: return "Eof";
    }
    return "Invalid";
}

enum class KeywordKind : uint32_t {
    New,
    Set
};

struct SourceLocation {
    uint32_t s_idx, e_idx;
    uint16_t s_col, e_col;
    uint32_t s_line, e_line;

    [[nodiscard]] SourceLocation extend(const SourceLocation &location) const noexcept {
        return SourceLocation{
            .s_idx = s_idx,
            .e_idx = location.e_idx,
            .s_col = s_col,
            .e_col = location.e_col,
            .s_line = s_line,
            .e_line = location.e_line,
        };
    }
};

template<>
struct std::formatter<SourceLocation> : std::formatter<std::string_view> {
    static auto format(const SourceLocation &location, format_context &ctx) {
        return std::format_to(
            ctx.out(), "{{\n"
            "   .s_idx = {},\n"
            "   .e_idx = {},\n"
            "   .s_col = {},\n"
            "   .e_col = {},\n"
            "   .s_line = {},\n"
            "   .e_line = {}\n"
            "}}",
            location.s_idx, location.e_idx, location.s_col, location.e_col, location.s_line, location.e_line);
    }
};


struct Token {
    SourceLocation location;
    TokenKind kind;
    std::string_view lexeme;
};

template<>
struct std::formatter<Token> : std::formatter<std::string_view> {
    static auto format(const Token &token, format_context &ctx) {
        return std::format_to(ctx.out(), "Token {{ .location = {}, .kind = {}, .lexeme = \"{}\" }}",
                              token.location, to_string(token.kind), token.lexeme);
    }
};

#endif //RPGEN_TOKEN_H
