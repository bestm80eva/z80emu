/* z80user.h
 * Add your code here to interface the emulated system with z80emu. See towards
 * the end of the file for an example for running zextest.
 *
 * Copyright (c) 2016 Lin Ke-Fong
 *
 * This program is free, do whatever you want with it.
 */

#ifndef __Z80USER_INCLUDED__
#define __Z80USER_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

/* Write the following macros for memory access and input/output on the Z80. 
 *
 * Z80_FETCH_BYTE() and Z80_FETCH_WORD() are used by the emulator to read the
 * code (opcode, constants, displacement, etc). The upper 16-bit of the address
 * parameters is undefined and must be reset to zero before actually reading 
 * memory (use & 0xffff). The value x read, must be an unsigned 8-bit or 16-bit
 * value in the endianness of the host processor.
 *
 * Z80_READ_BYTE(), Z80_WRITE_BYTE(), Z80_READ_WORD(), and Z80_WRITE_WORD()
 * are used for general memory access. They obey the same rule as the code 
 * reading macros. The upper bits of the value x to write may be non-zero.
 * 
 * Z80_INPUT_BYTE() and Z80_OUTPUT_BYTE() are for input and output. The upper
 * bits of the port number to read or write are always zero. The input byte x 
 * must be an unsigned 8-bit value. The value x to write is an unsigned 8-bit 
 * with its upper bits zeroed.
 *
 * All macros have access to the following variables:
 *
 *	context 	This is the (void *) context passed to the emulation 
 *			functions.
 *
 *      number_cycles   Number of cycles to emulate. You may decrease or 
 *                      increase the value of number_cycles to stop the 
 *			emulation earlier or later. In particular, if you set 
 *			it to zero, the emulator will stop after completion of
 *                      the current instruction. 
 * 
 *      elapsed_cycles  Number of cycles emulated. After executing each
 *			instruction, the emulator checks if elapsed_cycles is
 *			greater or equal to number_cycles, and will stops if 
 *			so. You may add wait states for slow memory accesses if
 *			needed. Because the macros are called during the 
 *			execution of the current instruction, this number is 
 *			only correct up to the previous one.
 *
 *      registers       Current register decoding table, use it to determine if
 * 			the current instruction is prefixed. It points on:
 *                      
 *				state->dd_register_table for 0xdd prefixes; 
 *                      	state->fd_register_table for 0xfd;
 *				state->register_table otherwise.
 *
 *      opcode          Opcode of the currently executing instruction.
 *
 *      instruction     Type of the currently executing instruction, see
 *                      instructions.h for a list.
 *
 *      pc              Current PC register (upper bits are undefined), points
 *                      on the displacement or constant to read for
 *                      Z80_FETCH_BYTE() and Z80_FETCH_WORD(), or on the next
 *                      instruction otherwise.
 *
 *      state           Pointer to the current Z80_STATE. Because the 
 *			instruction is currently executing, members may not be 
 *			fully up to date, depending when the macro is called.
 *			It is rather suggested to access the state when the 
 *			emulator is stopped. 
 */

/* Here are macros for emulating zextest. Read/write memory macros have been 
 * written for a linear 64k RAM. Input/output port macros are used to simulate 
 * system calls. Variables are global (not very elegant albeit simple), hence 
 * context is not used.
 */

#include "zextest.h"

#define Z80_FETCH_BYTE(address, x)                                      \
{                                                                       \
        (x) = memory[(address) & 0xffff];                               \
}

#define Z80_FETCH_WORD(address, x)                                      \
{                                                                       \
        (x) = memory[(address) & 0xffff]                                \
                | (memory[((address) + 1) & 0xffff] << 8);              \
}

#define Z80_READ_BYTE(address, x)                                       \
{                                                                       \
        (x) = memory[(address) & 0xffff];                               \
}

#define Z80_WRITE_BYTE(address, x)                                      \
{                                                                       \
        memory[(address) & 0xffff] = (x);                               \
}

#define Z80_READ_WORD(address, x)                                       \
{                                                                       \
        (x) = memory[(address) & 0xffff]                                \
                | (memory[((address) + 1) & 0xffff] << 8);              \
}

#define Z80_WRITE_WORD(address, x)                                      \
{                                                                       \
        memory[(address) & 0xffff] = x;                                 \
        memory[((address) + 1) & 0xffff] = x >> 8;                      \
}

#define Z80_INPUT_BYTE(port, x)                                         \
{                                                                       \
        (x) = 0;                /* Make compiler happy. */              \
        SystemCall(state);                                              \
}

#define Z80_OUTPUT_BYTE(port, x)                                        \
{                                                                       \
        number_cycles = 0;                                              \
        state->status |= FLAG_STOP_EMULATION;                           \
}

#ifdef __cplusplus
}
#endif

#endif
