#include <systemc>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

#include "nlohmann/json.hpp"
#include "sdp.h"

using json = nlohmann::json;

// input 1: csb_in -> relu_bypass = 0
// input 2: csb_in -> lut_bypass = 1
// input 3: csb_in -> producer = 1
// input 4: csb_in -> enable group 0
// input 5: data in, buffer not empty (csb_in arbitrary for group 1)
// input 6: data in, buffer empty 
// input 7: csb_in -> ...

std::string prog_frag_path;
std::string output_path;

SC_MODULE(Source) {
  sc_in<bool> clk{"clk"};

  sc_out < sc_biguint<32> > sdp_cacc_data[16];
  sc_out < sc_biguint<32> > sdp_mrdma_data[16];
  sc_out < sc_biguint<16> > sdp_regs_data[16];
  sc_out < sc_biguint<16> > sdp_dma_data[16];

  sc_out < sc_biguint<22> > sdp_csb_addr;
  sc_out < sc_biguint<32> > sdp_csb_data;
  sc_out < sc_biguint<1> > sdp_csb_write;
  sc_out < sc_biguint<1> > sdp_csb_vld;

  sc_out < sc_biguint<1> > sdp_fifo_clr;
  sc_out < sc_biguint<1> > sdp_done;

  sc_out< sc_biguint<1> > input_done;

  SC_CTOR(Source) {
    SC_THREAD(source_input);
    sensitive << clk.pos();
  }

  void source_input() {
    // reset the ports
    sdp_cacc_data = {};
    sdp_mrdma_data = {};
    sdp_regs_data = {};
    sdp_dma_data = {};

    sdp_csb_addr = 0;
    sdp_csb_data = 0;
    sdp_csb_write = 0;
    sdp_csb_vld = 0;

    sdp_fifo_clr = 0;
    sdp_done = 0;

    input_done = 0;

    wait(100, SC_NS);

    // read program fragment from file
    std::ifstream fin;
    fin.open(prog_frag_path);
    std::cout << "if prog_frag file open? " << fin.is_open() << std::endl;
    json cmd_seq;
    fin >> cmd_seq;

    // pass the command to the ports
    for (int i = 0; i < cmd_seq["program fragment"].size(); i++) {
      for (int j = 0; j < 16; j++) {
        sdp_cacc_data[j] = cmd_seq["program fragment"][i]["cacc_data_" + j].get<int>();
        sdp_mrdma_data[j] = cmd_seq["program fragment"][i]["mrdma_data_" + j].get<int>();
        sdp_regs_data[j] = cmd_seq["program fragment"][i]["regs_data" + j].get<int>();
        sdp_dma_data[j] = cmd_seq["program fragment"][i]["dma_data_" + j].get<int>();
      }

      sdp_csb_addr = (cmd_seq["program fragment"][i]["csb_addr"].get<std::string>()).c_str();
      sdp_csb_data = cmd_seq["program fragment"][i]["csb_data"].get<int>();
      sdp_csb_write = cmd_seq["program fragment"][i]["csb_write"].get<int>();
      sdp_csb_vld = cmd_seq["program fragment"][i]["csb_vld"].get<int>();

      sdp_fifo_clr = cmd_seq["program fragment"][i]["fifo_clr"].get<int>();
      sdp_done = cmd_seq["program fragment"][i]["done"].get<int>();

      wait(10, SC_NS);
    }

    input_done = 1;
  }

};

SC_MODULE(testbench) {
  sdp sdp_inst;
  Source src;

  sc_clock clk;
  sc_out < sc_biguint<32> > sdp_cacc_data_signal[16];
  sc_out < sc_biguint<32> > sdp_mrdma_data_signal[16];
  sc_out < sc_biguint<16> > sdp_regs_data_signal[16];
  sc_out < sc_biguint<16> > sdp_dma_data_signal[16];

  sc_out < sc_biguint<22> > sdp_csb_addr_signal;
  sc_out < sc_biguint<32> > sdp_csb_data_signal;
  sc_out < sc_biguint<1> > sdp_csb_write_signal;
  sc_out < sc_biguint<1> > sdp_csb_vld_signal;

  sc_out < sc_biguint<1> > sdp_fifo_clr_signal;
  sc_out < sc_biguint<1> > sdp_done_signal;

  sc_out< sc_biguint<1> > input_done;

  SC_CTOR(testbench) :
    clk("clk", 1, SC_NS),
    sdp_inst("sdp_inst"),
    src("source")
  {
    src.clk(clk);
    src.sdp_cacc_data(sdp_cacc_data_signal);
    src.sdp_mrdma_data(sdp_mrdma_data_signal);
    src.sdp_regs_data(sdp_regs_data_signal);
    src.sdp_dma_data(sdp_dma_data_signal);

    src.sdp_csb_addr(sdp_csb_addr_signal);
    src.sdp_csb_data(sdp_csb_data_signal);
    src.sdp_csb_write(sdp_csb_write_signal);
    src.sdp_csb_vld(sdp_csb_vld_signal);

    src.sdp_fifo_clr(sdp_fifo_clr_signal);
    src.sdp_done(sdp_done_signal);

    src.input_done(input_done);



    // will need to use a for loop here; basically, look at the corresponding signals in sdp.h
    for (int i = 0; i < 16; i++) {

    }

    sdp_inst.sdp_sdp_top_cacc_data_in(sdp_cacc_data_signal);
    sdp_inst.sdp_sdp_vir_mrdma_data_in(sdp_mrdma_data_signal);
    sdp_inst.sdp_sdp_vir_regs_data_in(sdp_regs_data_signal);
    sdp_inst.sdp_sdp_vir_dma_data_in(sdp_dma_data_signal);

    SC_THREAD(run);
  }

  void run() {
    sdp_inst.instr_log.open("instr_log.txt", ios::out | ios::trunc);
    sdp_inst.instr_update_log.open("instr_update_log.txt", ios::out | ios::trunc);

    std::cout << "start running" << std::endl;
    std::cout << "*********** simulation start ***********" << std::endl;
    wait(10, SC_NS);

    while (input_done == 0) {
    //  std::cout << "current simulation time: " << '\t' << sc_time_stamp() << "\r" << std::flush;
      wait(10, SC_NS);
    }

    wait(1000, SC_NS);

    ofstream fout;
    fout.open(output_path, ios::out | ios::trunc);

    fout << "    sdp_pdp_output_0 => " << std::hex << "0x" << sdp_pdp_output_0 << std::endl; 
    fout << "    sdp_pdp_output_1 => " << std::hex << "0x" << sdp_pdp_output_1 << std::endl; 
    fout << "    sdp_pdp_output_10 => " << std::hex << "0x" << sdp_pdp_output_10 << std::endl; 
    fout << "    sdp_pdp_output_11 => " << std::hex << "0x" << sdp_pdp_output_11 << std::endl; 
    fout << "    sdp_pdp_output_12 => " << std::hex << "0x" << sdp_pdp_output_12 << std::endl; 
    fout << "    sdp_pdp_output_13 => " << std::hex << "0x" << sdp_pdp_output_13 << std::endl; 
    fout << "    sdp_pdp_output_14 => " << std::hex << "0x" << sdp_pdp_output_14 << std::endl; 
    fout << "    sdp_pdp_output_15 => " << std::hex << "0x" << sdp_pdp_output_15 << std::endl; 
    fout << "    sdp_pdp_output_2 => " << std::hex << "0x" << sdp_pdp_output_2 << std::endl; 
    fout << "    sdp_pdp_output_3 => " << std::hex << "0x" << sdp_pdp_output_3 << std::endl; 
    fout << "    sdp_pdp_output_4 => " << std::hex << "0x" << sdp_pdp_output_4 << std::endl; 
    fout << "    sdp_pdp_output_5 => " << std::hex << "0x" << sdp_pdp_output_5 << std::endl; 
    fout << "    sdp_pdp_output_6 => " << std::hex << "0x" << sdp_pdp_output_6 << std::endl; 
    fout << "    sdp_pdp_output_7 => " << std::hex << "0x" << sdp_pdp_output_7 << std::endl; 
    fout << "    sdp_pdp_output_8 => " << std::hex << "0x" << sdp_pdp_output_8 << std::endl; 
    fout << "    sdp_pdp_output_9 => " << std::hex << "0x" << sdp_pdp_output_9 << std::endl; 

    fout << std::setw(2) << result;

    fout.close();
    std::cout << "outputs have been store at " << output_path << std::endl;

    wait(100000, SC_NS);
    std::cout << '\n' << std::endl;
    std::cout << "************* sc_stop **************" << std::endl;

    sdp_inst.instr_log.close();
    sc_stop(); 
  }
};

int sc_main(int argc, char* argv[]) {

  std::string file_name;
  file_name = argv[1];
  prog_frag_path = "./prog_frag/" + file_name + "_input.json";
  output_path = "./result/" + file_name + "_out.json";
  std::cout << "executing " << file_name << std::endl;
  testbench tb("tb");
  sc_start();

  return 0;
}
