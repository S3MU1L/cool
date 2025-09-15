#pragma once

namespace cool::compiler::analysis {
/* TODO, we will probably need to refactor this once we will have more types,
 * and also if we want to support user-defined types later.
 */
enum class Type { INT, STRING, BOOL, VOID, OBJECT, SELF_TYPE };

}; // namespace cool::compiler::analysis