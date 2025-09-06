#pragma once
#include "Expr.hpp"

namespace cool::compiler::ast {
struct Stmt
{
    virtual ~Stmt() = default;
};

struct VarDecl final : Stmt
{
    lexer::Token          name;
    lexer::Token          type;
    std::unique_ptr<Expr> initializer;
};

struct ExprStatement final : Stmt
{
    std::unique_ptr<Expr> expression;
};

struct If final : Stmt
{
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> then_branch;
    std::unique_ptr<Stmt> else_branch;
};

struct While final : Stmt
{
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;
};

struct For final : Stmt
{};

struct Return final : Stmt
{
    std::unique_ptr<Expr> expression;
};

struct Print final : Stmt
{
    std::unique_ptr<Expr> expression;
};

struct Function final : Stmt
{
    lexer::Token                                       name;
    std::vector<std::pair<lexer::Token, lexer::Token>> params;
    lexer::Token                                       return_type;
    std::unique_ptr<Stmt>                              body;
};

struct Class final : Stmt
{
    lexer::Token                       name;
    std::vector<std::unique_ptr<Stmt>> methods;
    std::vector<std::unique_ptr<Stmt>> attributes;
    lexer::Token                       parent;
};

struct Block final : Stmt
{
    std::vector<std::unique_ptr<Stmt>> statements;
};
} // namespace cool::compiler::ast