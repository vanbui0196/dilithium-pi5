#pragma once

extern "C" { 
    #include "fips202.h"
}
#include <cstdint>
#include <array>
#include <vector>
#include "constant.h"

typedef keccak_state stream128_state;
typedef keccak_state stream256_state;

namespace mldsa {
    namespace stream_function{

        // Block size
        constexpr uint32_t STREAM128_BLOCKBYTES = SHAKE128_RATE;
        constexpr uint32_t STREAM256_BLOCKBYTES = SHAKE256_RATE;

        // Shake init function
        void shake128_stream_init(keccak_state *state,
            const std::array<uint8_t, SEEDBYTES> seed,
            uint16_t nonce);
            

        void shake256_stream_init(keccak_state *state,
            const std::array<uint8_t, CRHBYTES> seed,
            uint16_t nonce);
    }
}