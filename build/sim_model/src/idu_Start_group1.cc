#include <sdp.h>
bool sdp::decode_sdp_Start_group1() {
sc_biguint<2> local_var_1 = 0;
bool local_var_2 = (sdp_group1_sdp_state == local_var_1);
sc_biguint<1> local_var_4 = 1;
bool local_var_5 = (sdp_s_consumer == local_var_4);
sc_biguint<1> local_var_7 = 1;
bool local_var_8 = (sdp_group1_d_op_en == local_var_7);
bool local_var_9 = (local_var_5 & local_var_8);
bool local_var_10 = (local_var_2 & local_var_9);
auto& univ_var_91 = local_var_10;
return univ_var_91;
}
void sdp::update_sdp_Start_group1() {
sc_biguint<2> local_var_0 = 2;
auto local_var_0_nxt_holder = local_var_0;
sdp_group1_sdp_state = local_var_0_nxt_holder;
#ifdef ILATOR_VERBOSE
instr_update_log << "No." << std::dec << GetInstrCntr() << '\t' << "Start_group1 state updates:" << std::endl;
instr_update_log << "    sdp_group1_sdp_state => " << std::hex << "0x" << sdp_group1_sdp_state << std::endl; 
instr_update_log << std::endl;
#endif
}
