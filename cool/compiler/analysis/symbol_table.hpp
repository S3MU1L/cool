#pragma once

#include "type.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace cool::compiler::ast {
struct Symbol
{
    std::string    name;
    analysis::Type type;
    int            scope_level;

    Symbol(std::string name, const analysis::Type type, const int scope_level)
        : name(std::move(name)), type(type), scope_level(scope_level)
    {}
};

struct Scope
{
    std::unordered_map<std::string, Symbol> symbols;
};

struct SymbolTable
{
    std::vector<Scope> scope_stack;

    void enter_scope()
    {
        scope_stack.emplace_back();
    }

    void leave_scope()
    {
        scope_stack.pop_back();
    }

    bool insert(const Symbol &symbol)
    {
        if (scope_stack.empty())
        {
            enter_scope();
        }
        auto &current_scope = scope_stack.back();
        auto [it, inserted] = current_scope.symbols.emplace(symbol.name, symbol);
        return inserted;
    }

    Symbol *lookup(const std::string &name)
    {
        for (auto it = scope_stack.rbegin(); it != scope_stack.rend(); ++it)
        {
            auto found = it->symbols.find(name);
            if (found != it->symbols.end())
            {
                return &found->second;
            }
        }
        return nullptr;
    }
};
} // namespace cool::compiler::ast