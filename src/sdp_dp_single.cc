// =============================================================================
// MIT License
//
// Copyright (c) 2022 Princeton University
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// =============================================================================

// File: sdp_dp_single.cc

#include <ilang/ilang++.h>
#include <iostream>
using namespace std;
//#include "uninterpreted_func/uninterpreted_func.h"
#include <sdp_config.h>

namespace ilang {

// TODO:
// - every output here has to end with c1; also separate into an sdp_dp_y and sdp_dp_x instructions (ie, throughput is different for x and y, acrually doesnt matter for software so imcorporate both here)
// - do we need to account for format conversions among int throughout the datapath?
// - next, need multi-step instructions like the ones on the unit description (i.e., batch norm)
// - make a copy of every instruction for group 1
// - looks like when doing configure, have producer & !group0; here do consumer & group0 (i.e., this is datapath running, no more config here)
// - remake the following instructions with general functions, will prevent need to have too much code re-write (ie, still have group instructions and just pass in name of group as an arg)

ExprRef IntLog2(ExprRef operand) {
    const int length = operand.bit_width();
    
    // Initialization
    ExprRef* result = (ExprRef*)malloc(sizeof(ExprRef) * (length + 1));
    for (int j = 0; j < length + 1; j++) {
        result[j] = BvConst(0, 32);
    }

    auto flag = BvConst(0, 1);
    for (int i = length - 1; i >= 0; i--) { 
        result[i] = Ite(SelectBit(flag, 0) == BvConst(0, 1) & SelectBit(operand, i) == BvConst(1, 1), BvConst(i, 32), result[i + 1]);
        flag = flag | Ite(SelectBit(operand, i) == 0x1, BvConst(1, 1), BvConst(0, 1));
    }

    return result[0];
}

// Technically is 1ull, so might be 64 bits
ExprRef IntLog2Frac(ExprRef operand, ExprRef index) {
    return operand & ((BvConst(1, 32) << index) - 1);
}

void DefineSDPInstrsDP_Single(Ila& m) {
    // m.AddInit(m.state(NVDLA_CSC_S_STATUS_0) == BvConst(0, 2));
    // m.AddInit(m.state(NVDLA_CSC_S_STATUS_1) == BvConst(0, 2));

    // CSB MIMO
    auto csb_addr = Extract(Concat(m.input("csb_addr"), BvConst(0,2)), 11, 0);
    auto csb_valid = (m.state("csb_rdy") == BvConst(1,1)) & (m.input("csb_vld") == BvConst(1,1));
    auto csb_write = m.input("csb_write") == BvConst(1,1);
    auto group0_unset = m.state(GetVarName("group0_", NVDLA_SDP_D_OP_ENABLE)) == BvConst(0,1);
    auto group1_unset = m.state(GetVarName("group1_", NVDLA_SDP_D_OP_ENABLE)) == BvConst(0,1);
    auto producer = m.state(NVDLA_SDP_S_PRODUCER);
    auto consumer = m.state(NVDLA_SDP_S_CONSUMER);
    auto csc_state = m.state("csc_state");

    // Status: 3
    // 1: IDLE
    // 2: Producer (op_enable) and Consumer
    // 3: Producer (op_enable) but not consumer

    { // ReLU computation - Group 0
    // should be !group_unset everywhere or use state == busy
        auto instr = m.NewInstr("Compute_ReLU");
        auto reg_group = "group0_";

        // Account for possibility that ReLU can be computed using either the X1 module or the X2 module
        auto x1_ok_g0 = (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x1) & 
                    (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_RELU_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_BYPASS)), 0) == 0x1);
        auto x2_ok_g0 = (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x1) & 
                    (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_RELU_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_BYPASS)), 0) == 0x1);
        auto group0_regs = (x1_ok_g0 | x2_ok_g0) & consumer == BvConst(0, 1) & !group0_unset;

        // config group 0 -> sdp_state = IDLE
        // enable group 0 -> sdp_state = BUSY
        // consumer is group 0 + config group 1 -> sdp_state = BUSY

        instr.SetDecode(group0_regs);

        // Determine the source of the input
        auto flying_mode = m.state(GetVarName(reg_group, NVDLA_SDP_D_FLYING_MODE));

        for (int i = 0; i < 16; i++) {
            auto input = Ite(SelectBit(flying_mode, 0) == 0x1, m.input(GetVarName("cacc_data_", (std::to_string(i)))), m.input(GetVarName("mrdma_data_", (std::to_string(i)))));
            auto pdp_output = m.state(GetVarName("pdp_output_", (std::to_string(i))));

            // Compute ReLU
            auto output = Ite((input > 0x0), input, BvConst(0, 32));
            instr.SetUpdate(pdp_output, output);
        }
    }

    { // ALU max computation - Group 0
        auto instr = m.NewInstr("Compute_Max");
        auto reg_group = "group0_";

        // Account for possibility that ALU max can be computed using either the X1 module, X2 module, or Y module
        auto x1_ok_g0 = (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x1) & 
                    (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_BYPASS)), 0) == 0x0 & Extract(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_ALGO)), 1, 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_RELU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_BYPASS)), 0) == 0x1);
        auto x2_ok_g0 = (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x1) & 
                    (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_BYPASS)), 0) == 0x0 & Extract(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_ALGO)), 1, 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_RELU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_BYPASS)), 0) == 0x1);
        // auto y_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS))) & 
        //             (!m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_ALU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_ALU_ALGO)) == 0x0 & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_LUT_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_MUL_BYPASS)));
        auto group0_regs = (x1_ok_g0 | x2_ok_g0) & consumer == BvConst(0, 1) & !group0_unset;

        instr.SetDecode(group0_regs);

        // Determine the source of the operands
        auto flying_mode = m.state(GetVarName(reg_group, NVDLA_SDP_D_FLYING_MODE));
        auto data_source = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_SRC));
        auto alu_shift = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_SHIFT_VALUE));

        for (int i = 0; i < 16; i++) {
            // Setup operands
            auto input = Ite(SelectBit(flying_mode, 0) == 0x1, m.input(GetVarName("cacc_data_", (std::to_string(i)))), m.input(GetVarName("mrdma_data_", (std::to_string(i)))));
            auto pdp_output = m.state(GetVarName("pdp_output_", (std::to_string(i))));
            auto operand = Ite(SelectBit(data_source, 0) == 0x1, m.input(GetVarName("dma_data_", (std::to_string(i)))), m.input(GetVarName("regs_data_", (std::to_string(i)))));

            // Compute maximum
            auto operand_shifted = operand << Concat(BvConst(0, 10), alu_shift);
            auto operand_shifted_extend = Concat(BvConst(0, 16), operand_shifted);
            auto output = Ite((input > operand_shifted_extend), input, operand_shifted_extend);
            instr.SetUpdate(pdp_output, output);
        }
    }

    { // ALU min computation - Group 0
        auto instr = m.NewInstr("Compute_Min");
        auto reg_group = "group0_";

        // Account for possibility that ALU min can be computed using either the X1 module, X2 module, or Y module
        auto x1_ok_g0 = (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x1) & 
                    (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_BYPASS)), 0) == 0x0 & Extract(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_ALGO)), 1, 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_RELU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_BYPASS)), 0) == 0x1);
        auto x2_ok_g0 = (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x1) & 
                    (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_BYPASS)), 0) == 0x0 & Extract(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_ALGO)), 1, 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_RELU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_BYPASS)), 0) == 0x1);
        // auto y_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS))) & 
        //             (!m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_ALU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_ALU_ALGO)) == 0x0 & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_LUT_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_MUL_BYPASS)));
        auto group0_regs = (x1_ok_g0 | x2_ok_g0) & consumer == BvConst(0, 1) & group0_unset;

        instr.SetDecode(group0_regs);

        // Determine the source of the operands
        auto flying_mode = m.state(GetVarName(reg_group, NVDLA_SDP_D_FLYING_MODE));
        auto data_source = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_SRC));
        auto alu_shift = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_SHIFT_VALUE));

        for (int i = 0; i < 16; i++) {
            // Setup operands
            auto input = Ite(SelectBit(flying_mode, 0) == 0x1, m.input(GetVarName("cacc_data_", (std::to_string(i)))), m.input(GetVarName("mrdma_data_", (std::to_string(i)))));
            auto pdp_output = m.state(GetVarName("pdp_output_", (std::to_string(i))));
            auto operand = Ite(SelectBit(data_source, 0) == 0x1, m.input(GetVarName("dma_data_", (std::to_string(i)))), m.input(GetVarName("regs_data_", (std::to_string(i)))));

            // Compute minimum
            auto operand_shifted = operand << Concat(BvConst(0, 10), alu_shift);
            auto operand_shifted_extend = Concat(BvConst(0, 16), operand_shifted);
            auto output = Ite((input < operand_shifted_extend), input, operand_shifted_extend);
            instr.SetUpdate(pdp_output, output);
        }
    }

    { // ALU add computation - Group 0
        auto instr = m.NewInstr("Compute_Add");
        auto reg_group = "group0_";

        // Account for possibility that ALU add can be computed using either the X1 module, X2 module, or Y module
        auto x1_ok_g0 = (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x1) & 
                    (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_BYPASS)), 0) == 0x0 & Extract(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_ALGO)), 1, 0) == 0x2 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_RELU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_BYPASS)), 0) == 0x1);
        auto x2_ok_g0 = (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x1) & 
                    (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_BYPASS)), 0) == 0x0 & Extract(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_ALGO)), 1, 0) == 0x2 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_RELU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_BYPASS)), 0) == 0x1);
        // auto y_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS))) & 
        //             (!m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_ALU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_ALU_ALGO)) == 0x0 & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_LUT_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_MUL_BYPASS)));
        auto group0_regs = (x1_ok_g0 | x2_ok_g0) & consumer == BvConst(0, 1) & group0_unset;

        instr.SetDecode(group0_regs);

        // Determine the source of the operands
        auto flying_mode = m.state(GetVarName(reg_group, NVDLA_SDP_D_FLYING_MODE));
        auto data_source = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_SRC));
        auto alu_shift = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_SHIFT_VALUE));

        for (int i = 0; i < 16; i++) {
            // Setup operands
            auto input = Ite(SelectBit(flying_mode, 0) == 0x1, m.input(GetVarName("cacc_data_", (std::to_string(i)))), m.input(GetVarName("mrdma_data_", (std::to_string(i)))));
            auto pdp_output = m.state(GetVarName("pdp_output_", (std::to_string(i))));
            auto operand = Ite(SelectBit(data_source, 0) == 0x1, m.input(GetVarName("dma_data_", (std::to_string(i)))), m.input(GetVarName("regs_data_", (std::to_string(i)))));

            // Compute add
            auto operand_shifted = operand << Concat(BvConst(0, 10), alu_shift);
            auto operand_shifted_extend = Concat(BvConst(0, 16), operand_shifted);
            auto output = input + operand_shifted_extend;
            instr.SetUpdate(pdp_output, output);
        }
    }

    { // MUL multiply computation - Group 0
        auto instr = m.NewInstr("Compute_Multiply");
        auto reg_group = "group0_";

        // Account for possibility that MUL multiply can be computed using either the X1 module or the X2 module
        auto x1_ok_g0 = (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x1) & 
                    (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_PRELU)), 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_RELU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_BYPASS)), 0) == 0x1);
        auto x2_ok_g0 = (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x1) & 
                    (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_PRELU)), 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_RELU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_BYPASS)), 0) == 0x1);
        // auto y_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS))) & 
        //             (!m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_ALU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_ALU_ALGO)) == 0x0 & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_LUT_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_MUL_BYPASS)));
        auto group0_regs = (x1_ok_g0 | x2_ok_g0) & consumer == BvConst(0, 1) & group0_unset;

        instr.SetDecode(group0_regs);

        // Determine the source of the operands
        auto flying_mode = m.state(GetVarName(reg_group, NVDLA_SDP_D_FLYING_MODE));
        auto data_source = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_SRC));
        auto mul_shift = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_SHIFT_VALUE));

        for (int i = 0; i < 16; i++) {
            // Setup operands
            auto input = Ite(SelectBit(flying_mode, 0) == 0x1, m.input(GetVarName("cacc_data_", (std::to_string(i)))), m.input(GetVarName("mrdma_data_", (std::to_string(i)))));
            auto pdp_output = m.state(GetVarName("pdp_output_", (std::to_string(i))));
            auto operand = Ite(SelectBit(data_source, 0) == 0x1, m.input(GetVarName("dma_data_", (std::to_string(i)))), m.input(GetVarName("regs_data_", (std::to_string(i)))));

            // Compute multiply
            auto operand_extend = Concat(BvConst(0, 16), operand);
            auto output = input * operand_extend;
            auto output_shifted = output >> Concat(BvConst(0, 24), mul_shift);
            instr.SetUpdate(pdp_output, output_shifted);
        }
    }

    { // MUL PReLU computation - Group 0
        auto instr = m.NewInstr("Compute_PReLU");
        auto reg_group = "group0_";

        // Account for possibility that MUL PReLU can be computed using either the X1 module or the X2 module
        auto x1_ok_g0 = (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x1) & 
                    (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_PRELU)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_RELU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_BYPASS)), 0) == 0x1);
        auto x2_ok_g0 = (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x1) & 
                    (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_PRELU)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_RELU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_BYPASS)), 0) == 0x1);
        // auto y_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS))) & 
        //             (!m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_ALU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_ALU_ALGO)) == 0x0 & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_LUT_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_MUL_BYPASS)));
        auto group0_regs = (x1_ok_g0 | x2_ok_g0) & consumer == BvConst(0, 1) & group0_unset;

        instr.SetDecode(group0_regs);

        // Determine the source of the operands
        auto flying_mode = m.state(GetVarName(reg_group, NVDLA_SDP_D_FLYING_MODE));
        auto data_source = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_SRC));
        auto mul_shift = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_SHIFT_VALUE));

        for (int i = 0; i < 16; i++) {
            // Setup operands
            auto input = Ite(SelectBit(flying_mode, 0) == 0x1, m.input(GetVarName("cacc_data_", (std::to_string(i)))), m.input(GetVarName("mrdma_data_", (std::to_string(i)))));
            auto pdp_output = m.state(GetVarName("pdp_output_", (std::to_string(i))));
            auto operand = Ite(SelectBit(data_source, 0) == 0x1, m.input(GetVarName("dma_data_", (std::to_string(i)))), m.input(GetVarName("regs_data_", (std::to_string(i)))));

            // Compute PReLU
            auto output = Ite((input > 0x0), input, (Concat(BvConst(0, 16), operand) * input) >> Concat(BvConst(0, 24), mul_shift));
            instr.SetUpdate(pdp_output, output);
        }
    }

    { // Write to LUT Table LE
        auto instr = m.NewInstr("Write_LUT_LE");

        // ... lut access type needs to be 1 for write, table id needs to be 1 for lo and 0 for le
        auto access_type = SelectBit(m.state(NVDLA_SDP_S_LUT_ACCESS_TYPE), 0);
        auto table_id = SelectBit(m.state(NVDLA_SDP_S_LUT_TABLE_ID), 0);

        instr.SetDecode(access_type == 0x1 & table_id == 0x0);

        // Set the relevant table
        auto lut = m.state("le_tbl");
        auto lut_addr = m.state(NVDLA_SDP_S_LUT_ADDR);
        auto lut_data = m.state(NVDLA_SDP_S_LUT_ACCESS_DATA);

        for (int i = 0; i < 65; i++) {
            // Write to table
            auto new_lut = Store(lut, BvConst(i, 7), lut_data);
            instr.SetUpdate(lut, new_lut);
        }
    }

    { // Write to LUT Table LO
        auto instr = m.NewInstr("Write_LUT_LO");

        // ... lut access type needs to be 1 for write, table id needs to be 1 for lo and 0 for le
        auto access_type = SelectBit(m.state(NVDLA_SDP_S_LUT_ACCESS_TYPE), 0);
        auto table_id = SelectBit(m.state(NVDLA_SDP_S_LUT_TABLE_ID), 0);

        instr.SetDecode(access_type == 0x1 & table_id == 0x1);

        // Set the relevant table
        auto lut = m.state("lo_tbl");
        auto lut_addr = m.state(NVDLA_SDP_S_LUT_ADDR);
        auto lut_data = m.state(NVDLA_SDP_S_LUT_ACCESS_DATA);

        for (int i = 0; i < 257; i++) {
            // Write to table
            auto new_lut = Store(lut, BvConst(i, 9), lut_data);
            instr.SetUpdate(lut, new_lut);
        }
    }

    { // Read index from LUT Table LE
        auto instr = m.NewInstr("Read_LUT_LE");
        auto reg_group = "group0_";

        // ... lut access type needs to be 1 for write, table id needs to be 1 for lo and 0 for le
        auto access_type = SelectBit(m.state(NVDLA_SDP_S_LUT_ACCESS_TYPE), 0);
        auto table_id = SelectBit(m.state(NVDLA_SDP_S_LUT_TABLE_ID), 0);
        auto lut_bypass = SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_LUT_BYPASS)), 0);

        instr.SetDecode(access_type == 0x0 & table_id == 0x0 & lut_bypass == 0x0);

        // Set the relevant table
        auto lut = m.state("le_tbl");
        auto lut_data = m.state(NVDLA_SDP_S_LUT_ACCESS_DATA);
        auto lut_start = m.state(NVDLA_SDP_S_LUT_LE_START);
        auto lut_index_offset = m.state(NVDLA_SDP_S_LUT_LE_INDEX_OFFSET);
        auto lut_index_select = m.state(NVDLA_SDP_S_LUT_LE_INDEX_SELECT);
        auto lut_function = SelectBit(m.state(NVDLA_SDP_S_LUT_LE_FUNCTION), 0);
        
        // Determine intermediate values for LUT index
        auto le_index_idx =
        Ite(lut_function == 0x0,
            IntLog2(Concat(BvConst(0, 16), lut_data) - lut_start),
            Concat(BvConst(0, 16), lut_data) - lut_start
        );

        auto le_index_s =
        Ite(lut_function == 0x0,
            le_index_idx - Concat(BvConst(0, 24), lut_index_offset),
            le_index_idx >> Concat(BvConst(0, 24), lut_index_select)
        );

        // Generate out-of-bounds flags
        auto le_uflow = 
        Ite(lut_function == 0x0,
            Ite(Concat(BvConst(0, 16), lut_data) <= lut_start,
                BvConst(1, 1),
                Ite(le_index_idx < Concat(BvConst(0, 24), lut_index_offset),
                    BvConst(1, 1),
                    BvConst(0, 1)
                )
            ),

            Ite(Concat(BvConst(0, 16), lut_data) <= lut_start,
                BvConst(1, 1),
                BvConst(0, 1)
            )
        );
        instr.SetUpdate(m.state("le_uflow"), le_uflow);

        auto le_oflow = 
        Ite(lut_function == 0x0,
            Ite(Concat(BvConst(0, 16), lut_data) <= lut_start,
                BvConst(0, 1),
                Ite(le_index_idx < Concat(BvConst(0, 24), lut_index_offset),
                    BvConst(0, 1),
                    Ite(le_index_s >= 0x40,
                        BvConst(1, 1),
                        BvConst(0, 1)
                    )
                )
            ),

            Ite(Concat(BvConst(0, 16), lut_data) <= lut_start,
                BvConst(0, 1),
                Ite(le_index_s >= 0x40,
                    BvConst(1, 1),
                    BvConst(0, 1)
                )
            )
        );
        instr.SetUpdate(m.state("le_oflow"), le_oflow);

        // Determine final value for LUT index
        auto le_index = 
        Ite(lut_function == 0x0,
            Ite(Concat(BvConst(0, 16), lut_data) <= lut_start,
                BvConst(1, 32),
                Ite(le_index_idx < Concat(BvConst(0, 24), lut_index_offset),
                    BvConst(1, 32),
                    Ite(le_index_s >= 0x40,
                        BvConst(64, 32),
                        le_index_s
                    )
                )
            ),

            Ite(Concat(BvConst(0, 16), lut_data) <= lut_start,
                BvConst(0, 32),
                Ite(le_index_s >= 0x40,
                        BvConst(64, 32),
                        le_index_s
                )
            )
        );
        le_index = Extract(le_index, 6, 0);
        instr.SetUpdate(m.state("le_index"), le_index);

        // Determine final value for LUT fraction; assume it is the same size as the LUT index
        auto le_fraction = 
        Ite(lut_function == 0x0,
            Ite(Concat(BvConst(0, 16), lut_data) <= lut_start,
                BvConst(0, 32),
                Ite(le_index_idx < Concat(BvConst(0, 24), lut_index_offset),
                    BvConst(0, 32),
                    Ite(le_index_s >= 0x40,
                        BvConst(0, 32),
                        IntLog2Frac(Concat(BvConst(0, 16), lut_data) - lut_start, le_index_idx) << (BvConst(35, 32) - le_index_idx)
                    )
                )
            ),

            Ite(Concat(BvConst(0, 16), lut_data) <= lut_start,
                BvConst(0, 32),
                Ite(le_index_s >= 0x40,
                        BvConst(0, 32),
                        (le_index_idx & ((BvConst(1, 32) << Concat(BvConst(0, 24), lut_index_select)) - 1)) >> (Concat(BvConst(0, 24), lut_index_select) - BvConst(35, 32))
                )
            )
        );
        le_fraction = Extract(le_fraction, 6, 0);
        instr.SetUpdate(m.state("le_fraction"), le_fraction);

    }

    { // Read index from LUT Table LO
        auto instr = m.NewInstr("Read_LUT_LO");
        auto reg_group = "group0_";

        // ... lut access type needs to be 1 for write, table id needs to be 1 for lo and 0 for le
        auto access_type = SelectBit(m.state(NVDLA_SDP_S_LUT_ACCESS_TYPE), 0);
        auto table_id = SelectBit(m.state(NVDLA_SDP_S_LUT_TABLE_ID), 0);
        auto lut_bypass = SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_LUT_BYPASS)), 0);

        instr.SetDecode(access_type == 0x0 & table_id == 0x1 & lut_bypass == 0x0);

        // Set the relevant table
        auto lut = m.state("lo_tbl");
        auto lut_data = m.state(NVDLA_SDP_S_LUT_ACCESS_DATA);
        auto lut_start = m.state(NVDLA_SDP_S_LUT_LO_START);
        auto lut_index_select = m.state(NVDLA_SDP_S_LUT_LO_INDEX_SELECT);

        // Determine intermediate values for LUT index
        auto lo_index_idx = Concat(BvConst(0, 16), lut_data) - lut_start;

        auto lo_index_s = lo_index_idx >> Concat(BvConst(0, 24), lut_index_select);

        // Generate out-of-bounds flags
        auto lo_uflow = 
        Ite(Concat(BvConst(0, 16), lut_data) <= lut_start,
            BvConst(1, 1),
            BvConst(0, 1)
        );
        instr.SetUpdate(m.state("lo_uflow"), lo_uflow);

        auto lo_oflow = 
        Ite(Concat(BvConst(0, 16), lut_data) <= lut_start,
            BvConst(0, 1),
            Ite(lo_index_s >= 0x100,
                BvConst(1, 1),
                BvConst(0, 1)
            )
        );
        instr.SetUpdate(m.state("lo_oflow"), lo_oflow);

        // Determine final value for LUT index
        auto lo_index = 
        Ite(Concat(BvConst(0, 16), lut_data) <= lut_start,
            BvConst(0, 32),
            Ite(lo_index_s >= 0x100,
                    BvConst(256, 32),
                    lo_index_s
            )
        );
        lo_index = Extract(lo_index, 8, 0);
        instr.SetUpdate(m.state("lo_index"), lo_index);

        // Determine final value for LUT fraction
        auto lo_fraction = 
        Ite(Concat(BvConst(0, 16), lut_data) <= lut_start,
            BvConst(0, 32),
            Ite(lo_index_s >= 0x100,
                    BvConst(0, 32),
                    (lo_index_idx & ((BvConst(1, 32) << Concat(BvConst(0, 24), lut_index_select)) - 1)) >> (Concat(BvConst(0, 24), lut_index_select) - BvConst(35, 32))
            )
        );
        lo_fraction = Extract(lo_fraction, 8, 0);
        instr.SetUpdate(m.state("lo_fraction"), lo_fraction);
    }

    { // Present LUT output
        auto instr = m.NewInstr("Present_LUT_Output");
        auto reg_group = "group0_";

        // ... lut access type needs to be 1 for write, table id needs to be 1 for lo and 0 for le
        auto access_type = SelectBit(m.state(NVDLA_SDP_S_LUT_ACCESS_TYPE), 0);
        auto table_id = SelectBit(m.state(NVDLA_SDP_S_LUT_TABLE_ID), 0);
        auto lut_bypass = SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_LUT_BYPASS)), 0);

        instr.SetDecode(access_type == 0x0 & table_id == 0x1 & lut_bypass == 0x0);

        // LUT parameters
        auto le_index = Extract(m.state("le_index"), 6, 0);
        auto lo_index = Extract(m.state("lo_index"), 8, 0);

        auto le_fraction = Extract(m.state("le_fraction"), 6, 0);
        auto lo_fraction = Extract(m.state("lo_fraction"), 8, 0);

        auto le_uflow = SelectBit(m.state("le_uflow"), 0);
        auto lo_uflow = SelectBit(m.state("lo_uflow"), 0);

        auto le_oflow = SelectBit(m.state("le_oflow"), 0);
        auto lo_oflow = SelectBit(m.state("lo_oflow"), 0);

        auto le_miss = le_uflow == 0x1 | le_oflow == 0x1;
        auto le_hit = le_miss == 0x0;
        auto lo_miss = lo_uflow == 0x1 | lo_oflow == 0x1;        
        auto lo_hit = lo_miss == 0x0;

        // LUT priorities
        auto uflow_priority = SelectBit(m.state(NVDLA_SDP_S_LUT_UFLOW_PRIORITY), 0);
        auto oflow_priority = SelectBit(m.state(NVDLA_SDP_S_LUT_OFLOW_PRIORITY), 0);
        auto hybrid_priority = SelectBit(m.state(NVDLA_SDP_S_LUT_HYBRID_PRIORITY), 0);

        auto index = 
        Ite(le_uflow == 0x1 & lo_uflow == 0x1,
            Ite(uflow_priority == 0x1, lo_index, Concat(BvConst(0, 2), le_index)),
            Ite(le_oflow == 0x1 & lo_oflow == 0x1,
                Ite(oflow_priority == 0x1, lo_index, Concat(BvConst(0, 2), le_index)),
                Ite(le_hit == 0x1 & lo_hit == 0x1,
                    Ite(hybrid_priority == 0x1, lo_index, Concat(BvConst(0, 2), le_index)),
                    Ite(le_miss == 0x1 & lo_miss == 0x1,
                        Ite(hybrid_priority == 0x1, lo_index, Concat(BvConst(0, 2), le_index)),
                        Ite(le_hit == 0x1,
                            Concat(BvConst(0, 2), le_index),
                            lo_index
                        )
                    )
                )
            )
        );
        instr.SetUpdate(m.state("index"), index);
    
        auto fraction = 
        Ite(le_uflow == 0x1 & lo_uflow == 0x1,
            Ite(uflow_priority == 0x1, lo_fraction, Concat(BvConst(0, 2), le_fraction)),
            Ite(le_oflow == 0x1 & lo_oflow == 0x1,
                Ite(oflow_priority == 0x1, lo_fraction, Concat(BvConst(0, 2), le_fraction)),
                Ite(le_hit == 0x1 & lo_hit == 0x1,
                    Ite(hybrid_priority == 0x1, lo_fraction, Concat(BvConst(0, 2), le_fraction)),
                    Ite(le_miss == 0x1 & lo_miss == 0x1,
                        Ite(hybrid_priority == 0x1, lo_fraction, Concat(BvConst(0, 2), le_fraction)),
                        Ite(le_hit == 0x1,
                            Concat(BvConst(0, 2), le_fraction),
                            lo_fraction
                        )
                    )
                )
            )
        );
        instr.SetUpdate(m.state("fraction"), fraction);

        auto uflow = 
        Ite(le_uflow == 0x1 & lo_uflow == 0x1,
            Ite(uflow_priority == 0x1, lo_uflow, le_uflow),
            Ite(le_oflow == 0x1 & lo_oflow == 0x1,
                BvConst(0, 1),
                Ite(le_hit == 0x1 & lo_hit == 0x1,
                    BvConst(0, 1),
                    Ite(le_miss == 0x1 & lo_miss == 0x1,
                        Ite(hybrid_priority == 0x1, lo_uflow, le_uflow),
                        BvConst(0, 1)
                    )
                )
            )
        );
        instr.SetUpdate(m.state("uflow"), uflow);

        auto oflow = 
        Ite(le_uflow == 0x1 & lo_uflow == 0x1,
            BvConst(0, 1),
            Ite(le_oflow == 0x1 & lo_oflow == 0x1,
                Ite(oflow_priority == 0x1, lo_oflow, le_oflow),
                Ite(le_hit == 0x1 & lo_hit == 0x1,
                    BvConst(0, 1),
                    Ite(le_miss == 0x1 & lo_miss == 0x1,
                        Ite(hybrid_priority == 0x1, lo_oflow, le_oflow),
                        BvConst(0, 1)
                    )
                )
            )
        );
        instr.SetUpdate(m.state("oflow"), oflow);

        auto id = 
        Ite(le_uflow == 0x1 & lo_uflow == 0x1,
            Ite(uflow_priority == 0x1, BvConst(1, 1), BvConst(0, 1)),
            Ite(le_oflow == 0x1 & lo_oflow == 0x1,
                Ite(oflow_priority == 0x1, BvConst(1, 1), BvConst(0, 1)),
                Ite(le_hit == 0x1 & lo_hit == 0x1,
                    Ite(hybrid_priority == 0x1, BvConst(1, 1), BvConst(0, 1)),
                    Ite(le_miss == 0x1 & lo_miss == 0x1,
                        Ite(hybrid_priority == 0x1, BvConst(1, 1), BvConst(0, 1)),
                        Ite(le_hit == 0x1,
                            BvConst(0, 1),
                            BvConst(1, 1)
                        )
                    )
                )
            )
        );
        instr.SetUpdate(m.state("id"), id);
    }

    { // Perform interpolation or extrapolation
        auto instr = m.NewInstr("Final_LUT_Processing");
        auto reg_group = "group0_";

        // ... lut access type needs to be 1 for write, table id needs to be 1 for lo and 0 for le
        auto access_type = SelectBit(m.state(NVDLA_SDP_S_LUT_ACCESS_TYPE), 0);
        auto table_id = SelectBit(m.state(NVDLA_SDP_S_LUT_TABLE_ID), 0);
        auto lut_bypass = SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_LUT_BYPASS)), 0);
        auto lut_data = m.state(NVDLA_SDP_S_LUT_ACCESS_DATA);
        auto lut_function = SelectBit(m.state(NVDLA_SDP_S_LUT_LE_FUNCTION), 0); 

        instr.SetDecode(access_type == 0x0 & table_id == 0x1 & lut_bypass == 0x0);

        // LUT parameters
        auto index = Extract(m.state("index"), 8, 0);
        auto fraction = Extract(m.state("fraction"), 8, 0);
        auto oflow = SelectBit(m.state("oflow"), 0);
        auto uflow = SelectBit(m.state("uflow"), 0);
        auto id = SelectBit(m.state("id"), 0);

        // LUT outputs       
        
        auto y0 = Ite(id == 0x1, Load(m.state("lo_tbl"), index), Load(m.state("le_tbl"), Extract(index, 6, 0)));
        auto y1 = 
        Ite(id == 0x1,
            Ite(oflow == 0x0 & uflow == 0x1, Load(m.state("lo_tbl"), index + 1), BvConst(0, 16)),
            Ite(oflow == 0x0 & uflow == 0x1, Load(m.state("le_tbl"), Extract(index, 6, 0) + 1), BvConst(0, 16))
        );    

        auto bias = 
        Ite((oflow == 0x0 | uflow == 0x0) & id == 0x0 & lut_function == 0x0 & oflow == 0 & m.state(NVDLA_SDP_S_LUT_LE_INDEX_OFFSET) > 0x0, 
            BvConst(1, 8) << m.state(NVDLA_SDP_S_LUT_LE_INDEX_OFFSET), 
            BvConst(0, 8)
        );
        
        auto scale = 
        Ite(oflow == 0x1 | uflow == 0x1,
            Ite(id == 0x0,
                Ite(oflow == 0x1, m.state(NVDLA_SDP_S_LUT_LE_SLOPE_OFLOW_SCALE), m.state(NVDLA_SDP_S_LUT_LE_SLOPE_UFLOW_SCALE)),
                Ite(oflow == 0x1, m.state(NVDLA_SDP_S_LUT_LO_SLOPE_OFLOW_SCALE), m.state(NVDLA_SDP_S_LUT_LO_SLOPE_UFLOW_SCALE))
            ),
            BvConst(0, 16)
        );

        auto shift = 
        Ite(oflow == 0x1 | uflow == 0x1,
            Ite(id == 0x0,
                Ite(oflow == 0x1, m.state(NVDLA_SDP_S_LUT_LE_SLOPE_OFLOW_SHIFT), m.state(NVDLA_SDP_S_LUT_LE_SLOPE_UFLOW_SHIFT)),
                Ite(oflow == 0x1, m.state(NVDLA_SDP_S_LUT_LO_SLOPE_OFLOW_SHIFT), m.state(NVDLA_SDP_S_LUT_LO_SLOPE_UFLOW_SHIFT))
            ),
            BvConst(0, 5)
        );

        auto offset = 
        Ite(oflow == 0x1 | uflow == 0x1,
            Ite(id == 0x0,
                Ite(oflow == 0x1, m.state(NVDLA_SDP_S_LUT_LE_END), m.state(NVDLA_SDP_S_LUT_LE_START)),
                Ite(oflow == 0x1, m.state(NVDLA_SDP_S_LUT_LO_END), m.state(NVDLA_SDP_S_LUT_LO_START))
            ),
            BvConst(0, 32)
        );

        // Interpolation and extrapolation
        auto o =
        Ite(oflow == 0x1 | uflow == 0x1,
            Concat(BvConst(0, 3), Concat(BvConst(0, 16), y0) + ((Concat(BvConst(0, 16), lut_data) - Concat(BvConst(0, 24), bias) - offset) * Concat(BvConst(0, 16), scale)) >> Concat(BvConst(0, 27), shift)),
            (((BvConst(1, 35) << 35) - Concat(BvConst(0, 26), fraction)) * Concat(BvConst(0, 19), y0)) + (Concat(BvConst(0, 26), fraction) * Concat(BvConst(0, 19), y1))
        );

        auto oMax = (BvConst(1, 32) << 31) - 1;
        auto oMin = -(BvConst(1, 32) << 31);
        auto out_data =
        Ite(oflow == 0x1 | uflow == 0x1,
            Ite(o > Concat(BvConst(0, 3), oMax), 
                Concat(BvConst(0, 3), oMax),
                Ite(o < Concat(BvConst(0, 3), oMin),
                    Concat(BvConst(0, 3), oMin),
                    o
                )            
            ),
            o >> 35
        );
    }

    // { // Perform conversion on inputs to the Y sub-unit
    //     // Determine the source of the operands
    //     auto flying_mode = m.state(GetVarName(reg_group, NVDLA_SDP_D_FLYING_MODE));
    //     auto data_source = m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_MUL_SRC));
    //     auto mul_shift = m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_MUL_SHIFT_VALUE));
    //     auto ew_mul_cvt_bypass = m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_MUL_CVT_BYPASS));
    //     auto ew_mul_cvt_offset = m.state(GetVarName(reg_group, NVDLA_SDP_D_DP_EW_MUL_CVT_OFFSET_VALUE));
    //     auto ew_mul_cvt_scale = m.state(GetVarName(reg_group, NVDLA_SDP_D_DP_EW_MUL_CVT_SCALE_VALUE));
    //     auto ew_mul_cvt_truncate = m.state(GetVarName(reg_group, NVDLA_SDP_D_DP_EW_MUL_CVT_TRUNCATE_VALUE));

    //     for (int i = 0; i < 16; i++) {
    //         // Setup operands
    //         auto input = Ite(SelectBit(flying_mode, 0) == 0x1, m.input(GetVarName("cacc_data_", (std::to_string(i)))), m.input(GetVarName("mrdma_data_", (std::to_string(i)))));
    //         auto pdp_output = m.state(GetVarName("pdp_output_", (std::to_string(i))));
    //         auto operand_alu = Ite(SelectBit(data_source, 0) == 0x1, m.input(GetVarName("dma_data_", (std::to_string(i)))), m.input(GetVarName("regs_data_", (std::to_string(i)))));

    //         // Converter
    //         operand_mul = Ite(SelectBit(ew_mul_cvt_bypass, 0), operand_alu, operand_alu - Extract(ew_mul_cvt_offset, 31, 0));
    //         operand_tct = Ite(SelectBit(ew_mul_cvt_bypass, 0), operand_mul, operand_mul * Extract(ew_mul_cvt_scale, 15, 0));
    //         operand = Ite(SelectBit(ew_mul_cvt_bypass, 0), operand_tct, operand_tct >> Extract(ew_mul_cvt_truncate, 5, 0));

    //         // Compute PReLU
    //         auto output = Ite((input > 0x0), input, (Concat(BvConst(0, 16), operand) * input) >> Concat(BvConst(0, 24), mul_shift));
    //         instr.SetUpdate(pdp_output, output);
    //     }
    // }

}


} // namespace ilang