#pragma once

#include <string>
#include <vector>

namespace cool::compiler::lexer
{
	struct Lexer
	{
		std::string content;
		std::vector<Token> tokens{};
		int start = 0;
		int current = 0;
		int line = 1;

		Lexer( std::string content ) : content{ std::move( content ) } {};

		char peek();

		bool is_end();

		void advance();

		bool match( char c );
	};
}