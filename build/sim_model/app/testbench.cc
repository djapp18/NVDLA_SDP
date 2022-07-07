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

SC_MODULE(Source) {
  sc_in<bool> clk{"clk"};

  sc_out < sc_biguint<32> > sdp_cacc_data[16];
  sc_out < sc_biguint<32> > sdp_mrdma_data[16];
  sc_out < sc_biguint<16> > sdp_regs_data[16];
  sc_out < sc_biguint<16> > sdp_dma_data[16];
  
  sc_out< sc_biguint<1> > input_done;

  SC_CTOR(Source) {
    SC_THREAD(source_input);
    sensitive << clk.pos();
  }

  void source_input() {
    // reset the port
    vta_instr_in = 0;
    vta_mem_mode_in = 0;
    vta_mem_addr_in = 0;
    vta_mem_uop_data_in = 0;
    vta_mem_inp_data_in = 0;
    vta_mem_wgt_data_in = 0;
    vta_mem_bias_data_in = 0;

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
      std::string instr = cmd_seq["program fragment"][i]["instr_in"].get<std::string>();
      vta_instr_in = instr.c_str();
      vta_mem_mode_in = cmd_seq["program fragment"][i]["mem_mode"].get<int>();
      vta_mem_addr_in = cmd_seq["program fragment"][i]["mem_idx"].get<int>();
      vta_mem_uop_data_in = 
        (cmd_seq["program fragment"][i]["mem_uop_in"].get<std::string>()).c_str();
      vta_mem_inp_data_in = 
        (cmd_seq["program fragment"][i]["mem_inp_in"].get<std::string>()).c_str();
      vta_mem_wgt_data_in = 
        (cmd_seq["program fragment"][i]["mem_wgt_in"].get<std::string>()).c_str();
      vta_mem_bias_data_in = 
        (cmd_seq["program fragment"][i]["mem_bias_in"].get<std::string>()).c_str();

      wait(10, SC_NS);
    }

    input_done = 1;
  }

};

SC_MODULE(testbench) {
  vta vta_inst;
  Source src;

  sc_clock clk;
  sc_signal< sc_biguint<128> > vta_instr_in_signal;
  sc_signal< sc_biguint<3> > vta_mem_mode_in_signal;
  sc_signal< sc_biguint<32> > vta_mem_addr_in_signal;
  sc_signal< sc_biguint<32> > vta_mem_uop_data_in_signal;
  sc_signal< sc_biguint<8> > vta_mem_inp_data_in_signal;
  sc_signal< sc_biguint<8> > vta_mem_wgt_data_in_signal;
  sc_signal< sc_biguint<32> > vta_mem_bias_data_in_signal;

  sc_signal< sc_biguint<1> > input_done;

  SC_CTOR(testbench) :
    clk("clk", 1, SC_NS),
    vta_inst("vta_inst"),
    src("source")
  {
    src.clk(clk);
    src.vta_instr_in(vta_instr_in_signal);
    src.vta_mem_mode_in(vta_mem_mode_in_signal);
    src.vta_mem_addr_in(vta_mem_addr_in_signal);
    src.vta_mem_uop_data_in(vta_mem_uop_data_in_signal);
    src.vta_mem_inp_data_in(vta_mem_inp_data_in_signal);
    src.vta_mem_wgt_data_in(vta_mem_wgt_data_in_signal);
    src.vta_mem_bias_data_in(vta_mem_bias_data_in_signal);

    src.input_done(input_done);
  

    vta_inst.vta_vta_top_instr_in_in(vta_instr_in_signal);
    vta_inst.vta_vta_vir_mem_mode_in_in(vta_mem_mode_in_signal);
    vta_inst.vta_vta_vir_mem_addr_in_in(vta_mem_addr_in_signal);
    vta_inst.vta_vta_vir_mem_uop_data_in_in(vta_mem_uop_data_in_signal);
    vta_inst.vta_vta_vir_mem_inp_data_in_in(vta_mem_inp_data_in_signal);
    vta_inst.vta_vta_vir_mem_wgt_data_in_in(vta_mem_wgt_data_in_signal);
    vta_inst.vta_vta_vir_mem_bias_data_in_in(vta_mem_bias_data_in_signal);

    SC_THREAD(run);
  }

  void run() {
    vta_inst.instr_log.open("instr_log.txt", ios::out | ios::trunc);
    vta_inst.instr_update_log.open("instr_update_log.txt", ios::out | ios::trunc);

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
    std::vector<json> result_v;
    for (auto it : vta_inst.vta_vta_virtual_dram_out) {
      std::stringstream s_addr;
      std::stringstream s_data;
      s_addr << std::hex << it.first;
      s_data << std::hex << it.second;
      json temp = {
        {"addr", "0x" + s_addr.str()},
        {"data", "0x" + s_data.str()}
      };
      result_v.push_back(temp);
    }
    json result = result_v;
    fout << std::setw(2) << result;

    fout.close();
    std::cout << "outputs have been store at " << output_path << std::endl;

    wait(100000, SC_NS);
    std::cout << '\n' << std::endl;
    std::cout << "************* sc_stop **************" << std::endl;

    vta_inst.instr_log.close();
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
