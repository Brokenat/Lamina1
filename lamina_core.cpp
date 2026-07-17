//
// Created by meian on 2026/4/8.
//

#include "include/lamina_core.h"


#include "compiler/ast_printer.hpp"
#include "compiler/hir.hpp"
#include "compiler/parser.hpp"
#include "compiler/lexer.hpp"
#include "runtime/vm.hpp"
#include <iostream>
#include <cstdlib>

LmState lmx_newState() {
    return LmState {LmLinkedNode{
            nullptr, nullptr
        }};
}
void lmx_deleteState(const LmState* state) {
    const LmLinkedNode* node = &state->n;
    while (node->next) {
        if (node->ptr) free(node->ptr);
        node = node->next;
    }
}


void lmx_printASTFromString(LmState *state, FILE *file, const char *code, const char* name) {
    std::string c = code;
    auto tokens = lmx::Lexer(c).tokenize(c);
    const auto node = lmx::Parser(tokens).parse_module(name);
    lmx::hir::HirContext().check_module(node.get());
    const auto ast_str = lmx::AstPrinter::print(*node);
    fwrite(ast_str.c_str(), ast_str.length(), 1, file);
}
