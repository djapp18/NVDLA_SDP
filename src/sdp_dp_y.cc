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

// File: sdp_dp_y.cc

#include <ilang/ilang++.h>
#include "uninterpreted_func/uninterpreted_func.h"
#include <sdp_config.h>

namespace ilang {

// TODO:
// - zero out remaining outputs (i.e., throughput difference with X; here it is only 4)

void DefineSDPInstrsDP_Y(Ila& m) {
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

    { // ALU max computation - Group 0
        auto instr = m.NewInstr("Compute_Max");
        auto reg_group = "group0_";

        // Account for possibility that ALU max can be computed using either the X1 module or the X2 module
        auto x1_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS))) & 
                    (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_ALGO)) == 0x0 & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_RELU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_BYPASS)));
        auto x2_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS))) & 
                    (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_ALGO)) == 0x0 & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_RELU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_BYPASS)));
        auto group0_regs = (x1_ok_g0 | x2_ok_g0) & producer == BvConst(0, 1) & !group0_unset;

        instr.SetDecode(group0_regs);

        // Determine the source of the operands
        auto flying_mode = m.state(GetVarName(reg_group, NVDLA_SDP_D_FLYING_MODE));
        auto data_source = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_SRC));
        auto alu_shift = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_SHIFT_VALUE));

        for (int i = 0; i < 16; i++) {
            // Setup operands
            auto input = Ite(flying_mode, m.input("cacc_data" + "_" + (std::to_string(i))), m.input("sdp_mrdma_data" + "_" + (std::to_string(i))));
            auto pdp_output = m.state("pdp_output" + "_" + (std::to_string(i));
            auto operand = Ite(data_source, m.input("dma_data" + "_" + (std::to_string(i))), m.input("regs_data" + "_" + (std::to_string(i))));

            // Compute maximum
            auto operand_shifted = operand << alu_shift;
            auto output = Ite((input > operand_shifted), input, operand_shifted);
            instr.SetUpdate(pdp_output, output);
        }
    }

    { // ALU min computation - Group 0
        auto instr = m.NewInstr("Compute_Min");
        auto reg_group = "group0_";

        // Account for possibility that ALU min can be computed using either the X1 module or the X2 module
        auto x1_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS))) & 
                    (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_ALGO)) == 0x1 & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_RELU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_BYPASS)));
        auto x2_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS))) & 
                    (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_ALGO)) == 0x1 & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_RELU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_BYPASS)));
        auto group0_regs = (x1_ok_g0 | x2_ok_g0) & producer == BvConst(0, 1) & !group0_unset;

        instr.SetDecode(group0_regs);

        // Determine the source of the operands
        auto flying_mode = m.state(GetVarName(reg_group, NVDLA_SDP_D_FLYING_MODE));
        auto data_source = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_SRC));
        auto alu_shift = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_SHIFT_VALUE));

        for (int i = 0; i < 16; i++) {
            // Setup operands
            auto input = Ite(flying_mode, m.input("cacc_data" + "_" + (std::to_string(i))), m.input("sdp_mrdma_data" + "_" + (std::to_string(i))));
            auto pdp_output = m.state("pdp_output" + "_" + (std::to_string(i));
            auto operand = Ite(data_source, m.input("dma_data" + "_" + (std::to_string(i))), m.input("regs_data" + "_" + (std::to_string(i))));

            // Compute minimum
            auto operand_shifted = operand << alu_shift;
            auto output = Ite((input < operand_shifted), input, operand_shifted);
            instr.SetUpdate(pdp_output, output);
        }
    }

    { // ALU add computation - Group 0
        auto instr = m.NewInstr("Compute_Add");
        auto reg_group = "group0_";

        // Account for possibility that ALU add can be computed using either the X1 module or the X2 module
        auto x1_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS))) & 
                    (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_ALGO)) == 0x2 & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_RELU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_BYPASS)));
        auto x2_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS))) & 
                    (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_ALGO)) == 0x2 & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_RELU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_BYPASS)));
        auto group0_regs = (x1_ok_g0 | x2_ok_g0) & producer == BvConst(0, 1) & !group0_unset;

        instr.SetDecode(group0_regs);

        // Determine the source of the operands
        auto flying_mode = m.state(GetVarName(reg_group, NVDLA_SDP_D_FLYING_MODE));
        auto data_source = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_SRC));
        auto alu_shift = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_SHIFT_VALUE));

        for (int i = 0; i < 16; i++) {
            // Setup operands
            auto input = Ite(flying_mode, m.input("cacc_data" + "_" + (std::to_string(i))), m.input("sdp_mrdma_data" + "_" + (std::to_string(i))));
            auto pdp_output = m.state("pdp_output" + "_" + (std::to_string(i));
            auto operand = Ite(data_source, m.input("dma_data" + "_" + (std::to_string(i))), m.input("regs_data" + "_" + (std::to_string(i))));

            // Compute add
            auto operand_shifted = operand << alu_shift;
            auto output = input + operand_shifted;
            instr.SetUpdate(pdp_output, output);
        }
    }

    { // MUL multiply computation - Group 0
        auto instr = m.NewInstr("Compute_Multiply");
        auto reg_group = "group0_";

        // Account for possibility that MUL multiply can be computed using either the X1 module or the X2 module
        auto x1_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS))) & 
                    (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_BYPASS)) & !m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_PRELU)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_RELU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_BYPASS)));
        auto x2_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS))) & 
                    (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_BYPASS)) & !m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_PRELU)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_RELU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_BYPASS)));
        auto group0_regs = (x1_ok_g0 | x2_ok_g0) & producer == BvConst(0, 1) & !group0_unset;

        instr.SetDecode(group0_regs);

        // Determine the source of the operands
        auto flying_mode = m.state(GetVarName(reg_group, NVDLA_SDP_D_FLYING_MODE));
        auto data_source = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_SRC));
        auto mul_shift = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_SHIFT_VALUE));

        for (int i = 0; i < 16; i++) {
            // Setup operands
            auto input = Ite(flying_mode, m.input("cacc_data" + "_" + (std::to_string(i))), m.input("sdp_mrdma_data" + "_" + (std::to_string(i))));
            auto pdp_output = m.state("pdp_output" + "_" + (std::to_string(i));
            auto operand = Ite(data_source, m.input("dma_data" + "_" + (std::to_string(i))), m.input("regs_data" + "_" + (std::to_string(i))));

            // Compute multiply
            auto output = input * operand;
            auto output_shifted = output >> mul_shift;
            instr.SetUpdate(pdp_output, output_shifted);
        }
    }

    { // MUL PReLU computation - Group 0
        auto instr = m.NewInstr("Compute_PReLU");
        auto reg_group = "group0_";

        // Account for possibility that MUL PReLU can be computed using either the X1 module or the X2 module
        auto x1_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS))) & 
                    (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_PRELU)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_RELU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_BYPASS)));
        auto x2_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS))) & 
                    (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_PRELU)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_RELU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_BYPASS)));
        auto group0_regs = (x1_ok_g0 | x2_ok_g0) & producer == BvConst(0, 1) & !group0_unset;

        instr.SetDecode(group0_regs);

        // Determine the source of the operands
        auto flying_mode = m.state(GetVarName(reg_group, NVDLA_SDP_D_FLYING_MODE));
        auto data_source = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_SRC));
        auto mul_shift = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_SHIFT_VALUE));

        for (int i = 0; i < 16; i++) {
            // Setup operands
            auto input = Ite(flying_mode, m.input("cacc_data" + "_" + (std::to_string(i))), m.input("sdp_mrdma_data" + "_" + (std::to_string(i))));
            auto pdp_output = m.state("pdp_output" + "_" + (std::to_string(i));
            auto operand = Ite(data_source, m.input("dma_data" + "_" + (std::to_string(i))), m.input("regs_data" + "_" + (std::to_string(i))));

            // Compute PReLU
            auto output = Ite((input > 0x0), input, (operand * input) >> mul_shift)
            instr.SetUpdate(pdp_output, output);
        }
    }

}


} // namespace ilang