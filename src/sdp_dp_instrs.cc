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

// File: sdp_dp_instrs.cc

#include <ilang/ilang++.h>
#include <iostream>
using namespace std;
//#include "uninterpreted_func/uninterpreted_func.h"
#include <sdp_config.h>
#include <components.h>
#include <converters.h>
#include <lut.h>

namespace ilang {

// =============================================================================
// Instruction categories
// =============================================================================

// ReLU operation
InstrRef ReLU_Compute(Ila& m, RegGroup& r) {

    auto instr = m.NewInstr("Compute_ReLU_" + r.group_name);
    auto consumer = m.state(NVDLA_SDP_S_CONSUMER);
    auto group_name = r.group_name + "_";

    // Account for possibility that ReLU can be computed using either the X1 module or the X2 module
    auto x1_ok = (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x1) & 
                (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_RELU_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_ALU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_MUL_BYPASS)), 0) == 0x1);
    auto x2_ok = (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x1) & 
                (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_RELU_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_ALU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_MUL_BYPASS)), 0) == 0x1);
    auto group_regs = (x1_ok ^ x2_ok) & (consumer == r.group_num) & !r.group_unset;

    instr.SetDecode(group_regs);

    // Determine the source of the input
    auto flying_mode = m.state(GetVarName(group_name, NVDLA_SDP_D_FLYING_MODE));

    for (size_t i = 0; i < 16; i++) {
        // Setup operands
        auto input = Ite(SelectBit(flying_mode, 0) == 0x1, m.input(GetVarName("cacc_data_", (std::to_string(i)))), m.input(GetVarName("mrdma_data_", (std::to_string(i)))));
        auto pdp_output = m.state(GetVarName("pdp_output_", (std::to_string(i))));

        // Compute ReLU
        auto output = Ite(SignedGreater(input, BvConst(0, 32), 32) == 0x1, input, BvConst(0, 32));
        //output = OutputCvt(m, r, output);...add this in later
        instr.SetUpdate(pdp_output, output);
    }

    return instr;
}

// ALU operations, optionally followed by LUT
InstrRef ALU_Compute(Ila& m, RegGroup& r, OpInfo& o, int lut_enable) {

    auto alu_op_name = o.op_name + "_" + r.group_name;

    auto instr = m.NewInstr("Compute_" + alu_op_name);
    auto consumer = m.state(NVDLA_SDP_S_CONSUMER);
    auto group_name = r.group_name + "_";

    // Account for possibility that ALU can be computed using either the X1 module, X2 module, or Y module
    auto x1_ok = (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_BYPASS)), 0) == !lut_enable) & 
                (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_ALU_BYPASS)), 0) == 0x0 & Extract(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_ALU_ALGO)), 1, 0) == o.op_id & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_RELU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_MUL_BYPASS)), 0) == 0x1);
    auto x2_ok = (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_BYPASS)), 0) == !lut_enable) & 
                (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_ALU_BYPASS)), 0) == 0x0 & Extract(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_ALU_ALGO)), 1, 0) == o.op_id & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_RELU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_MUL_BYPASS)), 0) == 0x1);
    auto y_ok = (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x1) & 
                (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_ALU_BYPASS)), 0) == 0x0 & Extract(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_ALU_ALGO)), 1, 0) == o.op_id & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_MUL_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_LUT_BYPASS)), 0) == !lut_enable);

    // If LUT is enabled, incorporate associated bypass logic into the decode function
    auto lut_ok = (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_LUT_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_ALU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_MUL_BYPASS)), 0) == 0x1);
    x1_ok = Ite(BvConst(lut_enable, 1) == 0x1, x1_ok & lut_ok, x1_ok);
    x2_ok = Ite(BvConst(lut_enable, 1) == 0x1, x2_ok & lut_ok, x2_ok);

    // Equality checking is only available on the Y sub-unit ALU
    auto regs_ok = Ite(Extract(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_ALU_ALGO)), 1, 0) == 0x3, y_ok, TISH_XOR(x1_ok, x2_ok, y_ok));
    auto group_regs = (regs_ok) & (consumer == r.group_num) & !r.group_unset;
    instr.SetDecode(group_regs);

    // Determine the source of the operands
    auto flying_mode = m.state(GetVarName(group_name, NVDLA_SDP_D_FLYING_MODE));
    auto data_source = Ite(x1_ok == 0x1,
        m.state(GetVarName(group_name, NVDLA_SDP_D_BS_ALU_SRC)),
        Ite(x2_ok == 0x1,
            m.state(GetVarName(group_name, NVDLA_SDP_D_BN_ALU_SRC)),
            m.state(GetVarName(group_name, NVDLA_SDP_D_EW_ALU_SRC))
        )       
    );
    auto alu_shift = Ite(x1_ok == 0x1,
        m.state(GetVarName(group_name, NVDLA_SDP_D_BS_ALU_SHIFT_VALUE)),
        Ite(x2_ok == 0x1,
            m.state(GetVarName(group_name, NVDLA_SDP_D_BN_ALU_SHIFT_VALUE)),
            BvConst(0, 6)
        )
    );

    // Output of X1 and X2 sub-unit ALU is always truncated
    auto mul_shift = Ite(x1_ok == 0x1,
        m.state(GetVarName(group_name, NVDLA_SDP_D_BS_MUL_SHIFT_VALUE)),
        Ite(x2_ok == 0x1,
            m.state(GetVarName(group_name, NVDLA_SDP_D_BN_MUL_SHIFT_VALUE)),
            BvConst(0, 8)
        )
    );

    for (size_t i = 0; i < 16; i++) {
        // Setup operands
        auto input = Ite(SelectBit(flying_mode, 0) == 0x1, m.input(GetVarName("cacc_data_", (std::to_string(i)))), m.input(GetVarName("mrdma_data_", (std::to_string(i)))));
        auto pdp_output = m.state(GetVarName("pdp_output_", (std::to_string(i))));
        auto operand = Ite(SelectBit(data_source, 0) == 0x1, m.input(GetVarName("dma_data_alu_", (std::to_string(i)))), m.input(GetVarName("regs_data_alu_", (std::to_string(i)))));
        operand = Ite(y_ok == 0x1, ALUCvt(m, r, operand), SignExtend(operand, 16, 32) << SignExtend(alu_shift, 6, 32));

        // Compute ALU operation
        auto output = ALU(input, operand, o);
        auto output_shifted = output >> SignExtend(mul_shift, 8, 32);

        // Optional LUT
        output_shifted = Ite(BvConst(lut_enable, 1) == 0x1, ReadLut(m, output_shifted), output_shifted);
        output_shifted = Ite(y_ok == 0x1, Extract(Saturation(SignExtend(output_shifted, 32, 64), 32), 31, 0), output_shifted);
        output_shifted = OutputCvt(m, r, output_shifted);
        instr.SetUpdate(pdp_output, output_shifted);
    }

    return instr;
}

// Mult operations, optionally followed by LUT
InstrRef Mult_Compute(Ila& m, RegGroup& r, OpInfo& o, int lut_enable) {
    auto mult_op_name = o.op_name + "_" + r.group_name;

    auto instr = m.NewInstr("Compute_" + mult_op_name);
    auto consumer = m.state(NVDLA_SDP_S_CONSUMER);
    auto group_name = r.group_name + "_";

    // Account for possibility that MUL multiply can be computed using either the X1 module, X2 module, or Y module
    auto x1_ok = (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_BYPASS)), 0) == !lut_enable) & 
                (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_MUL_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_MUL_PRELU)), 0) == o.op_id & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_RELU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_ALU_BYPASS)), 0) == 0x1);
    auto x2_ok = (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_BYPASS)), 0) == !lut_enable) & 
                (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_MUL_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_MUL_PRELU)), 0) == o.op_id & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_RELU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_ALU_BYPASS)), 0) == 0x1);
    auto y_ok = (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x1) & 
                (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_MUL_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_MUL_PRELU)), 0) == o.op_id & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_ALU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_LUT_BYPASS)), 0) == !lut_enable);

    // If LUT is enabled, incorporate associated bypass logic into the decode function
    auto lut_ok = (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_LUT_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_ALU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_MUL_BYPASS)), 0) == 0x1);
    x1_ok = Ite(BvConst(lut_enable, 1) == 0x1, x1_ok & lut_ok, x1_ok);
    x2_ok = Ite(BvConst(lut_enable, 1) == 0x1, x2_ok & lut_ok, x2_ok);

    auto group_regs = (TISH_XOR(x1_ok, x2_ok, y_ok)) & (consumer == r.group_num) & !r.group_unset;
    instr.SetDecode(group_regs);

    // Determine the source of the operands
    auto flying_mode = m.state(GetVarName(group_name, NVDLA_SDP_D_FLYING_MODE));
    auto data_source = Ite(x1_ok == 0x1,
        m.state(GetVarName(group_name, NVDLA_SDP_D_BS_MUL_SRC)),
        Ite(x2_ok == 0x1,
            m.state(GetVarName(group_name, NVDLA_SDP_D_BN_MUL_SRC)),
            m.state(GetVarName(group_name, NVDLA_SDP_D_EW_MUL_SRC))
        )       
    );
    auto mul_shift = Ite(x1_ok == 0x1,
        SignExtend(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_MUL_SHIFT_VALUE)), 8, 10),
        Ite(x2_ok == 0x1,
            SignExtend(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_MUL_SHIFT_VALUE)), 8, 10),
            m.state(GetVarName(group_name, NVDLA_SDP_D_DP_EW_TRUNCATE_VALUE))
        )
    );

    // Check PReLU status to determine if truncate is needed
    auto prelu_status = Ite(x1_ok == 0x1,
        SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_MUL_PRELU)), 0),
        Ite(x2_ok == 0x1,
            SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_MUL_PRELU)), 0),
            SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_MUL_PRELU)), 0)
        )
    );

    for (size_t i = 0; i < 16; i++) {
        // Setup operands
        auto input = Ite(SelectBit(flying_mode, 0) == 0x1, m.input(GetVarName("cacc_data_", (std::to_string(i)))), m.input(GetVarName("mrdma_data_", (std::to_string(i)))));
        auto pdp_output = m.state(GetVarName("pdp_output_", (std::to_string(i))));
        auto operand = Ite(SelectBit(data_source, 0) == 0x1, m.input(GetVarName("dma_data_mult_", (std::to_string(i)))), m.input(GetVarName("regs_data_mult_", (std::to_string(i)))));
        operand = Ite(y_ok == 0x1, MultCvt(m, r, operand), SignExtend(operand, 16, 32));

        // Compute multiply
        auto output = Mult(input, operand, o);
        auto output_shifted = Ite(prelu_status == 0x1 & SelectBit(input, 31) == 0x0, output, output >> SignExtend(mul_shift, 10, 32));

        // Optional LUT
        output_shifted = Ite(BvConst(lut_enable, 1) == 0x1, ReadLut(m, output_shifted), output_shifted);
        output_shifted = OutputCvt(m, r, output_shifted);
        instr.SetUpdate(pdp_output, output_shifted);

    }

    return instr;
}

// BatchNorm operation. Given mean m, variance v, and element x, computes x' = (x - m) / v.
// Optionally followed by ReLU.
InstrRef BatchNorm_Compute(Ila& m, RegGroup& r, int relu_enable) {

    auto instr = m.NewInstr("Compute_BatchNorm_" + r.group_name);
    auto consumer = m.state(NVDLA_SDP_S_CONSUMER);
    auto group_name = r.group_name + "_";

    // Account for possibility that BatchNorm can be computed using either the X1 module or the X2 module
    auto x1_ok = (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x1) & 
                (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_RELU_BYPASS)), 0) == !relu_enable & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_ALU_BYPASS)), 0) == 0x0 & Extract(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_ALU_ALGO)), 1, 0) == 0x2 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_MUL_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_MUL_PRELU)), 0) == 0x0);
    auto x2_ok = (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x1) & 
                (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_RELU_BYPASS)), 0) == !relu_enable & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_ALU_BYPASS)), 0) == 0x0 & Extract(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_ALU_ALGO)), 1, 0) == 0x2 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_MUL_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_MUL_PRELU)), 0) == 0x0);
    auto group_regs = (x1_ok ^ x2_ok) & (consumer == r.group_num) & !r.group_unset;

    instr.SetDecode(group_regs);

    // Determine the source of the input
    auto flying_mode = m.state(GetVarName(group_name, NVDLA_SDP_D_FLYING_MODE));

    // Determine the source of ALU operands
    OpInfo alu_add_struct = {"Add", 2};
    auto data_source_alu = Ite(x1_ok == 0x1,
        m.state(GetVarName(group_name, NVDLA_SDP_D_BS_MUL_SRC)),
        m.state(GetVarName(group_name, NVDLA_SDP_D_BN_MUL_SRC))      
    );
    auto alu_shift = Ite(x1_ok == 0x1,
        m.state(GetVarName(group_name, NVDLA_SDP_D_BS_ALU_SHIFT_VALUE)),
        m.state(GetVarName(group_name, NVDLA_SDP_D_BN_ALU_SHIFT_VALUE))
    );

    // Determine the source of multiplier operands
    OpInfo mul_multiply_struct = {"Multiply", 0};
    auto data_source_mult = Ite(x1_ok == 0x1,
        m.state(GetVarName(group_name, NVDLA_SDP_D_BS_MUL_SRC)),
        m.state(GetVarName(group_name, NVDLA_SDP_D_BN_MUL_SRC))   
    );
    auto mul_shift = Ite(x1_ok == 0x1,
        m.state(GetVarName(group_name, NVDLA_SDP_D_BS_MUL_SHIFT_VALUE)),
        m.state(GetVarName(group_name, NVDLA_SDP_D_BN_MUL_SHIFT_VALUE))
    );

    for (size_t i = 0; i < 16; i++) {
        // Setup operands
        auto input = Ite(SelectBit(flying_mode, 0) == 0x1, m.input(GetVarName("cacc_data_", (std::to_string(i)))), m.input(GetVarName("mrdma_data_", (std::to_string(i)))));
        auto pdp_output = m.state(GetVarName("pdp_output_", (std::to_string(i))));
        auto alu_operand = Ite(SelectBit(data_source_alu, 0) == 0x1, m.input(GetVarName("dma_data_alu_", (std::to_string(i)))), m.input(GetVarName("regs_data_alu_", (std::to_string(i)))));
        auto mult_operand = Ite(SelectBit(data_source_mult, 0) == 0x1, m.input(GetVarName("dma_data_mult_", (std::to_string(i)))), m.input(GetVarName("regs_data_mult_", (std::to_string(i)))));

        // Compute BatchNorm

        // Compute addition. Because the ALU in the SDP is only capable of addition and not subtraction, the
        // negation of the mean m should be sent through the secondary input channel.
        alu_operand = SignExtend(alu_operand, 16, 32) << SignExtend(alu_shift, 6, 32);
        auto alu_output = ALU(input, alu_operand, alu_add_struct);

        // Compute multiplication. Because the multiplier in the SDP is only capable of multiplication and not divison, 
        // the reciprocal of the variance v should be sent through the secondary input channel. Because the operand must
        // be an INT, this means v > 1 is not possible in this model.
        auto mult_output = Mult(alu_output, SignExtend(mult_operand, 16, 32), mul_multiply_struct);
        mult_output = mult_output >> SignExtend(mul_shift, 8, 32);

        // Optional ReLU
        auto output = 
        Ite(BvConst(relu_enable, 1) == 0x1,
            Ite(SignedGreater(mult_output, BvConst(0, 32), 32) == 0x1, mult_output, BvConst(0, 32)),
            mult_output
        );
        output = OutputCvt(m, r, output);
        instr.SetUpdate(pdp_output, output);
    }

    return instr;
}

// LUT write operation
InstrRef LUT_Write(Ila& m, LutInfo& l) {

    auto instr = m.NewInstr("Write_LUT_" + l.lut_name);

    auto access_type = SelectBit(m.state(NVDLA_SDP_S_LUT_ACCESS_TYPE), 0);
    auto table_id = SelectBit(m.state(NVDLA_SDP_S_LUT_TABLE_ID), 0);

    instr.SetDecode(access_type == 0x1 & table_id == l.lut_id);
 
    // Set the relevant table
    auto lut = m.state(l.lut_name);
    auto lut_addr = m.state(NVDLA_SDP_S_LUT_ADDR);
    auto lut_data = m.state(NVDLA_SDP_S_LUT_ACCESS_DATA);

    auto entries = (1 << (l.lut_size - 1)) + 1;

    for (size_t i = 0; i < entries; i++) {
        // Write to table - note that lut_data currently is not changing every cycle!
        auto new_lut = Store(lut, BvConst(i, l.lut_size), lut_data);
        instr.SetUpdate(lut, new_lut);
    }

    return instr;
}

// LUT read operation
InstrRef LUT_Read(Ila& m, RegGroup& r) {

    auto instr = m.NewInstr("Read_LUT_" + r.group_name);
    auto consumer = m.state(NVDLA_SDP_S_CONSUMER);
    auto group_name = r.group_name + "_";

    auto access_type = SelectBit(m.state(NVDLA_SDP_S_LUT_ACCESS_TYPE), 0);
    auto table_id = SelectBit(m.state(NVDLA_SDP_S_LUT_TABLE_ID), 0);

    // LUT is only available on the Y sub-unit
    auto y_ok = (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x1) & 
                (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_LUT_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_ALU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_MUL_BYPASS)), 0) == 0x1);
    auto group_regs = y_ok & (consumer == r.group_num) & !r.group_unset;
    instr.SetDecode(group_regs & access_type == 0x0);

    // Determine the source of the operands
    auto flying_mode = m.state(GetVarName(group_name, NVDLA_SDP_D_FLYING_MODE));

    for (size_t i = 0; i < 16; i++) {
        // Setup operands
        auto input = Ite(SelectBit(flying_mode, 0) == 0x1, m.input(GetVarName("cacc_data_", (std::to_string(i)))), m.input(GetVarName("mrdma_data_", (std::to_string(i)))));
        auto pdp_output = m.state(GetVarName("pdp_output_", (std::to_string(i))));

        // Read from the LUT
        auto output = ReadLut(m, input);
        output = OutputCvt(m, r, output);
        instr.SetUpdate(pdp_output, output);

    }

    return instr;
}

// Define SDP instructions relevant to datapath computation
void DefineSDPInstrsDP(Ila& m) {

    // =============================================================================
    // Setup
    // =============================================================================

    auto csb_addr = m.input("csb_addr");
    // auto csb_addr = Extract(Concat(m.input("csb_addr"), BvConst(0,2)), 11, 0);... is this the correct version?
    auto csb_valid = (m.input("csb_rdy") == BvConst(1,1)) & (m.input("csb_vld") == BvConst(1,1));
    auto csb_write = m.input("csb_write") == BvConst(1,1);
    auto group0_unset = SelectBit(m.state(GetVarName("group0_", NVDLA_SDP_D_OP_ENABLE)), 0) == BvConst(0,1);
    auto group1_unset = SelectBit(m.state(GetVarName("group1_", NVDLA_SDP_D_OP_ENABLE)), 0) == BvConst(0,1);
    auto producer = SelectBit(m.state(NVDLA_SDP_S_PRODUCER), 0);
    auto consumer = SelectBit(m.state(NVDLA_SDP_S_CONSUMER), 0);

    // Status: 3
    // 1: IDLE
    // 2: Producer (op_enable) and Consumer
    // 3: Producer (op_enable) but not consumer

    auto group0 = "group0";
    auto group1 = "group1";

    RegGroup group0_struct = {group0, BvConst(0, 1), group0_unset};
    RegGroup group1_struct = {group1, BvConst(1, 1), group1_unset};

    // =============================================================================
    // Instruction definitions
    // =============================================================================

    // ReLU Only
    { // Group 0
        auto instr = ReLU_Compute(m, group0_struct);  
    }

    { // Group 1
        auto instr = ReLU_Compute(m, group1_struct);  
    }

    // ALU Eltwise Max Only
    OpInfo alu_max_struct = {"Max", 0};

    { // Group 0
        auto instr = ALU_Compute(m, group0_struct, alu_max_struct, 0);
    }

    { // Group 1
        auto instr = ALU_Compute(m, group1_struct, alu_max_struct, 0);
    }

    // ALU Eltwise Max with LUT
    OpInfo alu_max_lut_struct = {"Max_LUT", 0};

    { // Group 0
        auto instr = ALU_Compute(m, group0_struct, alu_max_lut_struct, 1);
    }

    { // Group 1
        auto instr = ALU_Compute(m, group1_struct, alu_max_lut_struct, 1);
    }

    // ALU Eltwise Min Only
    OpInfo alu_min_struct = {"Min", 1};
    
    { // Group 0
        auto instr = ALU_Compute(m, group0_struct, alu_min_struct, 0);
    }

    { // Group 1
        auto instr = ALU_Compute(m, group1_struct, alu_min_struct, 0);
    }

    // ALU Eltwise Min with LUT
    OpInfo alu_min_lut_struct = {"Min_Lut", 1};
    
    { // Group 0
        auto instr = ALU_Compute(m, group0_struct, alu_min_lut_struct, 1);
    }

    { // Group 1
        auto instr = ALU_Compute(m, group1_struct, alu_min_lut_struct, 1);
    }

    // ALU Eltwise Add Only
    OpInfo alu_add_struct = {"Add", 2};
    
    { // Group 0
        auto instr = ALU_Compute(m, group0_struct, alu_add_struct, 0);
    }

    { // Group 1
        auto instr = ALU_Compute(m, group1_struct, alu_add_struct, 0);
    }

    // ALU Eltwise Add with LUT
    OpInfo alu_add_lut_struct = {"Add_Lut", 2};
    
    { // Group 0
        auto instr = ALU_Compute(m, group0_struct, alu_add_lut_struct, 1);
    }

    { // Group 1
        auto instr = ALU_Compute(m, group1_struct, alu_add_lut_struct, 1);
    }

    // ALU Eltwise Equal Only (LUT must be bypassed)
    OpInfo alu_equal_struct = {"Equal", 3};
    
    { // Group 0
        auto instr = ALU_Compute(m, group0_struct, alu_equal_struct, 0);
    }

    { // Group 1
        auto instr = ALU_Compute(m, group1_struct, alu_equal_struct, 0);
    }

    // MUL Eltwise Multiply
    OpInfo mul_multiply_struct = {"Multiply", 0};
    
    { // Group 0
        auto instr = Mult_Compute(m, group0_struct, mul_multiply_struct, 0);
    }

    { // Group 1
        auto instr = Mult_Compute(m, group1_struct, mul_multiply_struct, 0);
    }

    // MUL Eltwise Multiply with LUT
    OpInfo mul_multiply_lut_struct = {"Multiply_Lut", 0};
    
    { // Group 0
        auto instr = Mult_Compute(m, group0_struct, mul_multiply_lut_struct, 1);
    }

    { // Group 1
        auto instr = Mult_Compute(m, group1_struct, mul_multiply_lut_struct, 1);
    }

    // MUL Eltwise PReLU
    OpInfo mul_prelu_struct = {"PReLU", 1};
    
    { // Group 0
        auto instr = Mult_Compute(m, group0_struct, mul_prelu_struct, 0);
    }

    { // Group 1
        auto instr = Mult_Compute(m, group1_struct, mul_prelu_struct, 0);
    }

    // MUL Eltwise PReLU with LUT
    OpInfo mul_prelu_lut_struct = {"PReLU_Lut", 1};
    
    { // Group 0
        auto instr = Mult_Compute(m, group0_struct, mul_prelu_lut_struct, 1);
    }

    { // Group 1
        auto instr = Mult_Compute(m, group1_struct, mul_prelu_lut_struct, 1);
    }

    // BatchNorm
    { // Group 0
        auto instr = BatchNorm_Compute(m, group0_struct, 0);
    }

    { // Group 1
        auto instr = BatchNorm_Compute(m, group1_struct, 0);
    }

    // BatchNorm
    { // Group 0
        auto instr = BatchNorm_Compute(m, group0_struct, 1);
    }

    { // Group 1
        auto instr = BatchNorm_Compute(m, group1_struct, 1);
    }

    // LUT Write LE
    LutInfo lut_w_le_struct = {"le_tbl", 0, 7};
    
    {
        auto instr = LUT_Write(m, lut_w_le_struct);
    }

    // LUT Write LO
    LutInfo lut_w_lo_struct = {"lo_tbl", 1, 9};
    
    {
        auto instr = LUT_Write(m, lut_w_lo_struct);
    }

    // LUT Read    
    { // Group 0
        auto instr = LUT_Read(m, group0_struct);
    }

    { // Group 1
        auto instr = LUT_Read(m, group1_struct);
    }

}


} // namespace ilang