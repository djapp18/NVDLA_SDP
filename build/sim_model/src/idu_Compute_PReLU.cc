#include <sdp.h>
bool sdp::decode_sdp_Compute_PReLU() {
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
auto local_var_15 = sdp_group0_d_bs_mul_bypass.range(0, 0);
sc_biguint<1> local_var_16 = 0;
bool local_var_17 = (local_var_15 == local_var_16);
auto local_var_19 = sdp_group0_d_bn_mul_prelu.range(0, 0);
sc_biguint<1> local_var_20 = 1;
bool local_var_21 = (local_var_19 == local_var_20);
bool local_var_22 = (local_var_17 & local_var_21);
auto local_var_24 = sdp_group0_d_bs_relu_bypass.range(0, 0);
sc_biguint<1> local_var_25 = 1;
bool local_var_26 = (local_var_24 == local_var_25);
bool local_var_27 = (local_var_22 & local_var_26);
auto local_var_29 = sdp_group0_d_bs_alu_bypass.range(0, 0);
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
auto local_var_46 = sdp_group0_d_bn_mul_bypass.range(0, 0);
sc_biguint<1> local_var_47 = 0;
bool local_var_48 = (local_var_46 == local_var_47);
auto local_var_49 = sdp_group0_d_bn_mul_prelu.range(0, 0);
sc_biguint<1> local_var_50 = 1;
bool local_var_51 = (local_var_49 == local_var_50);
bool local_var_52 = (local_var_48 & local_var_51);
auto local_var_54 = sdp_group0_d_bn_relu_bypass.range(0, 0);
sc_biguint<1> local_var_55 = 1;
bool local_var_56 = (local_var_54 == local_var_55);
bool local_var_57 = (local_var_52 & local_var_56);
auto local_var_59 = sdp_group0_d_bn_alu_bypass.range(0, 0);
sc_biguint<1> local_var_60 = 1;
bool local_var_61 = (local_var_59 == local_var_60);
bool local_var_62 = (local_var_57 & local_var_61);
bool local_var_63 = (local_var_44 & local_var_62);
bool local_var_64 = (local_var_33 | local_var_63);
sc_biguint<1> local_var_66 = 0;
bool local_var_67 = (sdp_s_consumer == local_var_66);
bool local_var_68 = (local_var_64 & local_var_67);
sc_biguint<1> local_var_70 = 0;
bool local_var_71 = (sdp_group0_d_op_en == local_var_70);
bool local_var_72 = (local_var_68 & local_var_71);
auto& univ_var_227 = local_var_72;
return univ_var_227;
}
void sdp::update_sdp_Compute_PReLU() {
auto local_var_1 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_2 = 1;
bool local_var_3 = (local_var_1 == local_var_2);
auto local_var_6 = (local_var_3) ? sdp_cacc_data_0 : sdp_mrdma_data_0;
sc_biguint<32> local_var_7 = 0;
bool local_var_8 = (local_var_6 > local_var_7);
sc_biguint<16> local_var_9 = 0;
auto local_var_11 = sdp_group0_d_bs_mul_src.range(0, 0);
sc_biguint<1> local_var_12 = 1;
bool local_var_13 = (local_var_11 == local_var_12);
auto local_var_16 = (local_var_13) ? sdp_dma_data_0 : sdp_regs_data_0;
univ_var_228 = (sc_biguint<16>(local_var_9), sc_biguint<16>(local_var_16));
sc_biguint<32> local_var_18 = (univ_var_228 * local_var_6);
sc_biguint<24> local_var_19 = 0;
univ_var_229 = (sc_biguint<24>(local_var_19), sc_biguint<8>(sdp_group0_d_bs_mul_shift_value));
sc_biguint<32> local_var_22 = (local_var_18 >> univ_var_229);
auto local_var_23 = (local_var_8) ? local_var_6 : local_var_22;
auto local_var_23_nxt_holder = local_var_23;
auto local_var_24 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_25 = 1;
bool local_var_26 = (local_var_24 == local_var_25);
auto local_var_29 = (local_var_26) ? sdp_cacc_data_1 : sdp_mrdma_data_1;
sc_biguint<32> local_var_30 = 0;
bool local_var_31 = (local_var_29 > local_var_30);
sc_biguint<16> local_var_32 = 0;
auto local_var_33 = sdp_group0_d_bs_mul_src.range(0, 0);
sc_biguint<1> local_var_34 = 1;
bool local_var_35 = (local_var_33 == local_var_34);
auto local_var_38 = (local_var_35) ? sdp_dma_data_1 : sdp_regs_data_1;
univ_var_230 = (sc_biguint<16>(local_var_32), sc_biguint<16>(local_var_38));
sc_biguint<32> local_var_40 = (univ_var_230 * local_var_29);
sc_biguint<24> local_var_41 = 0;
univ_var_231 = (sc_biguint<24>(local_var_41), sc_biguint<8>(sdp_group0_d_bs_mul_shift_value));
sc_biguint<32> local_var_43 = (local_var_40 >> univ_var_231);
auto local_var_44 = (local_var_31) ? local_var_29 : local_var_43;
auto local_var_44_nxt_holder = local_var_44;
auto local_var_45 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_46 = 1;
bool local_var_47 = (local_var_45 == local_var_46);
auto local_var_50 = (local_var_47) ? sdp_cacc_data_10 : sdp_mrdma_data_10;
sc_biguint<32> local_var_51 = 0;
bool local_var_52 = (local_var_50 > local_var_51);
sc_biguint<16> local_var_53 = 0;
auto local_var_54 = sdp_group0_d_bs_mul_src.range(0, 0);
sc_biguint<1> local_var_55 = 1;
bool local_var_56 = (local_var_54 == local_var_55);
auto local_var_59 = (local_var_56) ? sdp_dma_data_10 : sdp_regs_data_10;
univ_var_232 = (sc_biguint<16>(local_var_53), sc_biguint<16>(local_var_59));
sc_biguint<32> local_var_61 = (univ_var_232 * local_var_50);
sc_biguint<24> local_var_62 = 0;
univ_var_233 = (sc_biguint<24>(local_var_62), sc_biguint<8>(sdp_group0_d_bs_mul_shift_value));
sc_biguint<32> local_var_64 = (local_var_61 >> univ_var_233);
auto local_var_65 = (local_var_52) ? local_var_50 : local_var_64;
auto local_var_65_nxt_holder = local_var_65;
auto local_var_66 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_67 = 1;
bool local_var_68 = (local_var_66 == local_var_67);
auto local_var_71 = (local_var_68) ? sdp_cacc_data_11 : sdp_mrdma_data_11;
sc_biguint<32> local_var_72 = 0;
bool local_var_73 = (local_var_71 > local_var_72);
sc_biguint<16> local_var_74 = 0;
auto local_var_75 = sdp_group0_d_bs_mul_src.range(0, 0);
sc_biguint<1> local_var_76 = 1;
bool local_var_77 = (local_var_75 == local_var_76);
auto local_var_80 = (local_var_77) ? sdp_dma_data_11 : sdp_regs_data_11;
univ_var_234 = (sc_biguint<16>(local_var_74), sc_biguint<16>(local_var_80));
sc_biguint<32> local_var_82 = (univ_var_234 * local_var_71);
sc_biguint<24> local_var_83 = 0;
univ_var_235 = (sc_biguint<24>(local_var_83), sc_biguint<8>(sdp_group0_d_bs_mul_shift_value));
sc_biguint<32> local_var_85 = (local_var_82 >> univ_var_235);
auto local_var_86 = (local_var_73) ? local_var_71 : local_var_85;
auto local_var_86_nxt_holder = local_var_86;
auto local_var_87 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_88 = 1;
bool local_var_89 = (local_var_87 == local_var_88);
auto local_var_92 = (local_var_89) ? sdp_cacc_data_12 : sdp_mrdma_data_12;
sc_biguint<32> local_var_93 = 0;
bool local_var_94 = (local_var_92 > local_var_93);
sc_biguint<16> local_var_95 = 0;
auto local_var_96 = sdp_group0_d_bs_mul_src.range(0, 0);
sc_biguint<1> local_var_97 = 1;
bool local_var_98 = (local_var_96 == local_var_97);
auto local_var_101 = (local_var_98) ? sdp_dma_data_12 : sdp_regs_data_12;
univ_var_236 = (sc_biguint<16>(local_var_95), sc_biguint<16>(local_var_101));
sc_biguint<32> local_var_103 = (univ_var_236 * local_var_92);
sc_biguint<24> local_var_104 = 0;
univ_var_237 = (sc_biguint<24>(local_var_104), sc_biguint<8>(sdp_group0_d_bs_mul_shift_value));
sc_biguint<32> local_var_106 = (local_var_103 >> univ_var_237);
auto local_var_107 = (local_var_94) ? local_var_92 : local_var_106;
auto local_var_107_nxt_holder = local_var_107;
auto local_var_108 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_109 = 1;
bool local_var_110 = (local_var_108 == local_var_109);
auto local_var_113 = (local_var_110) ? sdp_cacc_data_13 : sdp_mrdma_data_13;
sc_biguint<32> local_var_114 = 0;
bool local_var_115 = (local_var_113 > local_var_114);
sc_biguint<16> local_var_116 = 0;
auto local_var_117 = sdp_group0_d_bs_mul_src.range(0, 0);
sc_biguint<1> local_var_118 = 1;
bool local_var_119 = (local_var_117 == local_var_118);
auto local_var_122 = (local_var_119) ? sdp_dma_data_13 : sdp_regs_data_13;
univ_var_238 = (sc_biguint<16>(local_var_116), sc_biguint<16>(local_var_122));
sc_biguint<32> local_var_124 = (univ_var_238 * local_var_113);
sc_biguint<24> local_var_125 = 0;
univ_var_239 = (sc_biguint<24>(local_var_125), sc_biguint<8>(sdp_group0_d_bs_mul_shift_value));
sc_biguint<32> local_var_127 = (local_var_124 >> univ_var_239);
auto local_var_128 = (local_var_115) ? local_var_113 : local_var_127;
auto local_var_128_nxt_holder = local_var_128;
auto local_var_129 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_130 = 1;
bool local_var_131 = (local_var_129 == local_var_130);
auto local_var_134 = (local_var_131) ? sdp_cacc_data_14 : sdp_mrdma_data_14;
sc_biguint<32> local_var_135 = 0;
bool local_var_136 = (local_var_134 > local_var_135);
sc_biguint<16> local_var_137 = 0;
auto local_var_138 = sdp_group0_d_bs_mul_src.range(0, 0);
sc_biguint<1> local_var_139 = 1;
bool local_var_140 = (local_var_138 == local_var_139);
auto local_var_143 = (local_var_140) ? sdp_dma_data_14 : sdp_regs_data_14;
univ_var_240 = (sc_biguint<16>(local_var_137), sc_biguint<16>(local_var_143));
sc_biguint<32> local_var_145 = (univ_var_240 * local_var_134);
sc_biguint<24> local_var_146 = 0;
univ_var_241 = (sc_biguint<24>(local_var_146), sc_biguint<8>(sdp_group0_d_bs_mul_shift_value));
sc_biguint<32> local_var_148 = (local_var_145 >> univ_var_241);
auto local_var_149 = (local_var_136) ? local_var_134 : local_var_148;
auto local_var_149_nxt_holder = local_var_149;
auto local_var_150 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_151 = 1;
bool local_var_152 = (local_var_150 == local_var_151);
auto local_var_155 = (local_var_152) ? sdp_cacc_data_15 : sdp_mrdma_data_15;
sc_biguint<32> local_var_156 = 0;
bool local_var_157 = (local_var_155 > local_var_156);
sc_biguint<16> local_var_158 = 0;
auto local_var_159 = sdp_group0_d_bs_mul_src.range(0, 0);
sc_biguint<1> local_var_160 = 1;
bool local_var_161 = (local_var_159 == local_var_160);
auto local_var_164 = (local_var_161) ? sdp_dma_data_15 : sdp_regs_data_15;
univ_var_242 = (sc_biguint<16>(local_var_158), sc_biguint<16>(local_var_164));
sc_biguint<32> local_var_166 = (univ_var_242 * local_var_155);
sc_biguint<24> local_var_167 = 0;
univ_var_243 = (sc_biguint<24>(local_var_167), sc_biguint<8>(sdp_group0_d_bs_mul_shift_value));
sc_biguint<32> local_var_169 = (local_var_166 >> univ_var_243);
auto local_var_170 = (local_var_157) ? local_var_155 : local_var_169;
auto local_var_170_nxt_holder = local_var_170;
auto local_var_171 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_172 = 1;
bool local_var_173 = (local_var_171 == local_var_172);
auto local_var_176 = (local_var_173) ? sdp_cacc_data_2 : sdp_mrdma_data_2;
sc_biguint<32> local_var_177 = 0;
bool local_var_178 = (local_var_176 > local_var_177);
sc_biguint<16> local_var_179 = 0;
auto local_var_180 = sdp_group0_d_bs_mul_src.range(0, 0);
sc_biguint<1> local_var_181 = 1;
bool local_var_182 = (local_var_180 == local_var_181);
auto local_var_185 = (local_var_182) ? sdp_dma_data_2 : sdp_regs_data_2;
univ_var_244 = (sc_biguint<16>(local_var_179), sc_biguint<16>(local_var_185));
sc_biguint<32> local_var_187 = (univ_var_244 * local_var_176);
sc_biguint<24> local_var_188 = 0;
univ_var_245 = (sc_biguint<24>(local_var_188), sc_biguint<8>(sdp_group0_d_bs_mul_shift_value));
sc_biguint<32> local_var_190 = (local_var_187 >> univ_var_245);
auto local_var_191 = (local_var_178) ? local_var_176 : local_var_190;
auto local_var_191_nxt_holder = local_var_191;
auto local_var_192 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_193 = 1;
bool local_var_194 = (local_var_192 == local_var_193);
auto local_var_197 = (local_var_194) ? sdp_cacc_data_3 : sdp_mrdma_data_3;
sc_biguint<32> local_var_198 = 0;
bool local_var_199 = (local_var_197 > local_var_198);
sc_biguint<16> local_var_200 = 0;
auto local_var_201 = sdp_group0_d_bs_mul_src.range(0, 0);
sc_biguint<1> local_var_202 = 1;
bool local_var_203 = (local_var_201 == local_var_202);
auto local_var_206 = (local_var_203) ? sdp_dma_data_3 : sdp_regs_data_3;
univ_var_246 = (sc_biguint<16>(local_var_200), sc_biguint<16>(local_var_206));
sc_biguint<32> local_var_208 = (univ_var_246 * local_var_197);
sc_biguint<24> local_var_209 = 0;
univ_var_247 = (sc_biguint<24>(local_var_209), sc_biguint<8>(sdp_group0_d_bs_mul_shift_value));
sc_biguint<32> local_var_211 = (local_var_208 >> univ_var_247);
auto local_var_212 = (local_var_199) ? local_var_197 : local_var_211;
auto local_var_212_nxt_holder = local_var_212;
auto local_var_213 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_214 = 1;
bool local_var_215 = (local_var_213 == local_var_214);
auto local_var_218 = (local_var_215) ? sdp_cacc_data_4 : sdp_mrdma_data_4;
sc_biguint<32> local_var_219 = 0;
bool local_var_220 = (local_var_218 > local_var_219);
sc_biguint<16> local_var_221 = 0;
auto local_var_222 = sdp_group0_d_bs_mul_src.range(0, 0);
sc_biguint<1> local_var_223 = 1;
bool local_var_224 = (local_var_222 == local_var_223);
auto local_var_227 = (local_var_224) ? sdp_dma_data_4 : sdp_regs_data_4;
univ_var_248 = (sc_biguint<16>(local_var_221), sc_biguint<16>(local_var_227));
sc_biguint<32> local_var_229 = (univ_var_248 * local_var_218);
sc_biguint<24> local_var_230 = 0;
univ_var_249 = (sc_biguint<24>(local_var_230), sc_biguint<8>(sdp_group0_d_bs_mul_shift_value));
sc_biguint<32> local_var_232 = (local_var_229 >> univ_var_249);
auto local_var_233 = (local_var_220) ? local_var_218 : local_var_232;
auto local_var_233_nxt_holder = local_var_233;
auto local_var_234 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_235 = 1;
bool local_var_236 = (local_var_234 == local_var_235);
auto local_var_239 = (local_var_236) ? sdp_cacc_data_5 : sdp_mrdma_data_5;
sc_biguint<32> local_var_240 = 0;
bool local_var_241 = (local_var_239 > local_var_240);
sc_biguint<16> local_var_242 = 0;
auto local_var_243 = sdp_group0_d_bs_mul_src.range(0, 0);
sc_biguint<1> local_var_244 = 1;
bool local_var_245 = (local_var_243 == local_var_244);
auto local_var_248 = (local_var_245) ? sdp_dma_data_5 : sdp_regs_data_5;
univ_var_250 = (sc_biguint<16>(local_var_242), sc_biguint<16>(local_var_248));
sc_biguint<32> local_var_250 = (univ_var_250 * local_var_239);
sc_biguint<24> local_var_251 = 0;
univ_var_251 = (sc_biguint<24>(local_var_251), sc_biguint<8>(sdp_group0_d_bs_mul_shift_value));
sc_biguint<32> local_var_253 = (local_var_250 >> univ_var_251);
auto local_var_254 = (local_var_241) ? local_var_239 : local_var_253;
auto local_var_254_nxt_holder = local_var_254;
auto local_var_255 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_256 = 1;
bool local_var_257 = (local_var_255 == local_var_256);
auto local_var_260 = (local_var_257) ? sdp_cacc_data_6 : sdp_mrdma_data_6;
sc_biguint<32> local_var_261 = 0;
bool local_var_262 = (local_var_260 > local_var_261);
sc_biguint<16> local_var_263 = 0;
auto local_var_264 = sdp_group0_d_bs_mul_src.range(0, 0);
sc_biguint<1> local_var_265 = 1;
bool local_var_266 = (local_var_264 == local_var_265);
auto local_var_269 = (local_var_266) ? sdp_dma_data_6 : sdp_regs_data_6;
univ_var_252 = (sc_biguint<16>(local_var_263), sc_biguint<16>(local_var_269));
sc_biguint<32> local_var_271 = (univ_var_252 * local_var_260);
sc_biguint<24> local_var_272 = 0;
univ_var_253 = (sc_biguint<24>(local_var_272), sc_biguint<8>(sdp_group0_d_bs_mul_shift_value));
sc_biguint<32> local_var_274 = (local_var_271 >> univ_var_253);
auto local_var_275 = (local_var_262) ? local_var_260 : local_var_274;
auto local_var_275_nxt_holder = local_var_275;
auto local_var_276 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_277 = 1;
bool local_var_278 = (local_var_276 == local_var_277);
auto local_var_281 = (local_var_278) ? sdp_cacc_data_7 : sdp_mrdma_data_7;
sc_biguint<32> local_var_282 = 0;
bool local_var_283 = (local_var_281 > local_var_282);
sc_biguint<16> local_var_284 = 0;
auto local_var_285 = sdp_group0_d_bs_mul_src.range(0, 0);
sc_biguint<1> local_var_286 = 1;
bool local_var_287 = (local_var_285 == local_var_286);
auto local_var_290 = (local_var_287) ? sdp_dma_data_7 : sdp_regs_data_7;
univ_var_254 = (sc_biguint<16>(local_var_284), sc_biguint<16>(local_var_290));
sc_biguint<32> local_var_292 = (univ_var_254 * local_var_281);
sc_biguint<24> local_var_293 = 0;
univ_var_255 = (sc_biguint<24>(local_var_293), sc_biguint<8>(sdp_group0_d_bs_mul_shift_value));
sc_biguint<32> local_var_295 = (local_var_292 >> univ_var_255);
auto local_var_296 = (local_var_283) ? local_var_281 : local_var_295;
auto local_var_296_nxt_holder = local_var_296;
auto local_var_297 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_298 = 1;
bool local_var_299 = (local_var_297 == local_var_298);
auto local_var_302 = (local_var_299) ? sdp_cacc_data_8 : sdp_mrdma_data_8;
sc_biguint<32> local_var_303 = 0;
bool local_var_304 = (local_var_302 > local_var_303);
sc_biguint<16> local_var_305 = 0;
auto local_var_306 = sdp_group0_d_bs_mul_src.range(0, 0);
sc_biguint<1> local_var_307 = 1;
bool local_var_308 = (local_var_306 == local_var_307);
auto local_var_311 = (local_var_308) ? sdp_dma_data_8 : sdp_regs_data_8;
univ_var_256 = (sc_biguint<16>(local_var_305), sc_biguint<16>(local_var_311));
sc_biguint<32> local_var_313 = (univ_var_256 * local_var_302);
sc_biguint<24> local_var_314 = 0;
univ_var_257 = (sc_biguint<24>(local_var_314), sc_biguint<8>(sdp_group0_d_bs_mul_shift_value));
sc_biguint<32> local_var_316 = (local_var_313 >> univ_var_257);
auto local_var_317 = (local_var_304) ? local_var_302 : local_var_316;
auto local_var_317_nxt_holder = local_var_317;
auto local_var_318 = sdp_group0_d_flying_mode.range(0, 0);
sc_biguint<1> local_var_319 = 1;
bool local_var_320 = (local_var_318 == local_var_319);
auto local_var_323 = (local_var_320) ? sdp_cacc_data_9 : sdp_mrdma_data_9;
sc_biguint<32> local_var_324 = 0;
bool local_var_325 = (local_var_323 > local_var_324);
sc_biguint<16> local_var_326 = 0;
auto local_var_327 = sdp_group0_d_bs_mul_src.range(0, 0);
sc_biguint<1> local_var_328 = 1;
bool local_var_329 = (local_var_327 == local_var_328);
auto local_var_332 = (local_var_329) ? sdp_dma_data_9 : sdp_regs_data_9;
univ_var_258 = (sc_biguint<16>(local_var_326), sc_biguint<16>(local_var_332));
sc_biguint<32> local_var_334 = (univ_var_258 * local_var_323);
sc_biguint<24> local_var_335 = 0;
univ_var_259 = (sc_biguint<24>(local_var_335), sc_biguint<8>(sdp_group0_d_bs_mul_shift_value));
sc_biguint<32> local_var_337 = (local_var_334 >> univ_var_259);
auto local_var_338 = (local_var_325) ? local_var_323 : local_var_337;
auto local_var_338_nxt_holder = local_var_338;
sdp_pdp_output_0 = local_var_23_nxt_holder;
sdp_pdp_output_1 = local_var_44_nxt_holder;
sdp_pdp_output_10 = local_var_65_nxt_holder;
sdp_pdp_output_11 = local_var_86_nxt_holder;
sdp_pdp_output_12 = local_var_107_nxt_holder;
sdp_pdp_output_13 = local_var_128_nxt_holder;
sdp_pdp_output_14 = local_var_149_nxt_holder;
sdp_pdp_output_15 = local_var_170_nxt_holder;
sdp_pdp_output_2 = local_var_191_nxt_holder;
sdp_pdp_output_3 = local_var_212_nxt_holder;
sdp_pdp_output_4 = local_var_233_nxt_holder;
sdp_pdp_output_5 = local_var_254_nxt_holder;
sdp_pdp_output_6 = local_var_275_nxt_holder;
sdp_pdp_output_7 = local_var_296_nxt_holder;
sdp_pdp_output_8 = local_var_317_nxt_holder;
sdp_pdp_output_9 = local_var_338_nxt_holder;
#ifdef ILATOR_VERBOSE
instr_update_log << "No." << std::dec << GetInstrCntr() << '\t' << "Compute_PReLU state updates:" << std::endl;
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
