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

// File: converters.cc

#include <ilang/ilang++.h>
#include <iostream>
using namespace std;
#include <sdp_config.h>
#include <datapath_structs.h>
#include <components.h>

namespace ilang {

// =============================================================================
// SDP data converters
// =============================================================================

// Uses saturdation to convert the internal INT32 representation to either INT16 or INT8
ExprRef OutputCvt(Ila& m, RegGroup& r, ExprRef input) {
    auto group_name = r.group_name + "_";

    // Determine the source of the operands
    auto cvt_offset = m.state(GetVarName(group_name, NVDLA_SDP_D_CVT_OFFSET));
    auto cvt_scale = m.state(GetVarName(group_name, NVDLA_SDP_D_CVT_SCALE));
    auto cvt_shift = m.state(GetVarName(group_name, NVDLA_SDP_D_CVT_SHIFT));
    auto out_precision = m.state(GetVarName(group_name, NVDLA_SDP_D_OUT_PRECISION));

    // Converter
    auto operand_mul = input - cvt_offset;
    auto operand_tct = operand_mul * SignExtend(cvt_scale, 16, 32);

    // This shifter is supposed to have a saturation flag associated with it - it is unclear how this works.
    // See https://github.com/nvdla/hw/blob/8e06b1b9d85aab65b40d43d08eec5ea4681ff715/cmod/hls/sdp/sdp_c.cpp#L59
    auto operand = operand_tct >> SignExtend(cvt_shift, 6, 32);

    // Choose output precision
    auto output = Ite(SelectBit(out_precision, 0) == 0x0, Saturation(SignExtend(operand, 32, 64), 8), Saturation(SignExtend(operand, 32, 64), 16));

    return Extract(output, 31, 0);
}

// Apply conversion to the secondary input of the Y sub-unit ALU
ExprRef ALUCvt(Ila& m, RegGroup& r, ExprRef input) {
    auto group_name = r.group_name + "_";

    auto ew_alu_cvt_bypass = m.state(GetVarName(group_name, NVDLA_SDP_D_EW_ALU_CVT_BYPASS));
    auto ew_alu_cvt_offset = m.state(GetVarName(group_name, NVDLA_SDP_D_DP_EW_ALU_CVT_OFFSET_VALUE));
    auto ew_alu_cvt_scale = m.state(GetVarName(group_name, NVDLA_SDP_D_DP_EW_ALU_CVT_SCALE_VALUE));
    auto ew_alu_cvt_truncate = m.state(GetVarName(group_name, NVDLA_SDP_D_DP_EW_ALU_CVT_TRUNCATE_VALUE));

    // Converter
    input = SignExtend(input, 16, 32);
    ew_alu_cvt_scale = SignExtend(ew_alu_cvt_scale, 16, 32);
    ew_alu_cvt_truncate = SignExtend(ew_alu_cvt_truncate, 6, 32);
    auto operand_mul = Ite(SelectBit(ew_alu_cvt_bypass, 0) == 0x1, input, input - ew_alu_cvt_offset);
    auto operand_tct = Ite(SelectBit(ew_alu_cvt_bypass, 0) == 0x1, operand_mul, operand_mul * ew_alu_cvt_scale);
    auto operand = Ite(SelectBit(ew_alu_cvt_bypass, 0) == 0x1, operand_tct, operand_tct >> ew_alu_cvt_truncate);

    return operand;
}

// Apply conversion to the secondary input of the Y sub-unit multiplier
ExprRef MultCvt(Ila& m, RegGroup& r, ExprRef input) {
    auto group_name = r.group_name + "_";

    auto ew_mul_cvt_bypass = m.state(GetVarName(group_name, NVDLA_SDP_D_EW_MUL_CVT_BYPASS));
    auto ew_mul_cvt_offset = m.state(GetVarName(group_name, NVDLA_SDP_D_DP_EW_MUL_CVT_OFFSET_VALUE));
    auto ew_mul_cvt_scale = m.state(GetVarName(group_name, NVDLA_SDP_D_DP_EW_MUL_CVT_SCALE_VALUE));
    auto ew_mul_cvt_truncate = m.state(GetVarName(group_name, NVDLA_SDP_D_DP_EW_MUL_CVT_TRUNCATE_VALUE));

    // Converter
    input = SignExtend(input, 16, 32);
    ew_mul_cvt_scale = SignExtend(ew_mul_cvt_scale, 16, 32);
    ew_mul_cvt_truncate = SignExtend(ew_mul_cvt_truncate, 6, 32);
    auto operand_mul = Ite(SelectBit(ew_mul_cvt_bypass, 0) == 0x1, input, input - ew_mul_cvt_offset);
    auto operand_tct = Ite(SelectBit(ew_mul_cvt_bypass, 0) == 0x1, operand_mul, operand_mul * ew_mul_cvt_scale);
    auto operand = Ite(SelectBit(ew_mul_cvt_bypass, 0) == 0x1, operand_tct, operand_tct >> ew_mul_cvt_truncate);

    return operand;
}

} // namespace ilang