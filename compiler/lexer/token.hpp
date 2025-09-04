#pragma once

#include <string>
#include <variant>

namespace cool::compiler::lexer
{

	using Literal = std::variant< std::monostate, std::string, int, bool>;

	struct Token
	{
		TokenType type;
		Literal literal;
		std::string lexeme;
		int line;

		std::string to_string() {
			return lexeme + " ( line " + std::to_string( line ) + " )";
		}
	};
}