//
// Created by meian on 2026/4/8.
//
#include <cstdio>
#include <string>

#include "compiler/ast_printer.hpp"
#include "compiler/hir.hpp"
#include "compiler/lexer.hpp"
#include "compiler/parser.hpp"
#include "compiler/mir_builder.hpp"
#include "compiler/mir_printer.hpp"

int main(const int argc, char** argv) {
    auto code = std::string(R"(
func add(a int, b int) {
    a + b * 2
}
let v = add(1, 2)
)");
    auto tokens = lmx::Lexer(code).tokenize(code);
    auto ast = lmx::Parser(tokens).parse_module("test");
    lmx::hir::HirContext().check_module(ast.get());

    std::printf("=== AST ===\n%s\n", lmx::AstPrinter::print(*ast).c_str());

    auto mir = lmx::mir::MirBuilder::from_ast_module(ast);

    std::printf("=== MIR ===\n%s", lmx::mir::MirPrinter::print(mir).c_str());
    std::printf("=== done ===\n");
}
