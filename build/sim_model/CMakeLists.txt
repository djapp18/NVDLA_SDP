# CMakeLists.txt for sdp
cmake_minimum_required(VERSION 3.14.0)
project(sdp LANGUAGES CXX)

option(ILATOR_VERBOSE "Enable instruction sequence logging" OFF)
option(JSON_SUPPORT "Build JSON parser support" OFF)

find_package(SystemCLanguage CONFIG REQUIRED)
set(CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD})

aux_source_directory(extern extern_src)
add_executable(sdp
  ${CMAKE_CURRENT_SOURCE_DIR}/app/main.cc
  ${extern_src}
  ${CMAKE_CURRENT_SOURCE_DIR}/src/all_valid_funcs_in_hier.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/compute.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/constant_memory_def.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_CVT_OFFSET_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_CVT_OFFSET_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_CVT_SCALE_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_CVT_SCALE_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_CVT_SHIFT_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_CVT_SHIFT_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_Compute_Max.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_Compute_ReLU.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DATA_CUBE_CHANNEL_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DATA_CUBE_CHANNEL_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DATA_CUBE_HEIGHT_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DATA_CUBE_HEIGHT_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DATA_CUBE_WIDTH_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DATA_CUBE_WIDTH_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DATA_FORMAT_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DATA_FORMAT_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_BN_ALU_CFG_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_BN_ALU_CFG_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_BN_ALU_SRC_VALUE_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_BN_ALU_SRC_VALUE_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_BN_CFG_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_BN_CFG_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_BN_MUL_CFG_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_BN_MUL_CFG_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_BN_MUL_SRC_VALUE_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_BN_MUL_SRC_VALUE_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_BS_ALU_CFG_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_BS_ALU_CFG_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_BS_ALU_SRC_VALUE_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_BS_ALU_SRC_VALUE_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_BS_CFG_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_BS_CFG_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_BS_MUL_CFG_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_BS_MUL_CFG_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_BS_MUL_SRC_VALUE_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_BS_MUL_SRC_VALUE_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_EW_ALU_CFG_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_EW_ALU_CFG_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_EW_ALU_CVT_OFFSET_VALUE_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_EW_ALU_CVT_OFFSET_VALUE_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_EW_ALU_CVT_SCALE_VALUE_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_EW_ALU_CVT_SCALE_VALUE_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_EW_ALU_CVT_TRUNCATE_VALUE_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_EW_ALU_CVT_TRUNCATE_VALUE_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_EW_ALU_SRC_VALUE_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_EW_CFG_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_EW_CFG_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_EW_MUL_CFG_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_EW_MUL_CFG_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_EW_MUL_CVT_OFFSET_VALUE_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_EW_MUL_CVT_OFFSET_VALUE_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_EW_MUL_CVT_SCALE_VALUE_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_EW_MUL_CVT_SCALE_VALUE_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_EW_MUL_CVT_TRUNCATE_VALUE_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_EW_MUL_CVT_TRUNCATE_VALUE_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_EW_MUL_SRC_VALUE_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_EW_MUL_SRC_VALUE_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_EW_TRUNCATE_VALUE_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DP_EW_TRUNCATE_VALUE_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DST_BASE_ADDR_HIGH_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DST_BASE_ADDR_HIGH_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DST_BASE_ADDR_LOW_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DST_BASE_ADDR_LOW_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DST_BATCH_STRIDE_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DST_BATCH_STRIDE_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DST_DMA_CFG_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DST_DMA_CFG_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DST_LINE_STRIDE_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DST_LINE_STRIDE_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DST_SURFACE_STRIDE_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_DST_SURFACE_STRIDE_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_FEATURE_MODE_CFG_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_FEATURE_MODE_CFG_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_LUT_ACCESS_CFG.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_LUT_ACCESS_DATA.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_LUT_CFG.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_LUT_INFO.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_LUT_LE_END.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_LUT_LE_SLOPE_SCALE.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_LUT_LE_SLOPE_SHIFT.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_LUT_LE_START.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_LUT_LO_END.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_LUT_LO_SLOPE_SCALE.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_LUT_LO_SLOPE_SHIFT.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_LUT_LO_START.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_OP_ENABLE_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_OP_ENABLE_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_PERF_ENABLE_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_PERF_ENABLE_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_SET_PRODUCER.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/memory_update_functions_0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/setup_initial_condition.cc
)

target_include_directories(sdp PRIVATE include)
target_link_libraries(sdp SystemC::systemc)
if(${ILATOR_VERBOSE})
  target_compile_definitions(sdp PRIVATE ILATOR_VERBOSE)
endif()
if(${JSON_SUPPORT})
  include(FetchContent)
  FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.8.0
  )
  FetchContent_MakeAvailable(json)
  target_link_libraries(sdp nlohmann_json::nlohmann_json)
endif()
