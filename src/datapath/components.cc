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

// File: components.cc

#include <ilang/ilang++.h>
#include <iostream>
using namespace std;
#include <sdp_config.h>
#include <datapath_structs.h>

namespace ilang {

// =============================================================================
// Digital logic
// =============================================================================

// Three-Input-Single-High XOR, or TISH_XOR for short. Returns true if one and only one of the
// input arguments is high.
ExprRef TISH_XOR(ExprRef a, ExprRef b, ExprRef c) {
    return ((a == 0x1) & (b == 0x0) & (c == 0x0))| ((a == 0x0) & (b == 0x1) & (c == 0x0)) | ((a == 0x0) & (b == 0x0) & (c == 0x1));
}

// =============================================================================
// IntLog2
// =============================================================================

// IntLog2 takes in a positive integer operand and returns floor(log(operand)), in base 2.
// This is equivalent to operand.bit_width() - 1 after removing leading zeros. 
ExprRef IntLog2(ExprRef operand) {
    const int length = operand.bit_width();
    
    // Initialization
    ExprRef* result = (ExprRef*)malloc(sizeof(ExprRef) * (length + 1));
    for (int j = 0; j < length + 1; j++) {
        result[j] = BvConst(0, 32);
    }

    // IntLog2 operation. It works by checking for the most significant bit in operand which is 1.
    // Once it is found, it sets flag to true and cascades the associated index all the way down to
    // result[0]. This works similar to a ripple comparator design.
    auto flag = BvConst(0, 1);
    for (int i = length - 1; i >= 0; i--) { 
        result[i] = Ite((SelectBit(flag, 0) == BvConst(0, 1)) & (SelectBit(operand, i) == BvConst(1, 1)), BvConst(i, 32), result[i + 1]);
        flag = flag | Ite(SelectBit(operand, i) == 0x1, BvConst(1, 1), BvConst(0, 1));
    }

    return result[0];
}

// Assuming that index = IntLog2(operand), this function returns the remaining bits to the right
// of the bit in operand associated with index
ExprRef IntLog2Frac(ExprRef operand, ExprRef index) {
    // In the cmod this is 1ull, so might be BvConst(1, 64) instead. See the following link:
    // https://github.com/nvdla/hw/blob/8e06b1b9d85aab65b40d43d08eec5ea4681ff715/cmod/hls/include/nvdla_int.h#L402
    return operand & ((BvConst(1, 32) << index) - 1);
}

// =============================================================================
// Signed operation
// =============================================================================

// Produce a mask of all ones
ExprRef AllOnes(size_t length) {
    return (((BvConst(1, length) << (length - 1)) - 1) << 1) + 1;
}

// Determine Two's complement
ExprRef TwoComp(ExprRef a, size_t length) {

    // Flip the bits of a then add 1 to get Two's complement
    return ((a ^ AllOnes(length)) + 1);
}

// Signed comparison which returns 1 if argument "a" is greater than argument "b"
ExprRef SignedGreater(ExprRef a, ExprRef b, size_t length) {
    auto sign_bit_a = SelectBit(a, length - 1);
    auto sign_bit_b = SelectBit(b, length - 1);

    auto result = Ite((sign_bit_a == 0x0) & (sign_bit_b == 0x0),    // Case where "a" is positive, "b" is positive
        Ite(a > b,
            BvConst(1, 1),
            BvConst(0, 1)
        ),
        Ite((sign_bit_a == 0x0) & (sign_bit_b == 0x1),              // Case where "a" is positive, "b" is negative
            BvConst(1, 1),
            Ite((sign_bit_a == 0x1) & (sign_bit_b == 0x0),          // Case where "a" is negative, "b" is positive
                BvConst(0, 1),
                Ite(TwoComp(a, length) > TwoComp(b, length),        // Case where "a" is negative, "b" is negative
                    BvConst(0, 1),
                    BvConst(1, 1)
                )
            )
        )
    );

    return result;
}

// Extend a signed number
ExprRef SignExtend(ExprRef input, size_t input_length, size_t output_length) {
    auto is_positive = SignedGreater(input, BvConst(0, input_length), input_length);

    // Sign extend by padding to the left with sign bit.
    auto output = Ite(is_positive == 0x1, 
        Concat(BvConst(0, output_length - input_length), input),
        Concat(AllOnes(output_length - input_length), input)
    );

    return output;
}

// Limit the maximum and minimum values that a signed input can take
ExprRef Saturation(ExprRef input, size_t length) {
    // The limiting values correspond to the signed range associated with length bits
    auto oMax = (BvConst(1, 64) << (length - 1)) - 1;
    auto oMin = TwoComp(BvConst(1, 64) << (length - 1), 64);

    auto output = Ite(SignedGreater(input, oMax, 64) == 0x1, 
        oMax, 
        Ite(SignedGreater(oMin, input, 64) == 0x1,
            oMin,
            input
        )
    );

    return output;
}

// =============================================================================
// Datapath computation
// =============================================================================

// Based on the ALU design present in the SDP core. Returns the result of the operation specified 
// in the OpInfo struct.
ExprRef ALU(ExprRef input, ExprRef operand, OpInfo& o) {
    auto result = 
    Ite(BvConst(o.op_id, 2) == BvConst(0, 2),                       // Max operation
        Ite(input > operand,
            input,
            operand
        ),
        Ite(BvConst(o.op_id, 2) == BvConst(1, 2),                   // Min operation
            Ite(input < operand,
                input,
                operand
            ),
            Ite(BvConst(o.op_id, 2) == BvConst(2, 2),               // Add operation 
                input + operand,
                Ite(input == operand,                               // Equality operation 
                    BvConst(0, 32),
                    BvConst(1, 32)
                )
            )                      
        )
    );

    return result;
}

// Based on the multiplier design present in the SDP core. Returns the result of the operation 
// specified in the OpInfo struct.
ExprRef Mult(ExprRef input, ExprRef operand, OpInfo& o) {
    auto result = 
    Ite(BvConst(o.op_id, 1) == BvConst(0, 1),                       // Multiply operation 
        input * operand,
        Ite(input > 0x0,                                            // PReLU operation
            input,
            input * operand
        )
    );

    return result;
}

} // namespace ilang