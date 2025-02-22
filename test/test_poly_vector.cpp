#include <chrono>
#include <iostream>
#include <thread>
#include "poly_vector.h"
#include "poly_algo.h"

class Test {
private:
    PolyVector<K> a;
public:
    void print() {
        std::cout << a;
    }
};


int main(void) {

    PolyVector<4> a; a.fill(1); a.vector_NTT();
    PolyVector<4> b; b.fill(2); b.vector_NTT();

    auto c = a + b;
    
    
    // test
    Polynomial test_a;
    test_a.fill(1); test_a.NTT();

    Polynomial test_b;
    test_b.fill(2); test_b.NTT();
    auto test_c = ntt_domain_multiply(test_a, test_b);
    std::cout << test_a;
    std::cout << test_b;
    std::cout << test_c;
    int32_t numa = -3778841;
    int32_t numb = 822735;

    std::cout << "\n" << mldsa::utils::montgomery_reduce( (int64_t)numa * numb);
}
