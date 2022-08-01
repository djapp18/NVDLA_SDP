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
if (valid_sdp() && decode_sdp_Compute_ReLU_group0()) {
  update_sdp_Compute_ReLU_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Compute_ReLU_group0");
#endif
}
if (valid_sdp() && decode_sdp_Compute_ReLU_group1()) {
  update_sdp_Compute_ReLU_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Compute_ReLU_group1");
#endif
}
if (valid_sdp() && decode_sdp_Compute_Max_group0()) {
  update_sdp_Compute_Max_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Compute_Max_group0");
#endif
}
if (valid_sdp() && decode_sdp_Compute_Max_group1()) {
  update_sdp_Compute_Max_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Compute_Max_group1");
#endif
}
if (valid_sdp() && decode_sdp_Compute_Min_group0()) {
  update_sdp_Compute_Min_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Compute_Min_group0");
#endif
}
if (valid_sdp() && decode_sdp_Compute_Min_group1()) {
  update_sdp_Compute_Min_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Compute_Min_group1");
#endif
}
if (valid_sdp() && decode_sdp_Compute_Add_group0()) {
  update_sdp_Compute_Add_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Compute_Add_group0");
#endif
}
if (valid_sdp() && decode_sdp_Compute_Add_group1()) {
  update_sdp_Compute_Add_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Compute_Add_group1");
#endif
}
if (valid_sdp() && decode_sdp_Compute_Multiply_group0()) {
  update_sdp_Compute_Multiply_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Compute_Multiply_group0");
#endif
}
if (valid_sdp() && decode_sdp_Compute_Multiply_group1()) {
  update_sdp_Compute_Multiply_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Compute_Multiply_group1");
#endif
}
if (valid_sdp() && decode_sdp_Compute_PReLU_group0()) {
  update_sdp_Compute_PReLU_group0();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Compute_PReLU_group0");
#endif
}
if (valid_sdp() && decode_sdp_Compute_PReLU_group1()) {
  update_sdp_Compute_PReLU_group1();
  #ifdef ILATOR_VERBOSE
  LogInstrSequence("Compute_PReLU_group1");
#endif
}
while (1) {
  int schedule_counter = 0;
  if (schedule_counter == 0) {
    break;
  }
}
}
