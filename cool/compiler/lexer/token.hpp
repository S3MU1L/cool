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

    static std::string token_type_to_string(const TokenType type)
    {
        switch (type)
        {
        case DOT:
            return "DOT";
        case COMMA:
            return "COMMA";
        case COLON:
            return "COLON";
        case MINUS:
            return "MINUS";
        case PLUS:
            return "PLUS";
        case BANG:
            return "BANG";
        case EQUAL:
            return "EQUAL";
        case AMPERSAND:
            return "AMPERSAND";
        case PIPE:
            return "PIPE";
        case SLASH:
            return "SLASH";
        case STAR:
            return "STAR";
        case ASTRIX:
            return "ASTRIX";
        case LPAREN:
            return "LPAREN";
        case RPAREN:
            return "RPAREN";
        case LBRACE:
            return "LBRACE";
        case RBRACE:
            return "RBRACE";
        case PERCENT:
            return "PERCENT";
        case LESS:
            return "LESS";
        case GREATER:
            return "GREATER";
        case SEMICOLON:
            return "SEMICOLON";
        case ARROW:
            return "ARROW";
        case AND:
            return "AND";
        case OR:
            return "OR";
        case BANG_EQUAL:
            return "BANG_EQUAL";
        case LESS_EQUAL:
            return "LESS_EQUAL";
        case GREATER_EQUAL:
            return "GREATER_EQUAL";
        case EQUALS_EQUAL:
            return "EQUALS_EQUAL";
        case NUMBER:
            return "NUMBER";
        case STRING:
            return "STRING";
        case IDENTIFIER:
            return "IDENTIFIER";
        case FN:
            return "FN";
        case VAR:
            return "VAR";
        case VAL:
            return "VAL";
        case TRUE:
            return "TRUE";
        case FALSE:
            return "FALSE";
        case FOR:
            return "FOR";
        case WHILE:
            return "WHILE";
        case IF:
            return "IF";
        case EXTENDS:
            return "EXTENDS";
        case ELSE:
            return "ELSE";
        case PRINT:
            return "PRINT";
        case RETURN:
            return "RETURN";
        case CLASS:
            return "CLASS";
        case INT:
            return "INT";
        case STRING_TYPE:
            return "STRING_TYPE";
        case BOOL:
            return "BOOL";
        case VOID:
            return "VOID";
        case NULL_TYPE:
            return "NULL_TYPE";
        case END_OF_FILE:
            return "END_OF_FILE";
        default:
            return "UNKNOWN";
        }
    }

    [[nodiscard]] std::string to_string() const
    {
        return lexeme + " ( " + token_type_to_string(type) + " )";
    }
};
} // namespace cool::compiler::lexer