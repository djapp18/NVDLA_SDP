#include <sdp.h>
bool sdp::decode_sdp_Compute_Add() {
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
auto local_var_15 = sdp_group0_d_bs_alu_bypass.range(0, 0);
sc_biguint<1> local_var_16 = 0;
bool local_var_17 = (local_var_15 == local_var_16);
auto local_var_19 = sdp_group0_d_bs_alu_algo.range(1, 0);
sc_biguint<2> local_var_20 = 2;
bool local_var_21 = (local_var_19 == local_var_20);
bool local_var_22 = (local_var_17 & local_var_21);
auto local_var_24 = sdp_group0_d_bs_relu_bypass.range(0, 0);
sc_biguint<1> local_var_25 = 1;
bool local_var_26 = (local_var_24 == local_var_25);
bool local_var_27 = (local_var_22 & local_var_26);
auto local_var_29 = sdp_group0_d_bs_mul_bypass.range(0, 0);
sc_biguint<1> local_var_30 = 1;
bool local_var_31 = (local_var_29 == local_var_30);
bool local_var_32 = (local_var_27 & local_var_31);
bool local_var_33 = (local_var_13 & local_var_32);
auto local_var_34 = sdp_group0_d_bn_bypass.range(0, 0);
sc_biguint<1> local_var_35 = 0;
bool local_var_36 = (local_var_34 == local_var_35);
auto local_var_37 = sdp_group0_d_bs_bypass.range(0, 0);
sc_biguint<1> local_var_38 = 1;
bool local_var_39 = (local_var_37 == local_var_38);
bool local_var_40 = (local_var_36 & local_var_39);
auto local_var_41 = sdp_group0_d_ew_bypass.range(0, 0);
sc_biguint<1> local_var_42 = 1;
bool local_var_43 = (local_var_41 == local_var_42);
bool local_var_44 = (local_var_40 & local_var_43);
auto local_var_46 = sdp_group0_d_bn_alu_bypass.range(0, 0);
sc_biguint<1> local_var_47 = 0;
bool local_var_48 = (local_var_46 == local_var_47);
auto local_var_50 = sdp_group0_d_bn_alu_algo.range(1, 0);
sc_biguint<2> local_var_51 = 2;
bool local_var_52 = (local_var_50 == local_var_51);
bool local_var_53 = (local_var_48 & local_var_52);
auto local_var_55 = sdp_group0_d_bn_relu_bypass.range(0, 0);
sc_biguint<1> local_var_56 = 1;
bool local_var_57 = (local_var_55 == local_var_56);
bool local_var_58 = (local_var_53 & local_var_57);
auto local_var_60 = sdp_group0_d_bn_mul_bypass.range(0, 0);
sc_biguint<1> local_var_61 = 1;
bool local_var_62 = (local_var_60 == local_var_61);
bool local_var_63 = (local_var_58 & local_var_62);
bool local_var_64 = (local_var_44 & local_var_63);
bool local_var_65 = (local_var_33 | local_var_64);
sc_biguint<1> local_var_67 = 0;
bool local_var_68 = (sdp_s_consumer == local_var_67);
bool local_var_69 = (local_var_65 & local_var_68);
sc_biguint<1> local_var_71 = 0;
bool local_var_72 = (sdp_group0_d_op_en == local_var_71);
bool local_var_73 = (local_var_69 & local_var_72);
auto& univ_var_158 = local_var_73;
return univ_var_158;
}
void sdp::update_sdp_Compute_Add() {
auto local_var_1 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_2 = 1;
bool local_var_3 = (local_var_1 == local_var_2);
auto local_var_6 = (local_var_3) ? sdp_cacc_data_0 : sdp_mrdma_data_0;
sc_biguint<16> local_var_7 = 0;
auto local_var_9 = sdp_group0_d_bs_alu_src.range(0, 0);
sc_biguint<1> local_var_10 = 1;
bool local_var_11 = (local_var_9 == local_var_10);
auto local_var_14 = (local_var_11) ? sdp_dma_data_0 : sdp_regs_data_0;
sc_biguint<10> local_var_15 = 0;
univ_var_159 = (sc_biguint<10>(local_var_15), sc_biguint<6>(sdp_group0_d_bs_alu_shift_value));
sc_biguint<16> local_var_18 = (local_var_14 << univ_var_159);
univ_var_160 = (sc_biguint<16>(local_var_7), sc_biguint<16>(local_var_18));
sc_biguint<32> local_var_20 = (local_var_6 + univ_var_160);
auto local_var_20_nxt_holder = local_var_20;
auto local_var_21 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_22 = 1;
bool local_var_23 = (local_var_21 == local_var_22);
auto local_var_26 = (local_var_23) ? sdp_cacc_data_1 : sdp_mrdma_data_1;
sc_biguint<16> local_var_27 = 0;
auto local_var_28 = sdp_group0_d_bs_alu_src.range(0, 0);
sc_biguint<1> local_var_29 = 1;
bool local_var_30 = (local_var_28 == local_var_29);
auto local_var_33 = (local_var_30) ? sdp_dma_data_1 : sdp_regs_data_1;
sc_biguint<10> local_var_34 = 0;
univ_var_161 = (sc_biguint<10>(local_var_34), sc_biguint<6>(sdp_group0_d_bs_alu_shift_value));
sc_biguint<16> local_var_36 = (local_var_33 << univ_var_161);
univ_var_162 = (sc_biguint<16>(local_var_27), sc_biguint<16>(local_var_36));
sc_biguint<32> local_var_38 = (local_var_26 + univ_var_162);
auto local_var_38_nxt_holder = local_var_38;
auto local_var_39 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_40 = 1;
bool local_var_41 = (local_var_39 == local_var_40);
auto local_var_44 = (local_var_41) ? sdp_cacc_data_10 : sdp_mrdma_data_10;
sc_biguint<16> local_var_45 = 0;
auto local_var_46 = sdp_group0_d_bs_alu_src.range(0, 0);
sc_biguint<1> local_var_47 = 1;
bool local_var_48 = (local_var_46 == local_var_47);
auto local_var_51 = (local_var_48) ? sdp_dma_data_10 : sdp_regs_data_10;
sc_biguint<10> local_var_52 = 0;
univ_var_163 = (sc_biguint<10>(local_var_52), sc_biguint<6>(sdp_group0_d_bs_alu_shift_value));
sc_biguint<16> local_var_54 = (local_var_51 << univ_var_163);
univ_var_164 = (sc_biguint<16>(local_var_45), sc_biguint<16>(local_var_54));
sc_biguint<32> local_var_56 = (local_var_44 + univ_var_164);
auto local_var_56_nxt_holder = local_var_56;
auto local_var_57 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_58 = 1;
bool local_var_59 = (local_var_57 == local_var_58);
auto local_var_62 = (local_var_59) ? sdp_cacc_data_11 : sdp_mrdma_data_11;
sc_biguint<16> local_var_63 = 0;
auto local_var_64 = sdp_group0_d_bs_alu_src.range(0, 0);
sc_biguint<1> local_var_65 = 1;
bool local_var_66 = (local_var_64 == local_var_65);
auto local_var_69 = (local_var_66) ? sdp_dma_data_11 : sdp_regs_data_11;
sc_biguint<10> local_var_70 = 0;
univ_var_165 = (sc_biguint<10>(local_var_70), sc_biguint<6>(sdp_group0_d_bs_alu_shift_value));
sc_biguint<16> local_var_72 = (local_var_69 << univ_var_165);
univ_var_166 = (sc_biguint<16>(local_var_63), sc_biguint<16>(local_var_72));
sc_biguint<32> local_var_74 = (local_var_62 + univ_var_166);
auto local_var_74_nxt_holder = local_var_74;
auto local_var_75 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_76 = 1;
bool local_var_77 = (local_var_75 == local_var_76);
auto local_var_80 = (local_var_77) ? sdp_cacc_data_12 : sdp_mrdma_data_12;
sc_biguint<16> local_var_81 = 0;
auto local_var_82 = sdp_group0_d_bs_alu_src.range(0, 0);
sc_biguint<1> local_var_83 = 1;
bool local_var_84 = (local_var_82 == local_var_83);
auto local_var_87 = (local_var_84) ? sdp_dma_data_12 : sdp_regs_data_12;
sc_biguint<10> local_var_88 = 0;
univ_var_167 = (sc_biguint<10>(local_var_88), sc_biguint<6>(sdp_group0_d_bs_alu_shift_value));
sc_biguint<16> local_var_90 = (local_var_87 << univ_var_167);
univ_var_168 = (sc_biguint<16>(local_var_81), sc_biguint<16>(local_var_90));
sc_biguint<32> local_var_92 = (local_var_80 + univ_var_168);
auto local_var_92_nxt_holder = local_var_92;
auto local_var_93 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_94 = 1;
bool local_var_95 = (local_var_93 == local_var_94);
auto local_var_98 = (local_var_95) ? sdp_cacc_data_13 : sdp_mrdma_data_13;
sc_biguint<16> local_var_99 = 0;
auto local_var_100 = sdp_group0_d_bs_alu_src.range(0, 0);
sc_biguint<1> local_var_101 = 1;
bool local_var_102 = (local_var_100 == local_var_101);
auto local_var_105 = (local_var_102) ? sdp_dma_data_13 : sdp_regs_data_13;
sc_biguint<10> local_var_106 = 0;
univ_var_169 = (sc_biguint<10>(local_var_106), sc_biguint<6>(sdp_group0_d_bs_alu_shift_value));
sc_biguint<16> local_var_108 = (local_var_105 << univ_var_169);
univ_var_170 = (sc_biguint<16>(local_var_99), sc_biguint<16>(local_var_108));
sc_biguint<32> local_var_110 = (local_var_98 + univ_var_170);
auto local_var_110_nxt_holder = local_var_110;
auto local_var_111 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_112 = 1;
bool local_var_113 = (local_var_111 == local_var_112);
auto local_var_116 = (local_var_113) ? sdp_cacc_data_14 : sdp_mrdma_data_14;
sc_biguint<16> local_var_117 = 0;
auto local_var_118 = sdp_group0_d_bs_alu_src.range(0, 0);
sc_biguint<1> local_var_119 = 1;
bool local_var_120 = (local_var_118 == local_var_119);
auto local_var_123 = (local_var_120) ? sdp_dma_data_14 : sdp_regs_data_14;
sc_biguint<10> local_var_124 = 0;
univ_var_171 = (sc_biguint<10>(local_var_124), sc_biguint<6>(sdp_group0_d_bs_alu_shift_value));
sc_biguint<16> local_var_126 = (local_var_123 << univ_var_171);
univ_var_172 = (sc_biguint<16>(local_var_117), sc_biguint<16>(local_var_126));
sc_biguint<32> local_var_128 = (local_var_116 + univ_var_172);
auto local_var_128_nxt_holder = local_var_128;
auto local_var_129 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_130 = 1;
bool local_var_131 = (local_var_129 == local_var_130);
auto local_var_134 = (local_var_131) ? sdp_cacc_data_15 : sdp_mrdma_data_15;
sc_biguint<16> local_var_135 = 0;
auto local_var_136 = sdp_group0_d_bs_alu_src.range(0, 0);
sc_biguint<1> local_var_137 = 1;
bool local_var_138 = (local_var_136 == local_var_137);
auto local_var_141 = (local_var_138) ? sdp_dma_data_15 : sdp_regs_data_15;
sc_biguint<10> local_var_142 = 0;
univ_var_173 = (sc_biguint<10>(local_var_142), sc_biguint<6>(sdp_group0_d_bs_alu_shift_value));
sc_biguint<16> local_var_144 = (local_var_141 << univ_var_173);
univ_var_174 = (sc_biguint<16>(local_var_135), sc_biguint<16>(local_var_144));
sc_biguint<32> local_var_146 = (local_var_134 + univ_var_174);
auto local_var_146_nxt_holder = local_var_146;
auto local_var_147 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_148 = 1;
bool local_var_149 = (local_var_147 == local_var_148);
auto local_var_152 = (local_var_149) ? sdp_cacc_data_2 : sdp_mrdma_data_2;
sc_biguint<16> local_var_153 = 0;
auto local_var_154 = sdp_group0_d_bs_alu_src.range(0, 0);
sc_biguint<1> local_var_155 = 1;
bool local_var_156 = (local_var_154 == local_var_155);
auto local_var_159 = (local_var_156) ? sdp_dma_data_2 : sdp_regs_data_2;
sc_biguint<10> local_var_160 = 0;
univ_var_175 = (sc_biguint<10>(local_var_160), sc_biguint<6>(sdp_group0_d_bs_alu_shift_value));
sc_biguint<16> local_var_162 = (local_var_159 << univ_var_175);
univ_var_176 = (sc_biguint<16>(local_var_153), sc_biguint<16>(local_var_162));
sc_biguint<32> local_var_164 = (local_var_152 + univ_var_176);
auto local_var_164_nxt_holder = local_var_164;
auto local_var_165 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_166 = 1;
bool local_var_167 = (local_var_165 == local_var_166);
auto local_var_170 = (local_var_167) ? sdp_cacc_data_3 : sdp_mrdma_data_3;
sc_biguint<16> local_var_171 = 0;
auto local_var_172 = sdp_group0_d_bs_alu_src.range(0, 0);
sc_biguint<1> local_var_173 = 1;
bool local_var_174 = (local_var_172 == local_var_173);
auto local_var_177 = (local_var_174) ? sdp_dma_data_3 : sdp_regs_data_3;
sc_biguint<10> local_var_178 = 0;
univ_var_177 = (sc_biguint<10>(local_var_178), sc_biguint<6>(sdp_group0_d_bs_alu_shift_value));
sc_biguint<16> local_var_180 = (local_var_177 << univ_var_177);
univ_var_178 = (sc_biguint<16>(local_var_171), sc_biguint<16>(local_var_180));
sc_biguint<32> local_var_182 = (local_var_170 + univ_var_178);
auto local_var_182_nxt_holder = local_var_182;
auto local_var_183 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_184 = 1;
bool local_var_185 = (local_var_183 == local_var_184);
auto local_var_188 = (local_var_185) ? sdp_cacc_data_4 : sdp_mrdma_data_4;
sc_biguint<16> local_var_189 = 0;
auto local_var_190 = sdp_group0_d_bs_alu_src.range(0, 0);
sc_biguint<1> local_var_191 = 1;
bool local_var_192 = (local_var_190 == local_var_191);
auto local_var_195 = (local_var_192) ? sdp_dma_data_4 : sdp_regs_data_4;
sc_biguint<10> local_var_196 = 0;
univ_var_179 = (sc_biguint<10>(local_var_196), sc_biguint<6>(sdp_group0_d_bs_alu_shift_value));
sc_biguint<16> local_var_198 = (local_var_195 << univ_var_179);
univ_var_180 = (sc_biguint<16>(local_var_189), sc_biguint<16>(local_var_198));
sc_biguint<32> local_var_200 = (local_var_188 + univ_var_180);
auto local_var_200_nxt_holder = local_var_200;
auto local_var_201 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_202 = 1;
bool local_var_203 = (local_var_201 == local_var_202);
auto local_var_206 = (local_var_203) ? sdp_cacc_data_5 : sdp_mrdma_data_5;
sc_biguint<16> local_var_207 = 0;
auto local_var_208 = sdp_group0_d_bs_alu_src.range(0, 0);
sc_biguint<1> local_var_209 = 1;
bool local_var_210 = (local_var_208 == local_var_209);
auto local_var_213 = (local_var_210) ? sdp_dma_data_5 : sdp_regs_data_5;
sc_biguint<10> local_var_214 = 0;
univ_var_181 = (sc_biguint<10>(local_var_214), sc_biguint<6>(sdp_group0_d_bs_alu_shift_value));
sc_biguint<16> local_var_216 = (local_var_213 << univ_var_181);
univ_var_182 = (sc_biguint<16>(local_var_207), sc_biguint<16>(local_var_216));
sc_biguint<32> local_var_218 = (local_var_206 + univ_var_182);
auto local_var_218_nxt_holder = local_var_218;
auto local_var_219 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_220 = 1;
bool local_var_221 = (local_var_219 == local_var_220);
auto local_var_224 = (local_var_221) ? sdp_cacc_data_6 : sdp_mrdma_data_6;
sc_biguint<16> local_var_225 = 0;
auto local_var_226 = sdp_group0_d_bs_alu_src.range(0, 0);
sc_biguint<1> local_var_227 = 1;
bool local_var_228 = (local_var_226 == local_var_227);
auto local_var_231 = (local_var_228) ? sdp_dma_data_6 : sdp_regs_data_6;
sc_biguint<10> local_var_232 = 0;
univ_var_183 = (sc_biguint<10>(local_var_232), sc_biguint<6>(sdp_group0_d_bs_alu_shift_value));
sc_biguint<16> local_var_234 = (local_var_231 << univ_var_183);
univ_var_184 = (sc_biguint<16>(local_var_225), sc_biguint<16>(local_var_234));
sc_biguint<32> local_var_236 = (local_var_224 + univ_var_184);
auto local_var_236_nxt_holder = local_var_236;
auto local_var_237 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_238 = 1;
bool local_var_239 = (local_var_237 == local_var_238);
auto local_var_242 = (local_var_239) ? sdp_cacc_data_7 : sdp_mrdma_data_7;
sc_biguint<16> local_var_243 = 0;
auto local_var_244 = sdp_group0_d_bs_alu_src.range(0, 0);
sc_biguint<1> local_var_245 = 1;
bool local_var_246 = (local_var_244 == local_var_245);
auto local_var_249 = (local_var_246) ? sdp_dma_data_7 : sdp_regs_data_7;
sc_biguint<10> local_var_250 = 0;
univ_var_185 = (sc_biguint<10>(local_var_250), sc_biguint<6>(sdp_group0_d_bs_alu_shift_value));
sc_biguint<16> local_var_252 = (local_var_249 << univ_var_185);
univ_var_186 = (sc_biguint<16>(local_var_243), sc_biguint<16>(local_var_252));
sc_biguint<32> local_var_254 = (local_var_242 + univ_var_186);
auto local_var_254_nxt_holder = local_var_254;
auto local_var_255 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_256 = 1;
bool local_var_257 = (local_var_255 == local_var_256);
auto local_var_260 = (local_var_257) ? sdp_cacc_data_8 : sdp_mrdma_data_8;
sc_biguint<16> local_var_261 = 0;
auto local_var_262 = sdp_group0_d_bs_alu_src.range(0, 0);
sc_biguint<1> local_var_263 = 1;
bool local_var_264 = (local_var_262 == local_var_263);
auto local_var_267 = (local_var_264) ? sdp_dma_data_8 : sdp_regs_data_8;
sc_biguint<10> local_var_268 = 0;
univ_var_187 = (sc_biguint<10>(local_var_268), sc_biguint<6>(sdp_group0_d_bs_alu_shift_value));
sc_biguint<16> local_var_270 = (local_var_267 << univ_var_187);
univ_var_188 = (sc_biguint<16>(local_var_261), sc_biguint<16>(local_var_270));
sc_biguint<32> local_var_272 = (local_var_260 + univ_var_188);
auto local_var_272_nxt_holder = local_var_272;
auto local_var_273 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_274 = 1;
bool local_var_275 = (local_var_273 == local_var_274);
auto local_var_278 = (local_var_275) ? sdp_cacc_data_9 : sdp_mrdma_data_9;
sc_biguint<16> local_var_279 = 0;
auto local_var_280 = sdp_group0_d_bs_alu_src.range(0, 0);
sc_biguint<1> local_var_281 = 1;
bool local_var_282 = (local_var_280 == local_var_281);
auto local_var_285 = (local_var_282) ? sdp_dma_data_9 : sdp_regs_data_9;
sc_biguint<10> local_var_286 = 0;
univ_var_189 = (sc_biguint<10>(local_var_286), sc_biguint<6>(sdp_group0_d_bs_alu_shift_value));
sc_biguint<16> local_var_288 = (local_var_285 << univ_var_189);
univ_var_190 = (sc_biguint<16>(local_var_279), sc_biguint<16>(local_var_288));
sc_biguint<32> local_var_290 = (local_var_278 + univ_var_190);
auto local_var_290_nxt_holder = local_var_290;
sdp_pdp_output_0 = local_var_20_nxt_holder;
sdp_pdp_output_1 = local_var_38_nxt_holder;
sdp_pdp_output_10 = local_var_56_nxt_holder;
sdp_pdp_output_11 = local_var_74_nxt_holder;
sdp_pdp_output_12 = local_var_92_nxt_holder;
sdp_pdp_output_13 = local_var_110_nxt_holder;
sdp_pdp_output_14 = local_var_128_nxt_holder;
sdp_pdp_output_15 = local_var_146_nxt_holder;
sdp_pdp_output_2 = local_var_164_nxt_holder;
sdp_pdp_output_3 = local_var_182_nxt_holder;
sdp_pdp_output_4 = local_var_200_nxt_holder;
sdp_pdp_output_5 = local_var_218_nxt_holder;
sdp_pdp_output_6 = local_var_236_nxt_holder;
sdp_pdp_output_7 = local_var_254_nxt_holder;
sdp_pdp_output_8 = local_var_272_nxt_holder;
sdp_pdp_output_9 = local_var_290_nxt_holder;
#ifdef ILATOR_VERBOSE
instr_update_log << "No." << std::dec << GetInstrCntr() << '\t' << "Compute_Add state updates:" << std::endl;
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
instr_update_log << std::endl;
#endif
}
