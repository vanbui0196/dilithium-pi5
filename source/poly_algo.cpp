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
 * @brief Make the hint
 * 
 * @param lowbits_poly 
 * @param highbits_poly 
 * @param hints_poly 
 * @return uint32_t 
 */
uint32_t Polynomial::make_hint(Polynomial& lowbits_poly, Polynomial& highbits_poly, Polynomial& hints_poly) {
    // Calculate the total hints
    uint32_t sum  = 0;
    for(auto i = 0; i < N; i++) {
        // Update the hint poly
        hints_poly._coeffs.at(i) = mldsa::utils::make_hint(lowbits_poly._coeffs.at(i), highbits_poly._coeffs.at(i));
        sum = sum + hints_poly._coeffs.at(i);
    }
    return sum;
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
 * @brief Perform the polymomial multiplication in the ntt domain.
 * @ref poly_pointwise_montgomery
 * 
 * @param poly_left First operator (must be in ntt domain)
 * @param poly_right Second operator (must be the ntt domain)
 * @return Polynomial 
 */
Polynomial Polynomial::ntt_domain_multiply(const Polynomial& poly_left, const Polynomial& poly_right) {
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
                                        static_cast<int64_t>(poly_left._coeffs.at(index))
                                    );
        }
    }
    else 
    {
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

Polynomial operator+(Polynomial poly_left, const Polynomial& poly_right) {
    poly_left += poly_right;
    return poly_left;
}

Polynomial operator-(Polynomial poly_left, const Polynomial& poly_right) {
    poly_left -= poly_right;
    return poly_left;
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
    os << "\n=============================================================================================================";
    return os;
}