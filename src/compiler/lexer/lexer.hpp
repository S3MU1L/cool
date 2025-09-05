#pragma once

#include <string>
#include <vector>

#include "token.hpp"

#include <optional>

namespace cool::compiler::lexer {
struct Lexer
{
    std::string        content;
    std::vector<Token> tokens{};
    int                start   = 0;
    int                current = 0;
    int                line    = 1;

    explicit Lexer(std::string content);
    void                                   scan_tokens();
    void                                   scan_token();
    [[nodiscard]] char                     peek() const;
    [[nodiscard]] bool                     is_end() const;
    char                                   advance();
    bool                                   match(char c);
    void                                   add_token(TokenType type, const Literal &literal);
    void                                   string();
    void                                   number();
    static inline std::optional<TokenType> get_token_type_for_keyword(const std::string &string);
    void                                   identifier();
    static bool                            isalpha(char c);
    static bool                            isalnum(char c);
    static bool                            isdigit(char c);
};
} // namespace cool::compiler::lexer