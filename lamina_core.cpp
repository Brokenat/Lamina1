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
#include <cstring>

LmState lmx_newState() {
    auto* node = static_cast<LmLinkedNode *>(malloc(sizeof(LmLinkedNode)));
    memset(node, 0, sizeof(LmLinkedNode));
    return LmState {node};
}
void lmx_deleteState(const LmState* state) {
    const LmLinkedNode* node = state->n;
    while (node->last) {
        if (node->ptr) free(node->ptr);
        const auto last = node->last;
        free((void*)node);
        node = last;
    }
}
static LmLinkedNode* newLickedNode(LmLinkedNode* old) {
    auto* node = static_cast<LmLinkedNode *>(malloc(sizeof(LmLinkedNode)));
    node->last = old;
    return node;
}
static void lmx_state_addNode(LmState* state, void* ptr) {
    state->n = newLickedNode(state->n);
    state->n->ptr = ptr;
}

void lmx_printASTFromString(LmState *state, FILE *file, const char *code, const char* name) {
    std::string c = code;
    auto tokens = lmx::Lexer(c).tokenize(c);
    const auto node = lmx::Parser(tokens).parse_module(name);
    lmx::hir::HirContext().check_module(node.get());
    const auto ast_str = lmx::AstPrinter::print(*node);
    if (fwrite(ast_str.c_str(), 1, ast_str.length(), file) != ast_str.length()) {
        fprintf(stderr, "Error writing AST to file\n");
    }
}

LaminaVM* lmx_newLaminaVM(LmState* state, int argc, char** argv) {
    auto* vm = static_cast<LaminaVM*>(malloc(sizeof(lmx::runtime::LaminaVM)));
    new (vm) lmx::runtime::LaminaVM(nullptr, argc, argv);
    lmx_state_addNode(state, vm);
    return vm;
}
