
#include "lexer.hpp";

namespace cool::compiler::lexer
{

	Token Lexer::get_token() { }

	char Lexer::peek() { return content.at( current ); }

	bool Lexer::is_end() { return current == content.size(); }
	
}