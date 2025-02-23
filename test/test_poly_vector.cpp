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

    Polynomial c;
    vector_pointwise_multiply_accumulate(c, a, b);
    std::cout << c;

}
