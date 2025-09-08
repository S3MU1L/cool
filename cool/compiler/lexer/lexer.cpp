#include "lexer.hpp"

#include "../compiler.hpp"

#include <iostream>
#include <optional>
#include <unordered_map>

namespace cool::compiler::lexer {

Lexer::Lexer(std::string content) : content{std::move(content)} {}

void Lexer::scan_tokens()
{
    while (!is_end())
    {
        scan_token();
        start = current;
    }
    tokens.push_back(Token{END_OF_FILE, {}, "", line});
}

void Lexer::scan_token()
{
    switch (const char c = advance())
    {
    case '.':
        add_token(DOT, {});
        break;
    case ':':
        add_token(COLON, {});
        break;
    case ',':
        add_token(COMMA, {});
        break;
    case '-':
        add_token(match('>') ? ARROW : MINUS, {});
        break;
    case '+':
        add_token(PLUS, {});
        break;
    case '!':
        add_token(match('=') ? BANG_EQUAL : BANG, {});
        break;
    case '=':
        add_token(match('=') ? EQUALS_EQUAL : EQUAL, {});
        break;
    case '/':
        add_token(SLASH, {});
        break;
    case '&':
        add_token(match('&') ? AND : AMPERSAND, {});
        break;
    case '|':
        add_token(match('|') ? OR : PIPE, {});
        break;
    case '*':
        add_token(STAR, {});
        break;
    case '%':
        add_token(PERCENT, {});
        break;
    case '(':
        add_token(LPAREN, {});
        break;
    case ')':
        add_token(RPAREN, {});
        break;
    case '{':
        add_token(LBRACE, {});
        break;
    case '}':
        add_token(RBRACE, {});
        break;
    case ';':
        add_token(SEMICOLON, {});
        break;
    case '<':
        add_token(match('=') ? LESS_EQUAL : LESS, {});
        break;
    case '>':
        add_token(match('=') ? GREATER_EQUAL : GREATER, {});
        break;
    case '\n':
        line++;
        break;
    case ' ':
        break;
    default:
        if (c == '\"')
            string();
        else if (isdigit(c))
            number();
        else if (isalpha(c))
            identifier();
        else
            Compiler::error("Unexpected character.");
    }
}

char Lexer::peek() const
{
    if (is_end())
        return '\0';
    return content.at(current);
}
bool Lexer::is_end() const
{
    return current >= content.size();
}

char Lexer::advance()
{
    const char c = peek();
    current++;
    return c;
}

bool Lexer::match(char c)
{
    if (is_end() || content.at(current) != c)
    {
        return false;
    }
    current++;
    return true;
}

void Lexer::add_token(const TokenType type, const Literal &literal)
{
    const std::string lexeme = content.substr(start, current - start);
    tokens.push_back(Token{type, literal, lexeme, line});
}

void Lexer::string()
{
    while (!is_end() && peek() != '\"')
    {
        if (peek() == '\n')
            line++;
        advance();
    }
    if (is_end())
    {
        Compiler::error(line, "Unterminated string");
        return;
    }
    advance(); // move past the closing "
    const std::string value = content.substr(start + 1, current - start - 1);
    add_token(STRING, value);
}

void Lexer::number()
{
    while (!is_end() && isdigit(peek()))
    {
        advance();
    }
    std::string num_str = content.substr(start, current - start);
    Literal     value   = std::stod(num_str);
    add_token(NUMBER, value);
}

void Lexer::identifier()
{
    while (isalnum(peek()))
        advance();

    const std::string lexeme = content.substr(start, current - start);
    if (const auto tokenType = get_token_type_for_keyword(lexeme); tokenType.has_value())
    {
        add_token(tokenType.value(), {});
        return;
    }
    add_token(IDENTIFIER, {});
}

bool Lexer::isalpha(char c)
{
    return c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Lexer::isalnum(char c)
{
    return isalpha(c) || isdigit(c);
}

bool Lexer::isdigit(char c)
{
    return c >= '0' && c <= '9';
}

std::optional<TokenType> Lexer::get_token_type_for_keyword(const std::string &word)
{
    static const std::unordered_map<std::string, TokenType> keyword_map = {
            {"class", CLASS},    {"var", VAR},
            {"val", VAL},        {"print", PRINT},
            {"fn", FN},          {"true", TRUE},
            {"false", FALSE},    {"if", IF},
            {"else", ELSE},      {"while", WHILE},
            {"for", FOR},        {"return", RETURN},
            {"int", INT},        {"string", STRING_TYPE},
            {"bool", BOOL},      {"void", VOID},
            {"extends", EXTENDS}};

    if (const auto it = keyword_map.find(word); it != keyword_map.end())
        return it->second;
    return std::nullopt;
}
} // namespace cool::compiler::lexer