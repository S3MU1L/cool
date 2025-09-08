#pragma once
#include "../lexer/token.hpp"

#include <memory>
#include <vector>

namespace cool::compiler::ast {
struct Expr
{
    virtual ~Expr() = default;
};

struct Binary final : Expr
{
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;
    lexer::Token          op;
    Binary(std::unique_ptr<Expr> lhs, lexer::Token op, std::unique_ptr<Expr> rhs);
};

struct Unary final : Expr
{
    std::unique_ptr<Expr> operand;
    lexer::Token          op;
    Unary(lexer::Token op, std::unique_ptr<Expr> operand);
};

struct Logical final : Expr
{
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;
    lexer::Token          op;
    Logical(std::unique_ptr<Expr> lhs, lexer::Token op, std::unique_ptr<Expr> rhs);
};

struct Literal final : Expr
{
    lexer::Literal value;
    explicit Literal(lexer::Literal value);
};

struct Grouping final : Expr
{
    std::unique_ptr<Expr> expr;
    explicit Grouping(std::unique_ptr<Expr> expr);
};

struct Variable final : Expr
{
    lexer::Token name;
    explicit Variable(lexer::Token name);
};

struct Assignment final : Expr
{
    lexer::Token          name;
    std::unique_ptr<Expr> value;
    Assignment(lexer::Token name, std::unique_ptr<Expr> value);
};

struct Call final : Expr
{
    std::unique_ptr<Expr>              callee;
    lexer::Token                       paren;
    std::vector<std::unique_ptr<Expr>> arguments;
    Call(std::unique_ptr<Expr> callee, lexer::Token paren,
         std::vector<std::unique_ptr<Expr>> arguments);
};
} // namespace cool::compiler::ast