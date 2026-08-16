//
// Created by Nizam Mirza on 14/08/2026.
//
#include "lexer.h"

std::vector<Token> Lexer::tokenize() noexcept {
    std::vector<Token> tokens{};

    // Helper 1: Skipping characters
    auto skip = [&](auto is_kind) noexcept {
        while (idx < len && is_kind(source[idx])) {
            idx += 1;
            col += 1;
        }
    };

    // Helper 2: For multi-character tokens
    auto gather = [&](const TokenKind kind, auto is_kind) noexcept {
        const auto start_idx = idx;
        const auto start_col = col;
        skip(is_kind);
        tokens.emplace_back(
            SourceLocation{
                .s_idx = start_idx,
                .e_idx = idx,
                .s_col = start_col,
                .e_col = col,
                .s_line = line,
                .e_line = line,
            }, kind, source.substr(start_idx, idx - start_idx)
        );
    };

    // Helper 3: Pushing a single character
    auto push_single = [&](const TokenKind tk) noexcept {
        tokens.emplace_back(
            SourceLocation{
                .s_idx = idx,
                .e_idx = idx + 1,
                .s_col = col,
                .e_col = static_cast<uint16_t>(col + 1),
                .s_line = line,
                .e_line = line,
            }, tk, source.substr(idx, 1)
        );
        idx += 1;
        col += 1;
    };

    while (idx < len) {
        skip([](const unsigned char ch) noexcept {
            return ch != '\n' && std::isspace(ch);
        });
        if (idx >= len) {
            break;
        }
        switch (const auto c = source[idx]) {
            case '[':
                push_single(TokenKind::LeftBrace);
                break;
            case ']':
                push_single(TokenKind::RightBrace);
                break;
            case '(':
                push_single(TokenKind::LeftParenthesis);
                break;
            case ')':
                push_single(TokenKind::RightParenthesis);
                break;

            case '\n':
                col = 1;
                ++line;
                ++idx;
                break;
            default:
                const auto u_c = static_cast<unsigned char>(c);
                if (std::isdigit(u_c)) {
                    gather(TokenKind::Integer, [](const unsigned char ch) {
                        return std::isdigit(ch);
                    });
                } else if (std::isalpha(u_c) || c == '_') {
                    gather(TokenKind::Identifier, [](const unsigned char ch) {
                        return std::isalnum(ch) || ch == '_';
                    });
                } else {
                    // Prevent infinite loops on unhandled chars
                    push_single(TokenKind::Unknown);
                }
        }
    }
    return tokens;
}
