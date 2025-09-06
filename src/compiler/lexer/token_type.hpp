#pragma once

namespace cool::compiler::lexer {
enum TokenType {
    // Single-character tokens
    DOT,
    COMMA,
    COLON,
    MINUS,
    PLUS,
    BANG,
    EQUALS,
    SLASH,
    STAR,
    ASTRIX,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    PERCENT,
    LESS,
    GREATER,
    SEMICOLUMN,

    // Two-character tokens
    BANG_EQUALS,
    LESS_EQUAL,
    GREATER_EQUAL,
    EQUALS_EQUALS,

    // Literals
    NUMBER,
    STRING,
    IDENTIFIER,

    // Keywords
    FN,
    VAR,
    VAL,
    TRUE,
    FALSE,
    FOR,
    WHILE,
    IF,
    ELSE,
    PRINT,
    RETURN,
    CLASS,

    // Types
    INT,
    STRING_TYPE,
    BOOL,
    VOID,
    NULL_TYPE,

    // End of file
    END_OF_FILE
};
}