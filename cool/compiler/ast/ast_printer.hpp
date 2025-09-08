#pragma once

#include "stmt.hpp"

#include <memory>
#include <vector>

namespace cool::compiler::ast {
struct AstPrinter
{
    static void print(const std::vector<std::unique_ptr<Stmt>> &statements);
    static void print_indent(int indent);
    static void print_stmt(const Stmt *stmt, int indent);
    static void print_expr(const Expr *expr, int indent);
};

} // namespace cool::compiler::ast
