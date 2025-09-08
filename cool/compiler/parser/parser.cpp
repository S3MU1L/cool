#include "parser.hpp"

#include "../compiler.hpp"
#include "parse_error.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>
namespace cool::compiler::parser {
Parser::Parser(std::vector<lexer::Token> tokens) : tokens{std::move(tokens)} {}

std::vector<std::unique_ptr<ast::Stmt>> Parser::parse()
{
    std::vector<std::unique_ptr<ast::Stmt>> statements;
    while (!is_at_end())
    {
        statements.push_back(declaration());
    }
    return statements;
}

std::unique_ptr<ast::Stmt> Parser::declaration()
{
    try
    {
        if (match({lexer::CLASS}))
            return class_declaration();
        if (check(lexer::FN) && check_next(lexer::IDENTIFIER))
        {
            consume(lexer::FN, "Expected 'fn' before function declaration.");
            return function_declaration();
        }
        if (match({lexer::VAL, lexer::VAR}))
            return var_declaration();
        return statement();
    } catch (const std::runtime_error &error)
    {
        synchronize();
        return nullptr;
    }
}

std::unique_ptr<ast::Stmt> Parser::var_declaration()
{
    // TODO: we don't know if it is var or val
    lexer::Token name = consume(lexer::IDENTIFIER, "Expected variable name.");
    consume(lexer::COLON, "Expected ':' after variable name.");
    lexer::Token type = consume_any(type_tokens, "Expected type after ':'.");
    consume(lexer::EQUAL, "Expected '=' after variable name.");
    std::unique_ptr<ast::Expr> initializer = expression();
    consume(lexer::SEMICOLON, "Expected ';' after variable declaration.");
    return std::make_unique<ast::VarDecl>(name, type, std::move(initializer));
}

std::unique_ptr<ast::Stmt> Parser::class_declaration()
{
    lexer::Token name = consume(lexer::IDENTIFIER, "Expected class name.");
    lexer::Token parent;
    if (match({lexer::EXTENDS}))
    {
        consume(lexer::IDENTIFIER, "Expected parent class name.");
        parent = previous();
    }
    consume(lexer::LBRACE, "Expected '{' after class name.");
    std::vector<std::unique_ptr<ast::Stmt>> methods;
    std::vector<std::unique_ptr<ast::Stmt>> attributes;
    while (!check(lexer::RBRACE) && !is_at_end())
    {
        if (match({lexer::FN}))
        {
            methods.push_back(function_declaration());
        }
        else if (match({lexer::VAL, lexer::VAR}))
        {
            attributes.push_back(var_declaration());
        }
        else
        {
            throw std::runtime_error("Expected method or attribute declaration.");
        }
    }
    consume(lexer::RBRACE, "Expected '}' after class body.");
    return std::make_unique<ast::Class>(name, std::move(methods), std::move(attributes), parent);
}

std::unique_ptr<ast::Stmt> Parser::function_declaration()
{
    lexer::Token name = consume(lexer::IDENTIFIER, "Expected function name.");
    consume(lexer::LPAREN, "Expected '(' after function name.");
    std::vector<std::pair<lexer::Token, lexer::Token>> parameters;
    if (!check(lexer::RPAREN))
    {
        do
        {
            if (parameters.size() > 255)
            {
                error(peek(), "Cannot have more than 255 parameters.");
            }
            lexer::Token param_name = consume(lexer::IDENTIFIER, "Expected parameter name.");
            consume(lexer::COLON, "Expected ':' after parameter name.");
            lexer::Token param_type = consume_any(type_tokens, "Expected parameter type.");
            parameters.emplace_back(param_name, param_type);
        } while (match({lexer::COMMA}));
    }
    consume(lexer::RPAREN, "Expected ')' after parameters.");
    consume(lexer::ARROW, "Expected '->' after parameters.");
    lexer::Token               return_type = consume_any(type_tokens, "Expected return type.");
    std::unique_ptr<ast::Stmt> body        = block();
    return std::make_unique<ast::Function>(name, parameters, return_type, std::move(body));
}

std::unique_ptr<ast::Stmt> Parser::statement()
{
    if (match({lexer::FN}))
        return function_declaration();
    if (match({lexer::FOR}))
        return for_statement();
    if (match({lexer::IF}))
        return if_statement();
    if (match({lexer::PRINT}))
        return print_statement();
    if (match({lexer::RETURN}))
        return return_statement();
    if (match({lexer::WHILE}))
        return while_statement();
    if (match({lexer::LBRACE}))
        return block();
    return expr_statement();
}

std::unique_ptr<ast::Stmt> Parser::print_statement()
{
    std::unique_ptr<ast::Expr> expr = expression();
    consume(lexer::SEMICOLON, "Expected ';' after value.");
    return std::make_unique<ast::Print>(std::move(expr));
}

std::unique_ptr<ast::Stmt> Parser::expr_statement()
{
    std::unique_ptr<ast::Expr> expr = expression();
    consume(lexer::SEMICOLON, "Expected ';' after expression.");
    return std::make_unique<ast::ExprStatement>(std::move(expr));
}

std::unique_ptr<ast::Stmt> Parser::if_statement()
{
    consume(lexer::LBRACE, "Expected '{' after 'if'.");
    std::unique_ptr<ast::Expr> condition = expression();
    consume(lexer::RBRACE, "Expected '}' after if condition.");
    std::unique_ptr<ast::Stmt> then_branch = statement();
    std::unique_ptr<ast::Stmt> else_branch;
    if (match({lexer::ELSE}))
    {
        else_branch = statement();
    }
    return std::make_unique<ast::If>(std::move(condition), std::move(then_branch),
                                     std::move(else_branch));
}

std::unique_ptr<ast::Stmt> Parser::while_statement()
{
    consume(lexer::LBRACE, "Expected) '{' after 'while'.");
    std::unique_ptr<ast::Expr> condition = expression();
    consume(lexer::RBRACE, "Expected '}' after while condition.");
    std::unique_ptr<ast::Stmt> body = statement();
    return std::make_unique<ast::While>(std::move(condition), std::move(body));
}

std::unique_ptr<ast::Stmt> Parser::for_statement()
{
    consume(lexer::LBRACE, "Expected '{' after 'for'.");
    std::unique_ptr<ast::Stmt> initializer;
    if (match({lexer::SEMICOLON}))
        initializer = nullptr;
    else if (match({lexer::VAL, lexer::VAR}))
        initializer = var_declaration();
    else
        initializer = expr_statement();

    std::unique_ptr<ast::Expr> condition;
    if (!check(lexer::SEMICOLON))
        condition = expression();

    consume(lexer::SEMICOLON, "Expected ';' after loop condition.");
    std::unique_ptr<ast::Expr> increment;
    if (!check(lexer::RBRACE))
        increment = expression();

    consume(lexer::RBRACE, "Expected '}' after for clauses.");
    std::unique_ptr<ast::Stmt> body = statement();
    if (increment)
    {
        std::vector<std::unique_ptr<ast::Stmt>> statements;
        statements.push_back(std::move(body));
        statements.push_back(std::make_unique<ast::ExprStatement>(std::move(increment)));
        body = std::make_unique<ast::Block>(std::move(statements));
    }

    if (!condition)
        condition = std::make_unique<ast::Literal>(true);

    body = std::make_unique<ast::While>(std::move(condition), std::move(body));
    if (initializer)
    {
        std::vector<std::unique_ptr<ast::Stmt>> statements;
        statements.push_back(std::move(initializer));
        statements.push_back(std::move(body));
        body = std::make_unique<ast::Block>(std::move(statements));
    }
    return body;
}

std::unique_ptr<ast::Stmt> Parser::return_statement()
{
    std::unique_ptr<ast::Expr> expr = expression();
    consume(lexer::SEMICOLON, "Expected ';' after return value.");
    return std::make_unique<ast::Return>(std::move(expr));
}

std::unique_ptr<ast::Stmt> Parser::block()
{
    std::vector<std::unique_ptr<ast::Stmt>> statements;
    while (!check(lexer::RBRACE) && !is_at_end())
        statements.push_back(declaration());

    consume(lexer::RBRACE, "Expected '}' after block.");
    return std::make_unique<ast::Block>(std::move(statements));
}

std::unique_ptr<ast::Expr> Parser::expression()
{
    return assignment();
}

std::unique_ptr<ast::Expr> Parser::assignment()
{
    std::unique_ptr<ast::Expr> expr = logic_or();
    if (match({lexer::EQUAL}))
    {
        lexer::Token               equals = previous();
        std::unique_ptr<ast::Expr> value  = assignment();
        if (const auto *var = dynamic_cast<ast::Variable *>(expr.get()))
        {
            lexer::Token name = var->name;
            return std::make_unique<ast::Assignment>(name, std::move(value));
        }
        // TODO: one se have set/get we will add field assignment here
        error(peek(), "Invalid assignment target.");
    }
    return expr;
}

std::unique_ptr<ast::Expr> Parser::logic_or()
{
    std::unique_ptr<ast::Expr> expr = logic_and();
    while (match({lexer::OR}))
    {
        lexer::Token               op    = previous();
        std::unique_ptr<ast::Expr> value = logic_and();
        expr = std::make_unique<ast::Logical>(std::move(expr), op, std::move(value));
    }
    return expr;
}

std::unique_ptr<ast::Expr> Parser::logic_and()
{
    std::unique_ptr<ast::Expr> expr = equality();
    while (match({lexer::AND}))
    {
        lexer::Token               op    = previous();
        std::unique_ptr<ast::Expr> value = equality();
        expr = std::make_unique<ast::Logical>(std::move(expr), op, std::move(value));
    }
    return expr;
}

std::unique_ptr<ast::Expr> Parser::equality()
{
    std::unique_ptr<ast::Expr> expr = comparison();
    while (match({lexer::BANG_EQUAL, lexer::EQUALS_EQUAL}))
    {
        lexer::Token               op    = previous();
        std::unique_ptr<ast::Expr> value = comparison();
        expr = std::make_unique<ast::Binary>(std::move(expr), op, std::move(value));
    }
    return expr;
}

std::unique_ptr<ast::Expr> Parser::comparison()
{
    std::unique_ptr<ast::Expr> expr = term();
    while (match({lexer::GREATER, lexer::GREATER_EQUAL, lexer::LESS, lexer::LESS_EQUAL}))
    {
        lexer::Token               op    = previous();
        std::unique_ptr<ast::Expr> value = term();
        expr = std::make_unique<ast::Binary>(std::move(expr), op, std::move(value));
    }
    return expr;
}

std::unique_ptr<ast::Expr> Parser::term()
{
    std::unique_ptr<ast::Expr> expr = factor();
    while (match({lexer::MINUS, lexer::PLUS}))
    {
        lexer::Token               op    = previous();
        std::unique_ptr<ast::Expr> value = factor();
        expr = std::make_unique<ast::Binary>(std::move(expr), op, std::move(value));
    }
    return expr;
}

std::unique_ptr<ast::Expr> Parser::factor()
{
    std::unique_ptr<ast::Expr> expr = power();
    while (match({lexer::SLASH, lexer::STAR, lexer::PERCENT}))
    {
        lexer::Token               op    = previous();
        std::unique_ptr<ast::Expr> value = power();
        expr = std::make_unique<ast::Binary>(std::move(expr), op, std::move(value));
    }
    return expr;
}

std::unique_ptr<ast::Expr> Parser::power()
{
    std::unique_ptr<ast::Expr> expr = unary();
    // TODO: astrix should be bitwise xor, we should not have explicit power operator
    while (match({lexer::ASTRIX}))
    {
        lexer::Token               op    = previous();
        std::unique_ptr<ast::Expr> right = unary();
        expr = std::make_unique<ast::Binary>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<ast::Expr> Parser::unary()
{
    if (match({lexer::MINUS, lexer::BANG}))
    {
        lexer::Token               op    = previous();
        std::unique_ptr<ast::Expr> right = unary();
        return std::make_unique<ast::Unary>(op, std::move(right));
    }
    return call();
}

std::unique_ptr<ast::Expr> Parser::call()
{
    std::unique_ptr<ast::Expr> expr = primary();
    while (true)
    {
        if (match({lexer::LPAREN}))
        {
            std::vector<std::unique_ptr<ast::Expr>> arguments;
            if (!check(lexer::RPAREN))
            {
                // TODO: fields access can have dot
                do
                {
                    if (arguments.size() > 255)
                        error(peek(), "Cannot have more than 255 arguments.");
                    arguments.push_back(expression());
                } while (match({lexer::COMMA}));
            }
            lexer::Token paren = consume(lexer::RPAREN, "Expected ')' after arguments.");
            expr               = std::make_unique<ast::Call>(std::move(expr), std::move(paren),
                                                             std::move(arguments));
        }
        else
        {
            break;
        }
    }
    return expr;
}

std::unique_ptr<ast::Expr> Parser::primary()
{
    // Debug output: print current token type and value
    std::cout << "[primary] Token: " << peek().type << " Value: " << peek().lexeme << std::endl;
    if (match({lexer::TRUE}))
        return std::make_unique<ast::Literal>(true);
    if (match({lexer::FALSE}))
        return std::make_unique<ast::Literal>(false);
    if (match({lexer::NUMBER, lexer::STRING}))
        return std::make_unique<ast::Literal>(previous().literal);
    if (match({lexer::LPAREN}))
    {
        std::unique_ptr<ast::Expr> expr = expression();
        consume(lexer::RPAREN, "Expected ')' after expression.");
        return std::make_unique<ast::Grouping>(std::move(expr));
    }
    if (match({lexer::IDENTIFIER}))
        return std::make_unique<ast::Variable>(previous());

    error(previous(), "Invalid primary.");
    return nullptr;
}

bool Parser::match(const std::vector<lexer::TokenType> &types)
{
    for (auto &type : types)
    {
        if (check(type))
        {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(const lexer::TokenType type)
{
    if (is_at_end())
        return false;
    return peek().type == type;
}

bool Parser::check_next(const lexer::TokenType token) const
{
    if (is_at_end())
        return false;
    if (tokens[current + 1].type == lexer::TokenType::END_OF_FILE)
        return false;
    return tokens[current + 1].type == token;
}

void Parser::error(const lexer::Token &token, const std::string &message)
{
    Compiler::error(token, message);
    throw ParseError{message};
}

lexer::Token Parser::advance()
{
    lexer::Token curr_token = peek();
    if (!is_at_end())
        current++;
    return curr_token;
}

bool Parser::is_at_end() const
{
    return tokens[current].type == lexer::TokenType::END_OF_FILE;
}

lexer::Token Parser::peek()
{
    return tokens[current];
}

lexer::Token Parser::previous()
{
    return tokens[current - 1];
}

lexer::Token Parser::consume(const lexer::TokenType type, const std::string &message)
{
    if (check(type))
        return advance();

    error(peek(), message);
    return lexer::Token{};
}

lexer::Token Parser::consume_any(const std::vector<lexer::TokenType> &types,
                                 const std::string                   &message)
{
    for (const auto &type : types)
        if (check(type))
            return advance();

    error(peek(), message);
    return lexer::Token{};
}

void Parser::synchronize()
{
    advance();
    while (!is_at_end())
    {
        if (previous().type == lexer::SEMICOLON)
            return;
        switch (peek().type)
        {
        case lexer::CLASS:
        case lexer::FN:
        case lexer::VAL:
        case lexer::VAR:
        case lexer::FOR:
        case lexer::IF:
        case lexer::WHILE:
        case lexer::PRINT:
        case lexer::RETURN:
            return;
        default:
            break;
        }
        advance();
    }
}

} // namespace cool::compiler::parser