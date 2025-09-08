#pragma once
#include <stdexcept>

namespace cool::compiler::parser {
struct ParseError final : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
} // namespace cool::compiler::parser