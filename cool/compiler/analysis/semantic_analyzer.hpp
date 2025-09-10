#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace cool::compiler::ast {
struct Expr;
struct Stmt;
}
namespace cool::compiler::analysis {
struct Resolver {
    void resolve(const std::vector<std::unique_ptr<ast::Stmt>> &statements);

    std::pmr::unordered_map<std::string, std::string> symbol_table;
    void resolve_stmt(const std::unique_ptr<ast::Stmt> &stmt);
    void resolve_expr(const std::unique_ptr<ast::Expr> &expr);
}