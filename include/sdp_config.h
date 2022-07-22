// =============================================================================
// MIT License
//
// Copyright (c) 2022 Princeton University
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// =============================================================================

// File: sdp_config.h

#ifndef NVDLA_SDP_CONFIG_H__
#define NVDLA_SDP_CONFIG_H__

namespace ilang {

inline std::string GetVarName(const std::string& var_name0, const std::string& var_name1) {
 
  return var_name0 + var_name1;
}

/**********************************************************************/
// ********************** CONFIG REGISTERS ***************************//
/**********************************************************************/

// b000 done
#define NVDLA_SDP_S_STATUS_0 "s_status0" // read_only
#define NVDLA_SDP_S_STATUS_1 "s_status1" // read_only
#define NVDLA_SDP_S_STATUS_WIDTH 2

// b004 done
#define NVDLA_SDP_S_PRODUCER "s_producer"
#define NVDLA_SDP_S_PRODUCER_WIDTH 1
#define NVDLA_SDP_S_CONSUMER "s_consumer"
#define NVDLA_SDP_S_CONSUMER_WIDTH 1

// b008 done
#define NVDLA_SDP_S_LUT_ACCESS_TYPE "s_lut_access_type"
#define NVDLA_SDP_S_LUT_ACCESS_TYPE_WIDTH 1
#define NVDLA_SDP_S_LUT_TABLE_ID "s_lut_table_id"
#define NVDLA_SDP_S_LUT_TABLE_ID_WIDTH 1
#define NVDLA_SDP_S_LUT_ADDR "s_lut_addr"
#define NVDLA_SDP_S_LUT_ADDR_WIDTH 10

// b00c done
#define NVDLA_SDP_S_LUT_ACCESS_DATA  "s_lut_data"
#define NVDLA_SDP_S_LUT_ACCESS_DATA_WIDTH 16

// b010 done
#define NVDLA_SDP_S_LUT_HYBRID_PRIORITY "s_lut_hybrid_priority"
#define NVDLA_SDP_S_LUT_HYBRID_PRIORITY_WIDTH 1
#define NVDLA_SDP_S_LUT_OFLOW_PRIORITY "s_lut_oflow_priority"
#define NVDLA_SDP_S_LUT_OFLOW_PRIORITY_WIDTH 1
#define NVDLA_SDP_S_LUT_UFLOW_PRIORITY "s_lut_uflow_priority"
#define NVDLA_SDP_S_LUT_UFLOW_PRIORITY_WIDTH 1
#define NVDLA_SDP_S_LUT_LE_FUNCTION "s_lut_le_function"
#define NVDLA_SDP_S_LUT_LE_FUNCTION_WIDTH 1

// b014 done
#define NVDLA_SDP_S_LUT_LO_INDEX_SELECT "s_lut_lo_index_select"
#define NVDLA_SDP_S_LUT_LO_INDEX_SELECT_WIDTH 8
#define NVDLA_SDP_S_LUT_LE_INDEX_SELECT "s_lut_le_index_select"
#define NVDLA_SDP_S_LUT_LE_INDEX_SELECT_WIDTH 8
#define NVDLA_SDP_S_LUT_LE_INDEX_OFFSET "s_lut_le_index_offset"
#define NVDLA_SDP_S_LUT_LE_INDEX_OFFSET_WIDTH 8

// b018 done
#define NVDLA_SDP_S_LUT_LE_START "s_lut_le_start"
#define NVDLA_SDP_S_LUT_LE_START_WIDTH 32

// b01c done
#define NVDLA_SDP_S_LUT_LE_END "s_lut_le_end"
#define NVDLA_SDP_S_LUT_LE_END_WIDTH 32

// b020 done
#define NVDLA_SDP_S_LUT_LO_START "s_lut_lo_start"
#define NVDLA_SDP_S_LUT_LO_START_WIDTH 32

// b024 done
#define NVDLA_SDP_S_LUT_LO_END "s_lut_lo_end"
#define NVDLA_SDP_S_LUT_LO_END_WIDTH 32

// b028 done
#define NVDLA_SDP_S_LUT_LE_SLOPE_OFLOW_SCALE "s_lut_le_slope_oflow_scale"
#define NVDLA_SDP_S_LUT_LE_SLOPE_OFLOW_SCALE_WIDTH 16
#define NVDLA_SDP_S_LUT_LE_SLOPE_UFLOW_SCALE "s_lut_le_slope_uflow_scale"
#define NVDLA_SDP_S_LUT_LE_SLOPE_UFLOW_SCALE_WIDTH 16

// b02c done
#define NVDLA_SDP_S_LUT_LE_SLOPE_OFLOW_SHIFT "s_lut_le_slope_oflow_shift"
#define NVDLA_SDP_S_LUT_LE_SLOPE_OFLOW_SHIFT_WIDTH 5
#define NVDLA_SDP_S_LUT_LE_SLOPE_UFLOW_SHIFT "s_lut_le_slope_uflow_shift"
#define NVDLA_SDP_S_LUT_LE_SLOPE_UFLOW_SHIFT_WIDTH 5

// b030 done
#define NVDLA_SDP_S_LUT_LO_SLOPE_OFLOW_SCALE "s_lut_lo_slope_oflow_scale"
#define NVDLA_SDP_S_LUT_LO_SLOPE_OFLOW_SCALE_WIDTH 16
#define NVDLA_SDP_S_LUT_LO_SLOPE_UFLOW_SCALE "s_lut_lo_slope_uflow_scale"
#define NVDLA_SDP_S_LUT_LO_SLOPE_UFLOW_SCALE_WIDTH 16

// b034 done
#define NVDLA_SDP_S_LUT_LO_SLOPE_OFLOW_SHIFT "s_lut_lo_slope_oflow_shift"
#define NVDLA_SDP_S_LUT_LO_SLOPE_OFLOW_SHIFT_WIDTH 5
#define NVDLA_SDP_S_LUT_LO_SLOPE_UFLOW_SHIFT "s_lut_lo_slope_uflow_shift"
#define NVDLA_SDP_S_LUT_LO_SLOPE_UFLOW_SHIFT_WIDTH 5

// b038 done
#define NVDLA_SDP_D_OP_ENABLE "d_op_en"
#define NVDLA_SDP_D_OP_ENABLE_WIDTH 1

// b03c done
#define NVDLA_SDP_D_DATA_CUBE_WIDTH "d_width"
#define NVDLA_SDP_D_DATA_CUBE_WIDTH_WIDTH 13

// b040 done
#define NVDLA_SDP_D_DATA_CUBE_HEIGHT "d_height"
#define NVDLA_SDP_D_DATA_CUBE_HEIGHT_WIDTH 13

// b044 done
#define NVDLA_SDP_D_DATA_CUBE_CHANNEL "d_channel"
#define NVDLA_SDP_D_DATA_CUBE_CHANNEL_WIDTH 13

// b048 done
#define NVDLA_SDP_D_DST_BASE_ADDR_LOW "d_dst_base_addr_low"
#define NVDLA_SDP_D_DST_BASE_ADDR_LOW_WIDTH 27

// b04c done
#define NVDLA_SDP_D_DST_BASE_ADDR_HIGH "d_dst_base_addr_high"
#define NVDLA_SDP_D_DST_BASE_ADDR_HIGH_WIDTH 32

// b050 done
#define NVDLA_SDP_D_DST_LINE_STRIDE "d_dst_line_stride"
#define NVDLA_SDP_D_DST_LINE_STRIDE_WIDTH 27

// b054 done
#define NVDLA_SDP_D_DST_SURFACE_STRIDE "d_dst_surface_stride"
#define NVDLA_SDP_D_DST_SURFACE_STRIDE_WIDTH 27

// b058 done
#define NVDLA_SDP_D_BS_RELU_BYPASS "d_bs_relu_bypass"
#define NVDLA_SDP_D_BS_RELU_BYPASS_WIDTH 1
#define NVDLA_SDP_D_BS_MUL_PRELU "d_bs_mul_prelu"
#define NVDLA_SDP_D_BS_MUL_PRELU_WIDTH 1
#define NVDLA_SDP_D_BS_MUL_BYPASS "d_bs_mul_bypass"
#define NVDLA_SDP_D_BS_MUL_BYPASS_WIDTH 1
#define NVDLA_SDP_D_BS_ALU_ALGO "d_bs_alu_algo"
#define NVDLA_SDP_D_BS_ALU_ALGO_WIDTH 2
#define NVDLA_SDP_D_BS_ALU_BYPASS "d_bs_alu_bypass"
#define NVDLA_SDP_D_BS_ALU_BYPASS_WIDTH 1
#define NVDLA_SDP_D_BS_BYPASS "d_bs_bypass"
#define NVDLA_SDP_D_BS_BYPASS_WIDTH 1

// b05c done
#define NVDLA_SDP_D_BS_ALU_SHIFT_VALUE   "d_bs_alu_shift_value"
#define NVDLA_SDP_D_BS_ALU_SHIFT_VALUE_WIDTH 6
#define NVDLA_SDP_D_BS_ALU_SRC   "d_bs_alu_src"
#define NVDLA_SDP_D_BS_ALU_SRC_WIDTH 1

// b060 done
#define NVDLA_SDP_D_DP_BS_ALU_SRC_VALUE "d_bs_alu_operand"
#define NVDLA_SDP_D_DP_BS_ALU_SRC_VALUE_WIDTH 16

// b064 done
#define NVDLA_SDP_D_BS_MUL_SHIFT_VALUE   "d_bs_mul_shift_value"
#define NVDLA_SDP_D_BS_MUL_SHIFT_VALUE_WIDTH 8
#define NVDLA_SDP_D_BS_MUL_SRC   "d_bs_mul_src"
#define NVDLA_SDP_D_BS_MUL_SRC_WIDTH 1

// b068 done
#define NVDLA_SDP_D_DP_BS_MUL_SRC_VALUE "d_bs_mul_operand"
#define NVDLA_SDP_D_DP_BS_MUL_SRC_VALUE_WIDTH 16

// b06c done
#define NVDLA_SDP_D_BN_RELU_BYPASS "d_bn_relu_bypass"
#define NVDLA_SDP_D_BN_RELU_BYPASS_WIDTH 1
#define NVDLA_SDP_D_BN_MUL_PRELU "d_bn_mul_prelu"
#define NVDLA_SDP_D_BN_MUL_PRELU_WIDTH 1
#define NVDLA_SDP_D_BN_MUL_BYPASS "d_bn_mul_bypass"
#define NVDLA_SDP_D_BN_MUL_BYPASS_WIDTH 1
#define NVDLA_SDP_D_BN_ALU_ALGO "d_bn_alu_algo"
#define NVDLA_SDP_D_BN_ALU_ALGO_WIDTH 2
#define NVDLA_SDP_D_BN_ALU_BYPASS "d_bn_alu_bypass"
#define NVDLA_SDP_D_BN_ALU_BYPASS_WIDTH 1
#define NVDLA_SDP_D_BN_BYPASS "d_bn_bypass"
#define NVDLA_SDP_D_BN_BYPASS_WIDTH 1

// b070 done
#define NVDLA_SDP_D_BN_ALU_SHIFT_VALUE   "d_bn_alu_shift_value"
#define NVDLA_SDP_D_BN_ALU_SHIFT_VALUE_WIDTH 6
#define NVDLA_SDP_D_BN_ALU_SRC   "d_bn_alu_src"
#define NVDLA_SDP_D_BN_ALU_SRC_WIDTH 1

// b074 done
#define NVDLA_SDP_D_DP_BN_ALU_SRC_VALUE "d_bn_alu_operand"
#define NVDLA_SDP_D_DP_BN_ALU_SRC_VALUE_WIDTH 16

// b078 done
#define NVDLA_SDP_D_BN_MUL_SHIFT_VALUE   "d_bn_mul_shift_value"
#define NVDLA_SDP_D_BN_MUL_SHIFT_VALUE_WIDTH 8
#define NVDLA_SDP_D_BN_MUL_SRC   "d_bn_mul_src"
#define NVDLA_SDP_D_BN_MUL_SRC_WIDTH 1

// b07c done
#define NVDLA_SDP_D_DP_BN_MUL_SRC_VALUE "d_bn_mul_operand"
#define NVDLA_SDP_D_DP_BN_MUL_SRC_VALUE_WIDTH 16

// b080 done
#define NVDLA_SDP_D_EW_LUT_BYPASS "d_ew_lut_bypass"
#define NVDLA_SDP_D_EW_LUT_BYPASS_WIDTH 1
#define NVDLA_SDP_D_EW_MUL_PRELU "d_ew_mul_prelu"
#define NVDLA_SDP_D_EW_MUL_PRELU_WIDTH 1
#define NVDLA_SDP_D_EW_MUL_BYPASS "d_ew_mul_bypass"
#define NVDLA_SDP_D_EW_MUL_BYPASS_WIDTH 1
#define NVDLA_SDP_D_EW_ALU_ALGO "d_ew_alu_algo"
#define NVDLA_SDP_D_EW_ALU_ALGO_WIDTH 2
#define NVDLA_SDP_D_EW_ALU_BYPASS "d_ew_alu_bypass"
#define NVDLA_SDP_D_EW_ALU_BYPASS_WIDTH 1
#define NVDLA_SDP_D_EW_BYPASS "d_ew_bypass"
#define NVDLA_SDP_D_EW_BYPASS_WIDTH 1

// b084 done
#define NVDLA_SDP_D_EW_ALU_CVT_BYPASS   "d_ew_alu_cvt_bypass"
#define NVDLA_SDP_D_EW_ALU_CVT_BYPASS_WIDTH 1
#define NVDLA_SDP_D_EW_ALU_SRC   "d_ew_alu_src"
#define NVDLA_SDP_D_EW_ALU_SRC_WIDTH 1

// b088 done
#define NVDLA_SDP_D_DP_EW_ALU_SRC_VALUE "d_ew_alu_operand"
#define NVDLA_SDP_D_DP_EW_ALU_SRC_VALUE_WIDTH 32

// b08c done
#define NVDLA_SDP_D_DP_EW_ALU_CVT_OFFSET_VALUE "d_ew_alu_cvt_offset"
#define NVDLA_SDP_D_DP_EW_ALU_CVT_OFFSET_VALUE_WIDTH 32

// b090 done
#define NVDLA_SDP_D_DP_EW_ALU_CVT_SCALE_VALUE "d_ew_alu_cvt_scale"
#define NVDLA_SDP_D_DP_EW_ALU_CVT_SCALE_VALUE_WIDTH 16

// b094 done
#define NVDLA_SDP_D_DP_EW_ALU_CVT_TRUNCATE_VALUE "d_ew_alu_cvt_truncate"
#define NVDLA_SDP_D_DP_EW_ALU_CVT_TRUNCATE_VALUE_WIDTH 6

// b098 done
#define NVDLA_SDP_D_EW_MUL_CVT_BYPASS   "d_ew_mul_cvt_bypass"
#define NVDLA_SDP_D_EW_MUL_CVT_BYPASS_WIDTH 1
#define NVDLA_SDP_D_EW_MUL_SRC   "d_ew_mul_src"
#define NVDLA_SDP_D_EW_MUL_SRC_WIDTH 1

// b09c done
#define NVDLA_SDP_D_DP_EW_MUL_SRC_VALUE "d_ew_mul_operand"
#define NVDLA_SDP_D_DP_EW_MUL_SRC_VALUE_WIDTH 32

// b0a0 done
#define NVDLA_SDP_D_DP_EW_MUL_CVT_OFFSET_VALUE "d_ew_mul_cvt_offset"
#define NVDLA_SDP_D_DP_EW_MUL_CVT_OFFSET_VALUE_WIDTH 32

// b0a4 done
#define NVDLA_SDP_D_DP_EW_MUL_CVT_SCALE_VALUE "d_ew_mul_cvt_scale"
#define NVDLA_SDP_D_DP_EW_MUL_CVT_SCALE_VALUE_WIDTH 16

// b0a8 done
#define NVDLA_SDP_D_DP_EW_MUL_CVT_TRUNCATE_VALUE "d_ew_mul_cvt_truncate"
#define NVDLA_SDP_D_DP_EW_MUL_CVT_TRUNCATE_VALUE_WIDTH 6

// b0ac done
#define NVDLA_SDP_D_DP_EW_TRUNCATE_VALUE "d_dp_ew_truncate"
#define NVDLA_SDP_D_DP_EW_TRUNCATE_VALUE_WIDTH 10

// b0b0 done
#define NVDLA_SDP_D_BATCH_NUMBER "d_batch_number"
#define NVDLA_SDP_D_BATCH_NUMBER_WIDTH 5
#define NVDLA_SDP_D_NAN_TO_ZERO "d_nan_to_zero"
#define NVDLA_SDP_D_NAN_TO_ZERO_WIDTH 1
#define NVDLA_SDP_D_WINOGRAD "d_winograd"
#define NVDLA_SDP_D_WINOGRAD_WIDTH 1
#define NVDLA_SDP_D_OUTPUT_DST "d_output_dst"
#define NVDLA_SDP_D_OUTPUT_DST_WIDTH 1
#define NVDLA_SDP_D_FLYING_MODE "d_flying_mode"
#define NVDLA_SDP_D_FLYING_MODE_WIDTH 1

// b0b4 done
#define NVDLA_SDP_D_DST_DMA_CFG "d_dst_ram_type"
#define NVDLA_SDP_D_DST_DMA_CFG_WIDTH 1

// b0b8 done
#define NVDLA_SDP_D_DST_BATCH_STRIDE "d_dst_batch_stride"
#define NVDLA_SDP_D_DST_BATCH_STRIDE_WIDTH 27

// b0bc done
#define NVDLA_SDP_D_OUT_PRECISION "d_out_precision"
#define NVDLA_SDP_D_OUT_PRECISION_WIDTH 2
#define NVDLA_SDP_D_PROC_PRECISION "d_proc_precision"
#define NVDLA_SDP_D_PROC_PRECISION_WIDTH 2

// b0c0 done
#define NVDLA_SDP_D_CVT_OFFSET "d_cvt_offset"
#define NVDLA_SDP_D_CVT_OFFSET_WIDTH 32

// b0c4 done
#define NVDLA_SDP_D_CVT_SCALE "d_cvt_scale"
#define NVDLA_SDP_D_CVT_SCALE_WIDTH 16

// b0c8 done
#define NVDLA_SDP_D_CVT_SHIFT   "d_cvt_shift"
#define NVDLA_SDP_D_CVT_SHIFT_WIDTH 6

// b0cc done
#define NVDLA_SDP_D_STATUS   "d_status_unequal"
#define NVDLA_SDP_D_STATUS_WIDTH 1

// b0d0 done
#define NVDLA_SDP_D_STATUS_NAN_INPUT_NUM "d_status_nan_input_num"
#define NVDLA_SDP_D_STATUS_NAN_INPUT_NUM_WIDTH 32

// b0d4 done
#define NVDLA_SDP_D_STATUS_INF_INPUT_NUM "d_status_inf_input_num"
#define NVDLA_SDP_D_STATUS_INF_INPUT_NUM_WIDTH 32

// b0d8 done
#define NVDLA_SDP_D_STATUS_NAN_OUTPUT_NUM   "d_status_nan_output_num"
#define NVDLA_SDP_D_STATUS_NAN_OUTPUT_NUM_WIDTH 32

// b0dc done
#define NVDLA_SDP_D_PERF_NAN_INF_COUNT_EN  "d_perf_nan_inf_count_en"
#define NVDLA_SDP_D_PERF_NAN_INF_COUNT_EN_WIDTH 1
#define NVDLA_SDP_D_PERF_SAT_EN  "d_perf_sat_en"
#define NVDLA_SDP_D_PERF_SAT_EN_WIDTH 1
#define NVDLA_SDP_D_PERF_LUT_EN  "d_perf_lut_en"
#define NVDLA_SDP_D_PERF_LUT_EN_WIDTH 1
#define NVDLA_SDP_D_PERF_DMA_EN  "d_perf_dma_en"
#define NVDLA_SDP_D_PERF_DMA_EN_WIDTH 1

// b0e0 done
#define NVDLA_SDP_D_PERF_WDMA_WRITE_STALL   "d_wdma_stall"
#define NVDLA_SDP_D_PERF_WDMA_WRITE_STALL_WIDTH 32

// b0e4 done
#define NVDLA_SDP_D_PERF_LUT_UFLOW "d_lut_uflow"
#define NVDLA_SDP_D_ZERO_PADDING_WIDTH 32

// b0e8 done
#define NVDLA_SDP_D_PERF_LUT_OFLOW "d_lut_oflow"
#define NVDLA_SDP_D_PERF_LUT_OFLOW_WIDTH 32

// b0ec done
#define NVDLA_SDP_D_PERF_OUT_SATURATION   "d_out_saturation"
#define NVDLA_SDP_D_PERF_OUT_SATURATION_WIDTH 32

// b0f0 done
#define NVDLA_SDP_D_PERF_LUT_HYBRID   "d_lut_hybrid"
#define NVDLA_SDP_D_PERF_LUT_HYBRID_WIDTH 32

// b0f4 done
#define NVDLA_SDP_D_PERF_LUT_LE_HIT   "d_lut_le_hit"
#define NVDLA_SDP_D_PERF_LUT_LE_HIT_WIDTH 32

// b0f8 done
#define NVDLA_SDP_D_PERF_LUT_LO_HIT   "d_lut_lo_hit"
#define NVDLA_SDP_D_PERF_LUT_LO_HIT_WIDTH 32

} // namespace ilang

#endif // NVDLA_SDP_CONFIG_H__
