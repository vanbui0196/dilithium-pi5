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
    uint8_t test_buf_packw1[K * POLYW1_PACKEDBYTES + 5] = {0};
    vector_w1_pack.vector_packw1(test_buf_packw1 + 2);
    for(auto i = 0; i < K * POLYW1_PACKEDBYTES + 5; i++) {std::cout << (int32_t)test_buf_packw1[i] << " ";}
    std::cout << "\n";
    return 0;

}
