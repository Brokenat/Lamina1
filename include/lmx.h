//
// Created by meian on 2026/4/8.
//

#pragma once
#ifdef LMX_DLL
    #ifdef _WIN32
        #ifdef LMC_BUILD
            #define LM_API __declspec(dllexport)
        #else
            #define LM_API __declspec(dllimport)
        #endif
    #else
        #define LM_API __attribute__((visibility("default")))
    #endif
#else

    #define LM_API
#endif


#define LMX_VERSION     ((uint32_t)0x00000001)
#define LMX_MAGIC_NUM   ((uint32_t)0x4d4c5451)
#if __cplusplus

extern "C" {
#endif
#include <stdint.h>
#include <stdio.h>
#if UINTPTR_MAX == INT32_MAX
#if defined(_MSC_VER)
#define LM_CALL __cdecl
#else
#define LM_CALL __attribute__((cdecl))
#endif
#else
#define LM_CALL
#endif



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

LM_API LmModule* LM_CALL lmx_doString(LmState* state, const char* code);

LM_API LmModule* LM_CALL lmx_doFile(LmState* state, const char* name);

LM_API void LM_CALL lmx_printASTFromFile(LmState* state, FILE* file, const char* name);

LM_API void LM_CALL lmx_printASTFromString(LmState* state, FILE* file, const char* code, const char* name);
LM_API void LM_CALL lmx_printMIRFromString(LmState* state, FILE* file, const char* code, const char* name);

LM_API void LM_CALL lmx_moduleToFile(LmState* state, LmModule* module, const char* name);

LM_API LaminaVM* LM_CALL lmx_newLaminaVM(LmState* state, int argc, char** argv);

LM_API int LM_CALL lmx_vmRunModule(LmState* state, LaminaVM* vm, LmModule* module);

LM_API void LM_CALL lmx_vmEval(LmState* state, LaminaVM* vm, LmValue* result, const char* code);

#if __cplusplus
}
#endif
