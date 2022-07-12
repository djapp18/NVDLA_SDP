#include <sdp.h>
bool sdp::decode_sdp_Compute_ReLU() {
auto local_var_1 = sdp_group0_d_bs_bypass.range(0, 0);
sc_biguint<1> local_var_2 = 0;
bool local_var_3 = (local_var_1 == local_var_2);
auto local_var_5 = sdp_group0_d_bn_bypass.range(0, 0);
sc_biguint<1> local_var_6 = 1;
bool local_var_7 = (local_var_5 == local_var_6);
bool local_var_8 = (local_var_3 & local_var_7);
auto local_var_10 = sdp_group0_d_ew_bypass.range(0, 0);
sc_biguint<1> local_var_11 = 1;
bool local_var_12 = (local_var_10 == local_var_11);
bool local_var_13 = (local_var_8 & local_var_12);
auto local_var_15 = sdp_group0_d_bs_relu_bypass.range(0, 0);
sc_biguint<1> local_var_16 = 0;
bool local_var_17 = (local_var_15 == local_var_16);
auto local_var_19 = sdp_group0_d_bs_alu_bypass.range(0, 0);
sc_biguint<1> local_var_20 = 1;
bool local_var_21 = (local_var_19 == local_var_20);
bool local_var_22 = (local_var_17 & local_var_21);
auto local_var_24 = sdp_group0_d_bs_mul_bypass.range(0, 0);
sc_biguint<1> local_var_25 = 1;
bool local_var_26 = (local_var_24 == local_var_25);
bool local_var_27 = (local_var_22 & local_var_26);
bool local_var_28 = (local_var_13 & local_var_27);
auto local_var_29 = sdp_group0_d_bn_bypass.range(0, 0);
sc_biguint<1> local_var_30 = 0;
bool local_var_31 = (local_var_29 == local_var_30);
auto local_var_32 = sdp_group0_d_bs_bypass.range(0, 0);
sc_biguint<1> local_var_33 = 1;
bool local_var_34 = (local_var_32 == local_var_33);
bool local_var_35 = (local_var_31 & local_var_34);
auto local_var_36 = sdp_group0_d_ew_bypass.range(0, 0);
sc_biguint<1> local_var_37 = 1;
bool local_var_38 = (local_var_36 == local_var_37);
bool local_var_39 = (local_var_35 & local_var_38);
auto local_var_41 = sdp_group0_d_bn_relu_bypass.range(0, 0);
sc_biguint<1> local_var_42 = 0;
bool local_var_43 = (local_var_41 == local_var_42);
auto local_var_45 = sdp_group0_d_bn_alu_bypass.range(0, 0);
sc_biguint<1> local_var_46 = 1;
bool local_var_47 = (local_var_45 == local_var_46);
bool local_var_48 = (local_var_43 & local_var_47);
auto local_var_50 = sdp_group0_d_bn_mul_bypass.range(0, 0);
sc_biguint<1> local_var_51 = 1;
bool local_var_52 = (local_var_50 == local_var_51);
bool local_var_53 = (local_var_48 & local_var_52);
bool local_var_54 = (local_var_39 & local_var_53);
bool local_var_55 = (local_var_28 | local_var_54);
sc_biguint<1> local_var_57 = 0;
bool local_var_58 = (sdp_s_consumer == local_var_57);
bool local_var_59 = (local_var_55 & local_var_58);
sc_biguint<1> local_var_61 = 0;
bool local_var_62 = (sdp_group0_d_op_en == local_var_61);
bool local_var_63 = (local_var_59 & local_var_62);
auto& univ_var_90 = local_var_63;
return univ_var_90;
}
void sdp::update_sdp_Compute_ReLU() {
auto local_var_1 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_2 = 1;
bool local_var_3 = (local_var_1 == local_var_2);
auto local_var_6 = (local_var_3) ? sdp_cacc_data_0 : sdp_mrdma_data_0;
sc_biguint<32> local_var_7 = 0;
bool local_var_8 = (local_var_6 > local_var_7);
sc_biguint<32> local_var_9 = 0;
auto local_var_10 = (local_var_8) ? local_var_6 : local_var_9;
auto local_var_10_nxt_holder = local_var_10;
auto local_var_11 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_12 = 1;
bool local_var_13 = (local_var_11 == local_var_12);
auto local_var_16 = (local_var_13) ? sdp_cacc_data_1 : sdp_mrdma_data_1;
sc_biguint<32> local_var_17 = 0;
bool local_var_18 = (local_var_16 > local_var_17);
sc_biguint<32> local_var_19 = 0;
auto local_var_20 = (local_var_18) ? local_var_16 : local_var_19;
auto local_var_20_nxt_holder = local_var_20;
auto local_var_21 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_22 = 1;
bool local_var_23 = (local_var_21 == local_var_22);
auto local_var_26 = (local_var_23) ? sdp_cacc_data_10 : sdp_mrdma_data_10;
sc_biguint<32> local_var_27 = 0;
bool local_var_28 = (local_var_26 > local_var_27);
sc_biguint<32> local_var_29 = 0;
auto local_var_30 = (local_var_28) ? local_var_26 : local_var_29;
auto local_var_30_nxt_holder = local_var_30;
auto local_var_31 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_32 = 1;
bool local_var_33 = (local_var_31 == local_var_32);
auto local_var_36 = (local_var_33) ? sdp_cacc_data_11 : sdp_mrdma_data_11;
sc_biguint<32> local_var_37 = 0;
bool local_var_38 = (local_var_36 > local_var_37);
sc_biguint<32> local_var_39 = 0;
auto local_var_40 = (local_var_38) ? local_var_36 : local_var_39;
auto local_var_40_nxt_holder = local_var_40;
auto local_var_41 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_42 = 1;
bool local_var_43 = (local_var_41 == local_var_42);
auto local_var_46 = (local_var_43) ? sdp_cacc_data_12 : sdp_mrdma_data_12;
sc_biguint<32> local_var_47 = 0;
bool local_var_48 = (local_var_46 > local_var_47);
sc_biguint<32> local_var_49 = 0;
auto local_var_50 = (local_var_48) ? local_var_46 : local_var_49;
auto local_var_50_nxt_holder = local_var_50;
auto local_var_51 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_52 = 1;
bool local_var_53 = (local_var_51 == local_var_52);
auto local_var_56 = (local_var_53) ? sdp_cacc_data_13 : sdp_mrdma_data_13;
sc_biguint<32> local_var_57 = 0;
bool local_var_58 = (local_var_56 > local_var_57);
sc_biguint<32> local_var_59 = 0;
auto local_var_60 = (local_var_58) ? local_var_56 : local_var_59;
auto local_var_60_nxt_holder = local_var_60;
auto local_var_61 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_62 = 1;
bool local_var_63 = (local_var_61 == local_var_62);
auto local_var_66 = (local_var_63) ? sdp_cacc_data_14 : sdp_mrdma_data_14;
sc_biguint<32> local_var_67 = 0;
bool local_var_68 = (local_var_66 > local_var_67);
sc_biguint<32> local_var_69 = 0;
auto local_var_70 = (local_var_68) ? local_var_66 : local_var_69;
auto local_var_70_nxt_holder = local_var_70;
auto local_var_71 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_72 = 1;
bool local_var_73 = (local_var_71 == local_var_72);
auto local_var_76 = (local_var_73) ? sdp_cacc_data_15 : sdp_mrdma_data_15;
sc_biguint<32> local_var_77 = 0;
bool local_var_78 = (local_var_76 > local_var_77);
sc_biguint<32> local_var_79 = 0;
auto local_var_80 = (local_var_78) ? local_var_76 : local_var_79;
auto local_var_80_nxt_holder = local_var_80;
auto local_var_81 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_82 = 1;
bool local_var_83 = (local_var_81 == local_var_82);
auto local_var_86 = (local_var_83) ? sdp_cacc_data_2 : sdp_mrdma_data_2;
sc_biguint<32> local_var_87 = 0;
bool local_var_88 = (local_var_86 > local_var_87);
sc_biguint<32> local_var_89 = 0;
auto local_var_90 = (local_var_88) ? local_var_86 : local_var_89;
auto local_var_90_nxt_holder = local_var_90;
auto local_var_91 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_92 = 1;
bool local_var_93 = (local_var_91 == local_var_92);
auto local_var_96 = (local_var_93) ? sdp_cacc_data_3 : sdp_mrdma_data_3;
sc_biguint<32> local_var_97 = 0;
bool local_var_98 = (local_var_96 > local_var_97);
sc_biguint<32> local_var_99 = 0;
auto local_var_100 = (local_var_98) ? local_var_96 : local_var_99;
auto local_var_100_nxt_holder = local_var_100;
auto local_var_101 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_102 = 1;
bool local_var_103 = (local_var_101 == local_var_102);
auto local_var_106 = (local_var_103) ? sdp_cacc_data_4 : sdp_mrdma_data_4;
sc_biguint<32> local_var_107 = 0;
bool local_var_108 = (local_var_106 > local_var_107);
sc_biguint<32> local_var_109 = 0;
auto local_var_110 = (local_var_108) ? local_var_106 : local_var_109;
auto local_var_110_nxt_holder = local_var_110;
auto local_var_111 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_112 = 1;
bool local_var_113 = (local_var_111 == local_var_112);
auto local_var_116 = (local_var_113) ? sdp_cacc_data_5 : sdp_mrdma_data_5;
sc_biguint<32> local_var_117 = 0;
bool local_var_118 = (local_var_116 > local_var_117);
sc_biguint<32> local_var_119 = 0;
auto local_var_120 = (local_var_118) ? local_var_116 : local_var_119;
auto local_var_120_nxt_holder = local_var_120;
auto local_var_121 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_122 = 1;
bool local_var_123 = (local_var_121 == local_var_122);
auto local_var_126 = (local_var_123) ? sdp_cacc_data_6 : sdp_mrdma_data_6;
sc_biguint<32> local_var_127 = 0;
bool local_var_128 = (local_var_126 > local_var_127);
sc_biguint<32> local_var_129 = 0;
auto local_var_130 = (local_var_128) ? local_var_126 : local_var_129;
auto local_var_130_nxt_holder = local_var_130;
auto local_var_131 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_132 = 1;
bool local_var_133 = (local_var_131 == local_var_132);
auto local_var_136 = (local_var_133) ? sdp_cacc_data_7 : sdp_mrdma_data_7;
sc_biguint<32> local_var_137 = 0;
bool local_var_138 = (local_var_136 > local_var_137);
sc_biguint<32> local_var_139 = 0;
auto local_var_140 = (local_var_138) ? local_var_136 : local_var_139;
auto local_var_140_nxt_holder = local_var_140;
auto local_var_141 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_142 = 1;
bool local_var_143 = (local_var_141 == local_var_142);
auto local_var_146 = (local_var_143) ? sdp_cacc_data_8 : sdp_mrdma_data_8;
sc_biguint<32> local_var_147 = 0;
bool local_var_148 = (local_var_146 > local_var_147);
sc_biguint<32> local_var_149 = 0;
auto local_var_150 = (local_var_148) ? local_var_146 : local_var_149;
auto local_var_150_nxt_holder = local_var_150;
auto local_var_151 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_152 = 1;
bool local_var_153 = (local_var_151 == local_var_152);
auto local_var_156 = (local_var_153) ? sdp_cacc_data_9 : sdp_mrdma_data_9;
sc_biguint<32> local_var_157 = 0;
bool local_var_158 = (local_var_156 > local_var_157);
sc_biguint<32> local_var_159 = 0;
auto local_var_160 = (local_var_158) ? local_var_156 : local_var_159;
auto local_var_160_nxt_holder = local_var_160;
sc_biguint<20> local_var_161 = 7;
auto local_var_161_nxt_holder = local_var_161;
sc_biguint<12> local_var_162 = 0;
univ_var_91 = (sc_biguint<12>(local_var_162), sc_biguint<20>(local_var_161));
auto univ_var_91_nxt_holder = univ_var_91;
sdp_pdp_output_0 = local_var_10_nxt_holder;
sdp_pdp_output_1 = local_var_20_nxt_holder;
sdp_pdp_output_10 = local_var_30_nxt_holder;
sdp_pdp_output_11 = local_var_40_nxt_holder;
sdp_pdp_output_12 = local_var_50_nxt_holder;
sdp_pdp_output_13 = local_var_60_nxt_holder;
sdp_pdp_output_14 = local_var_70_nxt_holder;
sdp_pdp_output_15 = local_var_80_nxt_holder;
sdp_pdp_output_2 = local_var_90_nxt_holder;
sdp_pdp_output_3 = local_var_100_nxt_holder;
sdp_pdp_output_4 = local_var_110_nxt_holder;
sdp_pdp_output_5 = local_var_120_nxt_holder;
sdp_pdp_output_6 = local_var_130_nxt_holder;
sdp_pdp_output_7 = local_var_140_nxt_holder;
sdp_pdp_output_8 = local_var_150_nxt_holder;
sdp_pdp_output_9 = local_var_160_nxt_holder;
sdp_test = local_var_161_nxt_holder;
sdp_test2 = univ_var_91_nxt_holder;
#ifdef ILATOR_VERBOSE
instr_update_log << "No." << std::dec << GetInstrCntr() << '\t' << "Compute_ReLU state updates:" << std::endl;
instr_update_log << "    sdp_pdp_output_0 => " << std::hex << "0x" << sdp_pdp_output_0 << std::endl; 
instr_update_log << "    sdp_pdp_output_1 => " << std::hex << "0x" << sdp_pdp_output_1 << std::endl; 
instr_update_log << "    sdp_pdp_output_10 => " << std::hex << "0x" << sdp_pdp_output_10 << std::endl; 
instr_update_log << "    sdp_pdp_output_11 => " << std::hex << "0x" << sdp_pdp_output_11 << std::endl; 
instr_update_log << "    sdp_pdp_output_12 => " << std::hex << "0x" << sdp_pdp_output_12 << std::endl; 
instr_update_log << "    sdp_pdp_output_13 => " << std::hex << "0x" << sdp_pdp_output_13 << std::endl; 
instr_update_log << "    sdp_pdp_output_14 => " << std::hex << "0x" << sdp_pdp_output_14 << std::endl; 
instr_update_log << "    sdp_pdp_output_15 => " << std::hex << "0x" << sdp_pdp_output_15 << std::endl; 
instr_update_log << "    sdp_pdp_output_2 => " << std::hex << "0x" << sdp_pdp_output_2 << std::endl; 
instr_update_log << "    sdp_pdp_output_3 => " << std::hex << "0x" << sdp_pdp_output_3 << std::endl; 
instr_update_log << "    sdp_pdp_output_4 => " << std::hex << "0x" << sdp_pdp_output_4 << std::endl; 
instr_update_log << "    sdp_pdp_output_5 => " << std::hex << "0x" << sdp_pdp_output_5 << std::endl; 
instr_update_log << "    sdp_pdp_output_6 => " << std::hex << "0x" << sdp_pdp_output_6 << std::endl; 
instr_update_log << "    sdp_pdp_output_7 => " << std::hex << "0x" << sdp_pdp_output_7 << std::endl; 
instr_update_log << "    sdp_pdp_output_8 => " << std::hex << "0x" << sdp_pdp_output_8 << std::endl; 
instr_update_log << "    sdp_pdp_output_9 => " << std::hex << "0x" << sdp_pdp_output_9 << std::endl; 
instr_update_log << "    sdp_test => " << std::hex << "0x" << sdp_test << std::endl; 
instr_update_log << "    sdp_test2 => " << std::hex << "0x" << sdp_test2 << std::endl; 
instr_update_log << std::endl;
#endif
}
