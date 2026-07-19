//
// Created by meian on 2026/4/8.
//
#include <string>

#include "lmx.h"

int main(const int argc, char** argv) {
    constexpr auto code = R"(
func foo(a bool, b bool) {
    var c = false
    c = true
    a and b and c
}
let v = foo(false, true)
)";
    auto state = lmx_newState();

    lmx_printASTFromString(&state, stdout, code, "test");

    auto vm = lmx_newLaminaVM(&state, argc, argv);
    lmx_deleteState(&state);

}
