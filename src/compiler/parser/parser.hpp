#pragma once

#include <memory>
#include <vector>

#include "../ast/Expr.hpp"
#include "../ast/Stmt.hpp"
#include "../lexer/token.hpp"

namespace cool::compiler::parser {
struct Parser
{
    std::vector<lexer::Token> tokens;
    int                       current = 0;

    explicit Parser(std::vector<lexer::Token> tokens);
    std::vector<std::unique_ptr<ast::Stmt>> parse();
    std::unique_ptr<ast::Stmt>              declaration();
    std::unique_ptr<ast::Stmt>              var_declaration();
    std::unique_ptr<ast::Stmt>              statement();
    std::unique_ptr<ast::Stmt>              print_statement();
    std::unique_ptr<ast::Stmt>              expr_statement();
    std::unique_ptr<ast::Stmt>              if_statement();
    std::unique_ptr<ast::Stmt>              while_statement();
    std::unique_ptr<ast::Stmt>              for_statement();
    std::unique_ptr<ast::Stmt>              return_statement();
    std::unique_ptr<ast::Stmt>              function(const std::string &kind);
    std::unique_ptr<ast::Stmt>              class_declaration();
    std::unique_ptr<ast::Stmt>              block();
    std::unique_ptr<ast::Expr>              expression();
    std::unique_ptr<ast::Expr>              assignment();
    std::unique_ptr<ast::Expr>              logic_or();
    std::unique_ptr<ast::Expr>              logic_and();
    std::unique_ptr<ast::Expr>              equality();
    std::unique_ptr<ast::Expr>              comparison();
    std::unique_ptr<ast::Expr>              term();
    std::unique_ptr<ast::Expr>              factor();
    std::unique_ptr<ast::Expr>              unary();
    std::unique_ptr<ast::Expr>              call();
    std::unique_ptr<ast::Expr>              primary();
    bool                                    match(const std::vector<lexer::TokenType> &types);
    bool                                    check(lexer::TokenType type);
    lexer::Token                            advance();
    bool                                    is_at_end();
    lexer::Token                            peek();
    lexer::Token                            previous();
    lexer::Token consume(lexer::TokenType type, const std::string &message);
    void         synchronize();
};
} // namespace cool::compiler::parser