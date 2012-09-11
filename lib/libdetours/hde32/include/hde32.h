/*
* Hacker Disassembler Engine 32
* Copyright (c) 2006-2009, Vyacheslav Patkov.
* All rights reserved.
*
* hde32.h: C/C++ header file
*
*/

#ifndef _HDE32_H_
#define _HDE32_H_

#include <QtGlobal>

#define F_MODRM         0x00000001
#define F_SIB           0x00000002
#define F_IMM8          0x00000004
#define F_IMM16         0x00000008
#define F_IMM32         0x00000010
#define F_DISP8         0x00000020
#define F_DISP16        0x00000040
#define F_DISP32        0x00000080
#define F_RELATIVE      0x00000100
#define F_2IMM16        0x00000800
#define F_ERROR         0x00001000
#define F_ERROR_OPCODE  0x00002000
#define F_ERROR_LENGTH  0x00004000
#define F_ERROR_LOCK    0x00008000
#define F_ERROR_OPERAND 0x00010000
#define F_PREFIX_REPNZ  0x01000000
#define F_PREFIX_REPX   0x02000000
#define F_PREFIX_REP    0x03000000
#define F_PREFIX_66     0x04000000
#define F_PREFIX_67     0x08000000
#define F_PREFIX_LOCK   0x10000000
#define F_PREFIX_SEG    0x20000000
#define F_PREFIX_ANY    0x3f000000

#define PREFIX_SEGMENT_CS   0x2e
#define PREFIX_SEGMENT_SS   0x36
#define PREFIX_SEGMENT_DS   0x3e
#define PREFIX_SEGMENT_ES   0x26
#define PREFIX_SEGMENT_FS   0x64
#define PREFIX_SEGMENT_GS   0x65
#define PREFIX_LOCK         0xf0
#define PREFIX_REPNZ        0xf2
#define PREFIX_REPX         0xf3
#define PREFIX_OPERAND_SIZE 0x66
#define PREFIX_ADDRESS_SIZE 0x67

#pragma pack(push,1)

typedef struct {
	quint8 len;
	quint8 p_rep;
	quint8 p_lock;
	quint8 p_seg;
	quint8 p_66;
	quint8 p_67;
	quint8 opcode;
	quint8 opcode2;
	quint8 modrm;
	quint8 modrm_mod;
	quint8 modrm_reg;
	quint8 modrm_rm;
	quint8 sib;
	quint8 sib_scale;
	quint8 sib_index;
	quint8 sib_base;
	union {
		quint8 imm8;
		quint16 imm16;
		quint32 imm32;
	} imm;
	union {
		quint8 disp8;
		quint16 disp16;
		quint32 disp32;
	} disp;
	quint32 flags;
} hde32s;

#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

/* __cdecl */
unsigned int hde32_disasm(const void *code, hde32s *hs);

#ifdef __cplusplus
}
#endif

#endif /* _HDE32_H_ */
