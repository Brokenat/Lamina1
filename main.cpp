//
// Created by meian on 2026/4/8.
//
#include <string>

#include "lmx.h"

int main(int argc, char** argv) {
    const std::string code = R"(

let v = add(1, 2)

func add(a int, b int) {
    {
        var c = 5

        a + b * 2 + c
    }
}
)";
    auto state = lmx_newState();
    lmx_printMIRFromString(&state, stdout, code.c_str(), "test");

    lmx_deleteState(&state);
}
