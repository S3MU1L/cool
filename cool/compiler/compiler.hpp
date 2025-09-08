#pragma once

#include "compilation_result.hpp"
#include "lexer/token.hpp"

#include <string>
namespace cool::compiler {
struct Compiler
{
    static bool hasError;
    std::string file_name;

    explicit Compiler(std::string file_name);
    static bool                     check_file(const std::string &file_name);
    static void                     report(const std::string &message);
    static void                     error(const std::string &message);
    static void                     error(int line, const std::string &message);
    static void                     error(const lexer::Token &token, const std::string &message);
    static std::string              read_file(const std::ifstream &file);
    [[nodiscard]] CompilationResult compile() const;
};
} // namespace cool::compiler