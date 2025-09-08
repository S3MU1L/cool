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
    EQUAL,
    AMPERSAND,
    PIPE,
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
    SEMICOLON,

    // Two-character tokens
    ARROW,
    AND,
    OR,
    BANG_EQUAL,
    LESS_EQUAL,
    GREATER_EQUAL,
    EQUALS_EQUAL,

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
    EXTENDS,
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