#include <sdp.h>
bool sdp::decode_sdp_DATA_FORMAT_group1() {
sc_biguint<22> local_var_1 = 188;
bool local_var_2 = (sdp_csb_addr == local_var_1);
sc_biguint<1> local_var_4 = 1;
bool local_var_5 = (sdp_csb_rdy == local_var_4);
sc_biguint<1> local_var_7 = 1;
bool local_var_8 = (sdp_csb_vld == local_var_7);
bool local_var_9 = (local_var_5 & local_var_8);
bool local_var_10 = (local_var_2 & local_var_9);
sc_biguint<1> local_var_12 = 1;
bool local_var_13 = (sdp_csb_write == local_var_12);
bool local_var_14 = (local_var_10 & local_var_13);
auto local_var_16 = sdp_s_producer.range(0, 0);
sc_biguint<1> local_var_17 = 1;
bool local_var_18 = (local_var_16 == local_var_17);
bool local_var_19 = (local_var_14 & local_var_18);
auto local_var_21 = sdp_group1_d_op_en.range(0, 0);
sc_biguint<1> local_var_22 = 0;
bool local_var_23 = (local_var_21 == local_var_22);
bool local_var_24 = (local_var_19 & local_var_23);
auto& univ_var_80 = local_var_24;
return univ_var_80;
}
void sdp::update_sdp_DATA_FORMAT_group1() {
auto local_var_1 = sdp_csb_data.range(3, 2);
auto local_var_1_nxt_holder = local_var_1;
auto local_var_2 = sdp_csb_data.range(1, 0);
auto local_var_2_nxt_holder = local_var_2;
sdp_group1_d_out_precision = local_var_1_nxt_holder;
sdp_group1_d_proc_precision = local_var_2_nxt_holder;
#ifdef ILATOR_VERBOSE
instr_update_log << "No." << std::dec << GetInstrCntr() << '\t' << "DATA_FORMAT_group1 state updates:" << std::endl;
instr_update_log << "    sdp_group1_d_out_precision => " << std::hex << "0x" << sdp_group1_d_out_precision << std::endl; 
instr_update_log << "    sdp_group1_d_proc_precision => " << std::hex << "0x" << sdp_group1_d_proc_precision << std::endl; 
instr_update_log << std::endl;
#endif
}
