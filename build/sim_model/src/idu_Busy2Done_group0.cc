#include <sdp.h>
bool sdp::decode_sdp_Busy2Done_group0() {
sc_biguint<2> local_var_1 = 2;
bool local_var_2 = (sdp_group0_sdp_state == local_var_1);
auto local_var_4 = sdp_fifo_clr.range(0, 0);
sc_biguint<1> local_var_5 = 1;
bool local_var_6 = (local_var_4 == local_var_5);
bool local_var_7 = (local_var_2 & local_var_6);
auto& univ_var_92 = local_var_7;
return univ_var_92;
}
void sdp::update_sdp_Busy2Done_group0() {
sc_biguint<2> local_var_0 = 3;
auto local_var_0_nxt_holder = local_var_0;
sdp_group0_sdp_state = local_var_0_nxt_holder;
#ifdef ILATOR_VERBOSE
instr_update_log << "No." << std::dec << GetInstrCntr() << '\t' << "Busy2Done_group0 state updates:" << std::endl;
instr_update_log << "    sdp_group0_sdp_state => " << std::hex << "0x" << sdp_group0_sdp_state << std::endl; 
instr_update_log << std::endl;
#endif
}
