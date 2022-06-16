#include <ilang/ilang++.h>

// Function: computeReLU
// element-wise ReLU funtion, used in sdp_instr.cc
sc_biguint<32> computeReLU(sc_biguint<32> arg_0) {

  // Convert from unsigned to signed representation
  sc_bigint<32> arg_0_s = arg_0;
  
  spec::ActScalarType arg_0_ac = arg_0_s.to_int();
  spec::ActScalarType out_tmp;

  typedef ac_fixed<spec::kActWordWidth, spec::kActNumInt, true, AC_TRN, AC_WRAP> input_type;
  typedef ac_fixed<spec::kActWordWidth, spec::kActNumInt, false, AC_TRN, AC_WRAP> output_type;

  input_type in_ac; 
  output_type out_ac;
  
  in_ac.set_slc(0, arg_0_ac);

  //std::cout << "in_ac: " << in_ac << '\t';

  out_ac = ac_math::ac_sigmoid_pwl<output_type>(in_ac);

// correct one
  out_ac = (in_ac > 0) ? in_ac : 0;

  //std::cout << "out ac: " << out_ac << '\t';

  out_tmp.set_slc(0, nvhls::get_slc<spec::kActWordWidth>(out_ac, 0));

  //std::cout << "out_tmp: " << out_tmp << '\t';

  sc_bigint<32> result_s = out_tmp.to_int();
  sc_biguint<32> result = result_s;

  return result;
}