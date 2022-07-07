#include <sdp.h>
bool sdp::decode_sdp_LUT_ACCESS_DATA() {
sc_biguint<2> local_var_1 = 0;
univ_var_1 = (sc_biguint<22>(sdp_csb_addr), sc_biguint<2>(local_var_1));
auto local_var_3 = univ_var_1.range(11, 0);
sc_biguint<12> local_var_4 = 12;
bool local_var_5 = (local_var_3 == local_var_4);
sc_biguint<1> local_var_7 = 1;
bool local_var_8 = (sdp_csb_rdy == local_var_7);
sc_biguint<1> local_var_10 = 1;
bool local_var_11 = (sdp_csb_vld == local_var_10);
bool local_var_12 = (local_var_8 & local_var_11);
bool local_var_13 = (local_var_5 & local_var_12);
sc_biguint<1> local_var_15 = 1;
bool local_var_16 = (sdp_csb_write == local_var_15);
bool local_var_17 = (local_var_13 & local_var_16);
auto& univ_var_4 = local_var_17;
return univ_var_4;
}
void sdp::update_sdp_LUT_ACCESS_DATA() {
auto local_var_1 = sdp_csb_data.range(15, 0);
auto local_var_1_nxt_holder = local_var_1;
sdp_s_lut_data = local_var_1_nxt_holder;
#ifdef ILATOR_VERBOSE
instr_update_log << "No." << std::dec << GetInstrCntr() << '\t' << "LUT_ACCESS_DATA state updates:" << std::endl;
instr_update_log << "    sdp_s_lut_data => " << std::hex << "0x" << sdp_s_lut_data << std::endl; 
instr_update_log << std::endl;
#endif
}
