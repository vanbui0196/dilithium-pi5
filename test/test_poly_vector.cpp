#include <chrono>
#include <iostream>
#include <thread>
#include "poly_vector.h"
#include "poly_algo.h"


int main(void) {

    std::array<uint8_t, SEEDBYTES> seed_SEEDBYTES;
    for(auto i = 0; i < SEEDBYTES; i++) {
        seed_SEEDBYTES.at(i) = i;
    }

    std::array<uint8_t, CRHBYTES> seed_CRHBYTES;
    for(auto i = 0; i < CRHBYTES; i++) {
        seed_CRHBYTES.at(i) = i;
    }

    // test with the seed byte
    std::cout << "Test case 1:";
    PolyMatrix<L,K> test_matrix;
    test_matrix.expand(seed_SEEDBYTES);
    PolyVector<L> vector;
    vector.fill(10);
    vector.vector_NTT();
    PolyVector<4> result = matrix_multiply(test_matrix, vector);
    std::cout << result;
    std::cout << "\n";

    // PackW1 test
    std::cout << "Test case 2:\n";
    PolyVector<L> vector_w1_pack; vector_w1_pack.fill(100);
    uint8_t test_buf_packw1[K * POLYW1_PACKEDBYTES + 2] = {0};
    vector_w1_pack.vector_packw1(test_buf_packw1);
    for(auto i = 0; i < K * POLYW1_PACKEDBYTES + 2; i++) {std::cout << (int32_t)test_buf_packw1[i] << " ";}
    std::cout << "\n";

    // Poly decompose, highbit, lowbit
    std::cout << "Test case 3:\n";
    PolyVector<L> decompose_vector;
    decompose_vector.fill(100); 
    decompose_vector.vector_NTT();
    decompose_vector.vector_caddq();
    std::cout << decompose_vector;
    PolyVector<L> hightbits_vector, lowbit_vector, hints_vector, hint_vector_cpy, corectted_vector, corectted_vector_cpy;
    decompose_vector.vector_decompose(hightbits_vector, lowbit_vector);
    std::cout << "High bits vector: \n"  << hightbits_vector;
    std::cout << "Low bits vector: \n"  << lowbit_vector;
    uint32_t total_hint1 = vector_make_hint(hints_vector, lowbit_vector, hightbits_vector);
    std::cout << "Hints vector: \n" << hints_vector;
    uint32_t total_hint2 = hint_vector_cpy.vector_make_hint(lowbit_vector, hightbits_vector);
    std::cout << "Hints vector: \n" << hint_vector_cpy;
    vector_use_hint(corectted_vector, hightbits_vector, hints_vector);
    corectted_vector_cpy.vector_use_hint(hightbits_vector, hints_vector);
    std::cout << "Correct vector: \n" << corectted_vector;
    std::cout << "Correct vector: \n" << corectted_vector_cpy;
    std::cout << "\n";
    std::cout << "Total hint: " << total_hint1  << " + " << total_hint2 << std::endl;

    // Test case vector_uniform_eta
    std::cout << "Test case 4:\n";
    PolyVector<L> uniform_eta_vec;
    uniform_eta_vec.vector_uniform_eta(seed_CRHBYTES, 4);
    std::cout << "Uniform eta: \n" << uniform_eta_vec;


    // Test case vector_uniform gamma
    std::cout << "Test case 5:\n";
    PolyVector<L> gamma1_vector;
    gamma1_vector.vector_uniform_gamma1(seed_CRHBYTES, 4);
    std::cout << "Uniform gamma: \n" << gamma1_vector;


    // Test case adding, substract + nttdomain
    PolyVector<L> adding_vector1; adding_vector1.fill(10); adding_vector1.vector_NTT();
    PolyVector<L> adding_vector2; adding_vector2.fill(20); adding_vector2.vector_NTT();

    PolyVector<L> adding_result = adding_vector1 + adding_vector2;
    PolyVector<L> sub_result = adding_vector1 - adding_vector2;
    PolyVector<L> mul_result;
    vector_pointwise_multiply(mul_result, adding_vector1, adding_vector2); 
    std::cout << "Adding result: \n" << adding_result;
    std::cout << "Sub result: \n" << sub_result;
    std::cout << "Mul result: \n" << mul_result;

    // Test case for Power2round
    PolyVector<L> p2r_highbits;
    PolyVector<L> p2r_lowbits;
    mul_result.vector_power2round(p2r_highbits, p2r_lowbits);
    std::cout << "Power2Round HighBits result: \n" << p2r_highbits;
    std::cout << "Power2Round LowBits result: \n" << p2r_lowbits;

    // Test case shiftl
    mul_result.vector_shiftl();
    std::cout << "Shift result: \n" << mul_result;

    // Check norm testcase
    PolyVector<L> normcheck_vector; normcheck_vector.fill(10);
    std::cout << "Norm check: " << normcheck_vector.vector_checknorm(11) << std::endl;
    normcheck_vector.access_poly_at(L-1).set_value(N-1,11);
    std::cout << "Norm check: " << normcheck_vector.vector_checknorm(11) << std::endl;

    // Pointwise vector check
    Polynomial pointwise_vector_result;
    Polynomial pointwise_vector_ref_result;
    PolyVector<4> pointwise_vector_a; pointwise_vector_a.fill(200);
    PolyVector<4> pointwise_vector_b; pointwise_vector_b.fill(500);

    pointwise_vector_result = vector_pointwise_multiply_accumulate(pointwise_vector_a, pointwise_vector_b);
    vector_pointwise_multiply_accumulate(pointwise_vector_ref_result, pointwise_vector_a, pointwise_vector_b);

    std::cout << "Accumulate result: " << pointwise_vector_result << std::endl;
    std::cout << "Accumulate result: " << pointwise_vector_ref_result << std::endl;

    return 0;

}
