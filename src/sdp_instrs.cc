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

// File: sdp_instrs.cc

#include <ilang/ilang++.h>

#include <sdp_config.h>

namespace ilang {

void DefineSDPInstrs(Ila& m) {
    // m.AddInit(m.state(NVDLA_CSC_S_STATUS_0) == BvConst(0, 2));
    // m.AddInit(m.state(NVDLA_CSC_S_STATUS_1) == BvConst(0, 2));

    auto csb_addr = Extract(Concat(m.input("csb_addr"), BvConst(0,2)), 11, 0);
    auto csb_valid = (m.state("csb_rdy") == BvConst(1,1)) & (m.input("csb_vld") == BvConst(1,1));
    auto csb_write = m.input("csb_write") == BvConst(1,1);
    auto group0_unset = m.state(GetVarName("group0_", NVDLA_SDP_D_OP_ENABLE)) == BvConst(0,1);
    auto group1_unset = m.state(GetVarName("group1_", NVDLA_SDP_D_OP_ENABLE)) == BvConst(0,1);
    auto producer = m.state(NVDLA_SDP_S_PRODUCER);
    auto consumer = m.state(NVDLA_SDP_S_CONSUMER);
    auto group0_sdp_state = m.state("group0_sdp_state");
    auto group1_sdp_state = m.state("group1_sdp_state");

    // Status: 3
    // 1: IDLE
    // 2: Producer (op_enable) and Consumer
    // 3: Producer (op_enable) but not consumer

    // CSB MMIO
    { // 0004
        auto instr = m.NewInstr("SET_PRODUCER");
        instr.SetDecode(csb_addr == 0x004 & csb_valid & csb_write);

        instr.SetUpdate(producer, Extract(m.input("csb_data"), 0, 0));
    }

    // LUT ACCESS CFG
    { // 0008
        auto instr = m.NewInstr("LUT_ACCESS_CFG");
        instr.SetDecode(csb_addr == 0x008 & csb_valid & csb_write);

        instr.SetUpdate(m.state(NVDLA_SDP_S_LUT_ACCESS_TYPE), SelectBit(m.input("csb_data"), 17));
        instr.SetUpdate(m.state(NVDLA_SDP_S_LUT_ADDR), Extract(m.input("csb_data"), 9, 0));
        instr.SetUpdate(m.state(NVDLA_SDP_S_LUT_TABLE_ID), SelectBit(m.input("csb_data"), 16));
    }

    // LUT ACCESS DATA
    { // 000c
        auto instr = m.NewInstr("LUT_ACCESS_DATA");
        instr.SetDecode(csb_addr == 0x00c & csb_valid & csb_write);

        instr.SetUpdate(m.state(NVDLA_SDP_S_LUT_ACCESS_DATA), Extract(m.input("csb_data"), 15, 0));
    }

    // LUT CFG
    { // 0010
        auto instr = m.NewInstr("LUT_CFG");
        instr.SetDecode(csb_addr == 0x010 & csb_valid & csb_write);

        instr.SetUpdate(m.state(NVDLA_SDP_S_LUT_HYBRID_PRIORITY), SelectBit(m.input("csb_data"), 6));
        instr.SetUpdate(m.state(NVDLA_SDP_S_LUT_OFLOW_PRIORITY), SelectBit(m.input("csb_data"), 5));
        instr.SetUpdate(m.state(NVDLA_SDP_S_LUT_UFLOW_PRIORITY), SelectBit(m.input("csb_data"), 4));
        instr.SetUpdate(m.state(NVDLA_SDP_S_LUT_LE_FUNCTION), SelectBit(m.input("csb_data"), 0));
    }

    // LUT INFO
    { // 0014
        auto instr = m.NewInstr("LUT_INFO");
        instr.SetDecode(csb_addr == 0x014 & csb_valid & csb_write);

        instr.SetUpdate(m.state(NVDLA_SDP_S_LUT_LO_INDEX_SELECT), Extract(m.input("csb_data"), 23, 16));
        instr.SetUpdate(m.state(NVDLA_SDP_S_LUT_LE_INDEX_SELECT), Extract(m.input("csb_data"), 15, 8));
        instr.SetUpdate(m.state(NVDLA_SDP_S_LUT_LE_INDEX_OFFSET), Extract(m.input("csb_data"), 7, 0));
    }

    // LUT LE START
    { // 0018
        auto instr = m.NewInstr("LUT_LE_START");
        instr.SetDecode(csb_addr == 0x018 & csb_valid & csb_write);

        instr.SetUpdate(m.state(NVDLA_SDP_S_LUT_LE_START), Extract(m.input("csb_data"), 31, 0));
    }

    // LUT LE END
    { // 001c
        auto instr = m.NewInstr("LUT_LE_END");
        instr.SetDecode(csb_addr == 0x01c & csb_valid & csb_write);

        instr.SetUpdate(m.state(NVDLA_SDP_S_LUT_LE_END), Extract(m.input("csb_data"), 31, 0));
    }

    // LUT LO START
    { // 0020
        auto instr = m.NewInstr("LUT_LO_START");
        instr.SetDecode(csb_addr == 0x020 & csb_valid & csb_write);

        instr.SetUpdate(m.state(NVDLA_SDP_S_LUT_LO_START), Extract(m.input("csb_data"), 31, 0));
    }

    // LUT LO END
    { // 0024
        auto instr = m.NewInstr("LUT_LO_END");
        instr.SetDecode(csb_addr == 0x024 & csb_valid & csb_write);

        instr.SetUpdate(m.state(NVDLA_SDP_S_LUT_LO_END), Extract(m.input("csb_data"), 31, 0));
    }

    // LUT LE SLOPE SCALE
    { // 0028
        auto instr = m.NewInstr("LUT_LE_SLOPE_SCALE");
        instr.SetDecode(csb_addr == 0x028 & csb_valid & csb_write);

        instr.SetUpdate(m.state(NVDLA_SDP_S_LUT_LE_SLOPE_OFLOW_SCALE), Extract(m.input("csb_data"), 31, 16));
        instr.SetUpdate(m.state(NVDLA_SDP_S_LUT_LE_SLOPE_UFLOW_SCALE), Extract(m.input("csb_data"), 15, 0));
    }

    // LUT LE SLOPE SHIFT
    { // 002c
        auto instr = m.NewInstr("LUT_LE_SLOPE_SHIFT");
        instr.SetDecode(csb_addr == 0x028 & csb_valid & csb_write);

        instr.SetUpdate(m.state(NVDLA_SDP_S_LUT_LE_SLOPE_OFLOW_SHIFT), Extract(m.input("csb_data"), 9, 5));
        instr.SetUpdate(m.state(NVDLA_SDP_S_LUT_LE_SLOPE_UFLOW_SHIFT), Extract(m.input("csb_data"), 4, 0));
    }

    // LUT LO SLOPE SCALE
    { // 0030
        auto instr = m.NewInstr("LUT_LO_SLOPE_SCALE");
        instr.SetDecode(csb_addr == 0x030 & csb_valid & csb_write);

        instr.SetUpdate(m.state(NVDLA_SDP_S_LUT_LO_SLOPE_OFLOW_SCALE), Extract(m.input("csb_data"), 31, 16));
        instr.SetUpdate(m.state(NVDLA_SDP_S_LUT_LO_SLOPE_UFLOW_SCALE), Extract(m.input("csb_data"), 15, 0));
    }

    // LUT LO SLOPE SHIFT
    { // 0034
        auto instr = m.NewInstr("LUT_LO_SLOPE_SHIFT");
        instr.SetDecode(csb_addr == 0x034 & csb_valid & csb_write);

        instr.SetUpdate(m.state(NVDLA_SDP_S_LUT_LO_SLOPE_OFLOW_SHIFT), Extract(m.input("csb_data"), 9, 5));
        instr.SetUpdate(m.state(NVDLA_SDP_S_LUT_LO_SLOPE_UFLOW_SHIFT), Extract(m.input("csb_data"), 4, 0));
    }

    // OP ENABLE
    { // 0038_group0
        auto instr = m.NewInstr("OP_ENABLE_group0");
        instr.SetDecode(csb_addr == 0x038 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_OP_ENABLE)), SelectBit(m.input("csb_data"), 0));
    }

    { // 0038_group1
        auto instr = m.NewInstr("OP_ENABLE_group1");
        instr.SetDecode(csb_addr == 0x038 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_OP_ENABLE)), SelectBit(m.input("csb_data"), 0));
    }

    // DATA CUBE WIDTH
    { // 003c_group0
        auto instr = m.NewInstr("DATA_CUBE_WIDTH_group0");
        instr.SetDecode(csb_addr == 0x03c & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_DATA_CUBE_WIDTH)), Extract(m.input("csb_data"), 12, 0));
    }

    { // 003c_group1
        auto instr = m.NewInstr("DATA_CUBE_WIDTH_group1");
        instr.SetDecode(csb_addr == 0x03c & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_DATA_CUBE_WIDTH)), Extract(m.input("csb_data"), 12, 0));
    }

    // DATA CUBE HEIGHT
    { // 0040_group0
        auto instr = m.NewInstr("DATA_CUBE_HEIGHT_group0");
        instr.SetDecode(csb_addr == 0x040 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_DATA_CUBE_HEIGHT)), Extract(m.input("csb_data"), 12, 0));
    }

    { // 0040_group1
        auto instr = m.NewInstr("DATA_CUBE_HEIGHT_group1");
        instr.SetDecode(csb_addr == 0x040 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_DATA_CUBE_HEIGHT)), Extract(m.input("csb_data"), 12, 0));
    }

    // DATA CUBE CHANNEL
    { // 0044_group0
        auto instr = m.NewInstr("DATA_CUBE_CHANNEL_group0");
        instr.SetDecode(csb_addr == 0x044 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_DATA_CUBE_CHANNEL)), Extract(m.input("csb_data"), 12, 0));
    }

    { // 0044_group1
        auto instr = m.NewInstr("DATA_CUBE_CHANNEL_group1");
        instr.SetDecode(csb_addr == 0x044 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_DATA_CUBE_CHANNEL)), Extract(m.input("csb_data"), 12, 0));
    }

    // DST BASE ADDR LOW
    { // 0048_group0
        auto instr = m.NewInstr("DST_BASE_ADDR_LOW_group0");
        instr.SetDecode(csb_addr == 0x048 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_DST_BASE_ADDR_LOW)), Extract(m.input("csb_data"), 31, 5));
    }

    { // 0048_group1
        auto instr = m.NewInstr("DST_BASE_ADDR_LOW_group1");
        instr.SetDecode(csb_addr == 0x048 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_DST_BASE_ADDR_LOW)), Extract(m.input("csb_data"), 31, 5));
    }

    // DST BASE ADDR HIGH
    { // 004c_group0
        auto instr = m.NewInstr("DST_BASE_ADDR_HIGH_group0");
        instr.SetDecode(csb_addr == 0x04c & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_DST_BASE_ADDR_HIGH)), Extract(m.input("csb_data"), 31, 0));
    }

    { // 004c_group1
        auto instr = m.NewInstr("DST_BASE_ADDR_HIGH_group1");
        instr.SetDecode(csb_addr == 0x04c & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_DST_BASE_ADDR_HIGH)), Extract(m.input("csb_data"), 31, 0));
    }

    // DST LINE STRIDE
    { // 0050_group0
        auto instr = m.NewInstr("DST_LINE_STRIDE_group0");
        instr.SetDecode(csb_addr == 0x050 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_DST_LINE_STRIDE)), Extract(m.input("csb_data"), 31, 5));
    }

    { // 0050_group1
        auto instr = m.NewInstr("DST_LINE_STRIDE_group1");
        instr.SetDecode(csb_addr == 0x050 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_DST_LINE_STRIDE)), Extract(m.input("csb_data"), 31, 5));
    }

    // DST SURFACE STRIDE
    { // 0054_group0
        auto instr = m.NewInstr("DST_SURFACE_STRIDE_group0");
        instr.SetDecode(csb_addr == 0x054 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_DST_SURFACE_STRIDE)), Extract(m.input("csb_data"), 31, 5));
    }

    { // 0054_group1
        auto instr = m.NewInstr("DST_SURFACE_STRIDE_group1");
        instr.SetDecode(csb_addr == 0x054 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_DST_SURFACE_STRIDE)), Extract(m.input("csb_data"), 31, 5));
    }

    // DP BS CFG
    { // 0058_group0
        auto instr = m.NewInstr("DP_BS_CFG_group0");
        instr.SetDecode(csb_addr == 0x058 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_BS_RELU_BYPASS)), SelectBit(m.input("csb_data"), 6));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_BS_MUL_PRELU)), SelectBit(m.input("csb_data"), 5));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_BS_MUL_BYPASS)), SelectBit(m.input("csb_data"), 4));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_BS_ALU_ALGO)), Extract(m.input("csb_data"), 3, 2));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_BS_ALU_BYPASS)), SelectBit(m.input("csb_data"), 1));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_BS_BYPASS)), SelectBit(m.input("csb_data"), 0));
    }

    { // 0058_group1
        auto instr = m.NewInstr("DP_BS_CFG_group1");
        instr.SetDecode(csb_addr == 0x058 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_BS_RELU_BYPASS)), SelectBit(m.input("csb_data"), 6));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_BS_MUL_PRELU)), SelectBit(m.input("csb_data"), 5));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_BS_MUL_BYPASS)), SelectBit(m.input("csb_data"), 4));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_BS_ALU_ALGO)), Extract(m.input("csb_data"), 3, 2));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_BS_ALU_BYPASS)), SelectBit(m.input("csb_data"), 1));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_BS_BYPASS)), SelectBit(m.input("csb_data"), 0));
    }

    // DP BS ALU CFG
    { // 005c_group0
        auto instr = m.NewInstr("DP_BS_ALU_CFG_group0");
        instr.SetDecode(csb_addr == 0x05c & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_BS_ALU_SHIFT_VALUE)), Extract(m.input("csb_data"), 13, 8));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_BS_ALU_SRC)), SelectBit(m.input("csb_data"), 0));
    }

    { // 005c_group1
        auto instr = m.NewInstr("DP_BS_ALU_CFG_group1");
        instr.SetDecode(csb_addr == 0x05c & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_BS_ALU_SHIFT_VALUE)), Extract(m.input("csb_data"), 13, 8));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_BS_ALU_SRC)), SelectBit(m.input("csb_data"), 0));
    }

    // DP BS ALU SRC VALUE
    { // 0060_group0
        auto instr = m.NewInstr("DP_BS_ALU_SRC_VALUE_group0");
        instr.SetDecode(csb_addr == 0x060 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_DP_BS_ALU_SRC_VALUE)), Extract(m.input("csb_data"), 15, 0));
    }

    { // 0060_group1
        auto instr = m.NewInstr("DP_BS_ALU_SRC_VALUE_group1");
        instr.SetDecode(csb_addr == 0x060 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_DP_BS_ALU_SRC_VALUE)), Extract(m.input("csb_data"), 15, 0));
    }

    // DP BS MUL CFG
    { // 0064_group0
        auto instr = m.NewInstr("DP_BS_MUL_CFG_group0");
        instr.SetDecode(csb_addr == 0x064 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_BS_MUL_SHIFT_VALUE)), Extract(m.input("csb_data"), 15, 8));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_BS_MUL_SRC)), SelectBit(m.input("csb_data"), 0));
    }

    { // 0064_group1
        auto instr = m.NewInstr("DP_BS_MUL_CFG_group1");
        instr.SetDecode(csb_addr == 0x064 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_BS_MUL_SHIFT_VALUE)), Extract(m.input("csb_data"), 15, 8));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_BS_MUL_SRC)), SelectBit(m.input("csb_data"), 0));
    }

    // DP BS MUL SRC VALUE
    { // 0068_group0
        auto instr = m.NewInstr("DP_BS_MUL_SRC_VALUE_group0");
        instr.SetDecode(csb_addr == 0x068 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_DP_BS_MUL_SRC_VALUE)), Extract(m.input("csb_data"), 15, 0));
    }

    { // 0068_group1
        auto instr = m.NewInstr("DP_BS_MUL_SRC_VALUE_group1");
        instr.SetDecode(csb_addr == 0x068 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_DP_BS_MUL_SRC_VALUE)), Extract(m.input("csb_data"), 15, 0));
    }

    // DP BN CFG
    { // 006c_group0
        auto instr = m.NewInstr("DP_BN_CFG_group0");
        instr.SetDecode(csb_addr == 0x06c & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_BN_RELU_BYPASS)), SelectBit(m.input("csb_data"), 6));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_BN_MUL_PRELU)), SelectBit(m.input("csb_data"), 5));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_BN_MUL_BYPASS)), SelectBit(m.input("csb_data"), 4));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_BN_ALU_ALGO)), Extract(m.input("csb_data"), 3, 2));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_BN_ALU_BYPASS)), SelectBit(m.input("csb_data"), 1));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_BN_BYPASS)), SelectBit(m.input("csb_data"), 0));
    }

    { // 006c_group1
        auto instr = m.NewInstr("DP_BN_CFG_group1");
        instr.SetDecode(csb_addr == 0x06c & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_BN_RELU_BYPASS)), SelectBit(m.input("csb_data"), 6));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_BN_MUL_PRELU)), SelectBit(m.input("csb_data"), 5));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_BN_MUL_BYPASS)), SelectBit(m.input("csb_data"), 4));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_BN_ALU_ALGO)), Extract(m.input("csb_data"), 3, 2));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_BN_ALU_BYPASS)), SelectBit(m.input("csb_data"), 1));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_BN_BYPASS)), SelectBit(m.input("csb_data"), 0));
    }

    // DP BN ALU CFG
    { // 0070_group0
        auto instr = m.NewInstr("DP_BN_ALU_CFG_group0");
        instr.SetDecode(csb_addr == 0x070 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_BN_ALU_SHIFT_VALUE)), Extract(m.input("csb_data"), 13, 8));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_BN_ALU_SRC)), SelectBit(m.input("csb_data"), 0));
    }

    { // 0070_group1
        auto instr = m.NewInstr("DP_BN_ALU_CFG_group1");
        instr.SetDecode(csb_addr == 0x070 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_BN_ALU_SHIFT_VALUE)), Extract(m.input("csb_data"), 13, 8));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_BN_ALU_SRC)), SelectBit(m.input("csb_data"), 0));
    }

    // DP BN ALU SRC VALUE
    { // 0074_group0
        auto instr = m.NewInstr("DP_BN_ALU_SRC_VALUE_group0");
        instr.SetDecode(csb_addr == 0x074 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_DP_BN_ALU_SRC_VALUE)), Extract(m.input("csb_data"), 15, 0));
    }

    { // 0074_group1
        auto instr = m.NewInstr("DP_BN_ALU_SRC_VALUE_group1");
        instr.SetDecode(csb_addr == 0x074 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_DP_BN_ALU_SRC_VALUE)), Extract(m.input("csb_data"), 15, 0));
    }

    // DP BN MUL CFG
    { // 0078_group0
        auto instr = m.NewInstr("DP_BN_MUL_CFG_group0");
        instr.SetDecode(csb_addr == 0x078 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_BN_MUL_SHIFT_VALUE)), Extract(m.input("csb_data"), 15, 8));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_BN_MUL_SRC)), SelectBit(m.input("csb_data"), 0));
    }

    { // 0078_group1
        auto instr = m.NewInstr("DP_BN_MUL_CFG_group1");
        instr.SetDecode(csb_addr == 0x078 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_BN_MUL_SHIFT_VALUE)), Extract(m.input("csb_data"), 15, 8));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_BN_MUL_SRC)), SelectBit(m.input("csb_data"), 0));
    }

    // DP BN MUL SRC VALUE
    { // 007c_group0
        auto instr = m.NewInstr("DP_BN_MUL_SRC_VALUE_group0");
        instr.SetDecode(csb_addr == 0x07c & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_DP_BN_MUL_SRC_VALUE)), Extract(m.input("csb_data"), 15, 0));
    }

    { // 007c_group1
        auto instr = m.NewInstr("DP_BN_MUL_SRC_VALUE_group1");
        instr.SetDecode(csb_addr == 0x07c & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_DP_BN_MUL_SRC_VALUE)), Extract(m.input("csb_data"), 15, 0));
    }

    // DP EW CFG
    { // 0080_group0
        auto instr = m.NewInstr("DP_EW_CFG_group0");
        instr.SetDecode(csb_addr == 0x080 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_EW_LUT_BYPASS)), SelectBit(m.input("csb_data"), 6));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_EW_MUL_PRELU)), SelectBit(m.input("csb_data"), 5));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_EW_MUL_BYPASS)), SelectBit(m.input("csb_data"), 4));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_EW_ALU_ALGO)), Extract(m.input("csb_data"), 3, 2));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_EW_ALU_BYPASS)), SelectBit(m.input("csb_data"), 1));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_EW_BYPASS)), SelectBit(m.input("csb_data"), 0));
    }

    { // 0080_group1
        auto instr = m.NewInstr("DP_EW_CFG_group1");
        instr.SetDecode(csb_addr == 0x080 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_EW_LUT_BYPASS)), SelectBit(m.input("csb_data"), 6));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_EW_MUL_PRELU)), SelectBit(m.input("csb_data"), 5));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_EW_MUL_BYPASS)), SelectBit(m.input("csb_data"), 4));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_EW_ALU_ALGO)), Extract(m.input("csb_data"), 3, 2));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_EW_ALU_BYPASS)), SelectBit(m.input("csb_data"), 1));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_EW_BYPASS)), SelectBit(m.input("csb_data"), 0));
    }

    // DP EW ALU CFG
    { // 0084_group0
        auto instr = m.NewInstr("DP_EW_ALU_CFG_group0");
        instr.SetDecode(csb_addr == 0x084 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_EW_ALU_CVT_BYPASS)), SelectBit(m.input("csb_data"), 1));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_EW_ALU_SRC)), SelectBit(m.input("csb_data"), 0));
    }

    { // 0084_group1
        auto instr = m.NewInstr("DP_EW_ALU_CFG_group1");
        instr.SetDecode(csb_addr == 0x084 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_EW_ALU_CVT_BYPASS)), SelectBit(m.input("csb_data"), 1));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_EW_ALU_SRC)), SelectBit(m.input("csb_data"), 0));
    }

    // DP EW ALU SRC VALUE
    { // 0088_group0
        auto instr = m.NewInstr("DP_EW_ALU_SRC_VALUE_group0");
        instr.SetDecode(csb_addr == 0x088 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_DP_EW_ALU_SRC_VALUE)), Extract(m.input("csb_data"), 31, 0));
    }

    { // 0088_group1
        auto instr = m.NewInstr("DP_EW_ALU_SRC_VALUE_group0");
        instr.SetDecode(csb_addr == 0x088 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_DP_EW_ALU_SRC_VALUE)), Extract(m.input("csb_data"), 31, 0));
    }

    // DP EW ALU CVT OFFSET VALUE
    { // 008c_group0
        auto instr = m.NewInstr("DP_EW_ALU_CVT_OFFSET_VALUE_group0");
        instr.SetDecode(csb_addr == 0x08c & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_DP_EW_ALU_CVT_OFFSET_VALUE)), Extract(m.input("csb_data"), 31, 0));
    }

    { // 008c_group1
        auto instr = m.NewInstr("DP_EW_ALU_CVT_OFFSET_VALUE_group1");
        instr.SetDecode(csb_addr == 0x08c & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_DP_EW_ALU_CVT_OFFSET_VALUE)), Extract(m.input("csb_data"), 31, 0));
    }

    // DP EW ALU CVT SCALE VALUE
    { // 0090_group0
        auto instr = m.NewInstr("DP_EW_ALU_CVT_SCALE_VALUE_group0");
        instr.SetDecode(csb_addr == 0x090 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_DP_EW_ALU_CVT_SCALE_VALUE)), Extract(m.input("csb_data"), 15, 0));
    }

    { // 0090_group1
        auto instr = m.NewInstr("DP_EW_ALU_CVT_SCALE_VALUE_group1");
        instr.SetDecode(csb_addr == 0x090 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_DP_EW_ALU_CVT_SCALE_VALUE)), Extract(m.input("csb_data"), 15, 0));
    }

    // DP EW ALU CVT TRUNCATE VALUE
    { // 0094_group0
        auto instr = m.NewInstr("DP_EW_ALU_CVT_TRUNCATE_VALUE_group0");
        instr.SetDecode(csb_addr == 0x094 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_DP_EW_ALU_CVT_TRUNCATE_VALUE)), Extract(m.input("csb_data"), 5, 0));
    }

    { // 0094_group1
        auto instr = m.NewInstr("DP_EW_ALU_CVT_TRUNCATE_VALUE_group1");
        instr.SetDecode(csb_addr == 0x094 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_DP_EW_ALU_CVT_TRUNCATE_VALUE)), Extract(m.input("csb_data"), 5, 0));
    }

    // DP EW MUL CFG
    { // 0098_group0
        auto instr = m.NewInstr("DP_EW_MUL_CFG_group0");
        instr.SetDecode(csb_addr == 0x090 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_EW_MUL_CVT_BYPASS)), SelectBit(m.input("csb_data"), 1));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_EW_MUL_SRC)), SelectBit(m.input("csb_data"), 0));
    }

    { // 0098_group1
        auto instr = m.NewInstr("DP_EW_MUL_CFG_group1");
        instr.SetDecode(csb_addr == 0x098 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_EW_MUL_CVT_BYPASS)), SelectBit(m.input("csb_data"), 1));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_EW_MUL_SRC)), SelectBit(m.input("csb_data"), 0));
    }

    // DP EW MUL SRC VALUE
    { // 009c_group0
        auto instr = m.NewInstr("DP_EW_MUL_SRC_VALUE_group0");
        instr.SetDecode(csb_addr == 0x09c & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_DP_EW_MUL_SRC_VALUE)), Extract(m.input("csb_data"), 31, 0));
    }

    { // 009c_group1
        auto instr = m.NewInstr("DP_EW_MUL_SRC_VALUE_group1");
        instr.SetDecode(csb_addr == 0x09c & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_DP_EW_MUL_SRC_VALUE)), Extract(m.input("csb_data"), 31, 0));
    }

    // DP EW MUL CVT OFFSET VALUE
    { // 00a0_group0
        auto instr = m.NewInstr("DP_EW_MUL_CVT_OFFSET_VALUE_group0");
        instr.SetDecode(csb_addr == 0x0a0 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_DP_EW_MUL_CVT_OFFSET_VALUE)), Extract(m.input("csb_data"), 31, 0));
    }

    { // 00a0_group1
        auto instr = m.NewInstr("DP_EW_MUL_CVT_OFFSET_VALUE_group1");
        instr.SetDecode(csb_addr == 0x0a0 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_DP_EW_MUL_CVT_OFFSET_VALUE)), Extract(m.input("csb_data"), 31, 0));
    }

    // DP EW MUL CVT SCALE VALUE
    { // 00a4_group0
        auto instr = m.NewInstr("DP_EW_MUL_CVT_SCALE_VALUE_group0");
        instr.SetDecode(csb_addr == 0x0a4 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_DP_EW_MUL_CVT_SCALE_VALUE)), Extract(m.input("csb_data"), 15, 0));
    }

    { // 00a4_group1
        auto instr = m.NewInstr("DP_EW_MUL_CVT_SCALE_VALUE_group1");
        instr.SetDecode(csb_addr == 0x0a4 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_DP_EW_MUL_CVT_SCALE_VALUE)), Extract(m.input("csb_data"), 15, 0));
    }

    // DP EW MUL CVT TRUNCATE VALUE
    { // 00a8_group0
        auto instr = m.NewInstr("DP_EW_MUL_CVT_TRUNCATE_VALUE_group0");
        instr.SetDecode(csb_addr == 0x0a8 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_DP_EW_MUL_CVT_TRUNCATE_VALUE)), Extract(m.input("csb_data"), 5, 0));
    }

    { // 00a8_group1
        auto instr = m.NewInstr("DP_EW_MUL_CVT_TRUNCATE_VALUE_group1");
        instr.SetDecode(csb_addr == 0x0a8 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_DP_EW_MUL_CVT_TRUNCATE_VALUE)), Extract(m.input("csb_data"), 5, 0));
    }

    // DP EW TRUNCATE VALUE
    { // 00ac_group0
        auto instr = m.NewInstr("DP_EW_TRUNCATE_VALUE_group0");
        instr.SetDecode(csb_addr == 0x0ac & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_DP_EW_TRUNCATE_VALUE)), Extract(m.input("csb_data"), 9, 0));
    }

    { // 00ac_group1
        auto instr = m.NewInstr("DP_EW_TRUNCATE_VALUE_group1");
        instr.SetDecode(csb_addr == 0x0ac & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_DP_EW_TRUNCATE_VALUE)), Extract(m.input("csb_data"), 9, 0));
    }

    // FEATURE MODE CFG
    { // 00b0_group0
        auto instr = m.NewInstr("FEATURE_MODE_CFG_group0");
        instr.SetDecode(csb_addr == 0x0b0 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_BATCH_NUMBER)), Extract(m.input("csb_data"), 12, 8));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_NAN_TO_ZERO)), SelectBit(m.input("csb_data"), 3));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_WINOGRAD)), SelectBit(m.input("csb_data"), 2));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_OUTPUT_DST)), SelectBit(m.input("csb_data"), 1));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_FLYING_MODE)), SelectBit(m.input("csb_data"), 0));
    }

    { // 00b0_group1
        auto instr = m.NewInstr("FEATURE_MODE_CFG_group1");
        instr.SetDecode(csb_addr == 0x0b0 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_BATCH_NUMBER)), Extract(m.input("csb_data"), 12, 8));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_NAN_TO_ZERO)), SelectBit(m.input("csb_data"), 3));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_WINOGRAD)), SelectBit(m.input("csb_data"), 2));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_OUTPUT_DST)), SelectBit(m.input("csb_data"), 1));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_FLYING_MODE)), SelectBit(m.input("csb_data"), 0));
    }
    
    // DST DMA CFG
    { // 00b4_group0
        auto instr = m.NewInstr("DST_DMA_CFG_group0");
        instr.SetDecode(csb_addr == 0x0b4 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_DST_DMA_CFG)), SelectBit(m.input("csb_data"), 0));
    }

    { // 00b4_group1
        auto instr = m.NewInstr("DST_DMA_CFG_group1");
        instr.SetDecode(csb_addr == 0x0b4 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_DST_DMA_CFG)), SelectBit(m.input("csb_data"), 0));
    }

    // DST BATCH STRIDE
    { // 00b8_group0
        auto instr = m.NewInstr("DST_BATCH_STRIDE_group0");
        instr.SetDecode(csb_addr == 0x0b8 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_DST_BATCH_STRIDE)), Extract(m.input("csb_data"), 31, 5));
    }

    { // 00b8_group1
        auto instr = m.NewInstr("DST_BATCH_STRIDE_group1");
        instr.SetDecode(csb_addr == 0x0b8 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_DST_BATCH_STRIDE)), Extract(m.input("csb_data"), 31, 5));
    }

    // DATA FORMAT
    { // 00bc_group0
        auto instr = m.NewInstr("DATA_FORMAT_group0");
        instr.SetDecode(csb_addr == 0x0bc & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_OUT_PRECISION)), Extract(m.input("csb_data"), 3, 2));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_PROC_PRECISION)), Extract(m.input("csb_data"), 1, 0));
    }

    { // 00bc_group1
        auto instr = m.NewInstr("DATA_FORMAT_group1");
        instr.SetDecode(csb_addr == 0x0bc & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_OUT_PRECISION)), Extract(m.input("csb_data"), 3, 2));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_PROC_PRECISION)), Extract(m.input("csb_data"), 1, 0));
    }

    // CVT OFFSET
    { // 00c0_group0
        auto instr = m.NewInstr("CVT_OFFSET_group0");
        instr.SetDecode(csb_addr == 0x0c0 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_CVT_OFFSET)), Extract(m.input("csb_data"), 31, 0));
    }

    { // 00c0_group1
        auto instr = m.NewInstr("CVT_OFFSET_group1");
        instr.SetDecode(csb_addr == 0x0c0 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_CVT_OFFSET)), Extract(m.input("csb_data"), 31, 0));
    }

    // CVT SCALE
    { // 00c4_group0
        auto instr = m.NewInstr("CVT_SCALE_group0");
        instr.SetDecode(csb_addr == 0x0c4 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_CVT_SCALE)), Extract(m.input("csb_data"), 15, 0));
    }

    { // 00c4_group1
        auto instr = m.NewInstr("CVT_SCALE_group1");
        instr.SetDecode(csb_addr == 0x0c4 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_CVT_SCALE)), Extract(m.input("csb_data"), 15, 0));
    }

    // CVT SHIFT
    { // 00c8_group0
        auto instr = m.NewInstr("CVT_SHIFT_group0");
        instr.SetDecode(csb_addr == 0x0c8 & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_CVT_SHIFT)), Extract(m.input("csb_data"), 5, 0));
    }

    { // 00c8_group1
        auto instr = m.NewInstr("CVT_SHIFT_group1");
        instr.SetDecode(csb_addr == 0x0c8 & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_CVT_SHIFT)), Extract(m.input("csb_data"), 5, 0));
    }

    // PERF ENABLE
    { // 00dc_group0
        auto instr = m.NewInstr("PERF_ENABLE_group0");
        instr.SetDecode(csb_addr == 0x0dc & csb_valid & csb_write & producer == BvConst(0,1) & group0_unset);

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_PERF_NAN_INF_COUNT_EN)), SelectBit(m.input("csb_data"), 3));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_PERF_SAT_EN)), SelectBit(m.input("csb_data"), 2));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_PERF_LUT_EN)), SelectBit(m.input("csb_data"), 1));
        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_PERF_DMA_EN)), SelectBit(m.input("csb_data"), 0));
    }

    { // 00dc_group1
        auto instr = m.NewInstr("PERF_ENABLE_group1");
        instr.SetDecode(csb_addr == 0x0dc & csb_valid & csb_write & producer == BvConst(1,1) & group1_unset);

        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_PERF_NAN_INF_COUNT_EN)), SelectBit(m.input("csb_data"), 3));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_PERF_SAT_EN)), SelectBit(m.input("csb_data"), 2));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_PERF_LUT_EN)), SelectBit(m.input("csb_data"), 1));
        instr.SetUpdate(m.state(GetVarName("group1_", NVDLA_SDP_D_PERF_DMA_EN)), SelectBit(m.input("csb_data"), 0));
    }

    // Start register group from IDLE
    { // Group 0
        auto instr = m.NewInstr("Start_group0");
        auto group0_ok = consumer == BvConst(0,1) & m.state(GetVarName("group0_", NVDLA_SDP_D_OP_ENABLE)) == BvConst(1,1);
        instr.SetDecode(group0_sdp_state == IDLE & group0_ok);

        instr.SetUpdate(group0_sdp_state, BUSY);
    }

    { // Group 1
        auto instr = m.NewInstr("Start_group1");
        auto group1_ok = consumer == BvConst(1,1) & m.state(GetVarName("group1_", NVDLA_SDP_D_OP_ENABLE)) == BvConst(1,1);
        instr.SetDecode(group1_sdp_state == IDLE & group1_ok);

        instr.SetUpdate(group1_sdp_state, BUSY);
    }

    // Busy2Done
    { // Group 0
        auto instr = m.NewInstr("Busy2Done_group0");
        instr.SetDecode(group0_sdp_state == BUSY & SelectBit(m.input("fifo_clr"), 0) == 0x1);

        instr.SetUpdate(group0_sdp_state, DONE);
    }

    // Done2Idle0
    { // Group 0
        auto instr = m.NewInstr("Done2Idle_group0");
        instr.SetDecode(group0_sdp_state == DONE & m.input("done") & consumer == BvConst(0,1));

        instr.SetUpdate(group0_sdp_state, IDLE);
        instr.SetUpdate(consumer, BvConst(1,1));

        instr.SetUpdate(m.state(GetVarName("group0_", NVDLA_SDP_D_OP_ENABLE)), BvConst(0,1));
    }

}


} // namespace ilang