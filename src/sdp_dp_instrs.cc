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

namespace ilang {

// TODO:
// - every output here has to end with c1; also separate into an sdp_dp_y and sdp_dp_x instructions (ie, throughput is different for x and y, acrually doesnt matter for software so imcorporate both here)
// - do we need to account for format conversions among int throughout the datapath?
// - next, need multi-step instructions like the ones on the unit description (i.e., batch norm)
// - make a copy of every instruction for group 1
// - looks like when doing configure, have producer & !group0; here do consumer & group0 (i.e., this is datapath running, no more config here)
// - remake the following instructions with general functions, will prevent need to have too much code re-write (ie, still have group instructions and just pass in name of group as an arg)
// do this with a for loop eventually + add flag to alu_compute so it allows for LUT to follow

// =============================================================================
// Datapath modules
// =============================================================================

struct RegGroup {
    string group_name;
    ExprRef group_num;
    ExprRef group_unset;
};

struct OpInfo {
    string op_name;
    int op_id;
};

struct LutInfo {
    string lut_name;
    int lut_id;
    int lut_size;
};

struct LutData {
    ExprRef id;
    ExprRef uflow;
    ExprRef oflow;
    ExprRef index;
    ExprRef fraction;
};

ExprRef IntLog2(ExprRef operand) {
    const int length = operand.bit_width();
    
    // Initialization
    ExprRef* result = (ExprRef*)malloc(sizeof(ExprRef) * (length + 1));
    for (int j = 0; j < length + 1; j++) {
        result[j] = BvConst(0, 32);
    }

    // IntLog2 operation
    auto flag = BvConst(0, 1);
    for (int i = length - 1; i >= 0; i--) { 
        result[i] = Ite((SelectBit(flag, 0) == BvConst(0, 1)) & (SelectBit(operand, i) == BvConst(1, 1)), BvConst(i, 32), result[i + 1]);
        flag = flag | Ite(SelectBit(operand, i) == 0x1, BvConst(1, 1), BvConst(0, 1));
    }

    return result[0];
}

// Technically is 1ull, so might be 64 bits
ExprRef IntLog2Frac(ExprRef operand, ExprRef index) {
    return operand & ((BvConst(1, 32) << index) - 1);
}

// Three-Input-Single-High XOR, or TISH_XOR for short. Returns true if one and only one of the
// input arguments is high.
ExprRef TISH_XOR(ExprRef a, ExprRef b, ExprRef c) {
    return ((a == 0x1) & (b == 0x0) & (c == 0x0))| ((a == 0x0) & (b == 0x1) & (c == 0x0)) | ((a == 0x0) & (b == 0x0) & (c == 0x1));
}

// Determine Two's complement
ExprRef TwoComp(ExprRef a, int length) {
    auto mask = (((BvConst(1, length) << (length - 1)) - 1) << 1) + 1;
    return ((a ^ mask) + 1);
}

// Signed comparison which returns 1 if argument "a" is greater than argument "b"
ExprRef SignedGreater(ExprRef a, ExprRef b, int length) {
    auto sign_bit_a = SelectBit(a, length - 1);
    auto sign_bit_b = SelectBit(b, length - 1);

    auto result = Ite((sign_bit_a == 0x0) & (sign_bit_b == 0x0),    // Case where "a" is positive, "b" is positive
        Ite(a > b,
            BvConst(1, 1),
            BvConst(0, 1)
        ),
        Ite((sign_bit_a == 0x0) & (sign_bit_b == 0x1),              // Case where "a" is positive, "b" is negative
            BvConst(1, 1),
            Ite((sign_bit_a == 0x1) & (sign_bit_b == 0x0),          // Case where "a" is negative, "b" is positive
                BvConst(0, 1),
                Ite(TwoComp(a, length) > TwoComp(b, length),        // Case where "a" is negative, "b" is negative
                    BvConst(0, 1),
                    BvConst(1, 1)
                )
            )
        )
    );

    return result;
}

// make a signed comp function which returns 2 if a > b, 1 if equal, 0 if a < b
// to do this, first make unsigned comparator which does 2 if a > b, 1 if equal, 0 if a < b (write in ilang)
// two cases are similar to above SignedGreater and trivial based on first bit. both positive, used unsigned comparator. 
// both negative, use unsigned comparator with twos complement

ExprRef ALU(ExprRef input, ExprRef operand, OpInfo& o) {
    auto result = 
    Ite(BvConst(o.op_id, 2) == BvConst(0, 2),           // Max operation
        Ite(input > operand,
            input,
            operand
        ),
        Ite(BvConst(o.op_id, 2) == BvConst(1, 2),       // Min operation
            Ite(input < operand,
                input,
                operand
            ),
            Ite(BvConst(o.op_id, 2) == BvConst(2, 2),   // Add operation 
                input + operand,
                Ite(input == operand,                   // Equality operation 
                    BvConst(0, 32),
                    BvConst(1, 32)
                )
            )                      
        )
    );

    return result;
}

ExprRef Mult(ExprRef input, ExprRef operand, OpInfo& o) {
    auto result = 
    Ite(BvConst(o.op_id, 1) == BvConst(0, 1),      // Multiply operation 
        input * operand,
        Ite(input > 0x0,                           // PReLU operation
            input,
            input * operand
        )
    );

    return result;
}

ExprRef Saturation(ExprRef input, int length) {
    auto oMax = (BvConst(1, 32) << (length - 1)) - 1;
    auto oMin = -(BvConst(1, 32) << (length - 1));          // negative number

    auto output = Ite(input > oMax, 
        oMax, 
        Ite(input < oMin,
            oMin,
            input
        )
    );

    return output;
}

ExprRef OutputCvt(Ila& m, RegGroup& r, ExprRef input) {
    auto group_name = r.group_name + "_";

    // Determine the source of the operands
    auto cvt_offset = m.state(GetVarName(group_name, NVDLA_SDP_D_CVT_OFFSET));
    auto cvt_scale = m.state(GetVarName(group_name, NVDLA_SDP_D_CVT_SCALE));
    auto cvt_shift = m.state(GetVarName(group_name, NVDLA_SDP_D_CVT_SHIFT));
    auto out_precision = m.state(GetVarName(group_name, NVDLA_SDP_D_OUT_PRECISION));

    // Converter
    auto operand_mul = input - cvt_offset;
    auto operand_tct = operand_mul * Concat(BvConst(0, 16), cvt_scale);
    auto operand = operand_tct >> Concat(BvConst(0, 26), cvt_shift);         // The truncate can saturate - unclear how this works

    // Chose output precision
    auto output = Ite(SelectBit(out_precision, 0) == 0x0, Saturation(operand, 8), Saturation(operand, 16));

    return output;
}

ExprRef ALUCvt(Ila& m, RegGroup& r, ExprRef input) {
    auto group_name = r.group_name + "_";

    auto ew_alu_cvt_bypass = m.state(GetVarName(group_name, NVDLA_SDP_D_EW_ALU_CVT_BYPASS));
    auto ew_alu_cvt_offset = m.state(GetVarName(group_name, NVDLA_SDP_D_DP_EW_ALU_CVT_OFFSET_VALUE));
    auto ew_alu_cvt_scale = m.state(GetVarName(group_name, NVDLA_SDP_D_DP_EW_ALU_CVT_SCALE_VALUE));
    auto ew_alu_cvt_truncate = m.state(GetVarName(group_name, NVDLA_SDP_D_DP_EW_ALU_CVT_TRUNCATE_VALUE));

    // Converter
    auto operand_mul = Ite(SelectBit(ew_alu_cvt_bypass, 0) == 0x1, Concat(BvConst(0, 16), input), Concat(BvConst(0, 16), input) - ew_alu_cvt_offset);
    auto operand_tct = Ite(SelectBit(ew_alu_cvt_bypass, 0) == 0x1, operand_mul, operand_mul * Concat(BvConst(0, 16), ew_alu_cvt_scale));
    auto operand = Ite(SelectBit(ew_alu_cvt_bypass, 0) == 0x1, operand_tct, operand_tct >> Concat(BvConst(0, 26), ew_alu_cvt_truncate));

    return operand;
}

ExprRef MultCvt(Ila& m, RegGroup& r, ExprRef input) {
    auto group_name = r.group_name + "_";

    auto ew_mul_cvt_bypass = m.state(GetVarName(group_name, NVDLA_SDP_D_EW_MUL_CVT_BYPASS));
    auto ew_mul_cvt_offset = m.state(GetVarName(group_name, NVDLA_SDP_D_DP_EW_MUL_CVT_OFFSET_VALUE));
    auto ew_mul_cvt_scale = m.state(GetVarName(group_name, NVDLA_SDP_D_DP_EW_MUL_CVT_SCALE_VALUE));
    auto ew_mul_cvt_truncate = m.state(GetVarName(group_name, NVDLA_SDP_D_DP_EW_MUL_CVT_TRUNCATE_VALUE));

    // Converter
    auto operand_mul = Ite(SelectBit(ew_mul_cvt_bypass, 0) == 0x1, Concat(BvConst(0, 16), input), Concat(BvConst(0, 16), input) - ew_mul_cvt_offset);
    auto operand_tct = Ite(SelectBit(ew_mul_cvt_bypass, 0) == 0x1, operand_mul, operand_mul * Concat(BvConst(0, 16), ew_mul_cvt_scale));
    auto operand = Ite(SelectBit(ew_mul_cvt_bypass, 0) == 0x1, operand_tct, operand_tct >> Concat(BvConst(0, 26), ew_mul_cvt_truncate));

    return operand;
}

LutData ReadLutLE(Ila& m, ExprRef input) {
    // Set the relevant table
    auto lut = m.state("le_tbl");
    auto lut_start = m.state(NVDLA_SDP_S_LUT_LE_START);
    auto lut_index_offset = m.state(NVDLA_SDP_S_LUT_LE_INDEX_OFFSET);
    auto lut_index_select = m.state(NVDLA_SDP_S_LUT_LE_INDEX_SELECT);
    auto lut_function = SelectBit(m.state(NVDLA_SDP_S_LUT_LE_FUNCTION), 0);
    
    // Determine intermediate values for LUT index
    auto le_index_idx =
    Ite(lut_function == 0x0,
        IntLog2(input - lut_start),
        input - lut_start
    );

    auto le_index_s =
    Ite(lut_function == 0x0,
        le_index_idx - Concat(BvConst(0, 24), lut_index_offset),
        le_index_idx >> Concat(BvConst(0, 24), lut_index_select)
    );

    // Generate out-of-bounds flags
    auto le_uflow = 
    Ite(lut_function == 0x0,
        Ite(input <= lut_start,
            BvConst(1, 1),
            Ite(le_index_idx < Concat(BvConst(0, 24), lut_index_offset),
                BvConst(1, 1),
                BvConst(0, 1)
            )
        ),

        Ite(input <= lut_start,
            BvConst(1, 1),
            BvConst(0, 1)
        )
    );

    auto le_oflow = 
    Ite(lut_function == 0x0,
        Ite(input <= lut_start,
            BvConst(0, 1),
            Ite(le_index_idx < Concat(BvConst(0, 24), lut_index_offset),
                BvConst(0, 1),
                Ite(le_index_s >= 0x40,
                    BvConst(1, 1),
                    BvConst(0, 1)
                )
            )
        ),

        Ite(input <= lut_start,
            BvConst(0, 1),
            Ite(le_index_s >= 0x40,
                BvConst(1, 1),
                BvConst(0, 1)
            )
        )
    );

    // Determine final value for LUT index
    auto le_index = 
    Ite(lut_function == 0x0,
        Ite(input <= lut_start,
            BvConst(1, 32),
            Ite(le_index_idx < Concat(BvConst(0, 24), lut_index_offset),
                BvConst(1, 32),
                Ite(le_index_s >= 0x40,
                    BvConst(64, 32),
                    le_index_s
                )
            )
        ),

        Ite(input <= lut_start,
            BvConst(0, 32),
            Ite(le_index_s >= 0x40,
                    BvConst(64, 32),
                    le_index_s
            )
        )
    );
    le_index = Extract(le_index, 6, 0);

    // Determine final value for LUT fraction; assume it is the same size as the LUT index
    auto le_fraction = 
    Ite(lut_function == 0x0,
        Ite(input <= lut_start,
            BvConst(0, 32),
            Ite(le_index_idx < Concat(BvConst(0, 24), lut_index_offset),
                BvConst(0, 32),
                Ite(le_index_s >= 0x40,
                    BvConst(0, 32),
                    IntLog2Frac(input - lut_start, le_index_idx) << (BvConst(35, 32) - le_index_idx)
                )
            )
        ),

        Ite(input <= lut_start,
            BvConst(0, 32),
            Ite(le_index_s >= 0x40,
                    BvConst(0, 32),
                    (le_index_idx & ((BvConst(1, 32) << Concat(BvConst(0, 24), lut_index_select)) - 1)) >> (Concat(BvConst(0, 24), lut_index_select) - BvConst(35, 32))
            )
        )
    );
    le_fraction = Extract(le_fraction, 6, 0);
    
    LutData return_struct = {BvConst(0, 1), le_uflow, le_oflow, le_index, le_fraction};
    return return_struct;
}

LutData ReadLutLO(Ila& m, ExprRef input) {
    // Set the relevant table
    auto lut = m.state("lo_tbl");
    auto lut_start = m.state(NVDLA_SDP_S_LUT_LO_START);
    auto lut_index_select = m.state(NVDLA_SDP_S_LUT_LO_INDEX_SELECT);

    // Determine intermediate values for LUT index
    auto lo_index_idx = input - lut_start;

    auto lo_index_s = lo_index_idx >> Concat(BvConst(0, 24), lut_index_select);

    // Generate out-of-bounds flags
    auto lo_uflow = 
    Ite(input <= lut_start,
        BvConst(1, 1),
        BvConst(0, 1)
    );

    auto lo_oflow = 
    Ite(input <= lut_start,
        BvConst(0, 1),
        Ite(lo_index_s >= 0x100,
            BvConst(1, 1),
            BvConst(0, 1)
        )
    );

    // Determine final value for LUT index
    auto lo_index = 
    Ite(input <= lut_start,
        BvConst(0, 32),
        Ite(lo_index_s >= 0x100,
                BvConst(256, 32),
                lo_index_s
        )
    );
    lo_index = Extract(lo_index, 8, 0);

    // Determine final value for LUT fraction
    auto lo_fraction = 
    Ite(input <= lut_start,
        BvConst(0, 32),
        Ite(lo_index_s >= 0x100,
                BvConst(0, 32),
                (lo_index_idx & ((BvConst(1, 32) << Concat(BvConst(0, 24), lut_index_select)) - 1)) >> (Concat(BvConst(0, 24), lut_index_select) - BvConst(35, 32))
        )
    );
    lo_fraction = Extract(lo_fraction, 8, 0);

    LutData return_struct = {BvConst(1, 1), lo_uflow, lo_oflow, lo_index, lo_fraction};
    return return_struct;
}

LutData SelectLut(Ila& m, LutData& le_data, LutData& lo_data) {
    // LUT parameters
    auto le_index = le_data.index;
    auto lo_index = lo_data.index;

    auto le_fraction = le_data.fraction;
    auto lo_fraction = lo_data.fraction;

    auto le_uflow = le_data.uflow;
    auto lo_uflow = lo_data.uflow;

    auto le_oflow = le_data.oflow;
    auto lo_oflow = lo_data.oflow;

    auto le_miss = (le_uflow == 0x1) | (le_oflow == 0x1);
    auto le_hit = le_miss == 0x0;
    auto lo_miss = (lo_uflow == 0x1) | (lo_oflow == 0x1);        
    auto lo_hit = lo_miss == 0x0;

    // LUT priorities
    auto uflow_priority = SelectBit(m.state(NVDLA_SDP_S_LUT_UFLOW_PRIORITY), 0);
    auto oflow_priority = SelectBit(m.state(NVDLA_SDP_S_LUT_OFLOW_PRIORITY), 0);
    auto hybrid_priority = SelectBit(m.state(NVDLA_SDP_S_LUT_HYBRID_PRIORITY), 0);

    auto index = 
    Ite((le_uflow == 0x1) & (lo_uflow == 0x1),
        Ite(uflow_priority == 0x1, lo_index, Concat(BvConst(0, 2), le_index)),
        Ite((le_oflow == 0x1) & (lo_oflow == 0x1),
            Ite(oflow_priority == 0x1, lo_index, Concat(BvConst(0, 2), le_index)),
            Ite((le_hit == 0x1) & (lo_hit == 0x1),
                Ite(hybrid_priority == 0x1, lo_index, Concat(BvConst(0, 2), le_index)),
                Ite((le_miss == 0x1) & (lo_miss == 0x1),
                    Ite(hybrid_priority == 0x1, lo_index, Concat(BvConst(0, 2), le_index)),
                    Ite(le_hit == 0x1,
                        Concat(BvConst(0, 2), le_index),
                        lo_index
                    )
                )
            )
        )
    );

    auto fraction = 
    Ite((le_uflow == 0x1) & (lo_uflow == 0x1),
        Ite(uflow_priority == 0x1, lo_fraction, Concat(BvConst(0, 2), le_fraction)),
        Ite((le_oflow == 0x1) & (lo_oflow == 0x1),
            Ite(oflow_priority == 0x1, lo_fraction, Concat(BvConst(0, 2), le_fraction)),
            Ite((le_hit == 0x1) & (lo_hit == 0x1),
                Ite(hybrid_priority == 0x1, lo_fraction, Concat(BvConst(0, 2), le_fraction)),
                Ite((le_miss == 0x1) & (lo_miss == 0x1),
                    Ite(hybrid_priority == 0x1, lo_fraction, Concat(BvConst(0, 2), le_fraction)),
                    Ite(le_hit == 0x1,
                        Concat(BvConst(0, 2), le_fraction),
                        lo_fraction
                    )
                )
            )
        )
    );

    auto uflow = 
    Ite((le_uflow == 0x1) & (lo_uflow == 0x1),
        Ite(uflow_priority == 0x1, lo_uflow, le_uflow),
        Ite((le_oflow == 0x1) & (lo_oflow == 0x1),
            BvConst(0, 1),
            Ite((le_hit == 0x1) & (lo_hit == 0x1),
                BvConst(0, 1),
                Ite((le_miss == 0x1) & (lo_miss == 0x1),
                    Ite(hybrid_priority == 0x1, lo_uflow, le_uflow),
                    BvConst(0, 1)
                )
            )
        )
    );

    auto oflow = 
    Ite((le_uflow == 0x1) & (lo_uflow == 0x1),
        BvConst(0, 1),
        Ite((le_oflow == 0x1) & (lo_oflow == 0x1),
            Ite(oflow_priority == 0x1, lo_oflow, le_oflow),
            Ite((le_hit == 0x1) & (lo_hit == 0x1),
                BvConst(0, 1),
                Ite((le_miss == 0x1) & (lo_miss == 0x1),
                    Ite(hybrid_priority == 0x1, lo_oflow, le_oflow),
                    BvConst(0, 1)
                )
            )
        )
    );

    auto id = 
    Ite((le_uflow == 0x1) & (lo_uflow == 0x1),
        Ite(uflow_priority == 0x1, BvConst(1, 1), BvConst(0, 1)),
        Ite((le_oflow == 0x1) & (lo_oflow == 0x1),
            Ite(oflow_priority == 0x1, BvConst(1, 1), BvConst(0, 1)),
            Ite((le_hit == 0x1) & (lo_hit == 0x1),
                Ite(hybrid_priority == 0x1, BvConst(1, 1), BvConst(0, 1)),
                Ite((le_miss == 0x1) & (lo_miss == 0x1),
                    Ite(hybrid_priority == 0x1, BvConst(1, 1), BvConst(0, 1)),
                    Ite(le_hit == 0x1,
                        BvConst(0, 1),
                        BvConst(1, 1)
                    )
                )
            )
        )
    );

    LutData return_struct = {id, uflow, oflow, index, fraction};
    return return_struct;
}

ExprRef PresentLutOutput(Ila& m, ExprRef input, LutData& data) {
    // LUT parameters
    auto index = data.index;
    auto fraction = data.fraction;
    auto oflow = data.oflow;
    auto uflow = data.uflow;
    auto id = data.id;
    auto lut_function = SelectBit(m.state(NVDLA_SDP_S_LUT_LE_FUNCTION), 0);

    // LUT outputs
    auto y0 = Ite(id == 0x1, Load(m.state("lo_tbl"), index), Load(m.state("le_tbl"), Extract(index, 6, 0)));
    auto y1 = 
    Ite(id == 0x1,
        Ite((oflow == 0x0) & (uflow == 0x1), Load(m.state("lo_tbl"), index + 1), BvConst(0, 16)),
        Ite((oflow == 0x0) & (uflow == 0x1), Load(m.state("le_tbl"), Extract(index, 6, 0) + 1), BvConst(0, 16))
    );    

    auto bias = 
    Ite(((oflow == 0x0) | (uflow == 0x0)) & (id == 0x0) & (lut_function == 0x0) & (oflow == 0x0) & (m.state(NVDLA_SDP_S_LUT_LE_INDEX_OFFSET) > 0x0), 
        BvConst(1, 8) << m.state(NVDLA_SDP_S_LUT_LE_INDEX_OFFSET), 
        BvConst(0, 8)
    );
    
    auto scale = 
    Ite((oflow == 0x1) | (uflow == 0x1),
        Ite(id == 0x0,
            Ite(oflow == 0x1, m.state(NVDLA_SDP_S_LUT_LE_SLOPE_OFLOW_SCALE), m.state(NVDLA_SDP_S_LUT_LE_SLOPE_UFLOW_SCALE)),
            Ite(oflow == 0x1, m.state(NVDLA_SDP_S_LUT_LO_SLOPE_OFLOW_SCALE), m.state(NVDLA_SDP_S_LUT_LO_SLOPE_UFLOW_SCALE))
        ),
        BvConst(0, 16)
    );

    auto shift = 
    Ite((oflow == 0x1) | (uflow == 0x1),
        Ite(id == 0x0,
            Ite(oflow == 0x1, m.state(NVDLA_SDP_S_LUT_LE_SLOPE_OFLOW_SHIFT), m.state(NVDLA_SDP_S_LUT_LE_SLOPE_UFLOW_SHIFT)),
            Ite(oflow == 0x1, m.state(NVDLA_SDP_S_LUT_LO_SLOPE_OFLOW_SHIFT), m.state(NVDLA_SDP_S_LUT_LO_SLOPE_UFLOW_SHIFT))
        ),
        BvConst(0, 5)
    );

    auto offset = 
    Ite((oflow == 0x1) | (uflow == 0x1),
        Ite(id == 0x0,
            Ite(oflow == 0x1, m.state(NVDLA_SDP_S_LUT_LE_END), m.state(NVDLA_SDP_S_LUT_LE_START)),
            Ite(oflow == 0x1, m.state(NVDLA_SDP_S_LUT_LO_END), m.state(NVDLA_SDP_S_LUT_LO_START))
        ),
        BvConst(0, 32)
    );

    // Interpolation and extrapolation
    auto o =
    Ite((oflow == 0x1) | (uflow == 0x1),
        Concat(BvConst(0, 3), Concat(BvConst(0, 16), y0) + ((input - Concat(BvConst(0, 24), bias) - offset) * Concat(BvConst(0, 16), scale)) >> Concat(BvConst(0, 27), shift)),
        (((BvConst(1, 35) << 35) - Concat(BvConst(0, 26), fraction)) * Concat(BvConst(0, 19), y0)) + (Concat(BvConst(0, 26), fraction) * Concat(BvConst(0, 19), y1))
    );

    auto oMax = (BvConst(1, 32) << 31) - 1;
    auto oMin = -(BvConst(1, 32) << 31);
    auto out_data =
    Ite((oflow == 0x1) | (uflow == 0x1),
        Ite(o > Concat(BvConst(0, 3), oMax), 
            Concat(BvConst(0, 3), oMax),
            Ite(o < Concat(BvConst(0, 3), oMin),
                Concat(BvConst(0, 3), oMin),
                o
            )            
        ),
        o >> 35
    );

    return out_data;
}

ExprRef ReadLut(Ila& m, ExprRef input) {
    auto le_data = ReadLutLE(m, input);
    auto lo_data = ReadLutLO(m, input);
    auto data = SelectLut(m, le_data, lo_data);
    auto output = PresentLutOutput(m, input, data);

    return output;
}

// =============================================================================
// Instruction execution
// =============================================================================

// ReLU only
InstrRef ReLU_Compute(Ila& m, RegGroup& r) {

    auto instr = m.NewInstr("Compute_ReLU_" + r.group_name);
    auto consumer = m.state(NVDLA_SDP_S_CONSUMER);
    auto group_name = r.group_name + "_";

    // Account for possibility that ReLU can be computed using either the X1 module or the X2 module
    auto x1_ok = (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x1) & 
                (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_RELU_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_ALU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_MUL_BYPASS)), 0) == 0x1);
    auto x2_ok = (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x1) & 
                (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_RELU_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_ALU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_MUL_BYPASS)), 0) == 0x1);
    auto group_regs = (x1_ok ^ x2_ok) & consumer == r.group_num & !r.group_unset;

    instr.SetDecode(group_regs);

    // Determine the source of the input
    auto flying_mode = m.state(GetVarName(group_name, NVDLA_SDP_D_FLYING_MODE));

    for (size_t i = 0; i < 16; i++) {
        auto input = Ite(SelectBit(flying_mode, 0) == 0x1, m.input(GetVarName("cacc_data_", (std::to_string(i)))), m.input(GetVarName("mrdma_data_", (std::to_string(i)))));
        auto pdp_output = m.state(GetVarName("pdp_output_", (std::to_string(i))));

        // Compute ReLU
        auto output = Ite(SignedGreater(input, BvConst(0, 32), 32) == 0x1, input, BvConst(0, 32));
        //output = OutputCvt(m, r, output);
        instr.SetUpdate(pdp_output, output);
    }

    return instr;
}

// ALU only + LUT
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
                (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_ALU_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_MUL_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_LUT_BYPASS)), 0) == !lut_enable);

    auto lut_ok = (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_LUT_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_ALU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_MUL_BYPASS)), 0) == 0x1);
    x1_ok = Ite(BvConst(lut_enable, 1) == 0x1, x1_ok & lut_ok, x1_ok);
    x2_ok = Ite(BvConst(lut_enable, 1) == 0x1, x2_ok & lut_ok, x2_ok);

    auto regs_ok = Ite(Extract(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_ALU_ALGO)), 1, 0) == 0x3, y_ok, TISH_XOR(x1_ok, x2_ok, y_ok));
    auto group_regs = (regs_ok) & consumer == r.group_num & !r.group_unset;
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

    for (size_t i = 0; i < 16; i++) {
        // Setup operands
        auto input = Ite(SelectBit(flying_mode, 0) == 0x1, m.input(GetVarName("cacc_data_", (std::to_string(i)))), m.input(GetVarName("mrdma_data_", (std::to_string(i)))));
        auto pdp_output = m.state(GetVarName("pdp_output_", (std::to_string(i))));
        auto operand = Ite(SelectBit(data_source, 0) == 0x1, m.input(GetVarName("dma_data_", (std::to_string(i)))), m.input(GetVarName("regs_data_", (std::to_string(i)))));
        operand = Ite(y_ok == 0x1, ALUCvt(m, r, operand), Concat(BvConst(0, 16), operand) << Concat(BvConst(0, 26), alu_shift));

        // Compute ALU operation
        auto output = ALU(input, operand, o);

        // Optional LUT - check if LUT output is really 35 bits
        output = Ite(BvConst(lut_enable, 1) == 0x1, Extract(ReadLut(m, output), 31, 0), output);
        output = Ite(y_ok == 0x1, Saturation(output, 32), output);
        output = OutputCvt(m, r, output);
        instr.SetUpdate(pdp_output, output);
    }

    return instr;
}

// Mult only
InstrRef Mult_Compute(Ila& m, RegGroup& r, OpInfo& o, int lut_enable) {
    auto mult_op_name = o.op_name + "_" + r.group_name;

    auto instr = m.NewInstr("Compute_" + mult_op_name);
    auto consumer = m.state(NVDLA_SDP_S_CONSUMER);
    auto group_name = r.group_name + "_";

    // Account for possibility that MUL multiply can be computed using either the X1 module or the X2 module
    auto x1_ok = (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_BYPASS)), 0) == !lut_enable) & 
                (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_MUL_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_MUL_PRELU)), 0) == o.op_id & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_RELU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_ALU_BYPASS)), 0) == 0x1);
    auto x2_ok = (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_BYPASS)), 0) == !lut_enable) & 
                (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_MUL_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_MUL_PRELU)), 0) == o.op_id & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_RELU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_ALU_BYPASS)), 0) == 0x1);
    auto y_ok = (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x1) & 
                (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_MUL_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_ALU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_LUT_BYPASS)), 0) == !lut_enable);

    auto lut_ok = (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_LUT_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_ALU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_MUL_BYPASS)), 0) == 0x1);
    x1_ok = Ite(BvConst(lut_enable, 1) == 0x1, x1_ok & lut_ok, x1_ok);
    x2_ok = Ite(BvConst(lut_enable, 1) == 0x1, x2_ok & lut_ok, x2_ok);

    auto group_regs = (TISH_XOR(x1_ok, x2_ok, y_ok)) & consumer == r.group_num & !r.group_unset;
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
        Concat(BvConst(0, 2), m.state(GetVarName(group_name, NVDLA_SDP_D_BS_MUL_SHIFT_VALUE))),
        Ite(x2_ok == 0x1,
            Concat(BvConst(0, 2), m.state(GetVarName(group_name, NVDLA_SDP_D_BN_MUL_SHIFT_VALUE))),
            m.state(GetVarName(group_name, NVDLA_SDP_D_DP_EW_TRUNCATE_VALUE))                       // is this the state corresponding to shifting for y?
        )
    );

    for (size_t i = 0; i < 16; i++) {
        // Setup operands
        auto input = Ite(SelectBit(flying_mode, 0) == 0x1, m.input(GetVarName("cacc_data_", (std::to_string(i)))), m.input(GetVarName("mrdma_data_", (std::to_string(i)))));
        auto pdp_output = m.state(GetVarName("pdp_output_", (std::to_string(i))));
        auto operand = Ite(SelectBit(data_source, 0) == 0x1, m.input(GetVarName("dma_data_", (std::to_string(i)))), m.input(GetVarName("regs_data_", (std::to_string(i)))));
        operand = Ite(y_ok == 0x1, MultCvt(m, r, operand), Concat(BvConst(0, 16), operand));

        // Compute multiply
        auto output = Mult(input, operand, o);
        auto output_shifted = output >> Concat(BvConst(0, 22), mul_shift);

        // Optional LUT
        output_shifted = Ite(BvConst(lut_enable, 1) == 0x1, Extract(ReadLut(m, output), 31, 0), output_shifted);
        output_shifted = OutputCvt(m, r, output_shifted);
        instr.SetUpdate(pdp_output, output_shifted);

    }

    return instr;
}

// LUT write
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

// LUT read
InstrRef LUT_Read(Ila& m, RegGroup& r) {

    auto instr = m.NewInstr("Read_LUT_" + r.group_name);
    auto consumer = m.state(NVDLA_SDP_S_CONSUMER);
    auto group_name = r.group_name + "_";

    auto access_type = SelectBit(m.state(NVDLA_SDP_S_LUT_ACCESS_TYPE), 0);
    auto table_id = SelectBit(m.state(NVDLA_SDP_S_LUT_TABLE_ID), 0);

    auto y_ok = (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BS_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_BN_BYPASS)), 0) == 0x1) & 
                (SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_LUT_BYPASS)), 0) == 0x0 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_ALU_BYPASS)), 0) == 0x1 & SelectBit(m.state(GetVarName(group_name, NVDLA_SDP_D_EW_MUL_BYPASS)), 0) == 0x1);
    auto group_regs = y_ok & consumer == r.group_num & !r.group_unset;
    instr.SetDecode(group_regs & access_type == 0x0);

    // Determine the source of the operands
    auto flying_mode = m.state(GetVarName(group_name, NVDLA_SDP_D_FLYING_MODE));

    for (size_t i = 0; i < 16; i++) {
        // Setup operands
        auto input = Ite(SelectBit(flying_mode, 0) == 0x1, m.input(GetVarName("cacc_data_", (std::to_string(i)))), m.input(GetVarName("mrdma_data_", (std::to_string(i)))));
        auto pdp_output = m.state(GetVarName("pdp_output_", (std::to_string(i))));

        // Read from both LUT tables - assume lut_data is 32 bits for now
        auto output = ReadLut(m, input);
        output = OutputCvt(m, r, Extract(output, 31, 0));
        instr.SetUpdate(pdp_output, output);

    }

    return instr;
}

    // config group 0 -> sdp_state = IDLE
    // enable group 0 -> sdp_state = BUSY
    // consumer is group 0 + config group 1 -> sdp_state = BUSY

void DefineSDPInstrsDP(Ila& m) {

    // =============================================================================
    // Setup
    // =============================================================================

    // CSB MIMO
    auto csb_addr = Extract(Concat(m.input("csb_addr"), BvConst(0,2)), 11, 0);
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

    // ReLU Only
    { // Group 0
        auto instr = ReLU_Compute(m, group0_struct);  
    }

    { // Group 1
        auto instr = ReLU_Compute(m, group1_struct);  
    }

    // ALU Max Only
    OpInfo alu_max_struct = {"Max", 0};

    { // Group 0
        auto instr = ALU_Compute(m, group0_struct, alu_max_struct, 0);
    }

    { // Group 1
        auto instr = ALU_Compute(m, group1_struct, alu_max_struct, 0);
    }

    // ALU Max with LUT
    OpInfo alu_max_lut_struct = {"Max_LUT", 0};

    { // Group 0
        auto instr = ALU_Compute(m, group0_struct, alu_max_lut_struct, 1);
    }

    { // Group 1
        auto instr = ALU_Compute(m, group1_struct, alu_max_lut_struct, 1);
    }

    // ALU Min Only
    OpInfo alu_min_struct = {"Min", 1};
    
    { // Group 0
        auto instr = ALU_Compute(m, group0_struct, alu_min_struct, 0);
    }

    { // Group 1
        auto instr = ALU_Compute(m, group1_struct, alu_min_struct, 0);
    }

    // ALU Min with LUT
    OpInfo alu_min_lut_struct = {"Min_Lut", 1};
    
    { // Group 0
        auto instr = ALU_Compute(m, group0_struct, alu_min_lut_struct, 1);
    }

    { // Group 1
        auto instr = ALU_Compute(m, group1_struct, alu_min_lut_struct, 1);
    }

    // ALU Add Only
    OpInfo alu_add_struct = {"Add", 2};
    
    { // Group 0
        auto instr = ALU_Compute(m, group0_struct, alu_add_struct, 0);
    }

    { // Group 1
        auto instr = ALU_Compute(m, group1_struct, alu_add_struct, 0);
    }

    // ALU Add with LUT
    OpInfo alu_add_lut_struct = {"Add_Lut", 2};
    
    { // Group 0
        auto instr = ALU_Compute(m, group0_struct, alu_add_lut_struct, 1);
    }

    { // Group 1
        auto instr = ALU_Compute(m, group1_struct, alu_add_lut_struct, 1);
    }

    // ALU Equal Only
    OpInfo alu_equal_struct = {"Equal", 3};
    
    { // Group 0
        auto instr = ALU_Compute(m, group0_struct, alu_equal_struct, 0);
    }

    { // Group 1
        auto instr = ALU_Compute(m, group1_struct, alu_equal_struct, 0);
    }

    // ALU Equal with LUT
    OpInfo alu_equal_lut_struct = {"Equal_Lut", 3};
    
    { // Group 0
        auto instr = ALU_Compute(m, group0_struct, alu_equal_lut_struct, 1);
    }

    { // Group 1
        auto instr = ALU_Compute(m, group1_struct, alu_equal_lut_struct, 1);
    }

    // MUL multiply
    OpInfo mul_multiply_struct = {"Multiply", 0};
    
    { // Group 0
        auto instr = Mult_Compute(m, group0_struct, mul_multiply_struct, 0);
    }

    { // Group 1
        auto instr = Mult_Compute(m, group1_struct, mul_multiply_struct, 0);
    }

    // MUL multiply with LUT
    OpInfo mul_multiply_lut_struct = {"Multiply_Lut", 0};
    
    { // Group 0
        auto instr = Mult_Compute(m, group0_struct, mul_multiply_lut_struct, 1);
    }

    { // Group 1
        auto instr = Mult_Compute(m, group1_struct, mul_multiply_lut_struct, 1);
    }

    // MUL PReLU
    OpInfo mul_prelu_struct = {"PReLU", 1};
    
    { // Group 0
        auto instr = Mult_Compute(m, group0_struct, mul_prelu_struct, 0);
    }

    { // Group 1
        auto instr = Mult_Compute(m, group1_struct, mul_prelu_struct, 0);
    }

    // MUL PReLU with LUT
    OpInfo mul_prelu_lut_struct = {"PReLU_Lut", 1};
    
    { // Group 0
        auto instr = Mult_Compute(m, group0_struct, mul_prelu_lut_struct, 1);
    }

    { // Group 1
        auto instr = Mult_Compute(m, group1_struct, mul_prelu_lut_struct, 1);
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

    // { // Read index from LUT Table LE
    //     auto instr = m.NewInstr("Read_LUT_LE");
    //     auto reg_group = "group0_";

    //     // ... lut access type needs to be 1 for write, table id needs to be 1 for lo and 0 for le
    //     auto access_type = SelectBit(m.state(NVDLA_SDP_S_LUT_ACCESS_TYPE), 0);
    //     auto table_id = SelectBit(m.state(NVDLA_SDP_S_LUT_TABLE_ID), 0);
    //     auto lut_bypass = SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_LUT_BYPASS)), 0);

    //     instr.SetDecode(access_type == 0x0 & table_id == 0x0 & lut_bypass == 0x0);

    //     // Set the relevant table
    //     auto lut = m.state("le_tbl");
    //     auto lut_data = m.state(NVDLA_SDP_S_LUT_ACCESS_DATA);
    //     auto lut_start = m.state(NVDLA_SDP_S_LUT_LE_START);
    //     auto lut_index_offset = m.state(NVDLA_SDP_S_LUT_LE_INDEX_OFFSET);
    //     auto lut_index_select = m.state(NVDLA_SDP_S_LUT_LE_INDEX_SELECT);
    //     auto lut_function = SelectBit(m.state(NVDLA_SDP_S_LUT_LE_FUNCTION), 0);
        
    //     // Determine intermediate values for LUT index
    //     auto le_index_idx =
    //     Ite(lut_function == 0x0,
    //         IntLog2(Concat(BvConst(0, 16), lut_data) - lut_start),
    //         Concat(BvConst(0, 16), lut_data) - lut_start
    //     );

    //     auto le_index_s =
    //     Ite(lut_function == 0x0,
    //         le_index_idx - Concat(BvConst(0, 24), lut_index_offset),
    //         le_index_idx >> Concat(BvConst(0, 24), lut_index_select)
    //     );

    //     // Generate out-of-bounds flags
    //     auto le_uflow = 
    //     Ite(lut_function == 0x0,
    //         Ite(Concat(BvConst(0, 16), lut_data) <= lut_start,
    //             BvConst(1, 1),
    //             Ite(le_index_idx < Concat(BvConst(0, 24), lut_index_offset),
    //                 BvConst(1, 1),
    //                 BvConst(0, 1)
    //             )
    //         ),

    //         Ite(Concat(BvConst(0, 16), lut_data) <= lut_start,
    //             BvConst(1, 1),
    //             BvConst(0, 1)
    //         )
    //     );
    //     instr.SetUpdate(m.state("le_uflow"), le_uflow);

    //     auto le_oflow = 
    //     Ite(lut_function == 0x0,
    //         Ite(Concat(BvConst(0, 16), lut_data) <= lut_start,
    //             BvConst(0, 1),
    //             Ite(le_index_idx < Concat(BvConst(0, 24), lut_index_offset),
    //                 BvConst(0, 1),
    //                 Ite(le_index_s >= 0x40,
    //                     BvConst(1, 1),
    //                     BvConst(0, 1)
    //                 )
    //             )
    //         ),

    //         Ite(Concat(BvConst(0, 16), lut_data) <= lut_start,
    //             BvConst(0, 1),
    //             Ite(le_index_s >= 0x40,
    //                 BvConst(1, 1),
    //                 BvConst(0, 1)
    //             )
    //         )
    //     );
    //     instr.SetUpdate(m.state("le_oflow"), le_oflow);

    //     // Determine final value for LUT index
    //     auto le_index = 
    //     Ite(lut_function == 0x0,
    //         Ite(Concat(BvConst(0, 16), lut_data) <= lut_start,
    //             BvConst(1, 32),
    //             Ite(le_index_idx < Concat(BvConst(0, 24), lut_index_offset),
    //                 BvConst(1, 32),
    //                 Ite(le_index_s >= 0x40,
    //                     BvConst(64, 32),
    //                     le_index_s
    //                 )
    //             )
    //         ),

    //         Ite(Concat(BvConst(0, 16), lut_data) <= lut_start,
    //             BvConst(0, 32),
    //             Ite(le_index_s >= 0x40,
    //                     BvConst(64, 32),
    //                     le_index_s
    //             )
    //         )
    //     );
    //     le_index = Extract(le_index, 6, 0);
    //     instr.SetUpdate(m.state("le_index"), le_index);

    //     // Determine final value for LUT fraction; assume it is the same size as the LUT index
    //     auto le_fraction = 
    //     Ite(lut_function == 0x0,
    //         Ite(Concat(BvConst(0, 16), lut_data) <= lut_start,
    //             BvConst(0, 32),
    //             Ite(le_index_idx < Concat(BvConst(0, 24), lut_index_offset),
    //                 BvConst(0, 32),
    //                 Ite(le_index_s >= 0x40,
    //                     BvConst(0, 32),
    //                     IntLog2Frac(Concat(BvConst(0, 16), lut_data) - lut_start, le_index_idx) << (BvConst(35, 32) - le_index_idx)
    //                 )
    //             )
    //         ),

    //         Ite(Concat(BvConst(0, 16), lut_data) <= lut_start,
    //             BvConst(0, 32),
    //             Ite(le_index_s >= 0x40,
    //                     BvConst(0, 32),
    //                     (le_index_idx & ((BvConst(1, 32) << Concat(BvConst(0, 24), lut_index_select)) - 1)) >> (Concat(BvConst(0, 24), lut_index_select) - BvConst(35, 32))
    //             )
    //         )
    //     );
    //     le_fraction = Extract(le_fraction, 6, 0);
    //     instr.SetUpdate(m.state("le_fraction"), le_fraction);

    // }

    // { // Read index from LUT Table LO
    //     auto instr = m.NewInstr("Read_LUT_LO");
    //     auto reg_group = "group0_";

    //     // ... lut access type needs to be 1 for write, table id needs to be 1 for lo and 0 for le
    //     auto access_type = SelectBit(m.state(NVDLA_SDP_S_LUT_ACCESS_TYPE), 0);
    //     auto table_id = SelectBit(m.state(NVDLA_SDP_S_LUT_TABLE_ID), 0);
    //     auto lut_bypass = SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_LUT_BYPASS)), 0);

    //     instr.SetDecode(access_type == 0x0 & table_id == 0x1 & lut_bypass == 0x0);

    //     // Set the relevant table
    //     auto lut = m.state("lo_tbl");
    //     auto lut_data = m.state(NVDLA_SDP_S_LUT_ACCESS_DATA);
    //     auto lut_start = m.state(NVDLA_SDP_S_LUT_LO_START);
    //     auto lut_index_select = m.state(NVDLA_SDP_S_LUT_LO_INDEX_SELECT);

    //     // Determine intermediate values for LUT index
    //     auto lo_index_idx = Concat(BvConst(0, 16), lut_data) - lut_start;

    //     auto lo_index_s = lo_index_idx >> Concat(BvConst(0, 24), lut_index_select);

    //     // Generate out-of-bounds flags
    //     auto lo_uflow = 
    //     Ite(Concat(BvConst(0, 16), lut_data) <= lut_start,
    //         BvConst(1, 1),
    //         BvConst(0, 1)
    //     );
    //     instr.SetUpdate(m.state("lo_uflow"), lo_uflow);

    //     auto lo_oflow = 
    //     Ite(Concat(BvConst(0, 16), lut_data) <= lut_start,
    //         BvConst(0, 1),
    //         Ite(lo_index_s >= 0x100,
    //             BvConst(1, 1),
    //             BvConst(0, 1)
    //         )
    //     );
    //     instr.SetUpdate(m.state("lo_oflow"), lo_oflow);

    //     // Determine final value for LUT index
    //     auto lo_index = 
    //     Ite(Concat(BvConst(0, 16), lut_data) <= lut_start,
    //         BvConst(0, 32),
    //         Ite(lo_index_s >= 0x100,
    //                 BvConst(256, 32),
    //                 lo_index_s
    //         )
    //     );
    //     lo_index = Extract(lo_index, 8, 0);
    //     instr.SetUpdate(m.state("lo_index"), lo_index);

    //     // Determine final value for LUT fraction
    //     auto lo_fraction = 
    //     Ite(Concat(BvConst(0, 16), lut_data) <= lut_start,
    //         BvConst(0, 32),
    //         Ite(lo_index_s >= 0x100,
    //                 BvConst(0, 32),
    //                 (lo_index_idx & ((BvConst(1, 32) << Concat(BvConst(0, 24), lut_index_select)) - 1)) >> (Concat(BvConst(0, 24), lut_index_select) - BvConst(35, 32))
    //         )
    //     );
    //     lo_fraction = Extract(lo_fraction, 8, 0);
    //     instr.SetUpdate(m.state("lo_fraction"), lo_fraction);
    // }

    // { // Present LUT output
    //     auto instr = m.NewInstr("Present_LUT_Output");
    //     auto reg_group = "group0_";

    //     // ... lut access type needs to be 1 for write, table id needs to be 1 for lo and 0 for le
    //     auto access_type = SelectBit(m.state(NVDLA_SDP_S_LUT_ACCESS_TYPE), 0);
    //     auto table_id = SelectBit(m.state(NVDLA_SDP_S_LUT_TABLE_ID), 0);
    //     auto lut_bypass = SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_LUT_BYPASS)), 0);

    //     instr.SetDecode(access_type == 0x0 & table_id == 0x1 & lut_bypass == 0x0);

    //     // LUT parameters
    //     auto le_index = Extract(m.state("le_index"), 6, 0);
    //     auto lo_index = Extract(m.state("lo_index"), 8, 0);

    //     auto le_fraction = Extract(m.state("le_fraction"), 6, 0);
    //     auto lo_fraction = Extract(m.state("lo_fraction"), 8, 0);

    //     auto le_uflow = SelectBit(m.state("le_uflow"), 0);
    //     auto lo_uflow = SelectBit(m.state("lo_uflow"), 0);

    //     auto le_oflow = SelectBit(m.state("le_oflow"), 0);
    //     auto lo_oflow = SelectBit(m.state("lo_oflow"), 0);

    //     auto le_miss = le_uflow == 0x1 | le_oflow == 0x1;
    //     auto le_hit = le_miss == 0x0;
    //     auto lo_miss = lo_uflow == 0x1 | lo_oflow == 0x1;        
    //     auto lo_hit = lo_miss == 0x0;

    //     // LUT priorities
    //     auto uflow_priority = SelectBit(m.state(NVDLA_SDP_S_LUT_UFLOW_PRIORITY), 0);
    //     auto oflow_priority = SelectBit(m.state(NVDLA_SDP_S_LUT_OFLOW_PRIORITY), 0);
    //     auto hybrid_priority = SelectBit(m.state(NVDLA_SDP_S_LUT_HYBRID_PRIORITY), 0);

    //     auto index = 
    //     Ite(le_uflow == 0x1 & lo_uflow == 0x1,
    //         Ite(uflow_priority == 0x1, lo_index, Concat(BvConst(0, 2), le_index)),
    //         Ite(le_oflow == 0x1 & lo_oflow == 0x1,
    //             Ite(oflow_priority == 0x1, lo_index, Concat(BvConst(0, 2), le_index)),
    //             Ite(le_hit == 0x1 & lo_hit == 0x1,
    //                 Ite(hybrid_priority == 0x1, lo_index, Concat(BvConst(0, 2), le_index)),
    //                 Ite(le_miss == 0x1 & lo_miss == 0x1,
    //                     Ite(hybrid_priority == 0x1, lo_index, Concat(BvConst(0, 2), le_index)),
    //                     Ite(le_hit == 0x1,
    //                         Concat(BvConst(0, 2), le_index),
    //                         lo_index
    //                     )
    //                 )
    //             )
    //         )
    //     );
    //     instr.SetUpdate(m.state("index"), index);
    
    //     auto fraction = 
    //     Ite(le_uflow == 0x1 & lo_uflow == 0x1,
    //         Ite(uflow_priority == 0x1, lo_fraction, Concat(BvConst(0, 2), le_fraction)),
    //         Ite(le_oflow == 0x1 & lo_oflow == 0x1,
    //             Ite(oflow_priority == 0x1, lo_fraction, Concat(BvConst(0, 2), le_fraction)),
    //             Ite(le_hit == 0x1 & lo_hit == 0x1,
    //                 Ite(hybrid_priority == 0x1, lo_fraction, Concat(BvConst(0, 2), le_fraction)),
    //                 Ite(le_miss == 0x1 & lo_miss == 0x1,
    //                     Ite(hybrid_priority == 0x1, lo_fraction, Concat(BvConst(0, 2), le_fraction)),
    //                     Ite(le_hit == 0x1,
    //                         Concat(BvConst(0, 2), le_fraction),
    //                         lo_fraction
    //                     )
    //                 )
    //             )
    //         )
    //     );
    //     instr.SetUpdate(m.state("fraction"), fraction);

    //     auto uflow = 
    //     Ite(le_uflow == 0x1 & lo_uflow == 0x1,
    //         Ite(uflow_priority == 0x1, lo_uflow, le_uflow),
    //         Ite(le_oflow == 0x1 & lo_oflow == 0x1,
    //             BvConst(0, 1),
    //             Ite(le_hit == 0x1 & lo_hit == 0x1,
    //                 BvConst(0, 1),
    //                 Ite(le_miss == 0x1 & lo_miss == 0x1,
    //                     Ite(hybrid_priority == 0x1, lo_uflow, le_uflow),
    //                     BvConst(0, 1)
    //                 )
    //             )
    //         )
    //     );
    //     instr.SetUpdate(m.state("uflow"), uflow);

    //     auto oflow = 
    //     Ite(le_uflow == 0x1 & lo_uflow == 0x1,
    //         BvConst(0, 1),
    //         Ite(le_oflow == 0x1 & lo_oflow == 0x1,
    //             Ite(oflow_priority == 0x1, lo_oflow, le_oflow),
    //             Ite(le_hit == 0x1 & lo_hit == 0x1,
    //                 BvConst(0, 1),
    //                 Ite(le_miss == 0x1 & lo_miss == 0x1,
    //                     Ite(hybrid_priority == 0x1, lo_oflow, le_oflow),
    //                     BvConst(0, 1)
    //                 )
    //             )
    //         )
    //     );
    //     instr.SetUpdate(m.state("oflow"), oflow);

    //     auto id = 
    //     Ite(le_uflow == 0x1 & lo_uflow == 0x1,
    //         Ite(uflow_priority == 0x1, BvConst(1, 1), BvConst(0, 1)),
    //         Ite(le_oflow == 0x1 & lo_oflow == 0x1,
    //             Ite(oflow_priority == 0x1, BvConst(1, 1), BvConst(0, 1)),
    //             Ite(le_hit == 0x1 & lo_hit == 0x1,
    //                 Ite(hybrid_priority == 0x1, BvConst(1, 1), BvConst(0, 1)),
    //                 Ite(le_miss == 0x1 & lo_miss == 0x1,
    //                     Ite(hybrid_priority == 0x1, BvConst(1, 1), BvConst(0, 1)),
    //                     Ite(le_hit == 0x1,
    //                         BvConst(0, 1),
    //                         BvConst(1, 1)
    //                     )
    //                 )
    //             )
    //         )
    //     );
    //     instr.SetUpdate(m.state("id"), id);
    // }

    // { // Perform interpolation or extrapolation
    //     auto instr = m.NewInstr("Final_LUT_Processing");
    //     auto reg_group = "group0_";

    //     // ... lut access type needs to be 1 for write, table id needs to be 1 for lo and 0 for le
    //     auto access_type = SelectBit(m.state(NVDLA_SDP_S_LUT_ACCESS_TYPE), 0);
    //     auto table_id = SelectBit(m.state(NVDLA_SDP_S_LUT_TABLE_ID), 0);
    //     auto lut_bypass = SelectBit(m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_LUT_BYPASS)), 0);
    //     auto lut_data = m.state(NVDLA_SDP_S_LUT_ACCESS_DATA);
    //     auto lut_function = SelectBit(m.state(NVDLA_SDP_S_LUT_LE_FUNCTION), 0); 

    //     instr.SetDecode(access_type == 0x0 & table_id == 0x1 & lut_bypass == 0x0);

    //     // LUT parameters
    //     auto index = Extract(m.state("index"), 8, 0);
    //     auto fraction = Extract(m.state("fraction"), 8, 0);
    //     auto oflow = SelectBit(m.state("oflow"), 0);
    //     auto uflow = SelectBit(m.state("uflow"), 0);
    //     auto id = SelectBit(m.state("id"), 0);

    //     // LUT outputs       
        
    //     auto y0 = Ite(id == 0x1, Load(m.state("lo_tbl"), index), Load(m.state("le_tbl"), Extract(index, 6, 0)));
    //     auto y1 = 
    //     Ite(id == 0x1,
    //         Ite(oflow == 0x0 & uflow == 0x1, Load(m.state("lo_tbl"), index + 1), BvConst(0, 16)),
    //         Ite(oflow == 0x0 & uflow == 0x1, Load(m.state("le_tbl"), Extract(index, 6, 0) + 1), BvConst(0, 16))
    //     );    

    //     auto bias = 
    //     Ite((oflow == 0x0 | uflow == 0x0) & id == 0x0 & lut_function == 0x0 & oflow == 0 & m.state(NVDLA_SDP_S_LUT_LE_INDEX_OFFSET) > 0x0, 
    //         BvConst(1, 8) << m.state(NVDLA_SDP_S_LUT_LE_INDEX_OFFSET), 
    //         BvConst(0, 8)
    //     );
        
    //     auto scale = 
    //     Ite(oflow == 0x1 | uflow == 0x1,
    //         Ite(id == 0x0,
    //             Ite(oflow == 0x1, m.state(NVDLA_SDP_S_LUT_LE_SLOPE_OFLOW_SCALE), m.state(NVDLA_SDP_S_LUT_LE_SLOPE_UFLOW_SCALE)),
    //             Ite(oflow == 0x1, m.state(NVDLA_SDP_S_LUT_LO_SLOPE_OFLOW_SCALE), m.state(NVDLA_SDP_S_LUT_LO_SLOPE_UFLOW_SCALE))
    //         ),
    //         BvConst(0, 16)
    //     );

    //     auto shift = 
    //     Ite(oflow == 0x1 | uflow == 0x1,
    //         Ite(id == 0x0,
    //             Ite(oflow == 0x1, m.state(NVDLA_SDP_S_LUT_LE_SLOPE_OFLOW_SHIFT), m.state(NVDLA_SDP_S_LUT_LE_SLOPE_UFLOW_SHIFT)),
    //             Ite(oflow == 0x1, m.state(NVDLA_SDP_S_LUT_LO_SLOPE_OFLOW_SHIFT), m.state(NVDLA_SDP_S_LUT_LO_SLOPE_UFLOW_SHIFT))
    //         ),
    //         BvConst(0, 5)
    //     );

    //     auto offset = 
    //     Ite(oflow == 0x1 | uflow == 0x1,
    //         Ite(id == 0x0,
    //             Ite(oflow == 0x1, m.state(NVDLA_SDP_S_LUT_LE_END), m.state(NVDLA_SDP_S_LUT_LE_START)),
    //             Ite(oflow == 0x1, m.state(NVDLA_SDP_S_LUT_LO_END), m.state(NVDLA_SDP_S_LUT_LO_START))
    //         ),
    //         BvConst(0, 32)
    //     );

    //     // Interpolation and extrapolation
    //     auto o =
    //     Ite(oflow == 0x1 | uflow == 0x1,
    //         Concat(BvConst(0, 3), Concat(BvConst(0, 16), y0) + ((Concat(BvConst(0, 16), lut_data) - Concat(BvConst(0, 24), bias) - offset) * Concat(BvConst(0, 16), scale)) >> Concat(BvConst(0, 27), shift)),
    //         (((BvConst(1, 35) << 35) - Concat(BvConst(0, 26), fraction)) * Concat(BvConst(0, 19), y0)) + (Concat(BvConst(0, 26), fraction) * Concat(BvConst(0, 19), y1))
    //     );

    //     auto oMax = (BvConst(1, 32) << 31) - 1;
    //     auto oMin = -(BvConst(1, 32) << 31);
    //     auto out_data =
    //     Ite(oflow == 0x1 | uflow == 0x1,
    //         Ite(o > Concat(BvConst(0, 3), oMax), 
    //             Concat(BvConst(0, 3), oMax),
    //             Ite(o < Concat(BvConst(0, 3), oMin),
    //                 Concat(BvConst(0, 3), oMin),
    //                 o
    //             )            
    //         ),
    //         o >> 35
    //     );
    // }

    // { // Perform conversion on inputs to the Y sub-unit
    //     // Determine the source of the operands
    //     auto flying_mode = m.state(GetVarName(reg_group, NVDLA_SDP_D_FLYING_MODE));
    //     auto data_source = m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_MUL_SRC));
    //     auto mul_shift = m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_MUL_SHIFT_VALUE));
    //     auto ew_mul_cvt_bypass = m.state(GetVarName(reg_group, NVDLA_SDP_D_EW_MUL_CVT_BYPASS));
    //     auto ew_mul_cvt_offset = m.state(GetVarName(reg_group, NVDLA_SDP_D_DP_EW_MUL_CVT_OFFSET_VALUE));
    //     auto ew_mul_cvt_scale = m.state(GetVarName(reg_group, NVDLA_SDP_D_DP_EW_MUL_CVT_SCALE_VALUE));
    //     auto ew_mul_cvt_truncate = m.state(GetVarName(reg_group, NVDLA_SDP_D_DP_EW_MUL_CVT_TRUNCATE_VALUE));

    //     for (size_t i = 0; i < 16; i++) {
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