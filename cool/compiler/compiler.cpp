#include "compiler.hpp"

#include "lexer/lexer.hpp"
#include "parser/parser.hpp"

#include "ast/ast_printer.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace cool::compiler {

bool Compiler::hasError = false;

Compiler::Compiler(std::string file_name) : file_name{std::move(file_name)} {}

bool Compiler::check_file(const std::string &file_name)
{
    const std::size_t last_dot = file_name.rfind('.');
    if (last_dot == std::string::npos)
        return false;

    std::string substr = file_name.substr(last_dot);
    return substr == ".cl";
}

std::string Compiler::read_file(const std::ifstream &file)
{
    std::stringstream sstream;
    sstream << file.rdbuf();
    std::string content = sstream.str();
    return content;
}

CompilationResult Compiler::compile() const
{
    if (!check_file(file_name))
    {
        error("Unsupported file extension, please provide a .cl file");
        return FILE_ERROR;
    }

    if (!std::filesystem::exists(file_name))
    {
        std::cerr << "Error: File does not exist.\n";
        return FILE_NOT_FOUND;
    }

    std::ifstream file(file_name);
    std::string   content = read_file(file);
    lexer::Lexer  lexer{content};
    lexer.scan_tokens();
    if (hasError)
        return LEXICAL_ERROR;

    std::vector<lexer::Token> tokens = lexer.tokens;
    std::cout << "Tokens:\n";
    for (const auto &token : tokens)
        std::cout << token.to_string() << '\n';

    parser::Parser                          parser{tokens};
    std::vector<std::unique_ptr<ast::Stmt>> statements = parser.parse();
    if (hasError)
        return SYNTAX_ERROR;

    ast::AstPrinter::print(statements);
    return SUCCESS;
}

void Compiler::error(const std::string &message)
{
    report(message);
    hasError = true;
}

void Compiler::error(const int line, const std::string &message)
{
    report("[line " + std::to_string(line) + "] Error: " + message);
    hasError = true;
}

void Compiler::error(const lexer::Token &token, const std::string &message)
{
    if (token.type == lexer::TokenType::END_OF_FILE)
    {
        report("[line " + std::to_string(token.line) + "] at end: " + message);
    }
    else
    {
        report("[line " + std::to_string(token.line) + "] at '" + token.lexeme + "': " + message);
    }
    hasError = true;
}

void Compiler::report(const std::string &message)
{
    std::cerr << message << '\n';
}
} // namespace cool::compiler