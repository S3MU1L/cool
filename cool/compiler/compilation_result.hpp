#pragma once

namespace cool::compiler {
enum CompilationResult {
    SUCCESS,
    FILE_ERROR,
    FILE_NOT_FOUND,
    LEXICAL_ERROR,
    SYNTAX_ERROR,
    SEMANTIC_ERROR,
    UNKNOWN_ERROR
};
}