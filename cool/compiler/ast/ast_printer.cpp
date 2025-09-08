#include "ast_printer.hpp"

#include <iostream>
#include <vector>
namespace cool::compiler::ast {
struct Stmt;

void AstPrinter::print(const std::vector<std::unique_ptr<Stmt>> &statements)
{
    for (const auto &stmt : statements)
    {
        print_stmt(stmt.get(), 0);
    }
}

void AstPrinter::print_indent(const int indent)
{
    for (int i = 0; i < indent; ++i)
        std::cout << "  ";
}

void AstPrinter::print_stmt(const Stmt *stmt, const int indent)
{
    if (const auto *v = dynamic_cast<const VarDecl *>(stmt))
    {
        print_indent(indent);
        std::cout << "VarDecl: " << v->name.lexeme << ", type: " << v->type.lexeme << '\n';
        if (v->initializer)
            print_expr(v->initializer.get(), indent + 1);
    }
    else if (const auto *e = dynamic_cast<const ExprStatement *>(stmt))
    {
        print_indent(indent);
        std::cout << "ExprStatement:" << '\n';
        print_expr(e->expression.get(), indent + 1);
    }
    else if (const auto *i = dynamic_cast<const If *>(stmt))
    {
        print_indent(indent);
        std::cout << "If:" << '\n';
        print_indent(indent + 1);
        std::cout << "Condition:" << '\n';
        print_expr(i->condition.get(), indent + 2);
        print_indent(indent + 1);
        std::cout << "Then:" << '\n';
        print_stmt(i->then_branch.get(), indent + 2);
        if (i->else_branch)
        {
            print_indent(indent + 1);
            std::cout << "Else:" << '\n';
            print_stmt(i->else_branch.get(), indent + 2);
        }
    }
    else if (const auto *w = dynamic_cast<const While *>(stmt))
    {
        print_indent(indent);
        std::cout << "While:" << '\n';
        print_indent(indent + 1);
        std::cout << "Condition:" << '\n';
        print_expr(w->condition.get(), indent + 2);
        print_indent(indent + 1);
        std::cout << "Body:" << '\n';
        print_stmt(w->body.get(), indent + 2);
    }
    else if (const auto *r = dynamic_cast<const Return *>(stmt))
    {
        print_indent(indent);
        std::cout << "Return:" << '\n';
        if (r->expression)
            print_expr(r->expression.get(), indent + 1);
    }
    else if (const auto *p = dynamic_cast<const Print *>(stmt))
    {
        print_indent(indent);
        std::cout << "Print:" << '\n';
        print_expr(p->expression.get(), indent + 1);
    }
    else if (const auto *f = dynamic_cast<const Function *>(stmt))
    {
        print_indent(indent);
        std::cout << "Function: " << f->name.lexeme << " returns " << f->return_type.lexeme << '\n';
        print_indent(indent + 1);
        std::cout << "Params:";
        for (const auto &param : f->params)
        {
            std::cout << " (" << param.first.lexeme << ": " << param.second.lexeme << ")";
        }
        std::cout << '\n';
        print_indent(indent + 1);
        std::cout << "Body:" << '\n';
        print_stmt(f->body.get(), indent + 2);
    }
    else if (const auto *c = dynamic_cast<const Class *>(stmt))
    {
        print_indent(indent);
        std::cout << "Class: " << c->name.lexeme << " inherits " << c->parent.lexeme << '\n';
        print_indent(indent + 1);
        std::cout << "Attributes:" << '\n';
        for (const auto &attr : c->attributes)
            print_stmt(attr.get(), indent + 2);
        print_indent(indent + 1);
        std::cout << "Methods:" << '\n';
        for (const auto &method : c->methods)
            print_stmt(method.get(), indent + 2);
    }
    else if (const auto *b = dynamic_cast<const Block *>(stmt))
    {
        print_indent(indent);
        std::cout << "Block:" << '\n';
        for (const auto &s : b->statements)
            print_stmt(s.get(), indent + 1);
    }
    else
    {
        print_indent(indent);
        std::cout << "Unknown Stmt" << '\n';
    }
}

void AstPrinter::print_expr(const Expr *expr, int indent)
{
    if (const auto *b = dynamic_cast<const Binary *>(expr))
    {
        print_indent(indent);
        std::cout << "Binary: " << b->op.lexeme << '\n';
        print_expr(b->lhs.get(), indent + 1);
        print_expr(b->rhs.get(), indent + 1);
    }
    else if (const auto *u = dynamic_cast<const Unary *>(expr))
    {
        print_indent(indent);
        std::cout << "Unary: " << u->op.lexeme << '\n';
        print_expr(u->operand.get(), indent + 1);
    }
    else if (const auto *l = dynamic_cast<const Logical *>(expr))
    {
        print_indent(indent);
        std::cout << "Logical: " << l->op.lexeme << '\n';
        print_expr(l->lhs.get(), indent + 1);
        print_expr(l->rhs.get(), indent + 1);
    }
    else if (const auto *lit = dynamic_cast<const Literal *>(expr))
    {
        print_indent(indent);
        std::cout << "Literal: ";
        std::visit(
                [](auto &&arg) {
                    using T = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_same_v<T, std::monostate>)
                        std::cout << "null";
                    else if constexpr (std::is_same_v<T, std::string>)
                        std::cout << '"' << arg << '"';
                    else if constexpr (std::is_same_v<T, bool>)
                        std::cout << (arg ? "true" : "false");
                    else if constexpr (std::is_same_v<T, double>)
                        std::cout << arg;
                    else
                        std::cout << "unknown literal type";
                },
                lit->value);
        std::cout << '\n';
    }
    else if (const auto *g = dynamic_cast<const Grouping *>(expr))
    {
        print_indent(indent);
        std::cout << "Grouping:" << '\n';
        print_expr(g->expr.get(), indent + 1);
    }
    else if (const auto *v = dynamic_cast<const Variable *>(expr))
    {
        print_indent(indent);
        std::cout << "Variable: " << v->name.lexeme << '\n';
    }
    else if (const auto *a = dynamic_cast<const Assignment *>(expr))
    {
        print_indent(indent);
        std::cout << "Assignment: " << a->name.lexeme << '\n';
        print_expr(a->value.get(), indent + 1);
    }
    else if (const auto *c = dynamic_cast<const Call *>(expr))
    {
        print_indent(indent);
        std::cout << "Call:" << '\n';
        print_expr(c->callee.get(), indent + 1);
        print_indent(indent + 1);
        std::cout << "Arguments:" << '\n';
        for (const auto &arg : c->arguments)
            print_expr(arg.get(), indent + 2);
    }
    else
    {
        print_indent(indent);
        std::cout << "Unknown Expr" << '\n';
    }
}
} // namespace cool::compiler::ast