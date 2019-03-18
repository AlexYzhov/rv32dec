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

#ifndef _RV32I_DEF_H_
#define _RV32I_DEF_H_

#define rv32i_LUI       0
#define rv32i_AUIPC     1

#define rv32i_JAL       2
#define rv32i_JALR      3

#define rv32i_BEQ       4
#define rv32i_BNE       5
#define rv32i_BLT       6
#define rv32i_BGE       7
#define rv32i_BLTU      8
#define rv32i_BGEU      9

#define rv32i_LB        10
#define rv32i_LH        11
#define rv32i_LW        12
#define rv32i_LBU       13
#define rv32i_LHU       14
#define rv32i_SB        15
#define rv32i_SH        16
#define rv32i_SW        17

#define rv32i_ADDI      18
#define rv32i_SLTI      19
#define rv32i_SLTIU     20
#define rv32i_XORI      21
#define rv32i_ORI       22
#define rv32i_ANDI      23
#define rv32i_SLLI      24
#define rv32i_SRLI      25
#define rv32i_SRAI      26

#define rv32i_ADD       27
#define rv32i_SUB       28
#define rv32i_SLL       29
#define rv32i_SLT       30
#define rv32i_SLTU      31
#define rv32i_XOR       32
#define rv32i_SRL       33
#define rv32i_SRA       34
#define rv32i_OR        35
#define rv32i_AND       36

#define rv32i_FENCE     37
#define rv32i_FENCEI    38
#define rv32i_ECALL     39
#define rv32i_EBREA     40

#define rv32i_CSRRW     41
#define rv32i_CSRRS     42
#define rv32i_CSRRC     43
#define rv32i_CSRRWI    44
#define rv32i_CSRRSI    45
#define rv32i_CSRRCI    46

#endif
