/*
 * MIT License
 * 
 * Copyright (c) 2018 alex Yang
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _RV32DECODER_H_
#define _RV32DECODER_H_

typedef struct {
    uint32_t opcode:7;
    uint32_t rdidx:5;
    uint32_t funct3:3;
    uint32_t rs1idx:5;
    uint32_t rs2idx:5;
    uint32_t funct7:7;
} TypeR_t;

typedef struct {
    uint32_t opcode:7;
    uint32_t rdidx:5;
    uint32_t funct3:3;
    uint32_t rs1idx:5;
    uint32_t imm_11_0:12;
} TypeI_t;

typedef struct {
    uint32_t opcode:7;
    uint32_t imm_4_0:5;
    uint32_t funct3:3;
    uint32_t rs1idx:5;
    uint32_t rs2idx:5;
    uint32_t imm_11_5:7;
} TypeS_t;

typedef struct {
    uint32_t opcode:7;
    uint32_t imm_11:1;
    uint32_t imm_4_1:4;
    uint32_t funct3:3;
    uint32_t rs1idx:5;
    uint32_t rs2idx:5;
    uint32_t imm_10_5:6;
    uint32_t imm_12:1;
} TypeSB_t;

typedef struct {
    uint32_t opcode:7;
    uint32_t rdidx:5;
    uint32_t imm_31_12:20;
} TypeU_t;

typedef struct {
    uint32_t opcode:7;
    uint32_t rdidx:5;
    uint32_t imm_19_12:8;
    uint32_t imm_11:1;
    uint32_t imm_10_1:10;
    uint32_t imm_20:1;
} TypeUJ_t;

typedef struct {
    uint32_t val:7;
    uint32_t:25;
} opcode_t;

typedef struct {
    uint32_t:12;
    uint32_t val:3;
    uint32_t:17;
} funct3_t;

typedef struct {
    uint32_t:25;
    uint32_t val:7;
} funct7_t;

typedef struct {
    union {
        uint32_t val;
        funct7_t funct7;
        funct3_t funct3;
        opcode_t opcode;
        TypeR_t R;
        TypeI_t I;
        TypeS_t S;
        TypeSB_t SB;
        TypeU_t U;
        TypeUJ_t UJ;
    };
} rv32_t;

//////////////////////////

typedef union {
    struct {
        uint8_t UJ:1;
        uint8_t U:1;
        uint8_t SB:1;
        uint8_t S:1;
        uint8_t I:1;
        uint8_t R:1;
        uint8_t:2;
    };
    uint8_t val;
} instr_type_t;

typedef union {
    struct {
        uint8_t J:1;
        uint8_t U:1;
        uint8_t B:1;
        uint8_t S:1;
        uint8_t I:1;
        uint8_t:3;
    };
    uint8_t val;
} imm_type_t;

typedef struct {
    char         *name;
    char         *description;
    instr_type_t instr_type;
    imm_type_t   imm_type;
    int32_t      imm32;
} rv32_info_t;

#endif
