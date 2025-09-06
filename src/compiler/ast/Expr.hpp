#pragma once
#include "../lexer/token.hpp"

#include <vector>
#include <memory>

namespace cool::compiler::ast {
struct Expr
{
    virtual ~Expr() = default;
};

struct Binary final : Expr
{
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;
    lexer::Token op;
};

struct Unary final : Expr
{
    std::unique_ptr<Expr> operand;
    lexer::Token op;
};

struct Logical final : Expr
{
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;
    lexer::Token op;
};

struct Literal final : Expr
{
    lexer::Literal value;
};

struct Grouping final : Expr
{
    std::unique_ptr<Expr> expr;
};

struct Variable final : Expr
{
    lexer::Token name;
};

struct Assignment final : Expr
{
    lexer::Token name;
    std::unique_ptr<Expr> value;
};

struct Call final : Expr
{
    lexer::Token callee;
    std::vector<std::unique_ptr<Expr>> arguments;
};
} // namespace cool::compiler::ast