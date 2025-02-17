#include "poly_algo.h"
#include <chrono>
#include <iostream>
#include <thread>

int main(void) {
    
    // Polynomial a,b,c;
    // std::cout << "Initialization: \n" << a << std::endl;
    // a.fill(10);
    // std::cout << "Fill the value with 10: \n" << a << std::endl;

    // a.fill(Q - 10);
    // std::cout << "Fill the value with Q - 10: \n" << a << std::endl;

    // a.reduced();
    // std::cout << "Reduced the polynomial: \n" << a << std::endl;

    // a.negative_add_Q();
    // std::cout << "Negative add Q: \n" << a << std::endl;

    // b.fill(10); c.fill(10);
    // a = b + c;
    // std::cout << "Adding operator in a: \n" << a << std::endl;

    // a = b + c;
    // std::cout << "Adding operator in a: \n" << a << std::endl;

    // a+= b;
    // std::cout << "Adding operator in a: \n" << a << std::endl;
    
    // a.fill(1);

    // auto start = std::chrono::high_resolution_clock::now();
    // a.NTT();
    // a.NTT();
    // a.NTT();
    // a.NTT();
    // a.NTT();
    // a.NTT();
    // a.NTT();
    // a.NTT();
    // a.NTT();
    // auto end = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    // std::cout << "Result of NTT translation: \n" << a << std::endl;
    // std::cout << "Method 1 - Time taken: " << duration.count() << " microseconds\n";
    // a.invNTT();
    
    // // check the implementation of the code
    // a.fill(-10);
    // a.norm_check(11);
    // std::cout << "The status of check norm is: " << a.norm_check(11) << std::endl;
    // a.set_value(0, 11);
    // std::cout << "The status of check norm is: " << a.norm_check(11) << std::endl;

    Polynomial a;

    std::array<uint8_t, SEEDBYTES> seed_check = {0};
    for(auto i = 0; i < SEEDBYTES; i++) {
        seed_check[i] = i;
    }
    keccak_state state;
    uint16_t nonce = 0x1010;
    // mldsa::stream_function::shake128_stream_init(&state,
    //     seed_check,
    //     nonce);
    
    // uint8_t buf[5 * 168 + 2] = {0};
    // for(auto i = 0; i < (5 * 168 + 2); i++) {
    //     std::cout << std::hex << static_cast<int>(buf[i]) << " ";
    // } std::cout << std::endl;
    
    // shake128_squeezeblocks(buf, 5, &state);

    // for(auto i = 0; i < (5 * 168 + 2); i++) {
    //     std::cout << std::hex << static_cast<int>(buf[i]) << " ";
    // } std::cout << std::endl;

    a.polynomial_uniform(seed_check, nonce);
    std::cout << a;
}
