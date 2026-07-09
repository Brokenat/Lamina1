//
// Created by meian on 2026/4/8.
//

#include "compiler/parser.hpp"
#include "compiler/lexer.hpp"
#include "runtime/vm.hpp"

int main(int argc, char** argv) {
    std::string code = R"(
func foo() -> int {
    4
}
)";
    auto tokens = lmx::Lexer(code).tokenize(code);

    lmx::Parser parser(tokens);
    auto node = parser.parse_module("");

    lmx::runtime::LaminaVM vm(nullptr, argc, argv);
    while (true) {
        if (getchar() == 'c') {break;}
    }
}
