# AGENTS.md

C++ coding rules for this project. Read before writing or modifying code.

## Includes

- All standard library includes must go into the PCH file `include/util/common.h`, not into individual source files.
  - Do not add `#include <vector>`, `#include <string>`, etc. to `.cpp` or other `.h` files.
  - If a std header you need is missing from `common.h`, add it there once.
  - Third-party includes (glm, spdlog) follow the same rule and also live in `common.h`.

## Formatting

- After any code change, run `clang-format` on the modified files:
  `clang-format -i <file>`

## Comments

- Only add comments if necessary (non-obvious logic, intent, or rationale).
- Keep comments simple, clear, and easy to understand.
