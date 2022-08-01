#include <iomanip>
#include <sdp.h>
static int instr_cntr = 0;
int sdp::GetInstrCntr() {
  return instr_cntr;
}
void sdp::IncrementInstrCntr() {
  instr_cntr++;
}
void sdp::LogInstrSequence(const std::string& instr_name) {
  instr_log << "Instr No." << std::setw(5) << GetInstrCntr() << '\t';
  instr_log << instr_name << " is activated\n";
  IncrementInstrCntr();
}
static bool g_initialized = false;
void sdp::compute() {
if (!g_initialized) {
  setup_initial_condition();
  g_initialized = true;
}
sdp_csb_addr = sdp_csb_addr_in.read();
sdp_csb_data = sdp_csb_data_in.read();
sdp_csb_write = sdp_csb_write_in.read();
sdp_csb_vld = sdp_csb_vld_in.read();
sdp_fifo_clr = sdp_fifo_clr_in.read();
sdp_done = sdp_done_in.read();
sdp_cacc_data_0 = sdp_cacc_data_0_in.read();
sdp_mrdma_data_0 = sdp_mrdma_data_0_in.read();
sdp_regs_data_0 = sdp_regs_data_0_in.read();
sdp_dma_data_0 = sdp_dma_data_0_in.read();
sdp_cacc_data_1 = sdp_cacc_data_1_in.read();
sdp_mrdma_data_1 = sdp_mrdma_data_1_in.read();
sdp_regs_data_1 = sdp_regs_data_1_in.read();
sdp_dma_data_1 = sdp_dma_data_1_in.read();
sdp_cacc_data_2 = sdp_cacc_data_2_in.read();
sdp_mrdma_data_2 = sdp_mrdma_data_2_in.read();
sdp_regs_data_2 = sdp_regs_data_2_in.read();
sdp_dma_data_2 = sdp_dma_data_2_in.read();
sdp_cacc_data_3 = sdp_cacc_data_3_in.read();
sdp_mrdma_data_3 = sdp_mrdma_data_3_in.read();
sdp_regs_data_3 = sdp_regs_data_3_in.read();
sdp_dma_data_3 = sdp_dma_data_3_in.read();
sdp_cacc_data_4 = sdp_cacc_data_4_in.read();
sdp_mrdma_data_4 = sdp_mrdma_data_4_in.read();
sdp_regs_data_4 = sdp_regs_data_4_in.read();
sdp_dma_data_4 = sdp_dma_data_4_in.read();
sdp_cacc_data_5 = sdp_cacc_data_5_in.read();
sdp_mrdma_data_5 = sdp_mrdma_data_5_in.read();
sdp_regs_data_5 = sdp_regs_data_5_in.read();
sdp_dma_data_5 = sdp_dma_data_5_in.read();
sdp_cacc_data_6 = sdp_cacc_data_6_in.read();
sdp_mrdma_data_6 = sdp_mrdma_data_6_in.read();
sdp_regs_data_6 = sdp_regs_data_6_in.read();
sdp_dma_data_6 = sdp_dma_data_6_in.read();
sdp_cacc_data_7 = sdp_cacc_data_7_in.read();
sdp_mrdma_data_7 = sdp_mrdma_data_7_in.read();
sdp_regs_data_7 = sdp_regs_data_7_in.read();
sdp_dma_data_7 = sdp_dma_data_7_in.read();
sdp_cacc_data_8 = sdp_cacc_data_8_in.read();
sdp_mrdma_data_8 = sdp_mrdma_data_8_in.read();
sdp_regs_data_8 = sdp_regs_data_8_in.read();
sdp_dma_data_8 = sdp_dma_data_8_in.read();
sdp_cacc_data_9 = sdp_cacc_data_9_in.read();
sdp_mrdma_data_9 = sdp_mrdma_data_9_in.read();
sdp_regs_data_9 = sdp_regs_data_9_in.read();
sdp_dma_data_9 = sdp_dma_data_9_in.read();
sdp_cacc_data_10 = sdp_cacc_data_10_in.read();
sdp_mrdma_data_10 = sdp_mrdma_data_10_in.read();
sdp_regs_data_10 = sdp_regs_data_10_in.read();
sdp_dma_data_10 = sdp_dma_data_10_in.read();
sdp_cacc_data_11 = sdp_cacc_data_11_in.read();
sdp_mrdma_data_11 = sdp_mrdma_data_11_in.read();
sdp_regs_data_11 = sdp_regs_data_11_in.read();
sdp_dma_data_11 = sdp_dma_data_11_in.read();
sdp_cacc_data_12 = sdp_cacc_data_12_in.read();
sdp_mrdma_data_12 = sdp_mrdma_data_12_in.read();
sdp_regs_data_12 = sdp_regs_data_12_in.read();
sdp_dma_data_12 = sdp_dma_data_12_in.read();
sdp_cacc_data_13 = sdp_cacc_data_13_in.read();
sdp_mrdma_data_13 = sdp_mrdma_data_13_in.read();
sdp_regs_data_13 = sdp_regs_data_13_in.read();
sdp_dma_data_13 = sdp_dma_data_13_in.read();
sdp_cacc_data_14 = sdp_cacc_data_14_in.read();
sdp_mrdma_data_14 = sdp_mrdma_data_14_in.read();
sdp_regs_data_14 = sdp_regs_data_14_in.read();
sdp_dma_data_14 = sdp_dma_data_14_in.read();
sdp_cacc_data_15 = sdp_cacc_data_15_in.read();
sdp_mrdma_data_15 = sdp_mrdma_data_15_in.read();
sdp_regs_data_15 = sdp_regs_data_15_in.read();
sdp_dma_data_15 = sdp_dma_data_15_in.read();
if (valid_sdp() && decode_sdp_SET_PRODUCER()) {
  update_sdp_SET_PRODUCER();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("SET_PRODUCER");
#endif
}
if (valid_sdp() && decode_sdp_LUT_ACCESS_CFG()) {
  update_sdp_LUT_ACCESS_CFG();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("LUT_ACCESS_CFG");
#endif
}
if (valid_sdp() && decode_sdp_LUT_ACCESS_DATA()) {
  update_sdp_LUT_ACCESS_DATA();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("LUT_ACCESS_DATA");
#endif
}
if (valid_sdp() && decode_sdp_LUT_CFG()) {
  update_sdp_LUT_CFG();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("LUT_CFG");
#endif
}
if (valid_sdp() && decode_sdp_LUT_INFO()) {
  update_sdp_LUT_INFO();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("LUT_INFO");
#endif
}
if (valid_sdp() && decode_sdp_LUT_LE_START()) {
  update_sdp_LUT_LE_START();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("LUT_LE_START");
#endif
}
if (valid_sdp() && decode_sdp_LUT_LE_END()) {
  update_sdp_LUT_LE_END();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("LUT_LE_END");
#endif
}
if (valid_sdp() && decode_sdp_LUT_LO_START()) {
  update_sdp_LUT_LO_START();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("LUT_LO_START");
#endif
}
if (valid_sdp() && decode_sdp_LUT_LO_END()) {
  update_sdp_LUT_LO_END();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("LUT_LO_END");
#endif
}
if (valid_sdp() && decode_sdp_LUT_LE_SLOPE_SCALE()) {
  update_sdp_LUT_LE_SLOPE_SCALE();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("LUT_LE_SLOPE_SCALE");
#endif
}
if (valid_sdp() && decode_sdp_LUT_LE_SLOPE_SHIFT()) {
  update_sdp_LUT_LE_SLOPE_SHIFT();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("LUT_LE_SLOPE_SHIFT");
#endif
}
if (valid_sdp() && decode_sdp_LUT_LO_SLOPE_SCALE()) {
  update_sdp_LUT_LO_SLOPE_SCALE();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("LUT_LO_SLOPE_SCALE");
#endif
}
if (valid_sdp() && decode_sdp_LUT_LO_SLOPE_SHIFT()) {
  update_sdp_LUT_LO_SLOPE_SHIFT();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("LUT_LO_SLOPE_SHIFT");
#endif
}
if (valid_sdp() && decode_sdp_OP_ENABLE_group0()) {
  update_sdp_OP_ENABLE_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("OP_ENABLE_group0");
#endif
}
if (valid_sdp() && decode_sdp_OP_ENABLE_group1()) {
  update_sdp_OP_ENABLE_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("OP_ENABLE_group1");
#endif
}
if (valid_sdp() && decode_sdp_DATA_CUBE_WIDTH_group0()) {
  update_sdp_DATA_CUBE_WIDTH_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DATA_CUBE_WIDTH_group0");
#endif
}
if (valid_sdp() && decode_sdp_DATA_CUBE_WIDTH_group1()) {
  update_sdp_DATA_CUBE_WIDTH_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DATA_CUBE_WIDTH_group1");
#endif
}
if (valid_sdp() && decode_sdp_DATA_CUBE_HEIGHT_group0()) {
  update_sdp_DATA_CUBE_HEIGHT_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DATA_CUBE_HEIGHT_group0");
#endif
}
if (valid_sdp() && decode_sdp_DATA_CUBE_HEIGHT_group1()) {
  update_sdp_DATA_CUBE_HEIGHT_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DATA_CUBE_HEIGHT_group1");
#endif
}
if (valid_sdp() && decode_sdp_DATA_CUBE_CHANNEL_group0()) {
  update_sdp_DATA_CUBE_CHANNEL_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DATA_CUBE_CHANNEL_group0");
#endif
}
if (valid_sdp() && decode_sdp_DATA_CUBE_CHANNEL_group1()) {
  update_sdp_DATA_CUBE_CHANNEL_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DATA_CUBE_CHANNEL_group1");
#endif
}
if (valid_sdp() && decode_sdp_DST_BASE_ADDR_LOW_group0()) {
  update_sdp_DST_BASE_ADDR_LOW_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DST_BASE_ADDR_LOW_group0");
#endif
}
if (valid_sdp() && decode_sdp_DST_BASE_ADDR_LOW_group1()) {
  update_sdp_DST_BASE_ADDR_LOW_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DST_BASE_ADDR_LOW_group1");
#endif
}
if (valid_sdp() && decode_sdp_DST_BASE_ADDR_HIGH_group0()) {
  update_sdp_DST_BASE_ADDR_HIGH_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DST_BASE_ADDR_HIGH_group0");
#endif
}
if (valid_sdp() && decode_sdp_DST_BASE_ADDR_HIGH_group1()) {
  update_sdp_DST_BASE_ADDR_HIGH_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DST_BASE_ADDR_HIGH_group1");
#endif
}
if (valid_sdp() && decode_sdp_DST_LINE_STRIDE_group0()) {
  update_sdp_DST_LINE_STRIDE_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DST_LINE_STRIDE_group0");
#endif
}
if (valid_sdp() && decode_sdp_DST_LINE_STRIDE_group1()) {
  update_sdp_DST_LINE_STRIDE_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DST_LINE_STRIDE_group1");
#endif
}
if (valid_sdp() && decode_sdp_DST_SURFACE_STRIDE_group0()) {
  update_sdp_DST_SURFACE_STRIDE_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DST_SURFACE_STRIDE_group0");
#endif
}
if (valid_sdp() && decode_sdp_DST_SURFACE_STRIDE_group1()) {
  update_sdp_DST_SURFACE_STRIDE_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DST_SURFACE_STRIDE_group1");
#endif
}
if (valid_sdp() && decode_sdp_DP_BS_CFG_group0()) {
  update_sdp_DP_BS_CFG_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_BS_CFG_group0");
#endif
}
if (valid_sdp() && decode_sdp_DP_BS_CFG_group1()) {
  update_sdp_DP_BS_CFG_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_BS_CFG_group1");
#endif
}
if (valid_sdp() && decode_sdp_DP_BS_ALU_CFG_group0()) {
  update_sdp_DP_BS_ALU_CFG_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_BS_ALU_CFG_group0");
#endif
}
if (valid_sdp() && decode_sdp_DP_BS_ALU_CFG_group1()) {
  update_sdp_DP_BS_ALU_CFG_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_BS_ALU_CFG_group1");
#endif
}
if (valid_sdp() && decode_sdp_DP_BS_ALU_SRC_VALUE_group0()) {
  update_sdp_DP_BS_ALU_SRC_VALUE_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_BS_ALU_SRC_VALUE_group0");
#endif
}
if (valid_sdp() && decode_sdp_DP_BS_ALU_SRC_VALUE_group1()) {
  update_sdp_DP_BS_ALU_SRC_VALUE_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_BS_ALU_SRC_VALUE_group1");
#endif
}
if (valid_sdp() && decode_sdp_DP_BS_MUL_CFG_group0()) {
  update_sdp_DP_BS_MUL_CFG_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_BS_MUL_CFG_group0");
#endif
}
if (valid_sdp() && decode_sdp_DP_BS_MUL_CFG_group1()) {
  update_sdp_DP_BS_MUL_CFG_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_BS_MUL_CFG_group1");
#endif
}
if (valid_sdp() && decode_sdp_DP_BS_MUL_SRC_VALUE_group0()) {
  update_sdp_DP_BS_MUL_SRC_VALUE_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_BS_MUL_SRC_VALUE_group0");
#endif
}
if (valid_sdp() && decode_sdp_DP_BS_MUL_SRC_VALUE_group1()) {
  update_sdp_DP_BS_MUL_SRC_VALUE_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_BS_MUL_SRC_VALUE_group1");
#endif
}
if (valid_sdp() && decode_sdp_DP_BN_CFG_group0()) {
  update_sdp_DP_BN_CFG_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_BN_CFG_group0");
#endif
}
if (valid_sdp() && decode_sdp_DP_BN_CFG_group1()) {
  update_sdp_DP_BN_CFG_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_BN_CFG_group1");
#endif
}
if (valid_sdp() && decode_sdp_DP_BN_ALU_CFG_group0()) {
  update_sdp_DP_BN_ALU_CFG_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_BN_ALU_CFG_group0");
#endif
}
if (valid_sdp() && decode_sdp_DP_BN_ALU_CFG_group1()) {
  update_sdp_DP_BN_ALU_CFG_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_BN_ALU_CFG_group1");
#endif
}
if (valid_sdp() && decode_sdp_DP_BN_ALU_SRC_VALUE_group0()) {
  update_sdp_DP_BN_ALU_SRC_VALUE_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_BN_ALU_SRC_VALUE_group0");
#endif
}
if (valid_sdp() && decode_sdp_DP_BN_ALU_SRC_VALUE_group1()) {
  update_sdp_DP_BN_ALU_SRC_VALUE_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_BN_ALU_SRC_VALUE_group1");
#endif
}
if (valid_sdp() && decode_sdp_DP_BN_MUL_CFG_group0()) {
  update_sdp_DP_BN_MUL_CFG_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_BN_MUL_CFG_group0");
#endif
}
if (valid_sdp() && decode_sdp_DP_BN_MUL_CFG_group1()) {
  update_sdp_DP_BN_MUL_CFG_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_BN_MUL_CFG_group1");
#endif
}
if (valid_sdp() && decode_sdp_DP_BN_MUL_SRC_VALUE_group0()) {
  update_sdp_DP_BN_MUL_SRC_VALUE_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_BN_MUL_SRC_VALUE_group0");
#endif
}
if (valid_sdp() && decode_sdp_DP_BN_MUL_SRC_VALUE_group1()) {
  update_sdp_DP_BN_MUL_SRC_VALUE_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_BN_MUL_SRC_VALUE_group1");
#endif
}
if (valid_sdp() && decode_sdp_DP_EW_CFG_group0()) {
  update_sdp_DP_EW_CFG_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_EW_CFG_group0");
#endif
}
if (valid_sdp() && decode_sdp_DP_EW_CFG_group1()) {
  update_sdp_DP_EW_CFG_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_EW_CFG_group1");
#endif
}
if (valid_sdp() && decode_sdp_DP_EW_ALU_CFG_group0()) {
  update_sdp_DP_EW_ALU_CFG_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_EW_ALU_CFG_group0");
#endif
}
if (valid_sdp() && decode_sdp_DP_EW_ALU_CFG_group1()) {
  update_sdp_DP_EW_ALU_CFG_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_EW_ALU_CFG_group1");
#endif
}
if (valid_sdp() && decode_sdp_DP_EW_ALU_SRC_VALUE_group0()) {
  update_sdp_DP_EW_ALU_SRC_VALUE_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_EW_ALU_SRC_VALUE_group0");
#endif
}
if (valid_sdp() && decode_sdp_DP_EW_ALU_CVT_OFFSET_VALUE_group0()) {
  update_sdp_DP_EW_ALU_CVT_OFFSET_VALUE_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_EW_ALU_CVT_OFFSET_VALUE_group0");
#endif
}
if (valid_sdp() && decode_sdp_DP_EW_ALU_CVT_OFFSET_VALUE_group1()) {
  update_sdp_DP_EW_ALU_CVT_OFFSET_VALUE_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_EW_ALU_CVT_OFFSET_VALUE_group1");
#endif
}
if (valid_sdp() && decode_sdp_DP_EW_ALU_CVT_SCALE_VALUE_group0()) {
  update_sdp_DP_EW_ALU_CVT_SCALE_VALUE_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_EW_ALU_CVT_SCALE_VALUE_group0");
#endif
}
if (valid_sdp() && decode_sdp_DP_EW_ALU_CVT_SCALE_VALUE_group1()) {
  update_sdp_DP_EW_ALU_CVT_SCALE_VALUE_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_EW_ALU_CVT_SCALE_VALUE_group1");
#endif
}
if (valid_sdp() && decode_sdp_DP_EW_ALU_CVT_TRUNCATE_VALUE_group0()) {
  update_sdp_DP_EW_ALU_CVT_TRUNCATE_VALUE_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_EW_ALU_CVT_TRUNCATE_VALUE_group0");
#endif
}
if (valid_sdp() && decode_sdp_DP_EW_ALU_CVT_TRUNCATE_VALUE_group1()) {
  update_sdp_DP_EW_ALU_CVT_TRUNCATE_VALUE_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_EW_ALU_CVT_TRUNCATE_VALUE_group1");
#endif
}
if (valid_sdp() && decode_sdp_DP_EW_MUL_CFG_group0()) {
  update_sdp_DP_EW_MUL_CFG_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_EW_MUL_CFG_group0");
#endif
}
if (valid_sdp() && decode_sdp_DP_EW_MUL_CFG_group1()) {
  update_sdp_DP_EW_MUL_CFG_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_EW_MUL_CFG_group1");
#endif
}
if (valid_sdp() && decode_sdp_DP_EW_MUL_SRC_VALUE_group0()) {
  update_sdp_DP_EW_MUL_SRC_VALUE_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_EW_MUL_SRC_VALUE_group0");
#endif
}
if (valid_sdp() && decode_sdp_DP_EW_MUL_SRC_VALUE_group1()) {
  update_sdp_DP_EW_MUL_SRC_VALUE_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_EW_MUL_SRC_VALUE_group1");
#endif
}
if (valid_sdp() && decode_sdp_DP_EW_MUL_CVT_OFFSET_VALUE_group0()) {
  update_sdp_DP_EW_MUL_CVT_OFFSET_VALUE_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_EW_MUL_CVT_OFFSET_VALUE_group0");
#endif
}
if (valid_sdp() && decode_sdp_DP_EW_MUL_CVT_OFFSET_VALUE_group1()) {
  update_sdp_DP_EW_MUL_CVT_OFFSET_VALUE_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_EW_MUL_CVT_OFFSET_VALUE_group1");
#endif
}
if (valid_sdp() && decode_sdp_DP_EW_MUL_CVT_SCALE_VALUE_group0()) {
  update_sdp_DP_EW_MUL_CVT_SCALE_VALUE_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_EW_MUL_CVT_SCALE_VALUE_group0");
#endif
}
if (valid_sdp() && decode_sdp_DP_EW_MUL_CVT_SCALE_VALUE_group1()) {
  update_sdp_DP_EW_MUL_CVT_SCALE_VALUE_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_EW_MUL_CVT_SCALE_VALUE_group1");
#endif
}
if (valid_sdp() && decode_sdp_DP_EW_MUL_CVT_TRUNCATE_VALUE_group0()) {
  update_sdp_DP_EW_MUL_CVT_TRUNCATE_VALUE_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_EW_MUL_CVT_TRUNCATE_VALUE_group0");
#endif
}
if (valid_sdp() && decode_sdp_DP_EW_MUL_CVT_TRUNCATE_VALUE_group1()) {
  update_sdp_DP_EW_MUL_CVT_TRUNCATE_VALUE_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_EW_MUL_CVT_TRUNCATE_VALUE_group1");
#endif
}
if (valid_sdp() && decode_sdp_DP_EW_TRUNCATE_VALUE_group0()) {
  update_sdp_DP_EW_TRUNCATE_VALUE_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_EW_TRUNCATE_VALUE_group0");
#endif
}
if (valid_sdp() && decode_sdp_DP_EW_TRUNCATE_VALUE_group1()) {
  update_sdp_DP_EW_TRUNCATE_VALUE_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DP_EW_TRUNCATE_VALUE_group1");
#endif
}
if (valid_sdp() && decode_sdp_FEATURE_MODE_CFG_group0()) {
  update_sdp_FEATURE_MODE_CFG_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("FEATURE_MODE_CFG_group0");
#endif
}
if (valid_sdp() && decode_sdp_FEATURE_MODE_CFG_group1()) {
  update_sdp_FEATURE_MODE_CFG_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("FEATURE_MODE_CFG_group1");
#endif
}
if (valid_sdp() && decode_sdp_DST_DMA_CFG_group0()) {
  update_sdp_DST_DMA_CFG_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DST_DMA_CFG_group0");
#endif
}
if (valid_sdp() && decode_sdp_DST_DMA_CFG_group1()) {
  update_sdp_DST_DMA_CFG_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DST_DMA_CFG_group1");
#endif
}
if (valid_sdp() && decode_sdp_DST_BATCH_STRIDE_group0()) {
  update_sdp_DST_BATCH_STRIDE_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DST_BATCH_STRIDE_group0");
#endif
}
if (valid_sdp() && decode_sdp_DST_BATCH_STRIDE_group1()) {
  update_sdp_DST_BATCH_STRIDE_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DST_BATCH_STRIDE_group1");
#endif
}
if (valid_sdp() && decode_sdp_DATA_FORMAT_group0()) {
  update_sdp_DATA_FORMAT_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DATA_FORMAT_group0");
#endif
}
if (valid_sdp() && decode_sdp_DATA_FORMAT_group1()) {
  update_sdp_DATA_FORMAT_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("DATA_FORMAT_group1");
#endif
}
if (valid_sdp() && decode_sdp_CVT_OFFSET_group0()) {
  update_sdp_CVT_OFFSET_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("CVT_OFFSET_group0");
#endif
}
if (valid_sdp() && decode_sdp_CVT_OFFSET_group1()) {
  update_sdp_CVT_OFFSET_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("CVT_OFFSET_group1");
#endif
}
if (valid_sdp() && decode_sdp_CVT_SCALE_group0()) {
  update_sdp_CVT_SCALE_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("CVT_SCALE_group0");
#endif
}
if (valid_sdp() && decode_sdp_CVT_SCALE_group1()) {
  update_sdp_CVT_SCALE_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("CVT_SCALE_group1");
#endif
}
if (valid_sdp() && decode_sdp_CVT_SHIFT_group0()) {
  update_sdp_CVT_SHIFT_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("CVT_SHIFT_group0");
#endif
}
if (valid_sdp() && decode_sdp_CVT_SHIFT_group1()) {
  update_sdp_CVT_SHIFT_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("CVT_SHIFT_group1");
#endif
}
if (valid_sdp() && decode_sdp_PERF_ENABLE_group0()) {
  update_sdp_PERF_ENABLE_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("PERF_ENABLE_group0");
#endif
}
if (valid_sdp() && decode_sdp_PERF_ENABLE_group1()) {
  update_sdp_PERF_ENABLE_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("PERF_ENABLE_group1");
#endif
}
if (valid_sdp() && decode_sdp_Start_group0()) {
  update_sdp_Start_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Start_group0");
#endif
}
if (valid_sdp() && decode_sdp_Start_group1()) {
  update_sdp_Start_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Start_group1");
#endif
}
if (valid_sdp() && decode_sdp_Busy2Done_group0()) {
  update_sdp_Busy2Done_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Busy2Done_group0");
#endif
}
if (valid_sdp() && decode_sdp_Done2Idle_group0()) {
  update_sdp_Done2Idle_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Done2Idle_group0");
#endif
}
if (valid_sdp() && decode_sdp_Compute_ReLU()) {
  update_sdp_Compute_ReLU();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Compute_ReLU");
#endif
}
if (valid_sdp() && decode_sdp_Compute_Max()) {
  update_sdp_Compute_Max();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Compute_Max");
#endif
}
if (valid_sdp() && decode_sdp_Compute_Min()) {
  update_sdp_Compute_Min();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Compute_Min");
#endif
}
if (valid_sdp() && decode_sdp_Compute_Add()) {
  update_sdp_Compute_Add();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Compute_Add");
#endif
}
if (valid_sdp() && decode_sdp_Compute_Multiply()) {
  update_sdp_Compute_Multiply();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Compute_Multiply");
#endif
}
if (valid_sdp() && decode_sdp_Compute_PReLU()) {
  update_sdp_Compute_PReLU();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Compute_PReLU");
#endif
}
if (valid_sdp() && decode_sdp_Write_LUT_LE()) {
  update_sdp_Write_LUT_LE();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Write_LUT_LE");
#endif
}
if (valid_sdp() && decode_sdp_Write_LUT_LO()) {
  update_sdp_Write_LUT_LO();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Write_LUT_LO");
#endif
}
if (valid_sdp() && decode_sdp_Read_LUT_LE()) {
  update_sdp_Read_LUT_LE();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Read_LUT_LE");
#endif
}
if (valid_sdp() && decode_sdp_Read_LUT_LO()) {
  update_sdp_Read_LUT_LO();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Read_LUT_LO");
#endif
}
if (valid_sdp() && decode_sdp_Present_LUT_Output()) {
  update_sdp_Present_LUT_Output();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Present_LUT_Output");
#endif
}
if (valid_sdp() && decode_sdp_Final_LUT_Processing()) {
  update_sdp_Final_LUT_Processing();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Final_LUT_Processing");
#endif
}
while (1) {
  int schedule_counter = 0;
  if (schedule_counter == 0) {
    break;
  }
}
}
