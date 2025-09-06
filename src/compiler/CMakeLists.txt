add_executable(coolc main.cpp
        lexer/lexer.hpp
        compiler.cpp
        compilation_result.hpp
        lexer/lexer.cpp
        ast/Expr.hpp
        ast/Stmt.hpp
        parser/parser.hpp
        parser/parser.cpp
)
