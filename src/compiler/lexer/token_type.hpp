#pragma once

namespace cool::compiler::lexer
{	
	enum TokenType 
	{
		// Single-character tokens
		DOT, COMMA, MINUS, PLUS, BANG, EQUALS,
		SLASH, STAR, ASTRIX, LPAREN, RPAREN, LBRACE,
		RBRACE, PERCENT, LESS, GREATER, SEMICOLUMN, 

		// Two-character tokens
		BANG_EQUALS, LESS_EQUAL,
		GREATER_EQUAL, EQUALS_EQUALS, 
		
		// Literals
		NUMBER, STRING, IDENTIFIER,

		// Keywords
		FUN, VAR, TRUE, FALSE, FOR, WHILE,
		IF, ELSE, PRINT, 		

		EOF
	};
}