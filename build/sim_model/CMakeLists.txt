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
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_Compute_Add_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_Compute_Add_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_Compute_Max_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_Compute_Max_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_Compute_Min_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_Compute_Min_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_Compute_Multiply_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_Compute_Multiply_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_Compute_PReLU_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_Compute_PReLU_group1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_Compute_ReLU_group0.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/idu_Compute_ReLU_group1.cc
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
