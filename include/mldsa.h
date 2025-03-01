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
    PolyVector<K> _t;                       // Public key vector
    PolyVector<K> _t1;                      // Public key vector (high bits)
    PolyVector<L> _s1;                      // Secret key vector
    PolyVector<K> _s2;                      // Secret key vector
    PolyVector<K> _t0;                      // Secret key vector (low bits)
    PolyVector<L> _z;
    PolyVector<K> _h;
    bool test_mode;

    void SignInternal(uint8_t* SignMessage, size_t* SignMessageLength,
                      const uint8_t* Mesage, size_t MessageLength,
                      const uint8_t* Pre, size_t PreLenth, const std::array<uint8_t, RNDBYTES> randombuf, 
                      const std::array<uint8_t, CRYPTO_SECRETKEYBYTES>& secret_key);
public:
    MLDSA();
    MLDSA(bool mode);
    std::array<uint8_t, CRYPTO_PUBLICKEYBYTES> public_key = {0};
    std::array<uint8_t, CRYPTO_SECRETKEYBYTES> secret_key = {0};

    void pkEncode(const std::array<uint8_t, SEEDBYTES>& rho);
    
    void skEncode(const std::array<uint8_t, SEEDBYTES>& rho, 
                  const std::array<uint8_t, TRBYTES>& tr, 
                  const std::array<uint8_t, SEEDBYTES>& key);
                  
    void skDecode(std::array<uint8_t,SEEDBYTES>& rho, std::array<uint8_t, TRBYTES>& tr, std::array<uint8_t,SEEDBYTES>& key,
    PolyVector<K>& t0, PolyVector<L>& s1, PolyVector<K>& s2, const std::array<uint8_t, CRYPTO_SECRETKEYBYTES>& secret_key);

    void sigEncode(uint8_t* SignMessage, const std::array<uint8_t, CTILDEBYTES>& sample_in_ball_seed, 
        PolyVector<L>& z, 
        PolyVector<K>& h) const;

    void KeyGen();

    int Sign(uint8_t* SignMessage, size_t* SignMessageLength,
              const uint8_t* Mesage, size_t MessageLength,
              const uint8_t* ctx, size_t ctxlen, 
              const std::array<uint8_t, CRYPTO_SECRETKEYBYTES>& secret_key);

};