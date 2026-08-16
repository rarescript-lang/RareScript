//
// Created by Nizam Mirza on 13/08/2026.
//

#ifndef RPGEN_LEXER_H
#define RPGEN_LEXER_H
#include <cstdint>
#include <string_view>
#include <vector>

#include "token.h"

class Lexer {
    std::string_view source;
    uint32_t idx, len, line;
    uint16_t col;

public:
    explicit Lexer(const std::string_view source) : source(source), idx(0), len(source.length()), line(1), col(1) {
    }

    std::vector<Token> tokenize() noexcept;
};

#endif //RPGEN_LEXER_H
