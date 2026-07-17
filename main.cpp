//
// Created by meian on 2026/4/8.
//
#include <string>

#include "include/lamina_core.h"

int main(int argc, char** argv) {
    const std::string code = R"(
func foo(a bool, b bool) {
    var c = false
    c = true
    a and b and c
}
let v = foo(false, true)
)";
    auto state = lmx_newState();

    lmx_printASTFromString(&state, stdout, code.c_str(), "test");
    // lmx::runtime::LaminaVM vm(nullptr, argc, argv);
    lmx_deleteState(&state);

}
