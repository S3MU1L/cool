#include "Stmt.hpp"
#include <utility>

namespace cool::compiler::ast {
VarDecl::VarDecl(lexer::Token name, lexer::Token type, std::unique_ptr<Expr> initializer)
    : name(std::move(name)), type(std::move(type)), initializer(std::move(initializer))
{}

ExprStatement::ExprStatement(std::unique_ptr<Expr> expression) : expression{std::move(expression)}
{}

If::If(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> then_branch,
       std::unique_ptr<Stmt> else_branch)
    : condition{std::move(condition)}, then_branch{std::move(then_branch)},
      else_branch{std::move(else_branch)}
{}

While::While(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
    : condition{std::move(condition)}, body{std::move(body)}
{}

Return::Return(std::unique_ptr<Expr> expression) : expression{std::move(expression)} {}

Print::Print(std::unique_ptr<Expr> expression) : expression{std::move(expression)} {}

Function::Function(lexer::Token name, std::vector<std::pair<lexer::Token, lexer::Token>> params,
                   lexer::Token return_type, std::unique_ptr<Stmt> body)
    : name{std::move(name)}, params{std::move(params)}, return_type{std::move(return_type)},
      body{std::move(body)}
{}

Class::Class(lexer::Token name, std::vector<std::unique_ptr<Stmt>> methods,
             std::vector<std::unique_ptr<Stmt>> attributes, lexer::Token parent)
    : name{std::move(name)}, methods{std::move(methods)}, attributes{std::move(attributes)},
      parent{std::move(parent)}
{}

Block::Block(std::vector<std::unique_ptr<Stmt>> statements) : statements{std::move(statements)} {}
} // namespace cool::compiler::ast
