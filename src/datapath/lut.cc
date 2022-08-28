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

// File: lut.cc

#include <ilang/ilang++.h>
#include <iostream>
using namespace std;
#include <sdp_config.h>
#include <datapath_structs.h>
#include <components.h>

namespace ilang {

// =============================================================================
// LUT read operations
// =============================================================================

// Read index from the LE (Linear/Exponential) LUT sub-table
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
        le_index_idx - SignExtend(lut_index_offset, 8, 32),
        le_index_idx >> SignExtend(lut_index_select, 8, 32)
    );

    // Generate out-of-bounds flags
    auto le_uflow = 
    Ite(lut_function == 0x0,
        Ite(input <= lut_start,
            BvConst(1, 1),
            Ite(le_index_idx < SignExtend(lut_index_offset, 8, 32),
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
            BvConst(0, 32),
            Ite(le_index_idx < Concat(BvConst(0, 24), lut_index_offset),
                BvConst(0, 32),
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

// Read index from the LO (Linear Only) LUT sub-table
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

// Select LUT sub-table based on a combination of user preference and overflow/underflow logic
LutData SelectLut(Ila& m, LutData& le_data, LutData& lo_data) {
    // LUT parameters - note that these are all unsigned
    auto le_index = le_data.index;
    auto lo_index = lo_data.index;

    auto le_fraction = le_data.fraction;
    auto lo_fraction = lo_data.fraction;

    auto le_uflow = le_data.uflow;
    auto lo_uflow = lo_data.uflow;

    auto le_oflow = le_data.oflow;
    auto lo_oflow = lo_data.oflow;

    // Overflow/underflow logic for the two LUT sub-tables. There are six combinations possible:
    // le_uflow & lo_uflow, le_oflow & lo_oflow, le_hit & lo_hit, le_miss & lo_miss, le_hit, lo_hit
    auto le_miss = (le_uflow == 0x1) | (le_oflow == 0x1);
    auto le_hit = le_miss == 0x0;
    auto lo_miss = (lo_uflow == 0x1) | (lo_oflow == 0x1);        
    auto lo_hit = lo_miss == 0x0;

    // LUT priorities serve as user-defined tiebreakers whenever le_hit XNOR lo_hit
    auto uflow_priority = SelectBit(m.state(NVDLA_SDP_S_LUT_UFLOW_PRIORITY), 0);                // Activates in le_uflow & lo_uflow case
    auto oflow_priority = SelectBit(m.state(NVDLA_SDP_S_LUT_OFLOW_PRIORITY), 0);                // Activates in le_oflow & lo_oflow case
    auto hybrid_priority = SelectBit(m.state(NVDLA_SDP_S_LUT_HYBRID_PRIORITY), 0);              // Activates in le_hit & lo_hit and le_miss & lo_miss cases

    // Select index by considering every combination of overflow/underflow outcomes and LUT priorities
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

    // Select fraction by considering every combination of overflow/underflow outcomes and LUT priorities
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

    // Set uflow by considering every combination of overflow/underflow outcomes and LUT priorities
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

    // Set uflow by considering every combination of overflow/underflow outcomes and LUT priorities
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

    // Set LUT sub-table id by considering every combination of overflow/underflow outcomes and LUT priorities
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

// Present final LUT output by using interpolation/extrapolation
ExprRef PresentLutOutput(Ila& m, ExprRef input, LutData& data) {
    // LUT parameters - note that these are all unsigned
    auto index = data.index;
    auto fraction = data.fraction;
    auto oflow = data.oflow;
    auto uflow = data.uflow;
    auto id = data.id;
    auto lut_function = SelectBit(m.state(NVDLA_SDP_S_LUT_LE_FUNCTION), 0);

    // Determine LUT outputs
    auto y0 = Ite(id == 0x1, Load(m.state("lo_tbl"), index), Load(m.state("le_tbl"), Extract(index, 6, 0)));
    auto y1 = 
    Ite(id == 0x1,
        Ite((oflow == 0x0) & (uflow == 0x0), Load(m.state("lo_tbl"), index + 1), BvConst(0, 16)),
        Ite((oflow == 0x0) & (uflow == 0x0), Load(m.state("le_tbl"), Extract(index, 6, 0) + 1), BvConst(0, 16))
    );    

    // Determine parameters relevant for interpolation/extrapolation
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

    // If oflow | uflow is true, perform extrapolation. Otherwise, perform interpolation.
    auto o =
    Ite((oflow == 0x1) | (uflow == 0x1),
        Concat(BvConst(0, 3), Concat(BvConst(0, 16), y0) + ((input - Concat(BvConst(0, 24), bias) - offset) * Concat(BvConst(0, 16), scale)) >> Concat(BvConst(0, 27), shift)),
        (((BvConst(1, 35) << 35) - Concat(BvConst(0, 26), fraction)) * Concat(BvConst(0, 19), y0)) + (Concat(BvConst(0, 26), fraction) * Concat(BvConst(0, 19), y1))
    );

    // Present LUT output - further work is needed to determine proper bit widths here
    auto out_data =
    Ite((oflow == 0x1) | (uflow == 0x1),
        SignedConvert(Saturation(SignedConvert(o, 35, 64), 32), 64, 35),
        o >> 35
    );

    return SignedConvert(out_data, 35, 32);
}

// Wrapper function to perform LUT read operation
ExprRef ReadLut(Ila& m, ExprRef input) {
    auto le_data = ReadLutLE(m, input);
    auto lo_data = ReadLutLO(m, input);
    auto data = SelectLut(m, le_data, lo_data);
    auto output = PresentLutOutput(m, input, data);

    return output;
}

} // namespace ilang