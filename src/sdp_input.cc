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

// File: sdp_input.cc

#include <ilang/ilang++.h>

#include <sdp_config.h>

namespace ilang {

void DefineSDPInput(Ila& m) {
    // CSB MMIO
    // Total should be 63 bits, omit 8 bits for now
    m.NewBvInput("csb_addr", 22); // need to offset for 2 bit
    m.NewBvInput("csb_data", 32);
    m.NewBvInput("csb_write", 1);
    // m.NewBvInput("csb_other", 8);
    m.NewBvInput("csb_vld", 1);
    m.NewBvInput("csb_rdy", 1);

    // Control Signals
    m.NewBvInput("fifo_clr", 1);
    m.NewBvInput("done", 1);

    // Datapath
    for (int i = 0; i < 16; i++) {
        m.NewBvInput(GetVarName("cacc_data_", (std::to_string(i))), 32);
        m.NewBvInput(GetVarName("mrdma_data_", (std::to_string(i))), 32);
        
        m.NewBvInput(GetVarName("regs_data_", (std::to_string(i))), 16);
        m.NewBvInput(GetVarName("dma_data_", (std::to_string(i))), 16);
    }
}


} // namespace ilang