#include <sdp.h>
bool sdp::decode_sdp_Write_LUT_LO() {
auto local_var_1 = sdp_s_lut_access_type.range(0, 0);
sc_biguint<1> local_var_2 = 1;
bool local_var_3 = (local_var_1 == local_var_2);
auto local_var_5 = sdp_s_lut_table_id.range(0, 0);
sc_biguint<1> local_var_6 = 1;
bool local_var_7 = (local_var_5 == local_var_6);
bool local_var_8 = (local_var_3 & local_var_7);
auto& univ_var_293 = local_var_8;
return univ_var_293;
}
void sdp::update_sdp_Write_LUT_LO() {
std::map<sc_biguint<9>, sc_biguint<16>> local_var_0;
store_294(local_var_0);
for (auto& it : local_var_0) {
  sdp_lo_tbl[it.first] = it.second;
}
#ifdef ILATOR_VERBOSE
instr_update_log << "No." << std::dec << GetInstrCntr() << '\t' << "Write_LUT_LO state updates:" << std::endl;
instr_update_log << "    sdp_lo_tbl get updated" << std::endl;
instr_update_log << std::endl;
#endif
}
