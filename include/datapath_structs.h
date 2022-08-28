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

// File: datapath_structs.h

#ifndef NVDLA_DATAPATH_STRUCTS_H__
#define NVDLA_DATAPATH_STRUCTS_H__

#include <string>
#include <ilang/ilang++.h>
#include <sdp_config.h>

namespace ilang {

// =============================================================================
// Datapath structs
// =============================================================================

// Describe metadata of a register group (i.e., group 0 or group 1) 
struct RegGroup {
    string group_name;
    ExprRef group_num;
    ExprRef group_unset;
};

// Describe metadata of individual computational operations
struct OpInfo {
    string op_name;
    int op_id;
};

// Describe metadata of a LUT sub-table (i.e., LE or LO)
struct LutInfo {
    string lut_name;
    int lut_id;
    int lut_size;
};

// Bundle data relevant to the LUT
struct LutData {
    ExprRef id;
    ExprRef uflow;
    ExprRef oflow;
    ExprRef index;
    ExprRef fraction;
};

} // namespace ilang

#endif // NVDLA_DATAPATH_STRUCTS_H__
