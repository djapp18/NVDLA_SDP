#include <sdp.h>
bool sdp::decode_sdp_DONE() {
auto local_var_1 = sdp_done.range(0, 0);
sc_biguint<1> local_var_2 = 1;
bool local_var_3 = (local_var_1 == local_var_2);
auto& univ_var_89 = local_var_3;
return univ_var_89;
}
void sdp::update_sdp_DONE() {
sc_biguint<1> local_var_0 = 0;
auto local_var_0_nxt_holder = local_var_0;
auto local_var_2 = sdp_s_consumer.range(0, 0);
sc_biguint<1> local_var_3 = 0;
bool local_var_4 = (local_var_2 == local_var_3);
sc_biguint<1> local_var_5 = 1;
sc_biguint<1> local_var_6 = 0;
auto local_var_7 = (local_var_4) ? local_var_5 : local_var_6;
auto local_var_7_nxt_holder = local_var_7;
sdp_group0_d_op_en = local_var_0_nxt_holder;
sdp_s_consumer = local_var_7_nxt_holder;
#ifdef ILATOR_VERBOSE
instr_update_log << "No." << std::dec << GetInstrCntr() << '\t' << "DONE state updates:" << std::endl;
instr_update_log << "    sdp_group0_d_op_en => " << std::hex << "0x" << sdp_group0_d_op_en << std::endl; 
instr_update_log << "    sdp_s_consumer => " << std::hex << "0x" << sdp_s_consumer << std::endl; 
instr_update_log << std::endl;
#endif
}
