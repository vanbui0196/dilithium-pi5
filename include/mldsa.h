#pragma once
#include <cstdint>
#include <iostream>
#include <random>
#include <array>
#include <cstdint>
#include <algorithm>
#include <functional>
#include <cstring> 
#include "config.h"
#include "constant.h"
#include "poly_algo.h"
#include "poly_vector.h"
#include "utils.h"

class MLDSA { 
private:
    // Polynomial components
    PolyMatrix<L, K> _A;                    // Public matrix
    PolyVector<K> _t;                       // Public key vector (high bits)
    PolyVector<K> _t1;                      // Public key vector (high bits)
    PolyVector<L> _s1;                      // Secret key vector
    PolyVector<K> _s2;                      // Secret key vector
    PolyVector<K> _t0;                      // Secret key vector (low bits)

    // This part will be defined during the key generation
    //std::array<uint8_t, SEEDBYTES> rho;    // Public seed to generate matrix A
    //std::array<uint8_t, CRHBYTES> tr;      // Hash of public key (for signing)
public:
    MLDSA();
    std::array<uint8_t, CRYPTO_PUBLICKEYBYTES> public_key = {0};
    std::array<uint8_t, CRYPTO_SECRETKEYBYTES> secret_key = {0};

    void pkEncode(const std::array<uint8_t, SEEDBYTES>& rho);
    
    void skEncode(const std::array<uint8_t, SEEDBYTES>& rho, 
                  const std::array<uint8_t, TRBYTES>& tr, 
                  const std::array<uint8_t, SEEDBYTES>& key);

    void KeyGen(bool testmode); // determode for testing purpose, will be removed later
};