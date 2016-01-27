// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
//*****************************************************************************
// File: primitives.h
// 

//
// Platform-specific debugger primitives
//
//*****************************************************************************

#ifndef PRIMITIVES_H_
#define PRIMITIVES_H_

typedef const BYTE                  CORDB_ADDRESS_TYPE;
typedef DPTR(CORDB_ADDRESS_TYPE)    PTR_CORDB_ADDRESS_TYPE;

#define MAX_INSTRUCTION_LENGTH 8

// Given a return address retrieved during stackwalk,
// this is the offset by which it should be decremented to land at the call instruction.
#define STACKWALK_CONTROLPC_ADJUST_OFFSET 4

#define PRD_TYPE                               LONG
#define CORDbg_BREAK_INSTRUCTION_SIZE 4
#define CORDbg_BREAK_INSTRUCTION (LONG)0xD43E0000

inline CORDB_ADDRESS GetPatchEndAddr(CORDB_ADDRESS patchAddr)
{
    LIMITED_METHOD_DAC_CONTRACT;
    return patchAddr + CORDbg_BREAK_INSTRUCTION_SIZE;
}

#define InitializePRDToBreakInst(_pPRD)       *(_pPRD) = CORDbg_BREAK_INSTRUCTION
#define PRDIsBreakInst(_pPRD)                 (*(_pPRD) == CORDbg_BREAK_INSTRUCTION)


#define CORDbgGetInstructionEx(_buffer, _requestedAddr, _patchAddr, _dummy1, _dummy2)                          \
    CORDbgGetInstructionExImpl((CORDB_ADDRESS_TYPE *)((_buffer) + (_patchAddr) - (_requestedAddr)));
                    
#define CORDbgSetInstructionEx(_buffer, _requestedAddr, _patchAddr, _opcode, _dummy2)                          \
    CORDbgSetInstructionExImpl((CORDB_ADDRESS_TYPE *)((_buffer) + (_patchAddr) - (_requestedAddr)), (_opcode));

#define CORDbgInsertBreakpointEx(_buffer, _requestedAddr, _patchAddr, _dummy1, _dummy2)                        \
    CORDbgInsertBreakpointExImpl((CORDB_ADDRESS_TYPE *)((_buffer) + (_patchAddr) - (_requestedAddr)));


SELECTANY const CorDebugRegister g_JITToCorDbgReg[] =
{
    REGISTER_ARM64_X0,
    REGISTER_ARM64_X1,
    REGISTER_ARM64_X2,
    REGISTER_ARM64_X3,
    REGISTER_ARM64_X4,
    REGISTER_ARM64_X5,
    REGISTER_ARM64_X6,
    REGISTER_ARM64_X7,
    REGISTER_ARM64_X8,
    REGISTER_ARM64_X9,
    REGISTER_ARM64_X10,
    REGISTER_ARM64_X11,
    REGISTER_ARM64_X12,
    REGISTER_ARM64_X13,
    REGISTER_ARM64_X14,
    REGISTER_ARM64_X15,
    REGISTER_ARM64_X16,
    REGISTER_ARM64_X17,
    REGISTER_ARM64_X18,
    REGISTER_ARM64_X19,
    REGISTER_ARM64_X20,
    REGISTER_ARM64_X21,
    REGISTER_ARM64_X22,
    REGISTER_ARM64_X23,
    REGISTER_ARM64_X24,
    REGISTER_ARM64_X25,
    REGISTER_ARM64_X26,
    REGISTER_ARM64_X27,
    REGISTER_ARM64_X28,
    REGISTER_ARM64_FP,
    REGISTER_ARM64_LR,
    REGISTER_ARM64_SP,
    REGISTER_ARM64_PC
};

inline void CORDbgSetIP(DT_CONTEXT *context, LPVOID eip) {
    LIMITED_METHOD_CONTRACT;

    context->Pc = (DWORD64)eip;
}

inline LPVOID CORDbgGetSP(const DT_CONTEXT * context) {
    LIMITED_METHOD_CONTRACT;

    return (LPVOID)(size_t)(context->Sp);
}

inline void CORDbgSetSP(DT_CONTEXT *context, LPVOID esp) {
    LIMITED_METHOD_CONTRACT;

    context->Sp = (DWORD64)esp;
}

inline LPVOID CORDbgGetFP(const DT_CONTEXT * context) {
    LIMITED_METHOD_CONTRACT;

    return (LPVOID)(size_t)(context->Fp);
}

inline void CORDbgSetFP(DT_CONTEXT *context, LPVOID fp) {
    LIMITED_METHOD_CONTRACT;

    context->Fp = (DWORD64)fp;
}


inline BOOL CompareControlRegisters(const DT_CONTEXT * pCtx1, const DT_CONTEXT * pCtx2)
{
    LIMITED_METHOD_DAC_CONTRACT;

    // @ARMTODO: Sort out frame registers

    if ((pCtx1->Pc == pCtx2->Pc) &&
        (pCtx1->Sp == pCtx2->Sp) &&
        (pCtx1->Fp == pCtx2->Fp))
    {
        return TRUE;
    }

    return FALSE;
}

inline void CORDbgSetInstruction(CORDB_ADDRESS_TYPE* address,
                                 PRD_TYPE instruction)
{
    // In a DAC build, this function assumes the input is an host address.
    LIMITED_METHOD_DAC_CONTRACT;

    ULONGLONG ptraddr = dac_cast<ULONGLONG>(address);
    *(PRD_TYPE *)ptraddr = instruction;
    FlushInstructionCache(GetCurrentProcess(),
                          address,
                          sizeof(PRD_TYPE));
}

inline PRD_TYPE CORDbgGetInstruction(UNALIGNED CORDB_ADDRESS_TYPE* address)
{
    LIMITED_METHOD_CONTRACT;

    ULONGLONG ptraddr = dac_cast<ULONGLONG>(address);
    return *(PRD_TYPE *)ptraddr;
}


inline void SetSSFlag(DT_CONTEXT *pContext) 
{
    _ASSERTE(pContext != NULL);
    pContext->Cpsr |= 0x00200000;
}

inline void UnsetSSFlag(DT_CONTEXT *pContext) 
{
    _ASSERTE(pContext != NULL);
    pContext->Cpsr &= ~0x00200000;
}

inline bool IsSSFlagEnabled(DT_CONTEXT * pContext)
{
    _ASSERTE(pContext != NULL);
    return (pContext->Cpsr & 0x00200000) != 0;
}

#include "arm_primitives.h"
#endif // PRIMITIVES_H_
