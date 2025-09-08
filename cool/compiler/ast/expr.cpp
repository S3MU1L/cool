#include "expr.hpp"
#include <utility>

namespace cool::compiler::ast {
Binary::Binary(std::unique_ptr<Expr> lhs, lexer::Token op, std::unique_ptr<Expr> rhs)
    : lhs{std::move(lhs)}, rhs{std::move(rhs)}, op{std::move(op)}
{}

Unary::Unary(lexer::Token op, std::unique_ptr<Expr> operand)
    : operand{std::move(operand)}, op{std::move(op)}
{}

Logical::Logical(std::unique_ptr<Expr> lhs, lexer::Token op, std::unique_ptr<Expr> rhs)
    : lhs{std::move(lhs)}, rhs{std::move(rhs)}, op{std::move(op)}
{}

Literal::Literal(lexer::Literal value) : value{std::move(value)} {}

Grouping::Grouping(std::unique_ptr<Expr> expr) : expr{std::move(expr)} {}

Variable::Variable(lexer::Token name) : name{std::move(name)} {}

Assignment::Assignment(lexer::Token name, std::unique_ptr<Expr> value)
    : name{std::move(name)}, value{std::move(value)}
{}

Call::Call(std::unique_ptr<Expr> callee, lexer::Token paren,
           std::vector<std::unique_ptr<Expr>> arguments)
    : callee{std::move(callee)}, paren{std::move(paren)}, arguments{std::move(arguments)}
{}
} // namespace cool::compiler::ast
