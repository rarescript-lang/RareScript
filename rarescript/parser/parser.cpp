//
// Created by Nizam Mirza on 14/08/2026.
//

#include "parser.h"

#include <unordered_map>

#include "ast/ast.h"


[[nodiscard]]
static KeywordKind token_to_keyword(const Token &token) {
    static const std::unordered_map<std::string_view, KeywordKind> keyword_map = {
        {"new", KeywordKind::New},
        {"set", KeywordKind::Set},
    };

    if (const auto it = keyword_map.find(token.lexeme); it != keyword_map.end()) {
        return it->second;
    }
    // TODO:
    // - This condition is entered when the first condition failed
    //
    // Solution: Make this function report an error
    throw std::runtime_error("Invalid token to keyword");
}

namespace {
    enum class Precedence : uint8_t {
        LOWEST = 0,
        SUM, // + , -
        PRODUCT, // * , /
        PREFIX, // -x , !x
        CALL // a[b], a(...)
    };
}

[[nodiscard]]
static constexpr Precedence get_precedence(const TokenKind kind) noexcept {
    switch (kind) {
        case TokenKind::Plus:
        case TokenKind::Minus: return Precedence::SUM;
        case TokenKind::Star:
        case TokenKind::Slash: return Precedence::PRODUCT;
        case TokenKind::LeftBrace: return Precedence::CALL;
        default: return Precedence::LOWEST;
    }
}


TokenKind Parser::peek() const noexcept {
    return idx < len ? tokens[idx].kind : TokenKind::Eof;
}

Token Parser::advance() noexcept {
    return tokens[idx++];
}

Token Parser::expect(const TokenKind kind) {
    if (peek() == kind) {
        return advance();
    }
    // TODO:
    // - This condition is entered when the first condition failed
    //
    // Solution: Make this function report an error
    throw std::runtime_error(std::format("Unexpected token `{}`", to_string(kind)));
}

std::unique_ptr<Expr> Parser::operand() {
    switch (peek()) {
        case TokenKind::Identifier:
            return make_expr<IdentifierExpr>(advance());
        case TokenKind::Integer:
            return make_expr<IntegerExpr>(advance());
        case TokenKind::Minus: {
            const auto op = advance();
            auto operand = parse_expr(static_cast<uint8_t>(Precedence::PREFIX));
            return make_expr<UnaryExpr>(std::move(operand), UnaryKind::Negate, op);
        }
        case TokenKind::LeftParenthesis: {
            const auto lp = advance();
            auto value = parse_expr(static_cast<uint8_t>(Precedence::LOWEST));
            if (peek() != TokenKind::RightParenthesis) {
                // TODO:
                // - This condition is entered when the expected `)` is not present
                // - where this expr need one because of the starting `(`
                //
                // Solution: Make this function report an error
            }
            const auto rp = advance();
            value->source_location() = lp.location.extend(rp.location);
            return value;
        }
        default:
            // TODO:
            // - This condition is entered when there are not a valid operand
            //
            // Solution: Make this function report an error
            throw std::runtime_error("Invalid operand");
    }
}

std::unique_ptr<Expr> Parser::parse_expr(const uint8_t min_precedence) {
    if (peek() == TokenKind::Eof) {
        // TODO:
        // - This condition is entered when there is no expression (EOF)
        //
        // Solution: Make this function report an error
        throw std::runtime_error("Unexpected EOF, expected a value");
    }

    auto left = operand();

    while (true) {
        if (peek() == TokenKind::Eof) break;

        const auto precedence = static_cast<uint8_t>(get_precedence(peek()));

        // If the next operator binds weaker or equally to our context, we wrap up
        if (min_precedence >= precedence) {
            break;
        }

        // We commit to parsing this operator, so consume it
        const auto op = advance();

        auto right = parse_expr(precedence);
        OpKind op_kind;
        switch (op.kind) {
            case TokenKind::Plus:
                op_kind = OpKind::Add;
                break;
            case TokenKind::Minus:
                op_kind = OpKind::Subtract;
                break;
            case TokenKind::Star:
                op_kind = OpKind::Multiply;
                break;
            case TokenKind::Slash:
                op_kind = OpKind::Divide;
                break;
            default:
                return left;
        }
        left = make_expr<BinaryExpr>(std::move(left), op_kind, std::move(right));
    }

    return left;
}

std::vector<Ast> Parser::parse() {
    std::vector<Ast> asts{};
    auto current_vec = &asts;
    while (idx < len) {
        switch (const auto token = advance(); token.kind) {
            // Start of a segment block
            case TokenKind::LeftBrace: {
                const auto ident = expect(TokenKind::Identifier);
                const auto rb = expect(TokenKind::RightBrace);

                // Every segment must be the children of the global asts
                // Segment inside a segment is not allowed
                current_vec = &asts;

                current_vec->emplace_back(SegmentBlock(ident.lexeme, token.location.extend(rb.location)));

                // Now every ast below this segment are belong to this segment
                current_vec = &std::get<SegmentBlock>(current_vec->back()).children;
                break;
            }

            case TokenKind::Identifier: {
                switch (token_to_keyword(token)) {
                    case KeywordKind::New: {
                        const auto ident = expect(TokenKind::Identifier);
                        const auto type = expect(TokenKind::Identifier);
                        if (peek() == TokenKind::LeftParenthesis) {
                            // operand() consumes left parenthesis and ensure it's pair is present
                            auto value = operand();
                            current_vec->emplace_back(NewDeclaration(ident, type, std::move(value)));
                        } else {
                            // TODO:
                            // - This condition is entered because there is no left parenthesis
                            // - when a valid assignment require `T(...)` syntax
                            //
                            // Solution: Make this function report an error
                        }
                        break;
                    }
                    case KeywordKind::Set: {
                        const auto ident = expect(TokenKind::Identifier);
                        auto value = parse_expr(static_cast<uint8_t>(Precedence::LOWEST));
                        current_vec->emplace_back(SetDeclaration(ident, std::move(value)));
                    }
                }
                break;
            }
            default:
                // TODO:
                // - This condition is entered because there is no valid statement
                //
                // Solution: Make this function report an error
                throw std::runtime_error("Invalid start of statement");
        }
    }
    return asts;
}
