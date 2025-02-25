/**
 * @file utils.cpp
 * @author Bui Khanh Van=
 * @brief Clone version of the Dilithium reference code
 * @version 0.1
 * @date 2025-02-10
 * 
 */
 #include "utils.h"
 #include <iostream>
namespace mldsa
{
    namespace utils {
        /**
        * @brief Perform the fast modulo with the Montgomery Reduction
        * 
        * @param a Number
        * @return int32_t Result
        */
        int32_t montgomery_reduce(int64_t a) {
            int32_t t;
            
            t = (int64_t)(int32_t)a*QINV;
            t = (a - (int64_t)t*Q) >> 32;
            return t;
        }
        
        /**
        * @brief calculate the result = a mod Q
        * 
        * @param a Input numnber
        * @return int32_t result of the modulo
        */
        int32_t reduce32(int32_t a) {
            int32_t t;
            
            t = (a + (1 << 22)) >> 23;
            t = a - t*Q;
            return t;
        }
        
        
        /**
        * @brief Add the coefficient "Q" in case input is negative. Otherwise, keep the same value 
        * 
        * @param a Element in the finite filed
        * @return int32_t The valuewith adding up of Q
        */
        int32_t caddq(int32_t a) {
            // get the signed bit and adding the value
            a += (a >> 31) & Q;
            return a;
        }
        
        /**
        * @brief Compute the positive finite field element < Q
        * 
        * @param a Finite field element
        * @return int32_t The positive number in the finite field of Q
        */
        int32_t freeze(int32_t a)
        {
            // Calculate the \equiv value
            a = reduce32(a);
            
            // calculate and return the value after adding up the value of Q
            return caddq(a);
        }
        
        
        /**
        * @brief Decompose HighBits and LowBits with 2^D factor (FIPS.204)
        * 
        * @param a Element in the finite field
        * @return std::pair<int32_t, int32_t> {HighBits,LowBits} pair of the Power2Round
        */
        std::pair<int32_t, int32_t> power2round(int32_t a)  {
            int32_t a1, a0;
            
            // HighBits calculation
            a1 = (a + (1 << (D-1)) - 1) >> D;

            // LowBits calculation
            a0 = a - (a1 << D);

            // return the HighBits(a1) + LowBits(a0))
            return {a0, a1};
        }

        /**
         * @brief Decompose with 2*GAMMA2 factor
         * 
         * @param a Finite field element
         * @return std::pair<int32_t, int32_t> HighBits/LowBits in order
         */
        std::pair<int32_t, int32_t> decompose(int32_t a) {
            // return the value 
            int32_t a1{0}, a0{0};

            // calculate the HighBits
            a1  = (a + 127) >> 7;
            
            if(GAMMA2 == ((Q -1)/32)) 
            {
                a1  = (a1*1025 + (1 << 21)) >> 22;
                a1 &= 15;
            }
            else if ((GAMMA2) == ((Q-1)/88))
            {
                a1  = (a1*11275 + (1 << 23)) >> 24;
                a1 ^= ((43 - a1) >> 31) & a1;
            }
            
            // calculate the LowBits
            a0  = a - a1*2*GAMMA2;

            // Quite not optimized here since some step in C can not copy directly into the C++ code
            int32_t condition = (Q-1)/2 - a0;
            int32_t shifted = condition >> 31;
            int32_t masked = shifted & Q;
            a0 -= masked;
            // Return the a1 and the a0
            return {a0, a1};
        }

        /**
         * @brief Dilithium Ref Code: Calculate the hint
         * 
         * @param a0 
         * @param a1 
         * @return uint32_t Hint result
         */
         uint32_t make_hint(int32_t a0, int32_t a1) {
            
            // return value
            uint32_t retVal = 0;
            if((a0 > GAMMA2) || (a0 < -GAMMA2) || (a0 == -GAMMA2 && a1 != 0)) {
                retVal =  1;
            }
            return retVal;
        }

        int32_t use_hint(int32_t a, uint32_t hint) {
        /* Get the high bit and low bit according to the hint */
        auto [a0, a1] = decompose(a);

        /* no hint required, return the the default value */
        if(hint == 0)
        {
            return a1;
        }
        if(GAMMA2 == (Q-1)/32) 
        {
            if(a0 > 0)
            {
                return (a1 + 1) & 15;
            }

            else
            {
                return (a1 - 1) & 15;
            }

        }
        else if (GAMMA2 == (Q-1)/88)
        {
            if(a0 > 0)
            {
                return (a1 == 43) ?  0 : a1 + 1;
            }
            else
            {
                return (a1 ==  0) ? 43 : a1 - 1;
            }
        }
    }
}
    
} // namespace mldsa
