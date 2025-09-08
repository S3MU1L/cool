add_executable(coolc main.cpp
        lexer/lexer.hpp
        compiler.cpp
        compilation_result.hpp
        lexer/lexer.cpp
        ast/expr.hpp
        ast/stmt.hpp
        parser/parser.hpp
        parser/parser.cpp
        ast/expr.cpp
        ast/stmt.cpp
        ast/ast_printer.hpp
        parser/parse_error.hpp
        ast/ast_printer.cpp
)
