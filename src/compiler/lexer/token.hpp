#pragma once

#include <string>
#include <variant>

#include "token_type.hpp"

namespace cool::compiler::lexer {
using Literal = std::variant<std::monostate, std::string, bool, double>;

struct Token
{
    TokenType   type;
    Literal     literal;
    std::string lexeme;
    int         line;

    [[nodiscard]] std::string to_string() const
    {
        return lexeme + " ( line " + std::to_string(line) + " )";
    }
};
} // namespace cool::compiler::lexer