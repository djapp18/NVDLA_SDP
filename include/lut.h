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

// File: lut.h

#ifndef NVDLA_LUT_H__
#define NVDLA_LUT_H__

#include <string>
#include <ilang/ilang++.h>
#include <sdp_config.h>
#include <datapath_structs.h>

namespace ilang {

// =============================================================================
// LUT read operations - found in lut.cc
// =============================================================================

LutData ReadLutLE(Ila& m, ExprRef input);
LutData ReadLutLO(Ila& m, ExprRef input);
LutData SelectLut(Ila& m, LutData& le_data, LutData& lo_data);
ExprRef PresentLutOutput(Ila& m, ExprRef input, LutData& data);
ExprRef ReadLut(Ila& m, ExprRef input);

} // namespace ilang

#endif // NVDLA_LUT_H__
