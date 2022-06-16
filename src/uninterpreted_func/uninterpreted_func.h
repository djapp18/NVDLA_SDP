#include <ilang/ilang++.h>

#ifndef UNINTERPRETED_FUNC_H__
#define UNINTERPRETED_FUNC_H__

#define RELU_ACT_VECTOR_BITWIDTH 32

// uninterpreted functions in sdp_instrs.cc
static auto uf_act_in = SortRef::BV(RELU_ACT_VECTOR_BITWIDTH);
static auto uf_act_out = SortRef::BV(RELU_ACT_VECTOR_BITWIDTH);

static FuncRef computeReLU("computeReLU", uf_act_out, uf_act_in);

FuncRef EmeshAxiMasterBridge::unknown(unsigned width) {  
       return FuncRef("unknown" + std::to_string(nondet_counter++), SortRef::BV(width));
}

ExprRef EmeshAxiMasterBridge::unknownVal(unsigned width) {
  return unknown(width)();
}

#endif