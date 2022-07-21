#include <sdp.h>
bool sdp::decode_sdp_Write_LUT_LE() {
auto local_var_1 = sdp_s_lut_access_type.range(0, 0);
sc_biguint<1> local_var_2 = 1;
bool local_var_3 = (local_var_1 == local_var_2);
auto local_var_5 = sdp_s_lut_table_id.range(0, 0);
sc_biguint<1> local_var_6 = 0;
bool local_var_7 = (local_var_5 == local_var_6);
bool local_var_8 = (local_var_3 & local_var_7);
auto& univ_var_261 = local_var_8;
return univ_var_261;
}
void sdp::update_sdp_Write_LUT_LE() {
std::map<sc_biguint<7>, sc_biguint<16>> local_var_0;
store_262(local_var_0);
for (auto& it : local_var_0) {
  sdp_le_tbl[it.first] = it.second;
}
#ifdef ILATOR_VERBOSE
instr_update_log << "No." << std::dec << GetInstrCntr() << '\t' << "Write_LUT_LE state updates:" << std::endl;
instr_update_log << "    sdp_le_tbl get updated" << std::endl;
instr_update_log << std::endl;
#endif
}
