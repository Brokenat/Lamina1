//
// Created by meian on 2026/4/8.
//
#include <string>

#include "lmx.h"

int main(const int argc, char** argv) {
    const std::string code = R"(
func add(a int, b int) {
    return a + b * 2
}
let v = add(1, 2)
)";
    auto state = lmx_newState();
    lmx_printASTFromString(&state, stdout, code.c_str(), "test");

    lmx_deleteState(&state);
}
