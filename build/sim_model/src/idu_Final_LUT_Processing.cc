#include <sdp.h>
bool sdp::decode_sdp_Final_LUT_Processing() {
auto local_var_1 = sdp_s_lut_access_type.range(0, 0);
sc_biguint<1> local_var_2 = 0;
bool local_var_3 = (local_var_1 == local_var_2);
auto local_var_5 = sdp_s_lut_table_id.range(0, 0);
sc_biguint<1> local_var_6 = 1;
bool local_var_7 = (local_var_5 == local_var_6);
bool local_var_8 = (local_var_3 & local_var_7);
auto local_var_10 = sdp_group0_d_ew_lut_bypass.range(0, 0);
sc_biguint<1> local_var_11 = 0;
bool local_var_12 = (local_var_10 == local_var_11);
bool local_var_13 = (local_var_8 & local_var_12);
auto& univ_var_304 = local_var_13;
return univ_var_304;
}
void sdp::update_sdp_Final_LUT_Processing() {
#ifdef ILATOR_VERBOSE
instr_update_log << "No." << std::dec << GetInstrCntr() << '\t' << "Final_LUT_Processing state updates:" << std::endl;
instr_update_log << std::endl;
#endif
}
