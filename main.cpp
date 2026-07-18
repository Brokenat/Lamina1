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
    auto vm = lmx_newLaminaVM(&state, argc, argv);
    //while (true) {
    //    if (getchar() == 'c') break;
    //}
    lmx_deleteState(&state);

}
