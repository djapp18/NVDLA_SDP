#include <sdp.h>
bool sdp::decode_sdp_PERF_ENABLE_group1() {
sc_biguint<2> local_var_1 = 0;
univ_var_1 = (sc_biguint<22>(sdp_csb_addr), sc_biguint<2>(local_var_1));
auto local_var_3 = univ_var_1.range(11, 0);
sc_biguint<12> local_var_4 = 220;
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
auto local_var_19 = sdp_s_producer.range(0, 0);
sc_biguint<1> local_var_20 = 1;
bool local_var_21 = (local_var_19 == local_var_20);
bool local_var_22 = (local_var_17 & local_var_21);
auto local_var_24 = sdp_group1_d_op_en.range(0, 0);
sc_biguint<1> local_var_25 = 0;
bool local_var_26 = (local_var_24 == local_var_25);
bool local_var_27 = (local_var_22 & local_var_26);
auto& univ_var_89 = local_var_27;
return univ_var_89;
}
void sdp::update_sdp_PERF_ENABLE_group1() {
auto local_var_1 = sdp_csb_data.range(0, 0);
auto local_var_1_nxt_holder = local_var_1;
auto local_var_2 = sdp_csb_data.range(1, 1);
auto local_var_2_nxt_holder = local_var_2;
auto local_var_3 = sdp_csb_data.range(3, 3);
auto local_var_3_nxt_holder = local_var_3;
auto local_var_4 = sdp_csb_data.range(2, 2);
auto local_var_4_nxt_holder = local_var_4;
sdp_group1_d_perf_dma_en = local_var_1_nxt_holder;
sdp_group1_d_perf_lut_en = local_var_2_nxt_holder;
sdp_group1_d_perf_nan_inf_count_en = local_var_3_nxt_holder;
sdp_group1_d_perf_sat_en = local_var_4_nxt_holder;
#ifdef ILATOR_VERBOSE
instr_update_log << "No." << std::dec << GetInstrCntr() << '\t' << "PERF_ENABLE_group1 state updates:" << std::endl;
instr_update_log << "    sdp_group1_d_perf_dma_en => " << std::hex << "0x" << sdp_group1_d_perf_dma_en << std::endl; 
instr_update_log << "    sdp_group1_d_perf_lut_en => " << std::hex << "0x" << sdp_group1_d_perf_lut_en << std::endl; 
instr_update_log << "    sdp_group1_d_perf_nan_inf_count_en => " << std::hex << "0x" << sdp_group1_d_perf_nan_inf_count_en << std::endl; 
instr_update_log << "    sdp_group1_d_perf_sat_en => " << std::hex << "0x" << sdp_group1_d_perf_sat_en << std::endl; 
instr_update_log << std::endl;
#endif
}
