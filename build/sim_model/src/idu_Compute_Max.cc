#include <sdp.h>
bool sdp::decode_sdp_Compute_Max() {
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
sc_biguint<2> local_var_20 = 0;
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
sc_biguint<2> local_var_51 = 0;
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
bool local_var_73 = !local_var_72;
bool local_var_74 = (local_var_69 & local_var_73);
auto& univ_var_91 = local_var_74;
return univ_var_91;
}
void sdp::update_sdp_Compute_Max() {
#ifdef ILATOR_VERBOSE
instr_update_log << "No." << std::dec << GetInstrCntr() << '\t' << "Compute_Max state updates:" << std::endl;
instr_update_log << std::endl;
#endif
}
