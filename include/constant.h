#pragma once

#include "config.h"
#include <cstdint>

// Common constant for the dilithium
constexpr uint32_t SEEDBYTES = 32;
constexpr uint32_t CRHBYTES = 64;
constexpr uint32_t TRBYTES = 64;
constexpr uint32_t RNDBYTES = 32;
constexpr uint32_t N = 256;
constexpr uint32_t Q = 8380417;
constexpr uint32_t D = 13;
constexpr uint32_t ROOT_OF_UNITY = 1753;
#define MONT -4186625 // 2^32 % Q
#define QINV 58728449 // q^(-1) mod 2^32

// Mode-specific parameters
#if DILITHIUM_MODE == 2
    constexpr uint32_t K = 4;
    constexpr uint32_t L = 4;
    constexpr uint32_t ETA = 2;
    constexpr uint32_t TAU = 39;
    constexpr uint32_t BETA = 78;
    constexpr uint32_t GAMMA1 = (1 << 17);
    constexpr uint32_t GAMMA2 = ((Q-1)/88);
    constexpr uint32_t OMEGA = 80;
    constexpr uint32_t CTILDEBYTES = 32;
#elif DILITHIUM_MODE == 3
    constexpr uint32_t K = 6;
    constexpr uint32_t L = 5;
    constexpr uint32_t ETA = 4;
    constexpr uint32_t TAU = 49;
    constexpr uint32_t BETA = 196;
    constexpr uint32_t GAMMA1 = (1 << 19);
    constexpr uint32_t GAMMA2 = ((Q-1)/32);
    constexpr uint32_t OMEGA = 55;
    constexpr uint32_t CTILDEBYTES = 48;
#elif DILITHIUM_MODE == 5
    constexpr uint32_t K = 8;
    constexpr uint32_t L = 7;
    constexpr uint32_t ETA = 2;
    constexpr uint32_t TAU = 60;
    constexpr uint32_t BETA = 120;
    constexpr uint32_t GAMMA1 = (1 << 19);
    constexpr uint32_t GAMMA2 = ((Q-1)/32);
    constexpr uint32_t OMEGA = 75;
    constexpr uint32_t CTILDEBYTES = 64;
#endif


// Derived parameters for polynomial and vector packing
constexpr uint32_t POLYT1_PACKEDBYTES = 320;
constexpr uint32_t POLYT0_PACKEDBYTES = 416;
constexpr uint32_t POLYVECH_PACKEDBYTES = (OMEGA + K);

constexpr uint32_t POLYZ_PACKEDBYTES =
    (GAMMA1 == (1 << 17)) ? 576 :
    (GAMMA1 == (1 << 19)) ? 640 :
    0;


// Number of bytes in case of packed byte for the algorithm
constexpr uint32_t POLYW1_PACKEDBYTES = ((GAMMA2 == (Q-1)/88) ? 192 : 128);

constexpr uint32_t POLYETA_PACKEDBYTES = ((ETA == 2) ? 96 : 128);


// Crypto parameter sizes
constexpr uint32_t CRYPTO_PUBLICKEYBYTES = (SEEDBYTES + K*POLYT1_PACKEDBYTES);
constexpr uint32_t CRYPTO_SECRETKEYBYTES = (2*SEEDBYTES + TRBYTES + L*POLYETA_PACKEDBYTES + K*POLYETA_PACKEDBYTES + K*POLYT0_PACKEDBYTES);
constexpr uint32_t CRYPTO_BYTES = (CTILDEBYTES + L*POLYZ_PACKEDBYTES + POLYVECH_PACKEDBYTES);

