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

// File: components.h

#ifndef NVDLA_COMPONENTS_H__
#define NVDLA_COMPONENTS_H__

#include <string>
#include <ilang/ilang++.h>
#include <sdp_config.h>
#include <datapath_structs.h>

namespace ilang {

// =============================================================================
// General datapath components - found in components.cc
// =============================================================================

// Digital logic
ExprRef TISH_XOR(ExprRef a, ExprRef b, ExprRef c);

// IntLog2
ExprRef IntLog2(ExprRef operand);
ExprRef IntLog2Frac(ExprRef operand, ExprRef index);

// Signed operation
ExprRef AllOnes(size_t length);
ExprRef TwoComp(ExprRef a, size_t length);
ExprRef SignedGreater(ExprRef a, ExprRef b, size_t length);
ExprRef SignExtend(ExprRef input, size_t input_length, size_t output_length);
ExprRef Saturation(ExprRef input, size_t length);

// Datapath computation
ExprRef ALU(ExprRef input, ExprRef operand, OpInfo& o);
ExprRef Mult(ExprRef input, ExprRef operand, OpInfo& o);

} // namespace ilang

#endif // NVDLA_COMPONENTS_H__
