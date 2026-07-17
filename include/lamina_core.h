//
// Created by meian on 2026/4/8.
//

#pragma once

#if __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include <stdio.h>
struct LmLinkedNode {
    void* ptr;
    LmLinkedNode* last;
};
typedef LmLinkedNode LmLinkedNode;
struct LmState {
    LmLinkedNode* n;
};
typedef LmState LmState;
LmState lmx_newState();
void lmx_deleteState(const LmState* state);

struct LmValue;
typedef LmValue LmValue;

struct LmModule;
typedef LmModule LmModule;

struct LaminaVM;
typedef LaminaVM LaminaVM;

LmModule* lmx_doString(LmState* state, const char* code);

LmModule* lmx_doFile(LmState* state, const char* name);

void lmx_printASTFromFile(LmState* state, FILE* file, const char* name);

void lmx_printASTFromString(LmState* state, FILE* file, const char* code, const char* name);

void lmx_moduleToFile(LmState* state, LmModule* module, const char* name);

LaminaVM* lmx_newLaminaVM(LmState* state, int argc, char** argv);

int lmx_vmRunModule(LmState* state, LaminaVM* vm, LmModule* module);

#if __cplusplus
}
#endif
