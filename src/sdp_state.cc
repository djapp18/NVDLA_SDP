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

// File: sdp_state.cc

#include <ilang/ilang++.h>

#include <sdp_config.h>

namespace ilang {

void DefineSDPState(Ila& m) {
    // Config Registers

    // b000 done
    m.NewBvState(NVDLA_SDP_S_STATUS_0, NVDLA_SDP_S_STATUS_WIDTH);
    m.NewBvState(NVDLA_SDP_S_STATUS_1, NVDLA_SDP_S_STATUS_WIDTH);

    // b004 done
    m.NewBvState(NVDLA_SDP_S_PRODUCER, NVDLA_SDP_S_PRODUCER_WIDTH);
    m.NewBvState(NVDLA_SDP_S_CONSUMER, NVDLA_SDP_S_CONSUMER_WIDTH);

    // b008 done
    m.NewBvState(NVDLA_SDP_S_LUT_ACCESS_TYPE, NVDLA_SDP_S_LUT_ACCESS_TYPE_WIDTH);
    m.NewBvState(NVDLA_SDP_S_LUT_TABLE_ID, NVDLA_SDP_S_LUT_TABLE_ID_WIDTH);
    m.NewBvState(NVDLA_SDP_S_LUT_ADDR, NVDLA_SDP_S_LUT_ADDR_WIDTH);

    // b00c done
    m.NewBvState(NVDLA_SDP_S_LUT_ACCESS_DATA, NVDLA_SDP_S_LUT_ACCESS_DATA_WIDTH);

    // b010 done
    m.NewBvState(NVDLA_SDP_S_LUT_HYBRID_PRIORITY, NVDLA_SDP_S_LUT_HYBRID_PRIORITY_WIDTH);
    m.NewBvState(NVDLA_SDP_S_LUT_OFLOW_PRIORITY, NVDLA_SDP_S_LUT_OFLOW_PRIORITY_WIDTH);
    m.NewBvState(NVDLA_SDP_S_LUT_UFLOW_PRIORITY, NVDLA_SDP_S_LUT_UFLOW_PRIORITY_WIDTH);
    m.NewBvState(NVDLA_SDP_S_LUT_LE_FUNCTION, NVDLA_SDP_S_LUT_LE_FUNCTION_WIDTH);

    // b014 done
    m.NewBvState(NVDLA_SDP_S_LUT_LO_INDEX_SELECT, NVDLA_SDP_S_LUT_LO_INDEX_SELECT_WIDTH);
    m.NewBvState(NVDLA_SDP_S_LUT_LE_INDEX_SELECT, NVDLA_SDP_S_LUT_LE_INDEX_SELECT_WIDTH);
    m.NewBvState(NVDLA_SDP_S_LUT_LE_INDEX_OFFSET, NVDLA_SDP_S_LUT_LE_INDEX_OFFSET_WIDTH);

    // b018 done
    m.NewBvState(NVDLA_SDP_S_LUT_LE_START, NVDLA_SDP_S_LUT_LE_START_WIDTH);

    // b01c done
    m.NewBvState(NVDLA_SDP_S_LUT_LE_END, NVDLA_SDP_S_LUT_LE_END_WIDTH);

    // b020 done
    m.NewBvState(NVDLA_SDP_S_LUT_LO_START, NVDLA_SDP_S_LUT_LO_START_WIDTH);

    // b024 done
    m.NewBvState(NVDLA_SDP_S_LUT_LO_END, NVDLA_SDP_S_LUT_LO_END_WIDTH);

    // b028 done
    m.NewBvState(NVDLA_SDP_S_LUT_LE_SLOPE_OFLOW_SCALE, NVDLA_SDP_S_LUT_LE_SLOPE_OFLOW_SCALE_WIDTH);
    m.NewBvState(NVDLA_SDP_S_LUT_LE_SLOPE_UFLOW_SCALE, NVDLA_SDP_S_LUT_LE_SLOPE_UFLOW_SCALE_WIDTH);

    // b02c done
    m.NewBvState(NVDLA_SDP_S_LUT_LE_SLOPE_OFLOW_SHIFT, NVDLA_SDP_S_LUT_LE_SLOPE_OFLOW_SHIFT_WIDTH);
    m.NewBvState(NVDLA_SDP_S_LUT_LE_SLOPE_UFLOW_SHIFT, NVDLA_SDP_S_LUT_LE_SLOPE_UFLOW_SHIFT_WIDTH);

    // b030 done
    m.NewBvState(NVDLA_SDP_S_LUT_LO_SLOPE_OFLOW_SCALE, NVDLA_SDP_S_LUT_LO_SLOPE_OFLOW_SCALE_WIDTH);
    m.NewBvState(NVDLA_SDP_S_LUT_LO_SLOPE_UFLOW_SCALE, NVDLA_SDP_S_LUT_LO_SLOPE_UFLOW_SCALE_WIDTH);

    // b034 done
    m.NewBvState(NVDLA_SDP_S_LUT_LO_SLOPE_OFLOW_SHIFT, NVDLA_SDP_S_LUT_LO_SLOPE_OFLOW_SHIFT_WIDTH);
    m.NewBvState(NVDLA_SDP_S_LUT_LO_SLOPE_UFLOW_SHIFT, NVDLA_SDP_S_LUT_LO_SLOPE_UFLOW_SHIFT_WIDTH);

    for (auto i = 0; i < 2; i++) {

        // b038 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_OP_ENABLE, NVDLA_SDP_D_OP_ENABLE_WIDTH);

        // b03c done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_DATA_CUBE_WIDTH, NVDLA_SDP_D_DATA_CUBE_WIDTH_WIDTH);

        // b040 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_DATA_CUBE_HEIGHT, NVDLA_SDP_D_DATA_CUBE_HEIGHT_WIDTH);

        // b044 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_DATA_CUBE_CHANNEL, NVDLA_SDP_D_DATA_CUBE_CHANNEL_WIDTH);

        // b048 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_DST_BASE_ADDR_LOW, NVDLA_SDP_D_DST_BASE_ADDR_LOW_WIDTH);

        // b04c done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_DST_BASE_ADDR_HIGH, NVDLA_SDP_D_DST_BASE_ADDR_HIGH_WIDTH);

        // b050 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_DST_LINE_STRIDE, NVDLA_SDP_D_DST_LINE_STRIDE_WIDTH);

        // b054 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_DST_SURFACE_STRIDE, NVDLA_SDP_D_DST_SURFACE_STRIDE_WIDTH);

        // b058 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_BS_RELU_BYPASS, NVDLA_SDP_D_BS_RELU_BYPASS_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_BS_MUL_PRELU, NVDLA_SDP_D_BS_MUL_PRELU_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_BS_MUL_BYPASS, NVDLA_SDP_D_BS_MUL_BYPASS_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_BS_ALU_ALGO, NVDLA_SDP_D_BS_ALU_ALGO_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_BS_ALU_BYPASS, NVDLA_SDP_D_BS_ALU_BYPASS_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_BS_BYPASS, NVDLA_SDP_D_BS_BYPASS_WIDTH);

        // b05c done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_BS_ALU_SHIFT_VALUE, NVDLA_SDP_D_BS_ALU_SHIFT_VALUE_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_BS_ALU_SRC, NVDLA_SDP_D_BS_ALU_SRC_WIDTH);

        // b060 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_DP_BS_ALU_SRC_VALUE, NVDLA_SDP_D_DP_BS_ALU_SRC_VALUE_WIDTH);

        // b064 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_BS_MUL_SHIFT_VALUE, NVDLA_SDP_D_BS_MUL_SHIFT_VALUE_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_BS_MUL_SRC, NVDLA_SDP_D_BS_MUL_SRC_WIDTH);

        // b068 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_DP_BS_MUL_SRC_VALUE, NVDLA_SDP_D_DP_BS_MUL_SRC_VALUE_WIDTH);

        // b06c done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_BN_RELU_BYPASS, NVDLA_SDP_D_BN_RELU_BYPASS_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_BN_MUL_PRELU, NVDLA_SDP_D_BN_MUL_PRELU_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_BN_MUL_BYPASS, NVDLA_SDP_D_BN_MUL_BYPASS_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_BN_ALU_ALGO, NVDLA_SDP_D_BN_ALU_ALGO_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_BN_ALU_BYPASS, NVDLA_SDP_D_BN_ALU_BYPASS_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_BN_BYPASS, NVDLA_SDP_D_BN_BYPASS_WIDTH);

        // b070 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_BN_ALU_SHIFT_VALUE, NVDLA_SDP_D_BN_ALU_SHIFT_VALUE_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_BN_ALU_SRC, NVDLA_SDP_D_BN_ALU_SRC_WIDTH);

        // b074 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_DP_BN_ALU_SRC_VALUE, NVDLA_SDP_D_DP_BN_ALU_SRC_VALUE_WIDTH);

        // b078 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_BN_MUL_SHIFT_VALUE, NVDLA_SDP_D_BN_MUL_SHIFT_VALUE_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_BN_MUL_SRC, NVDLA_SDP_D_BN_MUL_SRC_WIDTH);

        // b07c done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_DP_BN_MUL_SRC_VALUE, NVDLA_SDP_D_DP_BN_MUL_SRC_VALUE_WIDTH);

        // b080 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_EW_LUT_BYPASS, NVDLA_SDP_D_EW_LUT_BYPASS_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_EW_MUL_PRELU, NVDLA_SDP_D_EW_MUL_PRELU_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_EW_MUL_BYPASS, NVDLA_SDP_D_EW_MUL_BYPASS_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_EW_ALU_ALGO, NVDLA_SDP_D_EW_ALU_ALGO_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_EW_ALU_BYPASS, NVDLA_SDP_D_EW_ALU_BYPASS_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_EW_BYPASS, NVDLA_SDP_D_EW_BYPASS_WIDTH);

        // b084 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_EW_ALU_CVT_BYPASS, NVDLA_SDP_D_EW_ALU_CVT_BYPASS_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_EW_ALU_SRC, NVDLA_SDP_D_EW_ALU_SRC_WIDTH);

        // b088 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_DP_EW_ALU_SRC_VALUE, NVDLA_SDP_D_DP_EW_ALU_SRC_VALUE_WIDTH);

        // b08c done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_DP_EW_ALU_CVT_OFFSET_VALUE, NVDLA_SDP_D_DP_EW_ALU_CVT_OFFSET_VALUE_WIDTH);

        // b090 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_DP_EW_ALU_CVT_SCALE_VALUE, NVDLA_SDP_D_DP_EW_ALU_CVT_SCALE_VALUE_WIDTH);

        // b094 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_DP_EW_ALU_CVT_TRUNCATE_VALUE, NVDLA_SDP_D_DP_EW_ALU_CVT_TRUNCATE_VALUE_WIDTH);

        // b098 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_EW_MUL_CVT_BYPASS, NVDLA_SDP_D_EW_MUL_CVT_BYPASS_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_EW_MUL_SRC, NVDLA_SDP_D_EW_MUL_SRC_WIDTH);

        // b09c done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_DP_EW_MUL_SRC_VALUE, NVDLA_SDP_D_DP_EW_MUL_SRC_VALUE_WIDTH);

        // b0a0 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_DP_EW_MUL_CVT_OFFSET_VALUE, NVDLA_SDP_D_DP_EW_MUL_CVT_OFFSET_VALUE_WIDTH);

        // b0a4 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_DP_EW_MUL_CVT_SCALE_VALUE, NVDLA_SDP_D_DP_EW_MUL_CVT_SCALE_VALUE_WIDTH);

        // b0a8 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_DP_EW_MUL_CVT_TRUNCATE_VALUE, NVDLA_SDP_D_DP_EW_MUL_CVT_TRUNCATE_VALUE_WIDTH);

        // b0ac done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_DP_EW_TRUNCATE_VALUE, NVDLA_SDP_D_DP_EW_TRUNCATE_VALUE_WIDTH);

        // b0b0 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_BATCH_NUMBER, NVDLA_SDP_D_BATCH_NUMBER_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_NAN_TO_ZERO, NVDLA_SDP_D_NAN_TO_ZERO_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_WINOGRAD, NVDLA_SDP_D_WINOGRAD_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_OUTPUT_DST, NVDLA_SDP_D_OUTPUT_DST_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_FLYING_MODE, NVDLA_SDP_D_FLYING_MODE_WIDTH);

        // b0b4 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_DST_DMA_CFG, NVDLA_SDP_D_DST_DMA_CFG_WIDTH);

        // b0b8 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_DST_BATCH_STRIDE, NVDLA_SDP_D_DST_BATCH_STRIDE_WIDTH);

        // b0bc done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_OUT_PRECISION, NVDLA_SDP_D_OUT_PRECISION_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_PROC_PRECISION, NVDLA_SDP_D_PROC_PRECISION_WIDTH);

        // b0c0 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_CVT_OFFSET, NVDLA_SDP_D_CVT_OFFSET_WIDTH);

        // b0c4 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_CVT_SCALE, NVDLA_SDP_D_CVT_SCALE_WIDTH);

        // b0c8 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_CVT_SHIFT, NVDLA_SDP_D_CVT_SHIFT_WIDTH);

        // b0cc done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_STATUS, NVDLA_SDP_D_STATUS_WIDTH);

        // b0d0 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_STATUS_NAN_INPUT_NUM, NVDLA_SDP_D_STATUS_NAN_INPUT_NUM_WIDTH);

        // b0d4 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_STATUS_INF_INPUT_NUM, NVDLA_SDP_D_STATUS_INF_INPUT_NUM_WIDTH);

        // b0d8 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_STATUS_NAN_OUTPUT_NUM, NVDLA_SDP_D_STATUS_NAN_OUTPUT_NUM_WIDTH);

        // b0dc done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_PERF_NAN_INF_COUNT_EN, NVDLA_SDP_D_PERF_NAN_INF_COUNT_EN_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_PERF_SAT_EN, NVDLA_SDP_D_PERF_SAT_EN_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_PERF_LUT_EN, NVDLA_SDP_D_PERF_LUT_EN_WIDTH);
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_PERF_DMA_EN, NVDLA_SDP_D_PERF_DMA_EN_WIDTH);

        // b0e0 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_PERF_WDMA_WRITE_STALL, NVDLA_SDP_D_PERF_WDMA_WRITE_STALL_WIDTH);

        // b0e4 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_PERF_LUT_UFLOW, NVDLA_SDP_D_ZERO_PADDING_WIDTH);

        // b0e8 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_PERF_LUT_OFLOW, NVDLA_SDP_D_PERF_LUT_OFLOW_WIDTH);

        // b0ec done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_PERF_OUT_SATURATION, NVDLA_SDP_D_PERF_OUT_SATURATION_WIDTH);

        // b0f0 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_PERF_LUT_HYBRID, NVDLA_SDP_D_PERF_LUT_HYBRID_WIDTH);

        // b0f4 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_PERF_LUT_LE_HIT, NVDLA_SDP_D_PERF_LUT_LE_HIT_WIDTH);

        // b0f8 done
        m.NewBvState("group" + (std::to_string(i)) + "_" + NVDLA_SDP_D_PERF_LUT_LO_HIT, NVDLA_SDP_D_PERF_LUT_LO_HIT_WIDTH);

    }

    // LUT - LE
    m.NewMemState("le_tbl", 7, 16);
    m.NewBvState("le_uflow", 1);
    m.NewBvState("le_oflow", 1);
    m.NewBvState("le_index", 9);
    m.NewBvState("le_fraction", 35);
        
    // LUT - LO
    m.NewMemState("lo_tbl", 9, 16);
    m.NewBvState("lo_uflow", 1);
    m.NewBvState("lo_oflow", 1);
    m.NewBvState("lo_index", 9);
    m.NewBvState("lo_fraction", 35);

    // LUT - General
    m.NewBvState("uflow", 1);
    m.NewBvState("oflow", 1);
    m.NewBvState("index", 9);
    m.NewBvState("fraction", 35);
    m.NewBvState("id", 1);

    // // States
    // m.NewBvState("credits", 9);

    // State
    m.NewBvState("sdp_state", 2);
}


} // namespace ilang