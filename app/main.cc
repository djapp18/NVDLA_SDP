// =============================================================================
// MIT License
//
// Copyright (c) 2019 Princeton University
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

// File: main.cc
#include <sdp.h>

#include <ilang/ilang++.h>
#include <ilang/util/log.h>
#include <iostream>
//#include <ilang/vtarget-out/vtarget_gen.h>
using namespace std;

using namespace ilang;


int main(int argc, char* argv[]) {
  SetToStdErr(1);

  cout << "start";
  // get the ILA model
  auto sdp = GetSDPIla("sdp");

  ILA_INFO << "#instr: " << sdp.instr_num();
  for (size_t i = 0; i < sdp.instr_num(); i++) {
    ILA_INFO << sdp.instr(i);
  }

  ILA_INFO << "#state: " << sdp.state_num();
  // auto total = 0;
  for (size_t i = 0; i < sdp.state_num(); i++) {
    ILA_INFO << sdp.state(i);
    // total += sdp.state(i).bit_width();
  }
  // cout << "\n";
  // cout << total;
  // cout << "\n";

  ILA_INFO << "#input: " << sdp.input_num();
  for (size_t i = 0; i < sdp.input_num(); i++) {
    ILA_INFO << sdp.input(i);
  }

//  std::vector<std::string> csc_design_files = {
//    "NV_NVDLA_csc_top.v"
//  };

//  auto vtg_cfg = SetConfiguration();

//  verify_csc(csc, vtg_cfg, csc_design_files);

  ExportSysCSim(sdp, "./sim_model");  

  return 0;
}

