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

// File: sdp_act.cc

#include <ilang/ilang++.h>
#include "uninterpreted_func/uninterpreted_func.h"
#include <sdp_config.h>

namespace ilang {

void DefineSDPInstrs(Ila& m) {
    // m.AddInit(m.state(NVDLA_CSC_S_STATUS_0) == BvConst(0, 2));
    // m.AddInit(m.state(NVDLA_CSC_S_STATUS_1) == BvConst(0, 2));

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
        // make a diagram for the datapath first (ala ECE 206), then make the instruction here by noting which registers are relevant for a single computation. 
        // note that when incorporating x1, x2, y, can treat as a "black box" (i.e., do not need to know full implementation details). Do not need to implement
        // DMA, but still provide as input. Finally, inspect SDP_CORE for relevant registers. one possibility for decode is have d_op_en true, then have everything
        // bypass except for the relu inside of x1. to understand how d_op_en is relevant, pick a register, go to SDP_CORE and look for correpsonding enable bit
        auto instr = m.NewInstr("Compute_ReLU");
        auto reg_group = "group0_";

        // Account for possibility that ReLU can be computed using either the X1 module or the X2 module
        auto x1_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS))) & 
                    (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_RELU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_BYPASS)));
        auto x2_ok_g0 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS))) & 
                    (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_RELU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_BYPASS)));
        auto group0_regs = (x1_ok_g0 | x2_ok_g0) & producer == BvConst(0, 1) & !group0_unset;

        instr.SetDecode(group0_regs);

        // Determine the source of the input
        auto flying_mode = m.state(GetVarName(reg_group, NVDLA_SDP_D_FLYING_MODE));
        auto input = Ite(flying_mode, m.input(GetVarName("cacc_data")), m.input(GetVarName("sdp_mrdma_data")));

        // Compute ReLU
        auto output = Ite((input > 0x0), input, 0x0);
 
        // Need outputs similar to the inputs?
        // instr.SetUpdate(m.state(GetVarName(registerGroup, NVDLA_CSC_D_OP_ENABLE)), Extract(m.input("csb_data"), 0, 0));
    }

    { // ReLU computation - Group 1
        // make a diagram for the datapath first (ala ECE 206), then make the instruction here by noting which registers are relevant for a single computation. 
        // note that when incorporating x1, x2, y, can treat as a "black box" (i.e., do not need to know full implementation details). Do not need to implement
        // DMA, but still provide as input. Finally, inspect SDP_CORE for relevant registers. one possibility for decode is have d_op_en true, then have everything
        // bypass except for the relu inside of x1. to understand how d_op_en is relevant, pick a register, go to SDP_CORE and look for correpsonding enable bit
        auto instr = m.NewInstr("Compute_ReLU");
        auto reg_group = "group1_";

        // Account for possibility that ReLU can be computed using either the X1 module or the X2 module
        auto x1_ok_g1 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS))) & 
                    (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_RELU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_ALU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_MUL_BYPASS)))
        auto x2_ok_g1 = (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BS_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_BYPASS))) & 
                    (!m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_RELU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_ALU_BYPASS)) & m.state(GetVarName(reg_group, NVDLA_SDP_D_BN_MUL_BYPASS)))
        auto group1_regs = (x1_ok_g1 | x2_ok_g1) & producer == BvConst(1, 1) & !group1_unset

        instr.SetDecode(group1_regs);

        // Determine the source of the input
        auto flying_mode = m.state(GetVarName(reg_group, NVDLA_SDP_D_FLYING_MODE));
        auto input = Ite(flying_mode, m.input(GetVarName("cacc_data")), m.input(GetVarName("sdp_mrdma_data")));

        // Compute ReLU
        auto output = Ite((input > 0x0), input, 0x0);
 
        // Need outputs similar to the inputs?
        // instr.SetUpdate(m.state(GetVarName(registerGroup, NVDLA_CSC_D_OP_ENABLE)), Extract(m.input("csb_data"), 0, 0));
    }



}


} // namespace ilang