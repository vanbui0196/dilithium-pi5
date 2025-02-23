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

    PolyMatrix<L,K> test_matrix;
    test_matrix.expand(seed_SEEDBYTES);
    std::cout << test_matrix;
    return 0;

}
