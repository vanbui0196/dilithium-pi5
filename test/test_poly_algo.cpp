#include "poly_algo.h"
#include <chrono>
#include <iostream>
#include <thread>

int main(void) {
    
    Polynomial a;
    a.fill(1);
    std::cout << a;
    uint8_t arr[5000] = {0};

    a.polyeta_pack(arr);

    a.fill(0);
    std::cout << a;
    a.polyeta_unpack(arr);
    std::cout << a;
}
