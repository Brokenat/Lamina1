//
// Created by meian on 2026/4/8.
//
#include <string>

#include "lmx.h"

int main(int argc, char** argv) {
    const std::string code = R"(

let v = add(1, 2)

func add(a int, b int) {
    if {
        var c = 5

        a + b * 2 + c
    } > 10 {24} else {12}
}
)";
    auto state = lmx_newState();
    // lmx_printMIRFromString(&state, stdout, code.c_str(), "test");
    const auto vm = lmx_newLaminaVM(&state, argc, argv);
    const auto module = lmx_doString(&state, code.c_str(), "test");
    auto result = lmx_vmRunModule(&state, vm, module);
    lmx_deleteState(&state);
}
