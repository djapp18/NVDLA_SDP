#include <sdp.h>
bool sdp::decode_sdp_Done2Idle_group0() {
sc_biguint<2> local_var_1 = 3;
bool local_var_2 = (sdp_group0_sdp_state == local_var_1);
sc_biguint<1> local_var_4 = 1;
bool local_var_5 = (sdp_done == local_var_4);
bool local_var_6 = (local_var_2 & local_var_5);
sc_biguint<1> local_var_8 = 0;
bool local_var_9 = (sdp_s_consumer == local_var_8);
bool local_var_10 = (local_var_6 & local_var_9);
auto& univ_var_93 = local_var_10;
return univ_var_93;
}
void sdp::update_sdp_Done2Idle_group0() {
sc_biguint<1> local_var_0 = 0;
auto local_var_0_nxt_holder = local_var_0;
sc_biguint<2> local_var_1 = 0;
auto local_var_1_nxt_holder = local_var_1;
sc_biguint<1> local_var_2 = 1;
auto local_var_2_nxt_holder = local_var_2;
sdp_group0_d_op_en = local_var_0_nxt_holder;
sdp_group0_sdp_state = local_var_1_nxt_holder;
sdp_s_consumer = local_var_2_nxt_holder;
#ifdef ILATOR_VERBOSE
instr_update_log << "No." << std::dec << GetInstrCntr() << '\t' << "Done2Idle_group0 state updates:" << std::endl;
instr_update_log << "    sdp_group0_d_op_en => " << std::hex << "0x" << sdp_group0_d_op_en << std::endl; 
instr_update_log << "    sdp_group0_sdp_state => " << std::hex << "0x" << sdp_group0_sdp_state << std::endl; 
instr_update_log << "    sdp_s_consumer => " << std::hex << "0x" << sdp_s_consumer << std::endl; 
instr_update_log << std::endl;
#endif
}
