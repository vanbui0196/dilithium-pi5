#include "stream.h"

namespace mldsa {
    namespace stream_function{

        /**
         * @brief SHAKE128 init stream
         * 
         * @param state Input state
         * @param seed Seed of SHAKE128
         * @param nonce 2 nonce bytes
         */
        void shake128_stream_init(keccak_state *state, const std::array<uint8_t, SEEDBYTES> seed, uint16_t nonce)
        {
            // Nonce 2 bytes buffer
            uint8_t buf_nonce[2];

            // Fill the nonce bytes into the buffer
            buf_nonce[0] = static_cast<uint8_t>(nonce);
            buf_nonce[1] = static_cast<uint8_t>(nonce >> 8u);

            // Init of the shake
            shake128_init(state);

            // Absorb of the shake
            shake128_absorb(state, 
                            seed.data(), 
                            SEEDBYTES);
            
            // 2nd absrob
            shake128_absorb(state,
                            buf_nonce,
                            2);

            // finialize
            shake128_finalize(state);
        }

        /**
         * @brief SHAKE256 init stream
         * 
         * @param state the Keccak state
         * @param seed  Seed buffer of the shake256 buffer
         * @param nonce 2 nonce bytes
         */
         void shake256_stream_init(keccak_state *state, const std::array<uint8_t, CRHBYTES> seed, uint16_t nonce)
         {
            // Nonce 2 bytes buffer
            uint8_t buf_nonce[2];
            
            // Fill the nonce bytes into the buffer
            buf_nonce[0] = static_cast<uint8_t>(nonce);
            buf_nonce[1] = static_cast<uint8_t>(nonce >> 8u);
            
            /* Init state */
            shake256_init(state);
            
            /* 1st absrob */
            shake256_absorb(state, 
                            seed.data(), 
                            CRHBYTES);
                
            /* 2nd absorb */
            shake256_absorb(state, 
                            buf_nonce, 
                            2);
                
            /* final state */
            shake256_finalize(state);
            }
}
}