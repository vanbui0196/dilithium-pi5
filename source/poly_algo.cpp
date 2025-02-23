#include "poly_algo.h"

static constexpr  std::array<int32_t, N>  zetas = {
       0,    25847, -2608894,  -518909,   237124,  -777960,  -876248,      466468,
 1826347,  2353451,  -359251, -2091905,  3119733, -2884855,  3111497,     2680103,
 2725464,  1024112, -1079900,  3585928,  -549488, -1119584,  2619752,    -2108549,
-2118186, -3859737, -1399561, -3277672,  1757237,   -19422,  4010497,      280005,
 2706023,    95776,  3077325,  3530437, -1661693, -3592148, -2537516,     3915439,
-3861115, -3043716,  3574422, -2867647,  3539968,  -300467,  2348700,     -539299,
-1699267, -1643818,  3505694, -3821735,  3507263, -2140649, -1600420,     3699596,
  811944,   531354,   954230,  3881043,  3900724, -2556880,  2071892,    -2797779,
-3930395, -1528703, -3677745, -3041255, -1452451,  3475950,  2176455,    -1585221,
-1257611,  1939314, -4083598, -1000202, -3190144, -3157330, -3632928,      126922,
 3412210,  -983419,  2147896,  2715295, -2967645, -3693493,  -411027,    -2477047,
 -671102, -1228525,   -22981, -1308169,  -381987,  1349076,  1852771,    -1430430,
-3343383,   264944,   508951,  3097992,    44288, -1100098,   904516,     3958618,
-3724342,    -8578,  1653064, -3249728,  2389356,  -210977,   759969,    -1316856,
  189548, -3553272,  3159746, -1851402, -2409325,  -177440,  1315589,     1341330,
 1285669, -1584928,  -812732, -1439742, -3019102, -3881060, -3628969,     3839961,
 2091667,  3407706,  2316500,  3817976, -3342478,  2244091, -2446433,    -3562462,
  266997,  2434439, -1235728,  3513181, -3520352, -3759364, -1197226,    -3193378,
  900702,  1859098,   909542,   819034,   495491, -1613174,   -43260,     -522500,
 -655327, -3122442,  2031748,  3207046, -3556995,  -525098,  -768622,    -3595838,
  342297,   286988, -2437823,  4108315,  3437287, -3342277,  1735879,      203044,
 2842341,  2691481, -2590150,  1265009,  4055324,  1247620,  2486353,     1595974,
-3767016,  1250494,  2635921, -3548272, -2994039,  1869119,  1903435,    -1050970,
-1333058,  1237275, -3318210, -1430225,  -451100,  1312455,  3306115,    -1962642,
-1279661,  1917081, -2546312, -1374803,  1500165,   777191,  2235880,     3406031,
 -542412, -2831860, -1671176, -1846953, -2584293, -3724270,   594136,    -3776993,
-2013608,  2432395,  2454455,  -164721,  1957272,  3369112,   185531,    -1207385,
-3183426,   162844,  1616392,  3014001,   810149,  1652634, -3694233,    -1799107,
-3038916,  3523897,  3866901,   269760,  2213111,  -975884,  1717735,      472078,
 -426683,  1723600, -1803090,  1910376, -1667432, -1104333,  -260646,    -3833893,
-2939036, -2235985,  -420899, -2286327,   183443,  -976891,  1612842,    -3545687,
 -554416,  3919660,   -48306, -1362209,  3937738,  1400424,  -846154,     1976782
};


/**
 * @brief Construct a new Polynomial:: Polynomial object. Only set the status flag of ntt_status to "false"
 * 
 */
Polynomial::Polynomial() : ntt_status{false} {
    // No feature at the moment
    // Only init the status of the ntt transform to false
    this->_coeffs.fill(0);
}

/**
 * @brief Return the current ntt_status. This can only be done with the call of NTT or invNTT methods
 * 
 * @return true in Frequency domain
 * @return false not in the frequency domain
 */
bool Polynomial::get_ntt_status() {
    return this->ntt_status;
}

/**
 * @brief Get the value at the position of the polynomial. Lateer
 * 
 * @param index Index value
 * @return int32_t The coefficient of the polynomials
 */
int32_t Polynomial::get_value(size_t index) {
    return this->_coeffs.at(index);
}

/**
 * @brief Set the value at the index of the coefficient. Later usage for pack and unpacked
 * 
 * @param index Index of coefficient 0 < index < 255
 * @param value Value for the index
 */
void Polynomial::set_value(size_t index, int32_t value) {
    this->_coeffs.at(index) = value;
}

/**
 * @brief Fill the coefficient with the intended value
 * 
 * @param value The value will be filled for all the coefficient
 */
void Polynomial::fill(int32_t value) {
    this->_coeffs.fill(value);
}


/**
 * @brief Reduced all the coefficient to the range [-6283008,6283008]
 * @ref poly_reduce
 * 
 */
void Polynomial::reduced() {
    for(auto& element : this->_coeffs) {
        element = mldsa::utils::reduce32(element);
    }
}

/**
 * @brief Adding the value of Q in case of the negative element. In case positive, leave the same
 * @ref poly_caddq
 */
void Polynomial::negative_add_Q() {
    for(auto& element : this->_coeffs) {
        element = mldsa::utils::caddq(element);
    }
}

/**
 * @brief Shift all the coefficient to the left with 2D factor. This will be used in the Verifying for (2^d * t1) vector
 * @ref poly_shiftl
 * 
 */
void Polynomial::shift_toleft_2D() {
    for(auto& element : this->_coeffs) {
        element <<= D;
    }
}

/**
 * @brief Decompose the the polynomial with the factor of 2^d
 * 
 * @param highbits_poly The poly that contain the HighBits coefficients
 * @param lowbits_poly The poly that contain the LowBits coefficients
 */
void Polynomial::power2round(Polynomial& lowbits_poly, Polynomial& highbits_poly) {
    for(auto i = 0; i < N; i++) {
        // Decompose the coefficient into 2 part
        std::pair<int32_t, int32_t> temp = mldsa::utils::power2round(this->_coeffs.at(i));
        
        // put the value into the reference
        lowbits_poly.set_value(i, temp.first);

        // put the value into the lowbit references
        highbits_poly.set_value(i , temp.second);
    }
}

/**
 * @brief Decompase the polynomial with the factor of 2*Gamma2
 * 
 * @param highbits_poly The poly that contain HighBits coefficients
 * @param lowbits_poly The poly that contain LowBits coefficients
 */
void Polynomial::decompose(Polynomial& lowbits_poly, Polynomial& highbits_poly) {
    for(auto i = 0; i < N; i++) {
        // Decompose the coefficient into 2 part
        std::pair<int32_t, int32_t> temp = mldsa::utils::decompose(this->_coeffs.at(i));
        
        // put the value into the lowbits reference
        lowbits_poly.set_value(i, temp.first);

        // put the value into the highbits references
        highbits_poly.set_value(i , temp.second);
    }
}

/**
 * @brief MakeHint for correct the HighBits in case of the boundary
 * 
 * @param lowbits_poly Constant reference to the LowBits polynomial
 * @param highbits_poly Constant reference to the HighBits polynomial
 * @param hints_poly Reference to the Hint polynoial
 * @return uint32_t 
 */
uint32_t Polynomial::make_hint(const Polynomial& lowbits_poly,const Polynomial& highbits_poly) {
    // Calculate the total hints
    uint32_t sum  = 0;
    for(auto i = 0; i < N; i++) {
        // Update the hint poly cast the value to the int32_t for safe handling
        this->_coeffs.at(i) = static_cast<int32_t>(mldsa::utils::make_hint(lowbits_poly._coeffs.at(i), highbits_poly._coeffs.at(i)));
        // Return the total hints
        sum = sum + this->_coeffs.at(i);
    }
    return sum;
}

/**
 * @brief MakeHint for correct the HighBits in case of the boundary
 * 
 * @param lowbits_poly Constant reference to the LowBits polynomial
 * @param highbits_poly Constant reference to the HighBits polynomial
 * @param hints_poly Reference to the Hint polynoial
 * @return uint32_t 
 */
uint32_t poly_make_hint(Polynomial& hints_poly, const Polynomial& lowbits_poly,const Polynomial& highbits_poly) {
    // Calculate the total hints
    uint32_t sum  = 0;
    for(auto i = 0; i < N; i++) {
        // Update the hint poly cast the value to the int32_t for safe handling
        hints_poly._coeffs.at(i) = static_cast<int32_t>(mldsa::utils::make_hint(lowbits_poly._coeffs.at(i), highbits_poly._coeffs.at(i)));
        // Return the total hints
        sum = sum + hints_poly._coeffs.at(i);
    }
    return sum;
}

/**
 * @brief Convert 3 bytes from the SHAKE output in to coefficient.
 * @ref rej_uniform
 * 
 * @param length Length of the the total coefficients that requrieds
 * @param buf Buffer data filled from SHAKE
 * @param bufflen Lenght of the buffer
 * @return uint32_t Number of the coeffient
 */
uint32_t Polynomial::_coefficient_from_3bytes(int32_t* coeff, uint32_t length, const uint8_t* buf, uint32_t buflen) {
    

    
    // local variable 
    size_t ctr{0}, pos{0}; // counter
    uint32_t sample_value; // sample value from the shake streaming
    
    while((ctr < length) && ((pos + 3) <= buflen)) {
        /* convert from 3 bytes in to an coefficient */
        sample_value  = buf[pos++];
        sample_value |= (uint32_t)buf[pos++] << 8;
        sample_value |= (uint32_t)buf[pos++] << 16;
        sample_value &= 0x7FFFFF;
        
        if(sample_value < Q) { 
            coeff[ctr++] = sample_value;
        }
    }
    return ctr;
}

/**
 * @brief Sampling the coefficient in T_{q}. This equal to the function RejNTTPoly in FIPS.204. Called by ExpandA fucnction
 * 
 * @ref poly_uniform
 * 
 * @param seed Seed for the Shake generation
 * @param nonce Two nonce byte
 */
void Polynomial::polynomial_poly_uniform(const std::array<uint8_t, SEEDBYTES>& seed, uint16_t nonce) { 
    size_t i{0}, ctr{0}, off{0};

    // get the buffer length for getting data from Shake function
    uint32_t buflen = this->POLY_UNIFORM_NBLOCKS * mldsa::stream_function::STREAM128_BLOCKBYTES;

    // buffer for getting byte form shake {init to 0}
    uint8_t buf[this->POLY_UNIFORM_NBLOCKS * mldsa::stream_function::STREAM128_BLOCKBYTES + 2] = {0};

    // state for the requesting
    stream128_state state;
    
    // init the streaming function
    mldsa::stream_function::shake128_stream_init(&state, seed, nonce);
        
    shake128_squeezeblocks(buf, this->POLY_UNIFORM_NBLOCKS, &state);
        
    //Convert from data to the coefficient
    ctr = this->_coefficient_from_3bytes(this->_coeffs.data(), N, buf, buflen);

    // Retry to to update the coefficient (Extremly rare case)
    while(ctr < N) {
        off = buflen % 3;
        for(i = 0; i < off; ++i)
        {
            buf[i] = buf[buflen - off + i];
        }
        shake128_squeezeblocks(buf + off, 1, &state);
        buflen = mldsa::stream_function::STREAM128_BLOCKBYTES + off;
        ctr += this->_coefficient_from_3bytes(this->_coeffs.data() + ctr, N - ctr, buf, buflen);
      }
}

/**
 * @brief This function equals to the coefficient from the half-byte. CoeffFromHalfByte
 * @ref rej_eta
 * @param coeff Coefficient array (this->_coefficient)
 * @param length Length of the requested data
 * @param buf Buffer from the SHAKE function
 * @param buflen Length of the buffer
 * @return uint32_t Number of sampled coefficients
 */
uint32_t Polynomial::_coefficient_from_halfbyte(int32_t* coeff, uint32_t length, const uint8_t* buf, uint32_t buflen) {
    // local variable 
    size_t ctr{0}, pos{0}; // counter
    uint32_t z0{0}, z1{0}; // sample value from the shake streaming
    
    while((ctr < length) && (pos < buflen))  {
        /* convert from 3 bytes in to an coefficient */
        z0 = buf[pos] & 0x0F;
        z1 = buf[pos++] >> 4;
        
        if (ETA == 2)
        {
            if(z0 < 15) 
            {
                z0 = z0 - (205*z0 >> 10)*5;
                coeff[ctr++] = 2 - z0;
            }
            if((z1 < 15) && (ctr < length)) 
            {
                z1 = z1 - (205*z1 >> 10)*5;
                coeff[ctr++] = 2 - z1;
            }
        }
        else if (ETA == 4) 
        {
            if(z0 < 9) {
                coeff[ctr++] = 4 - z0;
            }
            
            if((z1 < 9) && (ctr < length))
            {
                coeff[ctr++] = 4 - z1;
            }
        }
    }
    return ctr;
}

/**
 * @brief Sample poly in R with coefficient in [-η,η]. This equals to the function RejBoundedPoly in FIPS204. Called for ExpandS
 * 
 * @param seed Seed for SHAKE squeeze function (G:shake256)
 * @param nonce 2 nonce bytes
 */
void Polynomial::polynomial_uniform_eta(const std::array<uint8_t, CRHBYTES>& seed, uint16_t nonce) {
    
    size_t ctr{0};

    // get the buffer length for getting data from Shake function
    uint32_t buflen = this->POLY_UNIFORM_ETA_NBLOCKS * mldsa::stream_function::STREAM256_BLOCKBYTES;
    
    // buffer for getting byte form shake {init to 0}
    uint8_t buf[this->POLY_UNIFORM_ETA_NBLOCKS * mldsa::stream_function::STREAM256_BLOCKBYTES] = {0};

    // variable for storing the byte
    stream256_state state;
    // init the stream G (Shake256 function)
    mldsa::stream_function::shake256_stream_init(&state, seed, nonce);
    // get the psuedo random data from the byte
    shake256_squeezeblocks(buf, this->POLY_UNIFORM_ETA_NBLOCKS, &state);

    // convert the data from shake into the polynomial coefficient
    ctr = this->_coefficient_from_halfbyte(this->_coeffs.data(), N, buf, buflen);
    
    // handle the defekt case (Rare case)
    while(ctr < N) {
        shake256_squeezeblocks(buf, 1, &state);
        ctr += this->_coefficient_from_halfbyte(this->_coeffs.data(), N - ctr, buf, mldsa::stream_function::STREAM256_BLOCKBYTES);
    }
}

/**
 * @brief Unpack the coefficient from the buffer
 * 
 * @param buf Buffer that used for packing the coefficient
 */
void Polynomial::polyz_unpack(const uint8_t *buf) {
    size_t i{0};
    if(GAMMA1 == (1 << 17)) {
        for(i = 0; i < N/4; ++i) {
            this->_coeffs[4*i+0]  = buf[9*i+0];
            this->_coeffs[4*i+0] |= (uint32_t)buf[9*i+1] << 8;
            this->_coeffs[4*i+0] |= (uint32_t)buf[9*i+2] << 16;
            this->_coeffs[4*i+0] &= 0x3FFFF;
            
            this->_coeffs[4*i+1]  = buf[9*i+2] >> 2;
            this->_coeffs[4*i+1] |= (uint32_t)buf[9*i+3] << 6;
            this->_coeffs[4*i+1] |= (uint32_t)buf[9*i+4] << 14;
            this->_coeffs[4*i+1] &= 0x3FFFF;
            
            this->_coeffs[4*i+2]  = buf[9*i+4] >> 4;
            this->_coeffs[4*i+2] |= (uint32_t)buf[9*i+5] << 4;
            this->_coeffs[4*i+2] |= (uint32_t)buf[9*i+6] << 12;
            this->_coeffs[4*i+2] &= 0x3FFFF;
            
            this->_coeffs[4*i+3]  = buf[9*i+6] >> 6;
            this->_coeffs[4*i+3] |= (uint32_t)buf[9*i+7] << 2;
            this->_coeffs[4*i+3] |= (uint32_t)buf[9*i+8] << 10;
            this->_coeffs[4*i+3] &= 0x3FFFF;
            
            this->_coeffs[4*i+0] = GAMMA1 - this->_coeffs[4*i+0];
            this->_coeffs[4*i+1] = GAMMA1 - this->_coeffs[4*i+1];
            this->_coeffs[4*i+2] = GAMMA1 - this->_coeffs[4*i+2];
            this->_coeffs[4*i+3] = GAMMA1 - this->_coeffs[4*i+3];
        }
    }
    else if (GAMMA1 == (1 << 19)) {
        for(i = 0; i < N/2; ++i) {
            this->_coeffs[2*i+0]  = buf[5*i+0];
            this->_coeffs[2*i+0] |= (uint32_t)buf[5*i+1] << 8;
            this->_coeffs[2*i+0] |= (uint32_t)buf[5*i+2] << 16;
            this->_coeffs[2*i+0] &= 0xFFFFF;
            
            this->_coeffs[2*i+1]  = buf[5*i+2] >> 4;
            this->_coeffs[2*i+1] |= (uint32_t)buf[5*i+3] << 4;
            this->_coeffs[2*i+1] |= (uint32_t)buf[5*i+4] << 12;
            /* this->_coeffs[2*i+1] &= 0xFFFFF; */ /* No effect, since we're anyway at 20 bits */
            
            this->_coeffs[2*i+0] = GAMMA1 - this->_coeffs[2*i+0];
            this->_coeffs[2*i+1] = GAMMA1 - this->_coeffs[2*i+1];
        }
    }
}

/**
 * @brief This will sample the coefficient from the H.squeeze function
 * 
 * @param seed Seed for the H function
 * @param nonce 2 nonce bytes
 */
void Polynomial::polynomial_uniform_gamma1(const std::array<uint8_t, CRHBYTES>& seed, uint16_t nonce) {
    // buffer for querying data from H (shake256 function)
    uint8_t buf[this->POLY_UNIFORM_GAMMA1_NBLOCKS * mldsa::stream_function::STREAM256_BLOCKBYTES] = {0u};
    
    // squeeze data from H function
    stream256_state state;
    mldsa::stream_function::shake256_stream_init(&state,seed, nonce);
    shake256_squeezeblocks(buf, this->POLY_UNIFORM_GAMMA1_NBLOCKS, &state);

    // Unpack data from the shake function
    this->polyz_unpack(buf);
}


/**
 * @brief SampleInBall function that return correct τ \in {-1;1}
 * @ref poly_challenge
 * @param seed Seed for the H.squeeze() function
 */
void Polynomial::polynomial_sample_in_ball(const std::array<uint8_t, CTILDEBYTES>& seed) {
    
    
    size_t i{0}, b{0}, pos{0};
    uint64_t signs;
    uint8_t buf[SHAKE256_RATE];
    keccak_state state;

    shake256_init(&state);
    shake256_absorb(&state, seed.data(), CTILDEBYTES);
    shake256_finalize(&state);
    shake256_squeezeblocks(buf, 1, &state);

    signs = 0;
    for(i = 0; i < 8; ++i)
      signs |= (uint64_t)buf[i] << 8*i;
    pos = 8;
  
    for(i = 0; i < N; ++i)
      this->_coeffs[i] = 0;
    for(i = N-TAU; i < N; ++i) {
      do {
        if(pos >= SHAKE256_RATE) {
          shake256_squeezeblocks(buf, 1, &state);
          pos = 0;
        }
  
        b = buf[pos++];
      } while(b > i);
  
      this->_coeffs[i] = this->_coeffs[b];
      this->_coeffs[b] = 1 - 2*(signs & 1);
      signs >>= 1;
    }
}

/**
 * @brief Bit pack function with Poly in Eta range
 * 
 * @param buf Pointer to packed buffer
 */
void Polynomial::polyeta_pack(uint8_t* buf) {
    
    unsigned int i;
    uint8_t temp[8];
    
    if(ETA == 2) {
        for(i = 0; i < N/8; ++i) {
            temp[0] = ETA - this->_coeffs[8*i+0];
            temp[1] = ETA - this->_coeffs[8*i+1];
            temp[2] = ETA - this->_coeffs[8*i+2];
            temp[3] = ETA - this->_coeffs[8*i+3];
            temp[4] = ETA - this->_coeffs[8*i+4];
            temp[5] = ETA - this->_coeffs[8*i+5];
            temp[6] = ETA - this->_coeffs[8*i+6];
            temp[7] = ETA - this->_coeffs[8*i+7];
            
            buf[3*i+0]  = (temp[0] >> 0) | (temp[1] << 3) | (temp[2] << 6);
            buf[3*i+1]  = (temp[2] >> 2) | (temp[3] << 1) | (temp[4] << 4) | (temp[5] << 7);
            buf[3*i+2]  = (temp[5] >> 1) | (temp[6] << 2) | (temp[7] << 5);
        }
    }
    else if (ETA == 4) {
        for(i = 0; i < N/2; ++i) {
            temp[0] = ETA - this->_coeffs[2*i+0];
            temp[1] = ETA - this->_coeffs[2*i+1];
            buf[i] = temp[0] | (temp[1] << 4);
        }
    }
    
}

void Polynomial::polyeta_unpack(const uint8_t *a) {
    size_t i{0};
    if(ETA == 2) {
        for(i = 0; i < N/8; ++i) {
            this->_coeffs[8*i+0] =  (a[3*i+0] >> 0) & 7;
            this->_coeffs[8*i+1] =  (a[3*i+0] >> 3) & 7;
            this->_coeffs[8*i+2] = ((a[3*i+0] >> 6) | (a[3*i+1] << 2)) & 7;
            this->_coeffs[8*i+3] =  (a[3*i+1] >> 1) & 7;
            this->_coeffs[8*i+4] =  (a[3*i+1] >> 4) & 7;
            this->_coeffs[8*i+5] = ((a[3*i+1] >> 7) | (a[3*i+2] << 1)) & 7;
            this->_coeffs[8*i+6] =  (a[3*i+2] >> 2) & 7;
            this->_coeffs[8*i+7] =  (a[3*i+2] >> 5) & 7;
            
            this->_coeffs[8*i+0] = ETA - this->_coeffs[8*i+0];
            this->_coeffs[8*i+1] = ETA - this->_coeffs[8*i+1];
            this->_coeffs[8*i+2] = ETA - this->_coeffs[8*i+2];
            this->_coeffs[8*i+3] = ETA - this->_coeffs[8*i+3];
            this->_coeffs[8*i+4] = ETA - this->_coeffs[8*i+4];
            this->_coeffs[8*i+5] = ETA - this->_coeffs[8*i+5];
            this->_coeffs[8*i+6] = ETA - this->_coeffs[8*i+6];
            this->_coeffs[8*i+7] = ETA - this->_coeffs[8*i+7];
        }
    } else if(ETA == 4) { 
        for(i = 0; i < N/2; ++i) {
            this->_coeffs[2*i+0] = a[i] & 0x0F;
            this->_coeffs[2*i+1] = a[i] >> 4;
            this->_coeffs[2*i+0] = ETA - this->_coeffs[2*i+0];
            this->_coeffs[2*i+1] = ETA - this->_coeffs[2*i+1];
        }
    }
}

void Polynomial::polyt1_pack(uint8_t* buf) {
    size_t i;

    for(i = 0; i < N/4; ++i) {
        buf[5*i+0] = (this->_coeffs[4*i+0] >> 0);
        buf[5*i+1] = (this->_coeffs[4*i+0] >> 8) | (this->_coeffs[4*i+1] << 2);
        buf[5*i+2] = (this->_coeffs[4*i+1] >> 6) | (this->_coeffs[4*i+2] << 4);
        buf[5*i+3] = (this->_coeffs[4*i+2] >> 4) | (this->_coeffs[4*i+3] << 6);
        buf[5*i+4] = (this->_coeffs[4*i+3] >> 2);
      }
}

void Polynomial::polyt1_unpack(const uint8_t *a) {
    unsigned int i;
  
    for(i = 0; i < N/4; ++i) {
      this->_coeffs[4*i+0] = ((a[5*i+0] >> 0) | ((uint32_t)a[5*i+1] << 8)) & 0x3FF;
      this->_coeffs[4*i+1] = ((a[5*i+1] >> 2) | ((uint32_t)a[5*i+2] << 6)) & 0x3FF;
      this->_coeffs[4*i+2] = ((a[5*i+2] >> 4) | ((uint32_t)a[5*i+3] << 4)) & 0x3FF;
      this->_coeffs[4*i+3] = ((a[5*i+3] >> 6) | ((uint32_t)a[5*i+4] << 2)) & 0x3FF;
    }
}

void Polynomial::polyt0_pack(uint8_t* buf) {
    size_t i;
    uint32_t temp[8] = {0};

    for(i = 0; i < N/8; ++i) {
        temp[0] = (1 << (D-1)) - this->_coeffs[8*i+0];
        temp[1] = (1 << (D-1)) - this->_coeffs[8*i+1];
        temp[2] = (1 << (D-1)) - this->_coeffs[8*i+2];
        temp[3] = (1 << (D-1)) - this->_coeffs[8*i+3];
        temp[4] = (1 << (D-1)) - this->_coeffs[8*i+4];
        temp[5] = (1 << (D-1)) - this->_coeffs[8*i+5];
        temp[6] = (1 << (D-1)) - this->_coeffs[8*i+6];
        temp[7] = (1 << (D-1)) - this->_coeffs[8*i+7];
    
        buf[13*i+ 0]  =  temp[0];
        buf[13*i+ 1]  =  temp[0] >>  8;
        buf[13*i+ 1] |=  temp[1] <<  5;
        buf[13*i+ 2]  =  temp[1] >>  3;
        buf[13*i+ 3]  =  temp[1] >> 11;
        buf[13*i+ 3] |=  temp[2] <<  2;
        buf[13*i+ 4]  =  temp[2] >>  6;
        buf[13*i+ 4] |=  temp[3] <<  7;
        buf[13*i+ 5]  =  temp[3] >>  1;
        buf[13*i+ 6]  =  temp[3] >>  9;
        buf[13*i+ 6] |=  temp[4] <<  4;
        buf[13*i+ 7]  =  temp[4] >>  4;
        buf[13*i+ 8]  =  temp[4] >> 12;
        buf[13*i+ 8] |=  temp[5] <<  1;
        buf[13*i+ 9]  =  temp[5] >>  7;
        buf[13*i+ 9] |=  temp[6] <<  6;
        buf[13*i+10]  =  temp[6] >>  2;
        buf[13*i+11]  =  temp[6] >> 10;
        buf[13*i+11] |=  temp[7] <<  3;
        buf[13*i+12]  =  temp[7] >>  5;
      }
}

void Polynomial::polyt0_unpack(const uint8_t* a) {
    size_t i;
  
    for(i = 0; i < N/8; ++i) {
      this->_coeffs[8*i+0]  = a[13*i+0];
      this->_coeffs[8*i+0] |= (uint32_t)a[13*i+1] << 8;
      this->_coeffs[8*i+0] &= 0x1FFF;
  
      this->_coeffs[8*i+1]  = a[13*i+1] >> 5;
      this->_coeffs[8*i+1] |= (uint32_t)a[13*i+2] << 3;
      this->_coeffs[8*i+1] |= (uint32_t)a[13*i+3] << 11;
      this->_coeffs[8*i+1] &= 0x1FFF;
  
      this->_coeffs[8*i+2]  = a[13*i+3] >> 2;
      this->_coeffs[8*i+2] |= (uint32_t)a[13*i+4] << 6;
      this->_coeffs[8*i+2] &= 0x1FFF;
  
      this->_coeffs[8*i+3]  = a[13*i+4] >> 7;
      this->_coeffs[8*i+3] |= (uint32_t)a[13*i+5] << 1;
      this->_coeffs[8*i+3] |= (uint32_t)a[13*i+6] << 9;
      this->_coeffs[8*i+3] &= 0x1FFF;
  
      this->_coeffs[8*i+4]  = a[13*i+6] >> 4;
      this->_coeffs[8*i+4] |= (uint32_t)a[13*i+7] << 4;
      this->_coeffs[8*i+4] |= (uint32_t)a[13*i+8] << 12;
      this->_coeffs[8*i+4] &= 0x1FFF;
  
      this->_coeffs[8*i+5]  = a[13*i+8] >> 1;
      this->_coeffs[8*i+5] |= (uint32_t)a[13*i+9] << 7;
      this->_coeffs[8*i+5] &= 0x1FFF;
  
      this->_coeffs[8*i+6]  = a[13*i+9] >> 6;
      this->_coeffs[8*i+6] |= (uint32_t)a[13*i+10] << 2;
      this->_coeffs[8*i+6] |= (uint32_t)a[13*i+11] << 10;
      this->_coeffs[8*i+6] &= 0x1FFF;
  
      this->_coeffs[8*i+7]  = a[13*i+11] >> 3;
      this->_coeffs[8*i+7] |= (uint32_t)a[13*i+12] << 5;
      this->_coeffs[8*i+7] &= 0x1FFF;
  
      this->_coeffs[8*i+0] = (1 << (D-1)) - this->_coeffs[8*i+0];
      this->_coeffs[8*i+1] = (1 << (D-1)) - this->_coeffs[8*i+1];
      this->_coeffs[8*i+2] = (1 << (D-1)) - this->_coeffs[8*i+2];
      this->_coeffs[8*i+3] = (1 << (D-1)) - this->_coeffs[8*i+3];
      this->_coeffs[8*i+4] = (1 << (D-1)) - this->_coeffs[8*i+4];
      this->_coeffs[8*i+5] = (1 << (D-1)) - this->_coeffs[8*i+5];
      this->_coeffs[8*i+6] = (1 << (D-1)) - this->_coeffs[8*i+6];
      this->_coeffs[8*i+7] = (1 << (D-1)) - this->_coeffs[8*i+7];
    }
}

void Polynomial::polyz_pack(uint8_t* buf) {
    unsigned int i;
    uint32_t temp[4];
    
    if(GAMMA1 == (1 << 17)) { 
        for(i = 0; i < N/4; ++i) {
            temp[0] = GAMMA1 - this->_coeffs[4*i+0];
            temp[1] = GAMMA1 - this->_coeffs[4*i+1];
            temp[2] = GAMMA1 - this->_coeffs[4*i+2];
            temp[3] = GAMMA1 - this->_coeffs[4*i+3];
            
            buf[9*i+0]  = temp[0];
            buf[9*i+1]  = temp[0] >> 8;
            buf[9*i+2]  = temp[0] >> 16;
            buf[9*i+2] |= temp[1] << 2;
            buf[9*i+3]  = temp[1] >> 6;
            buf[9*i+4]  = temp[1] >> 14;
            buf[9*i+4] |= temp[2] << 4;
            buf[9*i+5]  = temp[2] >> 4;
            buf[9*i+6]  = temp[2] >> 12;
            buf[9*i+6] |= temp[3] << 6;
            buf[9*i+7]  = temp[3] >> 2;
            buf[9*i+8]  = temp[3] >> 10;
        }
    } else if (GAMMA1 == (1 << 19)) {
        for(i = 0; i < N/2; ++i) {
            temp[0] = GAMMA1 - this->_coeffs[2*i+0];
            temp[1] = GAMMA1 - this->_coeffs[2*i+1];
            
            buf[5*i+0]  = temp[0];
            buf[5*i+1]  = temp[0] >> 8;
            buf[5*i+2]  = temp[0] >> 16;
            buf[5*i+2] |= temp[1] << 4;
            buf[5*i+3]  = temp[1] >> 4;
            buf[5*i+4]  = temp[1] >> 12;
        }
    }
}


void Polynomial::polyw1_pack(uint8_t* buf) {
    size_t i{0};
    if (GAMMA2 == (Q-1)/88) {
        for(i = 0; i < N/4; ++i) {
            buf[3*i+0]  = this->_coeffs[4*i+0];
            buf[3*i+0] |= this->_coeffs[4*i+1] << 6;
            buf[3*i+1]  = this->_coeffs[4*i+1] >> 2;
            buf[3*i+1] |= this->_coeffs[4*i+2] << 4;
            buf[3*i+2]  = this->_coeffs[4*i+2] >> 4;
            buf[3*i+2] |= this->_coeffs[4*i+3] << 2;
        }
    } else if (GAMMA2 == (Q-1)/32) {
        for(i = 0; i < N/2; ++i)
        buf[i] = this->_coeffs[2*i+0] | (this->_coeffs[2*i+1] << 4);
    }
}

/**
 * @brief Correct the polynomial HightBits 
 * 
 * @param corrected_poly Reference to the polynomial will contain the corre cted HighBits
 * @param highbits_poly Current HighBits polynomial
 * @param hints_poly Hints
 */
void Polynomial::use_hint(const Polynomial& highbits_poly, const Polynomial& hints_poly) {
    for(auto i = 0; i < N; i++) {
        this->_coeffs.at(i) = mldsa::utils::use_hint(highbits_poly._coeffs.at(i), 
                                                              hints_poly._coeffs.at(i));
    }
}

/**
 * @brief Correct the polynomial HightBits 
 * 
 * @param corrected_poly Reference to the polynomial will contain the corrected HighBits
 * @param highbits_poly Current HighBits polynomial
 * @param hints_poly Hints
 */
void poly_use_hint(Polynomial& corrected_poly, const Polynomial& highbits_poly, const Polynomial& hints_poly) {
    for(auto i = 0; i < N; i++) {
        corrected_poly._coeffs.at(i) = mldsa::utils::use_hint(highbits_poly._coeffs.at(i), 
                                                              hints_poly._coeffs.at(i));
    }
}

/**
 * @brief Adding two polynomial coefficient together (not comming with the Q reduction)
 * 
 * @param poly 
 * @return Polynomial& 
 */
Polynomial& Polynomial::operator+=(const Polynomial& poly) {
    for(size_t index = 0; index < N; index++) {
        /* note: there are no modular reduction */
        this->_coeffs[index] = this->_coeffs[index] + poly._coeffs[index]; 
    }
    return *this;
}

/**
 * @brief Substrc two polynomial together (not comming with Q reduction)
 * 
 * @param poly 
 * @return Polynomial& 
 */
Polynomial& Polynomial::operator-=(const Polynomial& poly) {
    for(size_t index = 0; index < N; index++) {
        /* note: there are no modular reduction */
        this->_coeffs[index] = this->_coeffs[index] - poly._coeffs[index]; 
    }
    return *this;
}

/**
 * @brief Check if the infinitive norm of the boundary is smaller than given bound
 * @ref poly_chknorm
 * @param bound Boundary (GAMMA1 - BETA) or (GAMMA2 - BETA)
 * @return true 
 * @return false 
 */
bool Polynomial::norm_check(int32_t bound) const { 
    
    // local variable
    bool retVal = false; // retval
    int32_t coeff;       // current coefficient

    // check if the norm is smaller than the bound
    for(size_t i = 0; i < N; i++) {
        coeff = this->_coeffs.at(i);

        // get the absolute value
        coeff = std::abs(coeff);

        // coefficient greater that the boundary
        if(coeff >= bound) {
            retVal = true;
            break;
        }

    }
    return retVal;
}


/**
 * @brief Perform the polymomial multiplication in the ntt domain.
 * @ref poly_pointwise_montgomery
 * 
 * @param poly_left First operator (must be in ntt domain)
 * @param poly_right Second operator (must be the ntt domain)
 * @return Polynomial 
 */
Polynomial ntt_domain_multiply(const Polynomial& poly_left, const Polynomial& poly_right) {
    Polynomial returnPoly;
    if((poly_left.ntt_status == true) && (poly_right.ntt_status == true)) 
    {
        // the return result must be in the ntt domain
        returnPoly.ntt_status = true;

        // calculate the coefficient
        for(size_t index = 0; index < N; index++) 
        {
            returnPoly._coeffs.at(index) = mldsa::utils::montgomery_reduce(
                static_cast<int64_t>(poly_left._coeffs.at(index)) *
                static_cast<int64_t>(poly_right._coeffs.at(index))
            );
        }
    }
    else 
    {
        std::cout << "WARING: Not in the NTT domain";
        throw std::logic_error("Muliplication not in NTT domain");
    }
    return returnPoly;
}


/**
 * @brief Reservation for the ntt_domain_multiply with the same feature
 * 
 * @param poly The poly that we want to point wise multiplication
 * @return Polynomial& 
 */
Polynomial& Polynomial::operator*=(const Polynomial& poly) {
    if((this->ntt_status == true) && (poly.ntt_status == true)) {

    }
    else {
        throw std::logic_error("Not in ntt domain");
    }
    return *this;
}

Polynomial operator+(const Polynomial& poly_left, const Polynomial& poly_right) {
    Polynomial result;
    for(size_t index = 0; index < N; index++) {
        /* note: there are no modular reduction */
        result._coeffs[index] = poly_left._coeffs[index] + poly_right._coeffs[index]; 
    }
    return result;
}


Polynomial operator-(const Polynomial& poly_left, const Polynomial& poly_right) {
    Polynomial result;
    for(size_t index = 0; index < N; index++) {
        /* note: there are no modular reduction */
        result._coeffs[index] = poly_left._coeffs[index] - poly_right._coeffs[index]; 
    }
    return result;
}


/**
 * @brief Convert the polynomials into the ntt domain for multiplication
 * 
 */

void Polynomial::NTT(){
    size_t len, start, j, k;
    int32_t zeta, t;
    k = 0;
    for(len = 128; len > 0; len >>= 1) {
        for(start = 0; start < N; start = j + len) {
            zeta = zetas[++k];
            for(j = start; j < start + len; ++j) {

                t = mldsa::utils::montgomery_reduce((int64_t)zeta * static_cast<int64_t>(this->_coeffs.at(j + len)));

                // Butter fly operator
                this->_coeffs.at(j + len) = this->_coeffs.at(j) - t;
                this->_coeffs.at(j) = this->_coeffs.at(j) + t;
            }
        }
    }

    // Update the NTT status
    this->ntt_status = true;
}

/**
 * @brief Perform the inverse NTT transform.
 * 
 */

void Polynomial::invNTT() {
    unsigned int start, len, j, k;
    int32_t t, zeta;
    const int32_t f = 41978; // mont^2/256
  
    k = 256;
    for(len = 1; len < N; len <<= 1) {
      for(start = 0; start < N; start = j + len) {
        zeta = -zetas[--k];
        for(j = start; j < start + len; ++j) {
          t = this->_coeffs[j];
          this->_coeffs[j] = t + this->_coeffs[j + len];
          this->_coeffs[j + len] = t - this->_coeffs[j + len];
          this->_coeffs[j + len] = mldsa::utils::montgomery_reduce((int64_t)zeta * this->_coeffs[j + len]);
        }
      }
    }
  
    for(j = 0; j < N; ++j) {
        this->_coeffs[j] = mldsa::utils::montgomery_reduce((int64_t)f * this->_coeffs[j]);
    }

    // Send back to the time domain
    this->ntt_status = false;
  }

/***************************************************************************************
 * 
 *      TESTING ZONE
 * *************************************************************************************
*/
std::ostream& operator<<(std::ostream& os, const Polynomial& poly)
{
    for(const auto& element : poly._coeffs) {
        os << element << " ";
    }
    os << "\n";
    return os;
}