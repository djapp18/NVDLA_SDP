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

// need an intlof2 and intlog2 fraction as separate funcitons
// void IntLog2() {

// }

// void IntLog2Frac() {

// }

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
        auto instr = m.NewInstr("Compute_ReLU");
        auto reg_group = "group0_";

        // Account for possibility that ReLU can be computed using either the X1 module or the X2 module
        auto x1_ok_g0 = (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x1) & 
                    (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_RELU_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_BYPASS)), 0) == 0x1);
        auto x2_ok_g0 = (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x1) & 
                    (SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_RELU_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_BYPASS)), 0) == 0x1);
        auto group0_regs = (x1_ok_g0 | x2_ok_g0) & consumer == BvConst(0, 1) & group0_unset;

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

            auto test = BvConst(0x7, 6);
            cout << test;
            cout << "\n";
            auto test2 = Extract(test, 5, 0);
            cout << test2;
            cout << "\n";

            // // Compute maximum
            // auto operand_shifted = operand << Extract(alu_shift, 15, 0);
            // auto output = Ite((input > operand_shifted), input, operand_shifted);
            // instr.SetUpdate(pdp_output, output);
        }
    }

    // { // ALU min computation - Group 0
    //     auto instr = m.NewInstr("Compute_Min");
    //     auto reg_group = "group0_";

    //     // Account for possibility that ALU min can be computed using either the X1 module or the X2 module
    //     auto x1_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS))) & 
    //                 (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_ALGO)) == 0x1 & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_RELU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_BYPASS)));
    //     auto x2_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS))) & 
    //                 (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_ALGO)) == 0x1 & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_RELU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_BYPASS)));
    //     auto group0_regs = (x1_ok_g0 | x2_ok_g0) & producer == BvConst(0, 1) & !group0_unset;

    //     instr.SetDecode(group0_regs);

    //     // Determine the source of the operands
    //     auto flying_mode = m.state(GetVarName(reg_group, NVDLA_SDP_D_FLYING_MODE));
    //     auto data_source = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_SRC));
    //     auto alu_shift = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_SHIFT_VALUE));

    //     for (int i = 0; i < 16; i++) {
    //         // Setup operands
    //         auto input = Ite(flying_mode, m.input(GetVarName("cacc_data_", (std::to_string(i)))), m.input(GetVarName("mrdma_data_", (std::to_string(i)))));
    //         auto pdp_output = m.state(GetVarName("pdp_output_", (std::to_string(i))));
    //         auto operand = Ite(data_source, m.input(GetVarName("dma_data_", (std::to_string(i)))), m.input(GetVarName("regs_data_", (std::to_string(i)))));

    //         // Compute minimum
    //         auto operand_shifted = operand << alu_shift;
    //         auto output = Ite((input < operand_shifted), input, operand_shifted);
    //         instr.SetUpdate(pdp_output, output);
    //     }
    // }

    // { // ALU add computation - Group 0
    //     auto instr = m.NewInstr("Compute_Add");
    //     auto reg_group = "group0_";

    //     // Account for possibility that ALU add can be computed using either the X1 module or the X2 module
    //     auto x1_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS))) & 
    //                 (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_ALGO)) == 0x2 & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_RELU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_BYPASS)));
    //     auto x2_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS))) & 
    //                 (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_ALGO)) == 0x2 & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_RELU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_BYPASS)));
    //     auto group0_regs = (x1_ok_g0 | x2_ok_g0) & producer == BvConst(0, 1) & !group0_unset;

    //     instr.SetDecode(group0_regs);

    //     // Determine the source of the operands
    //     auto flying_mode = m.state(GetVarName(reg_group, NVDLA_SDP_D_FLYING_MODE));
    //     auto data_source = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_SRC));
    //     auto alu_shift = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_SHIFT_VALUE));

    //     for (int i = 0; i < 16; i++) {
    //         // Setup operands
    //         auto input = Ite(flying_mode, m.input(GetVarName("cacc_data_", (std::to_string(i)))), m.input(GetVarName("mrdma_data_", (std::to_string(i)))));
    //         auto pdp_output = m.state(GetVarName("pdp_output_", (std::to_string(i))));
    //         auto operand = Ite(data_source, m.input(GetVarName("dma_data_", (std::to_string(i)))), m.input(GetVarName("regs_data_", (std::to_string(i)))));

    //         // Compute add
    //         auto operand_shifted = operand << alu_shift;
    //         auto output = input + operand_shifted;
    //         instr.SetUpdate(pdp_output, output);
    //     }
    // }

    // { // MUL multiply computation - Group 0
    //     auto instr = m.NewInstr("Compute_Multiply");
    //     auto reg_group = "group0_";

    //     // Account for possibility that MUL multiply can be computed using either the X1 module or the X2 module
    //     auto x1_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS))) & 
    //                 (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_BYPASS)) & !m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_PRELU)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_RELU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_BYPASS)));
    //     auto x2_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS))) & 
    //                 (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_BYPASS)) & !m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_PRELU)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_RELU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_BYPASS)));
    //     auto group0_regs = (x1_ok_g0 | x2_ok_g0) & producer == BvConst(0, 1) & !group0_unset;

    //     instr.SetDecode(group0_regs);

    //     // Determine the source of the operands
    //     auto flying_mode = m.state(GetVarName(reg_group, NVDLA_SDP_D_FLYING_MODE));
    //     auto data_source = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_SRC));
    //     auto mul_shift = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_SHIFT_VALUE));

    //     for (int i = 0; i < 16; i++) {
    //         // Setup operands
    //         auto input = Ite(flying_mode, m.input(GetVarName("cacc_data_", (std::to_string(i)))), m.input(GetVarName("mrdma_data_", (std::to_string(i)))));
    //         auto pdp_output = m.state(GetVarName("pdp_output_", (std::to_string(i))));
    //         auto operand = Ite(data_source, m.input(GetVarName("dma_data_", (std::to_string(i)))), m.input(GetVarName("regs_data_", (std::to_string(i)))));

    //         // Compute multiply
    //         auto output = input * operand;
    //         auto output_shifted = output >> mul_shift;
    //         instr.SetUpdate(pdp_output, output_shifted);
    //     }
    // }

    // { // MUL PReLU computation - Group 0
    //     auto instr = m.NewInstr("Compute_PReLU");
    //     auto reg_group = "group0_";

    //     // Account for possibility that MUL PReLU can be computed using either the X1 module or the X2 module
    //     auto x1_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS))) & 
    //                 (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_PRELU)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_RELU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_BYPASS)));
    //     auto x2_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS))) & 
    //                 (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_PRELU)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_RELU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_BYPASS)));
    //     auto group0_regs = (x1_ok_g0 | x2_ok_g0) & producer == BvConst(0, 1) & !group0_unset;

    //     instr.SetDecode(group0_regs);

    //     // Determine the source of the operands
    //     auto flying_mode = m.state(GetVarName(reg_group, NVDLA_SDP_D_FLYING_MODE));
    //     auto data_source = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_SRC));
    //     auto mul_shift = m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_SHIFT_VALUE));

    //     for (int i = 0; i < 16; i++) {
    //         // Setup operands
    //         auto input = Ite(flying_mode, m.input(GetVarName("cacc_data_", (std::to_string(i)))), m.input(GetVarName("mrdma_data_", (std::to_string(i)))));
    //         auto pdp_output = m.state(GetVarName("pdp_output_", (std::to_string(i))));
    //         auto operand = Ite(data_source, m.input(GetVarName("dma_data_", (std::to_string(i)))), m.input(GetVarName("regs_data_", (std::to_string(i)))));

    //         // Compute PReLU
    //         auto output = Ite((input > 0x0), input, (operand * input) >> mul_shift);
    //         instr.SetUpdate(pdp_output, output);
    //     }
    // }

    // { // Write to LUT Table LE
    //     auto instr = m.NewInstr("Write_LUT_LE");

    //     // ... lut access type needs to be 1 for write, table id needs to be 1 for lo and 0 for le
    //     auto access_type = m.state(NVDLA_SDP_S_LUT_ACCESS_TYPE);
    //     auto table_id = m.state(NVDLA_SDP_S_LUT_TABLE_ID);

    //     instr.SetDecode(access_type & !table_id);

    //     // Set the relevant table
    //     auto lut = m.state("le_tbl");
    //     auto lut_addr = m.state(NVDLA_SDP_S_LUT_ADDR);
    //     auto lut_data = m.state(NVDLA_SDP_S_LUT_ACCESS_DATA);

    //     for (int i = 0; i < 65; i++) {
    //         // Write to table
    //         auto new_lut = ilang::Store(lut, BvConst(i, 7), lut_data);
    //         instr.SetUpdate(lut, new_lut);
    //     }
    // }

    // { // Write to LUT Table LO
    //     auto instr = m.NewInstr("Write_LUT_LO");

    //     // ... lut access type needs to be 1 for write, table id needs to be 1 for lo and 0 for le
    //     auto access_type = m.state(NVDLA_SDP_S_LUT_ACCESS_TYPE);
    //     auto table_id = m.state(NVDLA_SDP_S_LUT_TABLE_ID);

    //     instr.SetDecode(access_type & table_id);

    //     // Set the relevant table
    //     auto lut = m.state("lo_tbl");
    //     auto lut_addr = m.state(NVDLA_SDP_S_LUT_ADDR);
    //     auto lut_data = m.state(NVDLA_SDP_S_LUT_ACCESS_DATA);

    //     for (int i = 0; i < 257; i++) {
    //         // Write to table
    //         auto new_lut = Store(lut, BvConst(i, 9), lut_data);
    //         instr.SetUpdate(lut, new_lut);
    //     }
    // }

    // { // Read index from LUT Table LE
    //     auto instr = m.NewInstr("Read_LUT_LE");

    //     // ... lut access type needs to be 1 for write, table id needs to be 1 for lo and 0 for le
    //     auto access_type = m.state(NVDLA_SDP_S_LUT_ACCESS_TYPE);
    //     auto table_id = m.state(NVDLA_SDP_S_LUT_TABLE_ID);
    //     auto lut_bypass = m.state(NVDLA_SDP_D_EW_LUT_BYPASS);

    //     instr.SetDecode(!access_type & !table_id & !lut_bypass);

    //     // Set the relevant table
    //     auto lut = m.state("le_tbl");
    //     auto lut_data = m.state(NVDLA_SDP_S_LUT_ACCESS_DATA);
    //     auto lut_start = m.state(NVDLA_SDP_S_LUT_LE_START);
    //     auto lut_index_offset = m.state(NVDLA_SDP_S_LUT_LE_INDEX_OFFSET);
    //     auto lut_index_select = m.state(NVDLA_SDP_S_LUT_LE_INDEX_SELECT);
        
    //     // Determine intermediate values for LUT index
    //     auto le_index_idx =
    //     Ite(m.state(NVDLA_SDP_S_LUT_LE_FUNCTION) == 0,
    //         IntLog2(lut_data - lut_start),
    //         lut_data - lut_start
    //     );

    //     auto le_index_s =
    //     Ite(m.state(NVDLA_SDP_S_LUT_LE_FUNCTION) == 0,
    //         le_index_idx - lut_index_offset,
    //         le_index_idx >> lut_index_select
    //     );

    //     // Generate out-of-bounds flags
        // auto le_uflow = 
        // Ite(m.state(NVDLA_SDP_S_LUT_LE_FUNCTION) == 0,
        //     Ite(lut_data <= lut_start,
        //         0x1,
        //         Ite(le_index_idx < lut_index_offset,
        //             0x1,
        //             0x0
        //         )
        //     ),

        //     Ite(lut_data <= lut_start,
        //         0x1,
        //         0x0
        //     )
        // );
    //     instr.SetUpdate("le_uflow", le_uflow);

    //     auto le_oflow = 
    //     Ite(m.state(NVDLA_SDP_S_LUT_LE_FUNCTION) == 0,
    //         Ite(lut_data <= lut_start,
    //             0x0,
    //             Ite(le_index_idx < lut_index_offset,
    //                 0x0,
    //                 Ite(le_index_s >= 64,
    //                     0x1,
    //                     0x0
    //                 )
    //             )
    //         ),

    //         Ite(lut_data <= lut_start,
    //             0x0,
    //             Ite(le_index_s >= 64,
    //                 0x1,
    //                 0x0
    //             )
    //         )
    //     );
    //     instr.SetUpdate("le_oflow", le_oflow);

    //     // Determine final value for LUT index
    //     auto le_index = 
    //     Ite(m.state(NVDLA_SDP_S_LUT_LE_FUNCTION) == 0,
    //         Ite(lut_data <= lut_start,
    //             0x0,
    //             Ite(le_index_idx < lut_index_offset,
    //                 0x0,
    //                 Ite(le_index_s >= 64,
    //                     0x40,
    //                     le_index_s
    //                 )
    //             )
    //         ),

    //         Ite(lut_data <= lut_start,
    //             0x0,
    //             Ite(le_index_s >= 64,
    //                     0x40,
    //                     le_index_s
    //             )
    //         )
    //     );
    //     instr.SetUpdate("le_index", le_index);

    //     // Determine final value for LUT fraction
    //     auto le_fraction = 
    //     Ite(m.state(NVDLA_SDP_S_LUT_LE_FUNCTION) == 0,
    //         Ite(lut_data <= lut_start,
    //             0x0,
    //             Ite(le_index_idx < lut_index_offset,
    //                 0x0,
    //                 Ite(le_index_s >= 64,
    //                     0x0,
    //                     IntLog2Frac(lut_data - lut_start) << (35 - le_index_idx)
    //                 )
    //             )
    //         ),

    //         Ite(lut_data <= lut_start,
    //             0x0,
    //             Ite(le_index_s >= 64,
    //                     0x0,
    //                     (le_index_idx & ((0x1 << lut_index_select) - 1)) >> (lut_index_select - 35)
    //             )
    //         )
    //     );
    //     instr.SetUpdate("le_fraction", le_fraction);

    // }

    // { // Read index from LUT Table LO
    //     auto instr = m.NewInstr("Read_LUT_LO");

    //     // ... lut access type needs to be 1 for write, table id needs to be 1 for lo and 0 for le
    //     auto access_type = m.state(NVDLA_SDP_S_LUT_ACCESS_TYPE);
    //     auto table_id = m.state(NVDLA_SDP_S_LUT_TABLE_ID);
    //     auto lut_bypass = m.state(NVDLA_SDP_D_EW_LUT_BYPASS);

    //     instr.SetDecode(!access_type & table_id & !lut_bypass);

    //     // Set the relevant table
    //     auto lut = m.state("lo_tbl");
    //     auto lut_data = m.state(NVDLA_SDP_S_LUT_ACCESS_DATA);
    //     auto lut_start = m.state(NVDLA_SDP_S_LUT_LO_START);
    //     auto lut_index_select = m.state(NVDLA_SDP_S_LUT_LO_INDEX_SELECT);
        
    //     // Determine intermediate values for LUT index
    //     auto lo_index_idx = lut_data - lut_start;

    //     auto lo_index_s = le_index_idx >> lut_index_select;

    //     // Generate out-of-bounds flags
    //     auto lo_uflow = 
    //     Ite(lut_data <= lut_start,
    //         0x1,
    //         0x0
    //     );
    //     instr.SetUpdate("lo_uflow", lo_uflow);

    //     auto lo_oflow = 
    //     Ite(lut_data <= lut_start,
    //         0x0,
    //         Ite(lo_index_s >= 256,
    //             0x1,
    //             0x0
    //         )
    //     );
    //     instr.SetUpdate("lo_oflow", lo_oflow);

    //     // Determine final value for LUT index
    //     auto lo_index = 
    //     Ite(lut_data <= lut_start,
    //         0x0,
    //         Ite(lo_index_s >= 256,
    //                 0x100,
    //                 lo_index_s
    //         )
    //     );
    //     instr.SetUpdate("lo_index", lo_index);

    //     // Determine final value for LUT fraction
    //     auto lo_fraction = 
    //     Ite(lut_data <= lut_start,
    //         0x0,
    //         Ite(lo_index_s >= 256,
    //                 0x0,
    //                 (lo_index_idx & ((0x1 << lut_index_select) - 1)) >> (lut_index_select - 35)
    //         )
    //     );
    //     instr.SetUpdate("lo_fraction", lo_fraction);
    // }

    // { // Present final LUT output
    //     auto instr = m.NewInstr("Present_LUT_Output");

    //     // ... lut access type needs to be 1 for write, table id needs to be 1 for lo and 0 for le
    //     auto access_type = m.state(NVDLA_SDP_S_LUT_ACCESS_TYPE);
    //     auto table_id = m.state(NVDLA_SDP_S_LUT_TABLE_ID);
    //     auto lut_bypass = m.state(NVDLA_SDP_D_EW_LUT_BYPASS);

    //     instr.SetDecode(!access_type & table_id & !lut_bypass);

    //     auto le_index = m.state("le_index");
    //     auto lo_index = m.state("lo_index");

    //     auto le_uflow = m.state("le_uflow");
    //     auto lo_uflow = m.state("lo_uflow");

    //     auto le_oflow = m.state("le_oflow");
    //     auto lo_oflow = m.state("lo_oflow");

    //     auto le_function = m.state("le_function");
    //     auto lo_function = m.state("lo_function");

    //     auto le_miss = le_uflow | le_oflow;
    //     auto le_hit = !le_miss;
    //     auto lo_miss = lo_uflow | lo_oflow;        
    //     auto lo_hit = !lo_miss;

    //     auto index = 
    //     Ite(le_uflow & lo_uflow,
    //         Ite(m.state(NVDLA_SDP_S_LUT_UFLOW_PRIORITY), lo_index, le_index),
    //         Ite(le_oflow & lo_oflow,
    //             Ite(m.state(NVDLA_SDP_S_LUT_OFLOW_PRIORITY), lo_index, le_index),
    //             Ite(le_hit & lo_hit,
    //                 Ite(m.state(NVDLA_SDP_S_LUT_HYBRID_PRIORITY), lo_index, le_index),
    //                 Ite(le_miss & lo_miss,
    //                     Ite(m.state(NVDLA_SDP_S_LUT_HYBRID_PRIORITY), lo_index, le_index),
    //                     Ite(le_hit,
    //                         le_index,
    //                         lo_index
    //                     )
    //                 )
    //             )
    //         )
    //     );
    //     instr.SetUpdate("index", index);
    
    //     auto fraction = 
    //     Ite(le_uflow & lo_uflow,
    //         Ite(m.state(NVDLA_SDP_S_LUT_UFLOW_PRIORITY), lo_fraction, le_fraction),
    //         Ite(le_oflow & lo_oflow,
    //             Ite(m.state(NVDLA_SDP_S_LUT_OFLOW_PRIORITY), lo_fraction, le_fraction),
    //             Ite(le_hit & lo_hit,
    //                 Ite(m.state(NVDLA_SDP_S_LUT_HYBRID_PRIORITY), lo_fraction, le_fraction),
    //                 Ite(le_miss & lo_miss,
    //                     Ite(m.state(NVDLA_SDP_S_LUT_HYBRID_PRIORITY), lo_fraction, le_fraction),
    //                     Ite(le_hit,
    //                         le_fraction,
    //                         lo_fraction
    //                     )
    //                 )
    //             )
    //         )
    //     );
    //     instr.SetUpdate("fraction", fraction);

    //     auto uflow = 
    //     Ite(le_uflow & lo_uflow,
    //         Ite(m.state(NVDLA_SDP_S_LUT_UFLOW_PRIORITY), lo_uflow, le_uflow),
    //         Ite(le_oflow & lo_oflow,
    //             0x0,
    //             Ite(le_hit & lo_hit,
    //                 0x0,
    //                 Ite(le_miss & lo_miss,
    //                     Ite(m.state(NVDLA_SDP_S_LUT_HYBRID_PRIORITY), lo_uflow, le_uflow),
    //                     0x0
    //                 )
    //             )
    //         )
    //     );
    //     instr.SetUpdate("uflow", uflow);

    //     auto oflow = 
    //     Ite(le_uflow & lo_uflow,
    //         0x0,
    //         Ite(le_oflow & lo_oflow,
    //             Ite(m.state(NVDLA_SDP_S_LUT_OFLOW_PRIORITY), lo_oflow, le_oflow),
    //             Ite(le_hit & lo_hit,
    //                 0x0,
    //                 Ite(le_miss & lo_miss,
    //                     Ite(m.state(NVDLA_SDP_S_LUT_HYBRID_PRIORITY), lo_oflow, le_oflow),
    //                     0x0
    //                 )
    //             )
    //         )
    //     );
    //     instr.SetUpdate("oflow", oflow);

    //     auto id = 
    //     Ite(le_uflow & lo_uflow,
    //         Ite(m.state(NVDLA_SDP_S_LUT_UFLOW_PRIORITY), 0x1, 0x0),
    //         Ite(le_oflow & lo_oflow,
    //             Ite(m.state(NVDLA_SDP_S_LUT_OFLOW_PRIORITY), 0x1, 0x0),
    //             Ite(le_hit & lo_hit,
    //                 Ite(m.state(NVDLA_SDP_S_LUT_HYBRID_PRIORITY), 0x1, 0x0),
    //                 Ite(le_miss & lo_miss,
    //                     Ite(m.state(NVDLA_SDP_S_LUT_HYBRID_PRIORITY), 0x1, 0x0),
    //                     Ite(le_hit,
    //                         0x0,
    //                         0x1
    //                     )
    //                 )
    //             )
    //         )
    //     );
    //     instr.SetUpdate("id", id);
    // }

    // { // Perform interpolation or extrapolation
    //     auto instr = m.NewInstr("Final_LUT_Processing");

    //     // ... lut access type needs to be 1 for write, table id needs to be 1 for lo and 0 for le
    //     auto access_type = m.state(NVDLA_SDP_S_LUT_ACCESS_TYPE);
    //     auto table_id = m.state(NVDLA_SDP_S_LUT_TABLE_ID);
    //     auto lut_bypass = m.state(NVDLA_SDP_D_EW_LUT_BYPASS);
    //     auto lut_data = m.state(NVDLA_SDP_S_LUT_ACCESS_DATA);

    //     instr.SetDecode(!access_type & table_id & !lut_bypass);

    //     auto the_table = Ite(m.state("id"), m.state("lo_table", m.state("le_table")));
    //     auto y0 = Load(the_table, m.state("index"));
    //     auto y1 = Ite(!m.state("oflow") && !m.state("uflow"), Load(the_table, m.state("index") + 1), 0x0);
    //     auto fraction = m.state("fraction");

    //     auto bias = Ite((m.state("oflow") || m.state("uflow")) && the_table == 0x0 && m.state(NVDLA_SDP_S_LUT_LE_FUNCTION) == 0x0 && !m.state("oflow") && m.state(NVDLA_SDP_S_LUT_LE_INDEX_OFFSET) > 0x0, 0x1 << m.state(NVDLA_SDP_S_LUT_LE_INDEX_OFFSET), 0x0);
        
    //     auto scale = 
    //     Ite(m.state("oflow") || m.state("uflow"),
    //         Ite(m.state("id") == 0x0,
    //             Ite(m.state("oflow"), m.state(NVDLA_SDP_S_LUT_LE_SLOPE_OFLOW_SCALE), m.state(NVDLA_SDP_S_LUT_LE_SLOPE_UFLOW_SCALE)),
    //             Ite(m.state("oflow"), m.state(NVDLA_SDP_S_LUT_LO_SLOPE_OFLOW_SCALE), m.state(NVDLA_SDP_S_LUT_LO_SLOPE_UFLOW_SCALE)),
    //         ),
    //         0x0
    //     );

    //     auto shift = 
    //     Ite(m.state("oflow") || m.state("uflow"),
    //         Ite(m.state("id") == 0x0,
    //             Ite(m.state("oflow"), m.state(NVDLA_SDP_S_LUT_LE_SLOPE_OFLOW_SHIFT), m.state(NVDLA_SDP_S_LUT_LE_SLOPE_UFLOW_SHIFT)),
    //             Ite(m.state("oflow"), m.state(NVDLA_SDP_S_LUT_LO_SLOPE_OFLOW_SHIFT), m.state(NVDLA_SDP_S_LUT_LO_SLOPE_UFLOW_SHIFT)),
    //         ),
    //         0x0
    //     );

    //     auto offset = 
    //     Ite(m.state("oflow") || m.state("uflow"),
    //         Ite(m.state("id") == 0x0,
    //             Ite(m.state("oflow"), m.state(NVDLA_SDP_S_LUT_LE_END), m.state(NVDLA_SDP_S_LUT_LE_START)),
    //             Ite(m.state("oflow"), m.state(NVDLA_SDP_S_LUT_LO_END), m.state(NVDLA_SDP_S_LUT_LO_START)),
    //         ),
    //         0x0
    //     );

    //     // Interpolation and extrapolation
    //     auto o =
    //     Ite(m.state("oflow") || m.state("uflow"),
    //         y0 + ((lut_data - bias - offset) * scale) >> shift,
    //         (((1 << 35) - fraction) * y0) + (fraction * y1)
    //     );

    //     auto oMax = (1 << 31) - 1;
    //     auto oMin = -(1 << 31);
    //     auto out_data =
    //     Ite(m.state("oflow") || m.state("uflow"),
    //         Ite(o > oMax, 
    //             oMax,
    //             Ite(o < oMin,
    //                 oMin,
    //                 o
    //             )            
    //         ),
    //         o >> 35
    //     );
    }


} // namespace ilang