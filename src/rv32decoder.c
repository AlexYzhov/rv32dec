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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "console_color.h"

#include "rv32decoder.h"
#include "rv32i_def.h"

void decoder(rv32_info_t *info, rv32_t instr);
char *get_rv32i_name(bool rv32i_table[]);
char *get_rv32i_description(bool rv32i_table[]);
int32_t imm_gen(rv32_t instr, imm_type_t imm_type);
void log_out(rv32_info_t *info, rv32_t instr);

int main(int argc, char *argv[]) {
    rv32_info_t info;
    rv32_t instr;
    
    for(uint32_t i=1; i<argc; i++) {
        memset(&info, 0, sizeof(info));
        memset(&instr, 0, sizeof(instr));

        //读取获取指令
        instr.val = strtoul(argv[i], NULL, 16);

        //指令译码
        decoder(&info, instr);

        //格式化输出指令信�?
        log_out(&info, instr);
    }

    return 0;
}

void decoder(rv32_info_t *info, rv32_t instr) {
    bool opcode_1_0_00  = ((instr.opcode.val & 0x3) == 0x0);
    bool opcode_1_0_01  = ((instr.opcode.val & 0x3) == 0x1);
    bool opcode_1_0_10  = ((instr.opcode.val & 0x3) == 0x2);
    bool opcode_1_0_11  = ((instr.opcode.val & 0x3) == 0x3);

    bool opcode_4_2_000 = (((instr.opcode.val>>2) & 0x7) == 0x0);
    bool opcode_4_2_001 = (((instr.opcode.val>>2) & 0x7) == 0x1);
    bool opcode_4_2_010 = (((instr.opcode.val>>2) & 0x7) == 0x2);
    bool opcode_4_2_011 = (((instr.opcode.val>>2) & 0x7) == 0x3);
    bool opcode_4_2_100 = (((instr.opcode.val>>2) & 0x7) == 0x4);
    bool opcode_4_2_101 = (((instr.opcode.val>>2) & 0x7) == 0x5);
    bool opcode_4_2_110 = (((instr.opcode.val>>2) & 0x7) == 0x6);
    bool opcode_4_2_111 = (((instr.opcode.val>>2) & 0x7) == 0x7);

    bool opcode_6_5_00  = (((instr.opcode.val>>5) & 0x3) == 0x0);
    bool opcode_6_5_01  = (((instr.opcode.val>>5) & 0x3) == 0x1);
    bool opcode_6_5_10  = (((instr.opcode.val>>5) & 0x3) == 0x2);
    bool opcode_6_5_11  = (((instr.opcode.val>>5) & 0x3) == 0x3);

    bool opcode_LOAD      = (opcode_6_5_00 & opcode_4_2_000 & opcode_1_0_11);
    bool opcode_STORE     = (opcode_6_5_01 & opcode_4_2_000 & opcode_1_0_11);
    bool opcode_MADD      = (opcode_6_5_10 & opcode_4_2_000 & opcode_1_0_11);
    bool opcode_BRANCH    = (opcode_6_5_11 & opcode_4_2_000 & opcode_1_0_11);
    bool opcode_LOAD_FP   = (opcode_6_5_00 & opcode_4_2_001 & opcode_1_0_11);
    bool opcode_STORE_FP  = (opcode_6_5_01 & opcode_4_2_001 & opcode_1_0_11);
    bool opcode_MSUB      = (opcode_6_5_10 & opcode_4_2_001 & opcode_1_0_11);
    bool opcode_JALR      = (opcode_6_5_11 & opcode_4_2_001 & opcode_1_0_11);
    bool opcode_NMSUB     = (opcode_6_5_10 & opcode_4_2_010 & opcode_1_0_11);
    bool opcode_MISC_MEM  = (opcode_6_5_00 & opcode_4_2_011 & opcode_1_0_11);
    bool opcode_AMO       = (opcode_6_5_01 & opcode_4_2_011 & opcode_1_0_11);
    bool opcode_NMADD     = (opcode_6_5_10 & opcode_4_2_011 & opcode_1_0_11);
    bool opcode_JAL       = (opcode_6_5_11 & opcode_4_2_011 & opcode_1_0_11);
    bool opcode_OP_IMM    = (opcode_6_5_00 & opcode_4_2_100 & opcode_1_0_11);
    bool opcode_OP        = (opcode_6_5_01 & opcode_4_2_100 & opcode_1_0_11);
    bool opcode_OP_FP     = (opcode_6_5_10 & opcode_4_2_100 & opcode_1_0_11);
    bool opcode_SYSTEM    = (opcode_6_5_11 & opcode_4_2_100 & opcode_1_0_11);
    bool opcode_AUIPC     = (opcode_6_5_00 & opcode_4_2_101 & opcode_1_0_11);
    bool opcode_LUI       = (opcode_6_5_01 & opcode_4_2_101 & opcode_1_0_11);
    bool opcode_OP_IMM_32 = (opcode_6_5_00 & opcode_4_2_110 & opcode_1_0_11);
    bool opcode_OP_32     = (opcode_6_5_01 & opcode_4_2_110 & opcode_1_0_11);

    bool rv32_funct3_000 = (instr.funct3.val == 0x0);
    bool rv32_funct3_001 = (instr.funct3.val == 0x1);
    bool rv32_funct3_010 = (instr.funct3.val == 0x2);
    bool rv32_funct3_011 = (instr.funct3.val == 0x3);
    bool rv32_funct3_100 = (instr.funct3.val == 0x4);
    bool rv32_funct3_101 = (instr.funct3.val == 0x5);
    bool rv32_funct3_110 = (instr.funct3.val == 0x6);
    bool rv32_funct3_111 = (instr.funct3.val == 0x7);

    bool rv32_funct7_0000000 = (instr.funct7.val == 0x00);
    bool rv32_funct7_0100000 = (instr.funct7.val == 0x20);

    bool rv32i_table[47] = {
        [rv32i_LUI   ] = opcode_LUI,
        [rv32i_AUIPC ] = opcode_AUIPC,
        [rv32i_JAL   ] = opcode_JAL,
        [rv32i_JALR  ] = opcode_JALR,
        [rv32i_BEQ   ] = opcode_BRANCH & rv32_funct3_000,
        [rv32i_BNE   ] = opcode_BRANCH & rv32_funct3_001,
        [rv32i_BLT   ] = opcode_BRANCH & rv32_funct3_100,
        [rv32i_BGE   ] = opcode_BRANCH & rv32_funct3_101,
        [rv32i_BLTU  ] = opcode_BRANCH & rv32_funct3_110,
        [rv32i_BGEU  ] = opcode_BRANCH & rv32_funct3_111,
        [rv32i_LB    ] = opcode_LOAD & rv32_funct3_000,
        [rv32i_LH    ] = opcode_LOAD & rv32_funct3_001,
        [rv32i_LW    ] = opcode_LOAD & rv32_funct3_010,
        [rv32i_LBU   ] = opcode_LOAD & rv32_funct3_100,
        [rv32i_LHU   ] = opcode_LOAD & rv32_funct3_101,
        [rv32i_SB    ] = opcode_STORE & rv32_funct3_000,
        [rv32i_SH    ] = opcode_STORE & rv32_funct3_001,
        [rv32i_SW    ] = opcode_STORE & rv32_funct3_010,
        [rv32i_ADDI  ] = opcode_OP_IMM & rv32_funct3_000,
        [rv32i_SLTI  ] = opcode_OP_IMM & rv32_funct3_010,
        [rv32i_SLTIU ] = opcode_OP_IMM & rv32_funct3_011,
        [rv32i_XORI  ] = opcode_OP_IMM & rv32_funct3_100,
        [rv32i_ORI   ] = opcode_OP_IMM & rv32_funct3_110,
        [rv32i_ANDI  ] = opcode_OP_IMM & rv32_funct3_111,
        [rv32i_SLLI  ] = opcode_OP_IMM & rv32_funct7_0000000 & rv32_funct3_001,
        [rv32i_SRLI  ] = opcode_OP_IMM & rv32_funct7_0000000 & rv32_funct3_101,
        [rv32i_SRAI  ] = opcode_OP_IMM & rv32_funct7_0100000 & rv32_funct3_101,
        [rv32i_ADD   ] = opcode_OP & rv32_funct7_0000000 & rv32_funct3_000,
        [rv32i_SUB   ] = opcode_OP & rv32_funct7_0100000 & rv32_funct3_000,
        [rv32i_SLL   ] = opcode_OP & rv32_funct7_0000000 & rv32_funct3_001,
        [rv32i_SLT   ] = opcode_OP & rv32_funct7_0000000 & rv32_funct3_010,
        [rv32i_SLTU  ] = opcode_OP & rv32_funct7_0000000 & rv32_funct3_011,
        [rv32i_XOR   ] = opcode_OP & rv32_funct7_0000000 & rv32_funct3_100,
        [rv32i_SRL   ] = opcode_OP & rv32_funct7_0000000 & rv32_funct3_101,
        [rv32i_SRA   ] = opcode_OP & rv32_funct7_0100000 & rv32_funct3_101,
        [rv32i_OR    ] = opcode_OP & rv32_funct7_0000000 & rv32_funct3_110,
        [rv32i_AND   ] = opcode_OP & rv32_funct7_0000000 & rv32_funct3_111,
        [rv32i_FENCE ] = 0,
        [rv32i_FENCEI] = 0,
        [rv32i_ECALL ] = 0,
        [rv32i_EBREA ] = 0,
        [rv32i_CSRRW ] = 0,
        [rv32i_CSRRS ] = 0,
        [rv32i_CSRRC ] = 0,
        [rv32i_CSRRWI] = 0,
        [rv32i_CSRRSI] = 0,
        [rv32i_CSRRCI] = 0
    };

    info->name = get_rv32i_name(rv32i_table);
    info->description = get_rv32i_description(rv32i_table);

    info->instr_type.R = opcode_OP;
    info->instr_type.I = opcode_OP_IMM | opcode_LOAD;
    info->instr_type.S = opcode_STORE;
    info->instr_type.SB = opcode_BRANCH;
    info->instr_type.U = opcode_LUI | opcode_AUIPC;
    info->instr_type.UJ = opcode_JAL | opcode_JALR;

    info->imm_type.I = (opcode_OP_IMM | opcode_JALR | opcode_LOAD);
    info->imm_type.S = opcode_STORE;
    info->imm_type.B = opcode_BRANCH;
    info->imm_type.U = (opcode_LUI | opcode_AUIPC);
    info->imm_type.J = opcode_JAL;

    info->imm32 = imm_gen(instr, info->imm_type);
}

char *get_rv32i_name(bool rv32i_table[]) {
    for(uint8_t i=0; i<47; i++) {
        if(rv32i_table[i] == 1) {
            switch (i)
            {
                case rv32i_LUI   : return "LUI";
                case rv32i_AUIPC : return "AUIPC";
                case rv32i_JAL   : return "JAL";
                case rv32i_JALR  : return "JALR";
                case rv32i_BEQ   : return "BEQ";
                case rv32i_BNE   : return "BNE";
                case rv32i_BLT   : return "BLT";
                case rv32i_BGE   : return "BGE";
                case rv32i_BLTU  : return "BLTU";
                case rv32i_BGEU  : return "BGEU";
                case rv32i_LB    : return "LB";
                case rv32i_LH    : return "LH";
                case rv32i_LW    : return "LW";
                case rv32i_LBU   : return "LBU";
                case rv32i_LHU   : return "LHU";
                case rv32i_SB    : return "SB";
                case rv32i_SH    : return "SH";
                case rv32i_SW    : return "SW";
                case rv32i_ADDI  : return "ADDI";
                case rv32i_SLTI  : return "SLTI";
                case rv32i_SLTIU : return "SLTIU";
                case rv32i_XORI  : return "XORI";
                case rv32i_ORI   : return "ORI";
                case rv32i_ANDI  : return "ANDI";
                case rv32i_SLLI  : return "SLLI";
                case rv32i_SRLI  : return "SRLI";
                case rv32i_SRAI  : return "SRAI";
                case rv32i_ADD   : return "ADD";
                case rv32i_SUB   : return "SUB";
                case rv32i_SLL   : return "SLL";
                case rv32i_SLT   : return "SLT";
                case rv32i_SLTU  : return "SLTU";
                case rv32i_XOR   : return "XOR";
                case rv32i_SRL   : return "SRL";
                case rv32i_SRA   : return "SRA";
                case rv32i_OR    : return "OR";
                case rv32i_AND   : return "AND";
                case rv32i_FENCE : return "FENCE";
                case rv32i_FENCEI: return "FENCE.I";
                case rv32i_ECALL : return "ECALL";
                case rv32i_EBREA : return "EBREA";
                case rv32i_CSRRW : return "CSRRW";
                case rv32i_CSRRS : return "CSRRS";
                case rv32i_CSRRC : return "CSRRC";
                case rv32i_CSRRWI: return "CSRRWI";
                case rv32i_CSRRSI: return "CSRRSI";
                case rv32i_CSRRCI: return "CSRRCI";
            
                default: return "null";
            }
        }
    }
}

char *get_rv32i_description(bool rv32i_table[]) {
    for(uint8_t i=0; i<47; i++) {
        if(rv32i_table[i] == 1) {
            switch (i)
            {
                case rv32i_LUI   : return "LUI �?20位的立即数imm左移12�?, 补成一�?32位数imm32, 并将imm32写回rd寄存�?";
                case rv32i_AUIPC : return "AUIPC �?20位的立即数imm左移12�?, 补成一�?32位数imm32, 并将imm32与PC相加, 结果写回rd寄存�?";
                case rv32i_JAL   : return "JAL 使用20位的有符号立即数imm, 并将imm左移一位后得到offset, 使得PC = PC+offset, 实现跳转. 最后将PC+4写回rd寄存�?";
                case rv32i_JALR  : return "JALR 使用12位的有符号立即数imm作为offset, 使得PC = rs1+offset, 实现跳转. 最后将PC+4写回rd寄存�?";
                case rv32i_BEQ   : return "BEQ 使用12位的有符号立即数imm, 并将imm左移一位后得到offset. 当rs1 == rs2�?, PC = PC+offset, 实现跳转; 否则PC = PC+4";
                case rv32i_BNE   : return "BNE 使用12位的有符号立即数imm, 并将imm左移一位后得到offset. 当rs1 != rs2�?, PC = PC+offset, 实现跳转; 否则PC = PC+4";
                case rv32i_BLT   : return "BLT 使用12位的有符号立即数imm, 并将imm左移一位后得到offset. 当rs1 < rs2�?, PC = PC+offset, 实现跳转; 否则PC = PC+4";
                case rv32i_BGE   : return "BGE 使用12位的有符号立即数imm, 并将imm左移一位后得到offset. 当rs1 >= rs2�?, PC = PC+offset, 实现跳转; 否则PC = PC+4";
                case rv32i_BLTU  : return "BLTU 使用12位的有符号立即数imm, 并将imm左移一位后得到offset. 当无符号对比rs1 < rs2�?, PC = PC+offset, 实现跳转; 否则PC = PC+4";
                case rv32i_BGEU  : return "BGEU 使用12位的有符号立即数imm, 并将imm左移一位后得到offset. 当无符号对比rs1 == rs2�?, PC = PC+offset, 实现跳转; 否则PC = PC+4";
                case rv32i_LB    : return "LB 从memory[rs1+imm]中读回一�?8位数据data, 并将data进行符号扩展�?, 写回rd寄存�?";
                case rv32i_LH    : return "LH 从memory[rs1+imm]中读回一�?16位数据data, 并将data进行符号扩展�?, 写回rd寄存�?";
                case rv32i_LW    : return "LW 从memory[rs1+imm]中读回一�?32位数据data, 并将data写回rd寄存�?";
                case rv32i_LBU   : return "LBU 从memory[rs1+imm]中读回一�?8位数据data, 并将data进行高位�?0扩展�?, 写回rd寄存�?";
                case rv32i_LHU   : return "LHU 从memory[rs1+imm]中读回一�?16位数据data, 并将data进行高位�?0扩展�?, 写回rd寄存�?";
                case rv32i_SB    : return "SB 将rs2的低8位数�?, 写回memory[rs1+imm]�?";
                case rv32i_SH    : return "SH 将rs2的低16位数�?, 写回memory[rs1+imm]�?";
                case rv32i_SW    : return "SW 将rs2�?32位数�?, 写回memory[rs1+imm]�?";
                case rv32i_ADDI  : return "ADDI �?12位立即数imm进行符号位扩�?, 得到一�?32位立即数imm32, 并将rs1与imm32相加的结果写回rd寄存�?";
                case rv32i_SLTI  : return "SLTI �?12位立即数imm进行符号位扩�?, 得到一�?32位立即数imm32, 有符号比较若rs1 < imm32, 则rd寄存器写1; 否则rd寄存器写0";
                case rv32i_SLTIU : return "SLTIU �?12位立即数imm进行符号位扩�?, 得到一�?32位立即数imm32, 无符号比较若rs1 < imm32, 则rd寄存器写1; 否则rd寄存器写0";
                case rv32i_XORI  : return "XORI �?12位立即数imm进行符号位扩�?, 得到一�?32位立即数imm32, 并将rs1与imm32相异�?(xor, ^)的结果写回rd寄存�?";
                case rv32i_ORI   : return "ORI �?12位立即数imm进行符号位扩�?, 得到一�?32位立即数imm32, 并将rs1与imm32相或(or, |)的结果写回rd寄存�?";
                case rv32i_ANDI  : return "ANDI �?12位立即数imm进行符号位扩�?, 得到一�?32位立即数imm32, 并将rs1与imm32相与(and, &)的结果写回rd寄存�?";
                case rv32i_SLLI  : return "SLLI 取立即数imm字段�?5位作为offset, 并将rs1逻辑左移offset�?(低位�?0), 结果写回rd寄存�?";
                case rv32i_SRLI  : return "SRLI 取立即数imm字段�?5位作为offset, 并将rs1逻辑右移offset�?(高位�?0), 结果写回rd寄存�?";
                case rv32i_SRAI  : return "SRAI 取立即数imm字段�?5位作为offset, 并将rs1算术右移offset�?(高位补入符号�?), 结果写回rd寄存�?";
                case rv32i_ADD   : return "ADD 将rs1与rs2相加的结果写回rd寄存�?";
                case rv32i_SUB   : return "SUB 将rs1与rs2相减的结果写回rd寄存�?";
                case rv32i_SLL   : return "SLL 取rs2�?5位作为offset, 并将rs1逻辑左移offset�?(低位�?0), 结果写回rd寄存�?";
                case rv32i_SLT   : return "SLT 有符号比较若rs1 < rs2, 则rd寄存器写1; 否则rd寄存器写0";
                case rv32i_SLTU  : return "SLTU 无符号比较若rs1 < rs2, 则rd寄存器写1; 否则rd寄存器写0";
                case rv32i_XOR   : return "XOR 将rs1与rs2相异�?(xor, ^)的结果写回rd寄存�?";
                case rv32i_SRL   : return "SRL 取rs2�?5位作为offset, 并将rs1逻辑右移offset�?(高位�?0), 结果写回rd寄存�?";
                case rv32i_SRA   : return "SRA 取rs2�?5位作为offset, 并将rs1算术右移offset�?(高位补入符号�?), 结果写回rd寄存�?";
                case rv32i_OR    : return "OR 将rs1与rs2相或(or, |)的结果写回rd寄存�?";
                case rv32i_AND   : return "AND 将rs1与rs2相与(and, &)的结果写回rd寄存�?";
                case rv32i_FENCE : return "FENCE 用于屏障\"数据\"存储器访问请求的执行顺序";
                case rv32i_FENCEI: return "FENCE.I 用于同步指令和数据流";
                case rv32i_ECALL : return "ECALL 用于生成环境调用(Environment Call)异常";
                case rv32i_EBREA : return "EBREA 用于生成断点(Breakpoint)异常";
                case rv32i_CSRRW : return "CSRRW 将CSR寄存器的数据写回rd寄存�?(rd = CSR[csridx]); 同时将rs1的数值写入CSR寄存�?(CSR[csridx] = rs1)";
                case rv32i_CSRRS : return "CSRRS 将CSR寄存器的数据写回rd寄存�?(rd = CSR[csridx]); 同时CSR将与rs1对应�?1的比特位�?1(CSR |= rs1)";
                case rv32i_CSRRC : return "CSRRC 将CSR寄存器的数据写回rd寄存�?(rd = CSR[csridx]); 同时CSR将与rs1对应�?1的比特位清零(CSR &= ~rs1)";
                case rv32i_CSRRWI: return "CSRRWI 将CSR寄存器的数据写回rd寄存�?(rd = CSR[csridx]); 同时�?5位立即数imm高位�?0扩展�?, 写入CSR寄存�?";
                case rv32i_CSRRSI: return "CSRRSI 将CSR寄存器的数据写回rd寄存�?(rd = CSR[csridx]); 同时�?5位立即数imm高位�?0扩展得到imm32, 并且CSR将与imm32对应�?1的比特位�?1(CSR |= imm32)";
                case rv32i_CSRRCI: return "CSRRCI 将CSR寄存器的数据写回rd寄存�?(rd = CSR[csridx]); 同时�?5位立即数imm高位�?0扩展得到imm32, 并且CSR将与imm32对应�?1的比特位清零(CSR &= ~imm32)";
            
                default: return "尚未支持该指�?";
            }
        }
    }
}

int32_t imm_gen(rv32_t instr, imm_type_t imm_type) {
    uint32_t imm_I = (instr.I.imm_11_0);
    uint32_t imm_S = (instr.S.imm_11_5<<5|instr.S.imm_4_0);
    uint32_t imm_B = ((instr.SB.imm_12<<12) | (instr.SB.imm_11<<11) | (instr.SB.imm_10_5<<5) | (instr.SB.imm_4_1<<1));
    uint32_t imm_U = (instr.U.imm_31_12<<12);
    uint32_t imm_J = ((instr.UJ.imm_20<<20) | (instr.UJ.imm_19_12<<12) | (instr.UJ.imm_11<<11) | (instr.UJ.imm_10_1<<1));

    int32_t imm32_I = (!imm_type.I) ? 0xDEADBEEF : (instr.I.imm_11_0>>11)  ? (0xFFFFF800 | imm_I) : imm_I;
    int32_t imm32_S = (!imm_type.S) ? 0xDEADBEEF : (instr.S.imm_11_5>>6)   ? (0xFFFFF800 | imm_S) : imm_S;
    int32_t imm32_B = (!imm_type.B) ? 0xDEADBEEF : (instr.SB.imm_12)       ? (0xFFFFE000 | imm_B) : imm_B;
    int32_t imm32_U = imm_U;
    int32_t imm32_J = (!imm_type.J) ? 0xDEADBEEF : (instr.UJ.imm_20)       ? (0xFFF00000 | imm_J) : imm_J;
    
    switch (imm_type.val & 0x1F)
    {
        case 16: return imm32_I;
        case 8:  return imm32_S;
        case 4:  return imm32_B;
        case 2:  return imm32_U;
        case 1:  return imm32_J;
        default:
            return 0xDEADBEEF;
    }
}

uint32_t dec2bin(uint32_t dec) {
    int p = 1, y = 0, mod;
    while(1)
    {
        mod = dec%2;
        dec /= 2;
        y += mod*p;
        p *= 10;
        if(dec < 2)
        {
            y += dec*p;
            break;
        }
    }
    return y;
}

void log_out(rv32_info_t *info, rv32_t instr) {
    switch (info->instr_type.val & 0x3F)
    {
        case 32: printf("Type R: "); 
                 printf("%s%s%s %sx%02d%s, %sx%02d%s, %sx%02d%s\r\n", KMAG info->name END, KBLU instr.R.rdidx END, KCYN instr.R.rs1idx END, KYEL instr.R.rs2idx END); 
                 printf("%s%s%s\r\n", KORG info->description END);
                 printf("------------------------------\r\n");
                 printf("funct7  rs2 rs1 funct3 rd opcode\r\n");
                 printf("%s%07d%s %s%02d%s  %s%02d%s  %s%03d%s    %s%02d%s %s%07d%s\r\n", KWHT dec2bin(instr.R.funct7) END, KYEL instr.R.rs2idx END, KCYN instr.R.rs1idx END, KWHT dec2bin(instr.R.funct3) END, KBLU instr.R.rdidx END, KGRN dec2bin(instr.R.opcode) END);
                 break;
        case 16: printf("Type I: ");
                 printf("%s%s%s %sx%02d%s, %sx%02d%s, %s%d%s\r\n", KMAG info->name END, KBLU instr.I.rdidx END, KCYN instr.I.rs1idx END, KRED info->imm32 END);
                 printf("%s%s%s\r\n", KORG info->description END);
                 printf("------------------------------\r\n");
                 printf("funct3 rd rs1 opcode\r\n");
                 printf("%s%03d%s    %s%02d%s %s%02d%s  %s%07d%s\r\n", KWHT dec2bin(instr.I.funct3) END, KBLU instr.I.rdidx END, KCYN instr.I.rs1idx END, KGRN dec2bin(instr.I.opcode) END);
                 break;
        case 8:  printf("Type S: "); 
                 printf("%s%s%s %sx%02d%s, %s%d%s(%sx%02d%s)\r\n", KMAG info->name END, KYEL instr.S.rs2idx END, KRED info->imm32 END, KCYN instr.S.rs1idx END);
                 printf("%s%s%s\r\n", KORG info->description END);
                 printf("------------------------------\r\n");
                 printf("rs2 rs1 funct3 opcode  imm32\r\n");
                 printf("%s%02d%s  %s%02d%s  %s%03d%s    %s%07d%s %s%d%s\r\n", KYEL instr.S.rs2idx END, KCYN instr.S.rs1idx END, KWHT dec2bin(instr.S.funct3) END, KGRN dec2bin(instr.S.opcode) END, KRED info->imm32 END);
                 break;
        case 4:  printf("Type SB: ");
                 printf("%s%s%s %sx%02d%s, %sx%02d%s, %s%d%s\r\n", KMAG info->name END, KYEL instr.SB.rs2idx END, KCYN instr.SB.rs1idx END, KRED info->imm32 END);
                 printf("%s%s%s\r\n", KORG info->description END);
                 printf("------------------------------\r\n");
                 printf("rs2 rs1 funct3 opcode  imm32\r\n");
                 printf("%s%02d%s  %s%02d%s  %s%03d%s    %s%07d%s %s%d%s\r\n", KYEL instr.SB.rs2idx END, KCYN instr.SB.rs1idx END, KWHT dec2bin(instr.SB.funct3) END, KGRN dec2bin(instr.SB.opcode) END, KRED info->imm32 END);
                 break;
        case 2:  printf("Type U: ");
                 printf("%s%s%s %sx%02d%s, (%s%d%s)<<12 = %d\r\n", KMAG info->name END, KBLU instr.U.rdidx END, KRED instr.U.imm_31_12 END, info->imm32);
                 printf("%s%s%s\r\n", KORG info->description END);
                 printf("------------------------------\r\n");
                 printf("rd opcode      imm\r\n");
                 printf("%s%02d%s %s%07d%s (%s%d%s)<<12 = %d\r\n", KBLU instr.U.rdidx END, KGRN dec2bin(instr.U.opcode) END, KRED instr.U.imm_31_12 END, info->imm32);
                 break;
        case 1:  printf("Type UJ: ");
                 printf("%s%s%s %sx%02d%s, %s%d%s\r\n", KMAG info->name END, KBLU instr.UJ.rdidx END, KRED info->imm32 END);
                 printf("%s%s%s\r\n", KORG info->description END);
                 printf("------------------------------\r\n");
                 printf("rd opcode  imm\r\n");
                 printf("%s%02d%s %s%07d%s %s%d%s\r\n", KBLU instr.UJ.rdidx END, KGRN dec2bin(instr.UJ.opcode) END, KRED info->imm32 END);
                 break;
        default:
            printf("RISC-V Instruction not supported.\r\n"); break;
    }

    printf("\r\n\r\n");

    return;
}