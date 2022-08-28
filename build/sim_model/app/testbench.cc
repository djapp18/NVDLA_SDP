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

// File: testbench.cc (this is a copy of main.cc)
#include <systemc>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

#include "nlohmann/json.hpp"
#include "sdp.h"

using json = nlohmann::json;

std::string prog_frag_path;
std::string output_path;

// Module for reading data from prog_frag files
SC_MODULE(Source) {
  sc_in<bool> clk{"clk"};

  sc_out < sc_biguint<32> > sdp_cacc_data[16];
  sc_out < sc_biguint<32> > sdp_mrdma_data[16];
  sc_out < sc_biguint<16> > sdp_regs_data_alu[16];
  sc_out < sc_biguint<16> > sdp_regs_data_mult[16];
  sc_out < sc_biguint<16> > sdp_dma_data_alu[16];
  sc_out < sc_biguint<16> > sdp_dma_data_mult[16];

  sc_out < sc_biguint<22> > sdp_csb_addr;
  sc_out < sc_biguint<32> > sdp_csb_data;
  sc_out < sc_biguint<1> > sdp_csb_write;
  sc_out < sc_biguint<1> > sdp_csb_vld;
  sc_out < sc_biguint<1> > sdp_csb_rdy;

  sc_out < sc_biguint<1> > sdp_fifo_clr;
  sc_out < sc_biguint<1> > sdp_done;

  sc_out< sc_biguint<1> > input_done;

  SC_CTOR(Source) {
    SC_THREAD(source_input);
    sensitive << clk.pos();
  }

  // Read data from prog_frag files
  void source_input() {
    // Reset the ports
    std::fill(sdp_cacc_data, sdp_cacc_data + 16, 0);
    std::fill(sdp_mrdma_data, sdp_mrdma_data + 16, 0);
    std::fill(sdp_regs_data_alu, sdp_regs_data_alu + 16, 0);
    std::fill(sdp_regs_data_mult, sdp_regs_data_mult + 16, 0);
    std::fill(sdp_dma_data_alu, sdp_dma_data_alu + 16, 0);
    std::fill(sdp_dma_data_mult, sdp_dma_data_mult + 16, 0);

    sdp_csb_addr = 0;
    sdp_csb_data = 0;
    sdp_csb_write = 0;
    sdp_csb_vld = 0;
    sdp_csb_rdy = 0;

    sdp_fifo_clr = 0;
    sdp_done = 0;

    input_done = 0;

    wait(100, SC_NS);

    // Read program fragment from file
    std::ifstream fin;
    fin.open(prog_frag_path);
    std::cout << "if prog_frag file open? " << fin.is_open() << std::endl;
    json cmd_seq;
    cmd_seq = json::parse(fin);

    // Pass the command to the ports
    for (size_t i = 0; i < cmd_seq["program fragment"].size(); i++) {
      for (size_t j = 0; j < 16; j++) {
        sdp_cacc_data[j] = cmd_seq["program fragment"][i]["cacc_data_" + std::to_string(j)].get<int>();
        sdp_mrdma_data[j] = cmd_seq["program fragment"][i]["mrdma_data_" + std::to_string(j)].get<int>();
        sdp_regs_data_alu[j] = cmd_seq["program fragment"][i]["regs_data_alu_" + std::to_string(j)].get<int>();
        sdp_regs_data_mult[j] = cmd_seq["program fragment"][i]["regs_data_mult_" + std::to_string(j)].get<int>();
        sdp_dma_data_alu[j] = cmd_seq["program fragment"][i]["dma_data_alu_" + std::to_string(j)].get<int>();
        sdp_dma_data_mult[j] = cmd_seq["program fragment"][i]["dma_data_mult_" + std::to_string(j)].get<int>();
      }

      sdp_csb_addr = std::stoi((cmd_seq["program fragment"][i]["csb_addr"].get<std::string>()).c_str(), nullptr, 16);

      sdp_csb_data = cmd_seq["program fragment"][i]["csb_data"].get<int>();
      sdp_csb_write = cmd_seq["program fragment"][i]["csb_write"].get<int>();
      sdp_csb_vld = cmd_seq["program fragment"][i]["csb_vld"].get<int>();
      sdp_csb_rdy = cmd_seq["program fragment"][i]["csb_rdy"].get<int>();

      sdp_fifo_clr = cmd_seq["program fragment"][i]["fifo_clr"].get<int>();
      sdp_done = cmd_seq["program fragment"][i]["done"].get<int>();

      wait(10, SC_NS);
    }

    input_done = 1;
  }

};

// Link the input data from the prog_frag file with the signals in the sdp.h SystemC model
SC_MODULE(testbench) {
  sdp sdp_inst;
  Source src;

  sc_clock clk;
  sc_out < sc_biguint<32> > sdp_cacc_data_signal[16];
  sc_out < sc_biguint<32> > sdp_mrdma_data_signal[16];
  sc_out < sc_biguint<16> > sdp_regs_data_alu_signal[16];
  sc_out < sc_biguint<16> > sdp_regs_data_mult_signal[16];
  sc_out < sc_biguint<16> > sdp_dma_data_alu_signal[16];
  sc_out < sc_biguint<16> > sdp_dma_data_mult_signal[16];

  sc_out < sc_biguint<22> > sdp_csb_addr_signal{"sdp_csb_addr_signal"};
  sc_out < sc_biguint<32> > sdp_csb_data_signal{"sdp_csb_data_signal"};
  sc_out < sc_biguint<1> > sdp_csb_write_signal{"sdp_csb_write_signal"};
  sc_out < sc_biguint<1> > sdp_csb_vld_signal{"sdp_csb_vld_signal"};
  sc_out < sc_biguint<1> > sdp_csb_rdy_signal{"sdp_csb_rdy_signal"};

  sc_out < sc_biguint<1> > sdp_fifo_clr_signal{"sdp_fifo_clr_signal"};
  sc_out < sc_biguint<1> > sdp_done_signal{"sdp_done_signal"};

  sc_out< sc_biguint<1> > input_done{"input_done"};

  SC_CTOR(testbench) :
    clk("clk", 1, SC_NS),
    sdp_inst("sdp_inst"),
    src("source")
  {

    // Read in signals from the prog_frag file
    src.clk(clk);
    
    for (size_t i = 0; i < 16; i++) {
        src.sdp_cacc_data[i](sdp_cacc_data_signal[i]);
        src.sdp_mrdma_data[i](sdp_mrdma_data_signal[i]);
        src.sdp_regs_data_alu[i](sdp_regs_data_alu_signal[i]);
        src.sdp_regs_data_mult[i](sdp_regs_data_mult_signal[i]);
        src.sdp_dma_data_alu[i](sdp_dma_data_alu_signal[i]);
        src.sdp_dma_data_mult[i](sdp_dma_data_mult_signal[i]);
    }

    src.sdp_csb_addr(sdp_csb_addr_signal);
    src.sdp_csb_data(sdp_csb_data_signal);
    src.sdp_csb_write(sdp_csb_write_signal);
    src.sdp_csb_vld(sdp_csb_vld_signal);
    src.sdp_csb_rdy(sdp_csb_rdy_signal);

    src.sdp_fifo_clr(sdp_fifo_clr_signal);
    src.sdp_done(sdp_done_signal);

    src.input_done(input_done);

    // Link with the sdp.h SystemC model

    // All CACC input signals
    sdp_inst.sdp_cacc_data_0_in(sdp_cacc_data_signal[0]);
    sdp_inst.sdp_cacc_data_1_in(sdp_cacc_data_signal[1]);
    sdp_inst.sdp_cacc_data_2_in(sdp_cacc_data_signal[2]);
    sdp_inst.sdp_cacc_data_3_in(sdp_cacc_data_signal[3]);
    sdp_inst.sdp_cacc_data_4_in(sdp_cacc_data_signal[4]);
    sdp_inst.sdp_cacc_data_5_in(sdp_cacc_data_signal[5]);
    sdp_inst.sdp_cacc_data_6_in(sdp_cacc_data_signal[6]);
    sdp_inst.sdp_cacc_data_7_in(sdp_cacc_data_signal[7]);
    sdp_inst.sdp_cacc_data_8_in(sdp_cacc_data_signal[8]);
    sdp_inst.sdp_cacc_data_9_in(sdp_cacc_data_signal[9]);
    sdp_inst.sdp_cacc_data_10_in(sdp_cacc_data_signal[10]);
    sdp_inst.sdp_cacc_data_11_in(sdp_cacc_data_signal[11]);
    sdp_inst.sdp_cacc_data_12_in(sdp_cacc_data_signal[12]);
    sdp_inst.sdp_cacc_data_13_in(sdp_cacc_data_signal[13]);
    sdp_inst.sdp_cacc_data_14_in(sdp_cacc_data_signal[14]);
    sdp_inst.sdp_cacc_data_15_in(sdp_cacc_data_signal[15]);

    // All MRDMA input signals
    sdp_inst.sdp_mrdma_data_0_in(sdp_mrdma_data_signal[0]);
    sdp_inst.sdp_mrdma_data_1_in(sdp_mrdma_data_signal[1]);
    sdp_inst.sdp_mrdma_data_2_in(sdp_mrdma_data_signal[2]);
    sdp_inst.sdp_mrdma_data_3_in(sdp_mrdma_data_signal[3]);
    sdp_inst.sdp_mrdma_data_4_in(sdp_mrdma_data_signal[4]);
    sdp_inst.sdp_mrdma_data_5_in(sdp_mrdma_data_signal[5]);
    sdp_inst.sdp_mrdma_data_6_in(sdp_mrdma_data_signal[6]);
    sdp_inst.sdp_mrdma_data_7_in(sdp_mrdma_data_signal[7]);
    sdp_inst.sdp_mrdma_data_8_in(sdp_mrdma_data_signal[8]);
    sdp_inst.sdp_mrdma_data_9_in(sdp_mrdma_data_signal[9]);
    sdp_inst.sdp_mrdma_data_10_in(sdp_mrdma_data_signal[10]);
    sdp_inst.sdp_mrdma_data_11_in(sdp_mrdma_data_signal[11]);
    sdp_inst.sdp_mrdma_data_12_in(sdp_mrdma_data_signal[12]);
    sdp_inst.sdp_mrdma_data_13_in(sdp_mrdma_data_signal[13]);
    sdp_inst.sdp_mrdma_data_14_in(sdp_mrdma_data_signal[14]);
    sdp_inst.sdp_mrdma_data_15_in(sdp_mrdma_data_signal[15]);

    // All regs ALU input signals
    sdp_inst.sdp_regs_data_alu_0_in(sdp_regs_data_alu_signal[0]);
    sdp_inst.sdp_regs_data_alu_1_in(sdp_regs_data_alu_signal[1]);
    sdp_inst.sdp_regs_data_alu_2_in(sdp_regs_data_alu_signal[2]);
    sdp_inst.sdp_regs_data_alu_3_in(sdp_regs_data_alu_signal[3]);
    sdp_inst.sdp_regs_data_alu_4_in(sdp_regs_data_alu_signal[4]);
    sdp_inst.sdp_regs_data_alu_5_in(sdp_regs_data_alu_signal[5]);
    sdp_inst.sdp_regs_data_alu_6_in(sdp_regs_data_alu_signal[6]);
    sdp_inst.sdp_regs_data_alu_7_in(sdp_regs_data_alu_signal[7]);
    sdp_inst.sdp_regs_data_alu_8_in(sdp_regs_data_alu_signal[8]);
    sdp_inst.sdp_regs_data_alu_9_in(sdp_regs_data_alu_signal[9]);
    sdp_inst.sdp_regs_data_alu_10_in(sdp_regs_data_alu_signal[10]);
    sdp_inst.sdp_regs_data_alu_11_in(sdp_regs_data_alu_signal[11]);
    sdp_inst.sdp_regs_data_alu_12_in(sdp_regs_data_alu_signal[12]);
    sdp_inst.sdp_regs_data_alu_13_in(sdp_regs_data_alu_signal[13]);
    sdp_inst.sdp_regs_data_alu_14_in(sdp_regs_data_alu_signal[14]);
    sdp_inst.sdp_regs_data_alu_15_in(sdp_regs_data_alu_signal[15]);

    // All regs mult input signals
    sdp_inst.sdp_regs_data_mult_0_in(sdp_regs_data_mult_signal[0]);
    sdp_inst.sdp_regs_data_mult_1_in(sdp_regs_data_mult_signal[1]);
    sdp_inst.sdp_regs_data_mult_2_in(sdp_regs_data_mult_signal[2]);
    sdp_inst.sdp_regs_data_mult_3_in(sdp_regs_data_mult_signal[3]);
    sdp_inst.sdp_regs_data_mult_4_in(sdp_regs_data_mult_signal[4]);
    sdp_inst.sdp_regs_data_mult_5_in(sdp_regs_data_mult_signal[5]);
    sdp_inst.sdp_regs_data_mult_6_in(sdp_regs_data_mult_signal[6]);
    sdp_inst.sdp_regs_data_mult_7_in(sdp_regs_data_mult_signal[7]);
    sdp_inst.sdp_regs_data_mult_8_in(sdp_regs_data_mult_signal[8]);
    sdp_inst.sdp_regs_data_mult_9_in(sdp_regs_data_mult_signal[9]);
    sdp_inst.sdp_regs_data_mult_10_in(sdp_regs_data_mult_signal[10]);
    sdp_inst.sdp_regs_data_mult_11_in(sdp_regs_data_mult_signal[11]);
    sdp_inst.sdp_regs_data_mult_12_in(sdp_regs_data_mult_signal[12]);
    sdp_inst.sdp_regs_data_mult_13_in(sdp_regs_data_mult_signal[13]);
    sdp_inst.sdp_regs_data_mult_14_in(sdp_regs_data_mult_signal[14]);
    sdp_inst.sdp_regs_data_mult_15_in(sdp_regs_data_mult_signal[15]);

    // All DMA ALU input signals
    sdp_inst.sdp_dma_data_alu_0_in(sdp_dma_data_alu_signal[0]);
    sdp_inst.sdp_dma_data_alu_1_in(sdp_dma_data_alu_signal[1]);
    sdp_inst.sdp_dma_data_alu_2_in(sdp_dma_data_alu_signal[2]);
    sdp_inst.sdp_dma_data_alu_3_in(sdp_dma_data_alu_signal[3]);
    sdp_inst.sdp_dma_data_alu_4_in(sdp_dma_data_alu_signal[4]);
    sdp_inst.sdp_dma_data_alu_5_in(sdp_dma_data_alu_signal[5]);
    sdp_inst.sdp_dma_data_alu_6_in(sdp_dma_data_alu_signal[6]);
    sdp_inst.sdp_dma_data_alu_7_in(sdp_dma_data_alu_signal[7]);
    sdp_inst.sdp_dma_data_alu_8_in(sdp_dma_data_alu_signal[8]);
    sdp_inst.sdp_dma_data_alu_9_in(sdp_dma_data_alu_signal[9]);
    sdp_inst.sdp_dma_data_alu_10_in(sdp_dma_data_alu_signal[10]);
    sdp_inst.sdp_dma_data_alu_11_in(sdp_dma_data_alu_signal[11]);
    sdp_inst.sdp_dma_data_alu_12_in(sdp_dma_data_alu_signal[12]);
    sdp_inst.sdp_dma_data_alu_13_in(sdp_dma_data_alu_signal[13]);
    sdp_inst.sdp_dma_data_alu_14_in(sdp_dma_data_alu_signal[14]);
    sdp_inst.sdp_dma_data_alu_15_in(sdp_dma_data_alu_signal[15]);

    // All DMA mult input signals
    sdp_inst.sdp_dma_data_mult_0_in(sdp_dma_data_mult_signal[0]);
    sdp_inst.sdp_dma_data_mult_1_in(sdp_dma_data_mult_signal[1]);
    sdp_inst.sdp_dma_data_mult_2_in(sdp_dma_data_mult_signal[2]);
    sdp_inst.sdp_dma_data_mult_3_in(sdp_dma_data_mult_signal[3]);
    sdp_inst.sdp_dma_data_mult_4_in(sdp_dma_data_mult_signal[4]);
    sdp_inst.sdp_dma_data_mult_5_in(sdp_dma_data_mult_signal[5]);
    sdp_inst.sdp_dma_data_mult_6_in(sdp_dma_data_mult_signal[6]);
    sdp_inst.sdp_dma_data_mult_7_in(sdp_dma_data_mult_signal[7]);
    sdp_inst.sdp_dma_data_mult_8_in(sdp_dma_data_mult_signal[8]);
    sdp_inst.sdp_dma_data_mult_9_in(sdp_dma_data_mult_signal[9]);
    sdp_inst.sdp_dma_data_mult_10_in(sdp_dma_data_mult_signal[10]);
    sdp_inst.sdp_dma_data_mult_11_in(sdp_dma_data_mult_signal[11]);
    sdp_inst.sdp_dma_data_mult_12_in(sdp_dma_data_mult_signal[12]);
    sdp_inst.sdp_dma_data_mult_13_in(sdp_dma_data_mult_signal[13]);
    sdp_inst.sdp_dma_data_mult_14_in(sdp_dma_data_mult_signal[14]);
    sdp_inst.sdp_dma_data_mult_15_in(sdp_dma_data_mult_signal[15]);

    // All CSB input signals
    sdp_inst.sdp_csb_addr_in(sdp_csb_addr_signal);
    sdp_inst.sdp_csb_data_in(sdp_csb_data_signal);
    sdp_inst.sdp_csb_write_in(sdp_csb_write_signal);
    sdp_inst.sdp_csb_vld_in(sdp_csb_vld_signal);
    sdp_inst.sdp_csb_rdy_in(sdp_csb_rdy_signal);

    // All remaining input signals
    sdp_inst.sdp_fifo_clr_in(sdp_fifo_clr_signal);
    sdp_inst.sdp_done_in(sdp_done_signal);

    SC_THREAD(run);
  }

  // Run the SystemC simuation and log outputs
  void run() {
    // Log instructions activated during the simulation
    sdp_inst.instr_log.open("instr_log.txt", ios::out | ios::trunc);
    sdp_inst.instr_update_log.open("instr_update_log.txt", ios::out | ios::trunc);

    // Run simulation
    std::cout << "start running" << std::endl;
    std::cout << "*********** simulation start ***********" << std::endl;
    wait(10, SC_NS);

    while (input_done == 0) {
    //  std::cout << "current simulation time: " << '\t' << sc_time_stamp() << "\r" << std::flush;
      wait(10, SC_NS);
    }

    wait(1000, SC_NS);

    // Log final outputs
    std::ofstream fout;
    fout.open(output_path, ios::out | ios::trunc);
 
    fout << "    sdp_pdp_output_0 => " << std::hex << "0x" << sdp_inst.sdp_pdp_output_0 << std::endl; 
    fout << "    sdp_pdp_output_1 => " << std::hex << "0x" << sdp_inst.sdp_pdp_output_1 << std::endl; 
    fout << "    sdp_pdp_output_2 => " << std::hex << "0x" << sdp_inst.sdp_pdp_output_2 << std::endl; 
    fout << "    sdp_pdp_output_3 => " << std::hex << "0x" << sdp_inst.sdp_pdp_output_3 << std::endl; 
    fout << "    sdp_pdp_output_4 => " << std::hex << "0x" << sdp_inst.sdp_pdp_output_4 << std::endl; 
    fout << "    sdp_pdp_output_5 => " << std::hex << "0x" << sdp_inst.sdp_pdp_output_5 << std::endl; 
    fout << "    sdp_pdp_output_6 => " << std::hex << "0x" << sdp_inst.sdp_pdp_output_6 << std::endl; 
    fout << "    sdp_pdp_output_7 => " << std::hex << "0x" << sdp_inst.sdp_pdp_output_7 << std::endl; 
    fout << "    sdp_pdp_output_8 => " << std::hex << "0x" << sdp_inst.sdp_pdp_output_8 << std::endl; 
    fout << "    sdp_pdp_output_9 => " << std::hex << "0x" << sdp_inst.sdp_pdp_output_9 << std::endl; 
    fout << "    sdp_pdp_output_10 => " << std::hex << "0x" << sdp_inst.sdp_pdp_output_10 << std::endl; 
    fout << "    sdp_pdp_output_11 => " << std::hex << "0x" << sdp_inst.sdp_pdp_output_11 << std::endl; 
    fout << "    sdp_pdp_output_12 => " << std::hex << "0x" << sdp_inst.sdp_pdp_output_12 << std::endl; 
    fout << "    sdp_pdp_output_13 => " << std::hex << "0x" << sdp_inst.sdp_pdp_output_13 << std::endl; 
    fout << "    sdp_pdp_output_14 => " << std::hex << "0x" << sdp_inst.sdp_pdp_output_14 << std::endl; 
    fout << "    sdp_pdp_output_15 => " << std::hex << "0x" << sdp_inst.sdp_pdp_output_15 << std::endl; 

    fout.close();
    std::cout << "outputs have been store at " << output_path << std::endl;

    wait(100000, SC_NS);
    std::cout << '\n' << std::endl;
    std::cout << "************* sc_stop **************" << std::endl;

    sdp_inst.instr_log.close();
    sc_stop(); 
  }
};

// Main function
int sc_main(int argc, char* argv[]) {

  // Dummy ports
  sc_signal < sc_biguint<32> > sdp_cacc_data_main[16];
  sc_signal < sc_biguint<32> > sdp_mrdma_data_main[16];
  sc_signal < sc_biguint<16> > sdp_regs_data_alu_main[16];
  sc_signal < sc_biguint<16> > sdp_regs_data_mult_main[16];
  sc_signal < sc_biguint<16> > sdp_dma_data_alu_main[16];
  sc_signal < sc_biguint<16> > sdp_dma_data_mult_main[16];

  sc_signal < sc_biguint<22> > sdp_csb_addr_main;
  sc_signal < sc_biguint<32> > sdp_csb_data_main;
  sc_signal < sc_biguint<1> > sdp_csb_write_main;
  sc_signal < sc_biguint<1> > sdp_csb_vld_main;
  sc_signal < sc_biguint<1> > sdp_csb_rdy_main;

  sc_signal < sc_biguint<1> > sdp_fifo_clr_main;
  sc_signal < sc_biguint<1> > sdp_done_main;

  sc_signal< sc_biguint<1> > input_done_main;

  // Parse input file name
  std::string file_name;
  file_name = argv[1];
  prog_frag_path = "../prog_frag/" + file_name + "_input.json";
  output_path = "../result/" + file_name + "_out.json";
  std::cout << "executing " << file_name << std::endl;

  // Begin simulation
  testbench tb("tb");

  // Linking to dummy ports
  for (size_t i = 0; i < 16; i++) {
      tb.sdp_cacc_data_signal[i](sdp_cacc_data_main[i]);
      tb.sdp_mrdma_data_signal[i](sdp_mrdma_data_main[i]);
      tb.sdp_regs_data_alu_signal[i](sdp_regs_data_alu_main[i]);
      tb.sdp_regs_data_mult_signal[i](sdp_regs_data_mult_main[i]);
      tb.sdp_dma_data_alu_signal[i](sdp_dma_data_alu_main[i]);
      tb.sdp_dma_data_mult_signal[i](sdp_dma_data_mult_main[i]);
  }

  tb.sdp_csb_addr_signal(sdp_csb_addr_main);
  tb.sdp_csb_data_signal(sdp_csb_data_main);
  tb.sdp_csb_write_signal(sdp_csb_write_main);
  tb.sdp_csb_vld_signal(sdp_csb_vld_main);
  tb.sdp_csb_rdy_signal(sdp_csb_rdy_main);

  tb.sdp_fifo_clr_signal(sdp_fifo_clr_main);
  tb.sdp_done_signal(sdp_done_main);

  tb.input_done(input_done_main);

  sc_start();

  return 0;
}
