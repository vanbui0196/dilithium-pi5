#pragma once
#include <vector>
#include "poly_algo.h"

template <size_t Mm>
class PolyVector {
private:
    std::array<Polynomial, Mm> _poly_vector;
    size_t _vector_size;
public:

    /**
     * @brief Construct a new Poly Vector object
     * 
     */
    explicit PolyVector() : _vector_size{Mm} {

    }

    /**
     * @brief Uniform the vector with the value of
     * 
     * @param seed 
     * @param nonce 
     */
    void vector_uniform_eta(const std::array<uint8_t, CRHBYTES>& seed, uint16_t nonce) {
        for(auto ctr = 0; ctr < this->_vector_size; ctr++) {
            this->_poly_vector.at(ctr).polynomial_uniform_eta(seed, nonce++);
        }
    }
    
    /**
     * @brief Uniform vector in the gamma range
     * 
     * @param seed Seed for Shake function
     * @param nonce Nonce byte
     */
    void vector_uniform_gamma1(const std::array<uint8_t, CRHBYTES>& seed, uint16_t nonce) {
        for(auto ctr = 0; ctr < this->_vector_size; ctr++) {
            this->_poly_vector.at(ctr).polynomial_uniform_eta(seed, nonce++);
        }
    }

    /**
     * @brief Reduced the polynomial
     * 
     */
    void vector_reduced() {
        for(Polynomial& element : this->_poly_vector) {
            element.reduced();
        }
    }

    /**
     * @brief Add two two Poly vector together (return by value)
     * @status not-tested
     * @param a First Vector
     * @param b Second Vector
     * @return PolyVector<Mm> 
     */
    friend PolyVector<Mm> operator+(const PolyVector<Mm>& a, const PolyVector<Mm>& b) {
        PolyVector<Mm> result;
        
        for(auto i = 0; i < Mm; i++) {
            result._poly_vector[i] = a._poly_vector[i] + b._poly_vector[i];
        }
        
        return result;
    }

        /**
     * @brief Substract two Poly vector (return by value)
     * @status not-tested
     * @param a First Vector
     * @param b Second Vector
     * @return PolyVector<Mm> 
     */
    friend PolyVector<Mm> operator-(const PolyVector<Mm>& a, const PolyVector<Mm>& b) {
        PolyVector<Mm> result;
        
        for(auto i = 0; i < Mm; i++) {
            result._poly_vector[i] = a._poly_vector[i] - b._poly_vector[i];
        }
        
        return result;
    }

    /**
     * @brief Convert the poly vector into the NTT domain (Or Frequency domain)
     * @status: tested
     */
    void vector_NTT() {
        for(Polynomial& poly : this->_poly_vector) {
            poly.NTT();
        }
    }

    /**
     * @brief Convert the poly back to Polynomial domain (or time domain)
     * @status: tested
     */
    void vector_invNTT() {
        for(Polynomial& poly : this->_poly_vector) {
            poly.invNTT();
        }
    }

    /**
     * @brief Perform the pointwise polynomial in the NTT domain. Return by value
     * @status: tested
     * @param a First operand
     * @param b Second operand
     * @return PolyVector<Mm> Poly that contain the pointwise multiplication
     */
    friend PolyVector<Mm> vector_pointwise_multiply(const PolyVector<Mm>& a, const PolyVector<Mm>& b) {
        PolyVector<Mm> result;
        for(size_t i = 0; i < Mm; i++) {
            result._poly_vector.at(i) = ntt_domain_multiply(a._poly_vector.at(i), b._poly_vector.at(i));
        }
        return result;
    }

    /**
     * @brief Multiply in the NTT domain. Return by the reference
     * @status not-tested
     * @param result Reference return
     * @param a Poly a
     * @param b Poly b
     */
    friend void vector_pointwise_multiply(PolyVector<Mm>& result,const PolyVector<Mm>& a, const PolyVector<Mm>& b) {
        for(size_t i = 0; i < Mm; i++) {
            result._poly_vector.at(i) = ntt_domain_multiply(a._poly_vector.at(i), b._poly_vector.at(i));
        }
    }

    /**
     * @brief Vector multiply in the NTT domain (return by value)
     * status: not-tested
     * @param a First operand
     * @param b Second operand
     * @return Polynomial Return by value
     */
    friend Polynomial vector_pointwise_multiply_accumulate(const PolyVector<Mm>& a, const PolyVector<Mm>& b) {
        Polynomial result;
        Polynomial temp;
        // first initial
        result = ntt_domain_multiply(a._poly_vector.at(0), b._poly_vector.at(0));

        for(size_t i = 1; i < Mm; i++) {
            temp = ntt_domain_multiply(a._poly_vector.at(i), b._poly_vector.at(i));
            result += temp;
        }
        return result;
    }

    /**
     * @brief Vector multiply in the NTT domain. (Return by reference)
     * @status tested
     * @param result Vector multiplication result
     * @param a First operand
     * @param b Second operand
     */
    friend void vector_pointwise_multiply_accumulate(Polynomial& result, const PolyVector<Mm>& a, const PolyVector<Mm>& b) {
        Polynomial temp;
        // first initial
        result = ntt_domain_multiply(a._poly_vector.at(0), b._poly_vector.at(0));

        for(size_t i = 1; i < Mm; i++) {
            temp = ntt_domain_multiply(a._poly_vector.at(i), b._poly_vector.at(i));
            result += temp;
        }
    }

    /**
     * @brief Check if the norm is larger than expected
     * @status not-tested
     * @param bound Bound for the validation
     * @return true Bound constraint is not valid
     * @return false Good coefficient
     */
    bool vector_checknorm(int32_t bound) const {
        bool retVal = false;
        for(Polynomial& element : this->_poly_vector) {
            if(element.norm_check(bound)) {
                retVal = true;
                break;
            }
        }
        return retVal;
    }

    /**
     * @brief Add the value of the Q in case of the negative
     * @status not-tested
     */
    void vector_caddq(void) {
        for(Polynomial& element : this->_poly_vector) {
            element.negative_add_Q();
        }
    }

    /**
     * @brief Shift all the coefficient in the vector to left 2^D element
     * @status not-tested
     */
    void vector_shiftl(void) {
        for(Polynomial& element : this->_poly_vector) {
            element.shift_toleft_2D();
        }
    }

    /**
     * @brief Split the vector of Power2Round (2^D)
     * @status not-tested
     * @param vector_highbits HighBits reference vector
     * @param vector_lowbits LowBits reference vector
     */
    void vector_power2round(PolyVector<Mm>& vector_highbits, PolyVector<Mm>& vector_lowbits) {
        for(size_t i = 0; i < Mm; i++) { 
            this->_poly_vector.at(i).power2round(vector_lowbits._poly_vector.at(i), 
                                                 vector_highbits._poly_vector.at(i));
        }
    }

    /**
     * @brief Split the vector into 2 part with the factor of 2*GAMMA2
     * @ status not-tested
     * 
     * @param vector_highbits HighBits vector reference
     * @param vector_lowbits LowBits vector reference
     */
    void vector_decompose(PolyVector<Mm>& vector_highbits, PolyVector<Mm>& vector_lowbits) {
        for(size_t i = 0; i < Mm; i++) { 
            this->_poly_vector.at(i).decompose(vector_lowbits._poly_vector.at(i), 
                                                 vector_highbits._poly_vector.at(i));
        }
    }

    /**
     * @brief Friend function: Make hints for Sig verification (with input of HighBits and LowBits) with reference parameter
     * @status not-tested
     * @param vector_hints reference to the Hints Vector
     * @param vector_lowbits (constant) reference to 
     * @param vector_highbits 
     * @return uint32_t Total hint that needed
     */
    friend uint32_t vector_make_hint(PolyVector<Mm>& vector_hints, const PolyVector<Mm>& vector_lowbits, 
        const PolyVector<Mm>& vector_highbits) {
        // local variable for the return value
        uint32_t sum{0};
        // make hint from input vector
        for(size_t i = 0; i < Mm; i++) {
            sum += poly_make_hint(vector_hints._poly_vector.at(i),
            vector_lowbits._poly_vector.at(i),
            vector_highbits._poly_vector.at(i));
        }
        return sum;
    }

    /**
     * @brief (Method based) Make hint based for Sig verification 
     * @status not-tested
     * @param vector_lowbits Vector that contain LowBits
     * @param vector_highbits Vector that contain HighBits 
     * @return uint32_t Total hint that needed
     */
    uint32_t vector_make_hint(const PolyVector<Mm>& vector_lowbits,  const PolyVector<Mm>& vector_highbits) {
        // return value
        uint32_t sum {0};
        // make hint and store into the current vector
        for(size_t i = 0; i < Mm; i++) {
            sum += this->_poly_vector.at(i).make_hint(vector_lowbits._poly_vector.at(i), 
                                                      vector_highbits._poly_vector.at(i));
        }
        return sum;
    }

    /**
     * @brief (Friend function) Use hint and update the correct vector
     * @status not-tested
     * @param vector_corrected HighBits vector with corrected value
     * @param vector_highbits Input highbits for checking
     * @param vector_hints Hint vector
     */
    friend void vector_use_hint(PolyVector<Mm>& vector_corrected, const PolyVector<Mm>& vector_highbits, const PolyVector<Mm> vector_hints) {
        for(size_t i = 0; i < Mm; i++) {
            poly_use_hint(vector_corrected._poly_vector.at(i), 
                            vector_highbits._poly_vector.at(i), 
                            vector_hints._poly_vector.at(i)
                         );
        }
    }
    
    /**
     * @brief (Method based) Use hint and update corrected vector (object attribute)
     * @status not-tested
     * @param vector_highbits HighBits needed for correction
     * @param vector_hints Hints for the correction
     */
    void vector_use_hint(const PolyVector<Mm>& vector_highbits, const PolyVector<Mm> vector_hints) {
        for(size_t i =0; i < Mm; i++) {
            this->_poly_vector.at(i).use_hint(vector_highbits._poly_vector.at(i), vector_hints._poly_vector.at(i));
        }
    }

    /**
     * @brief Pack the poly of the w1 into the buffer;
     * 
     * @param buf Buffer for packaging
     */
    void vector_packw1(uint8_t* buf) const {
        for(Polynomial& element : this->_poly_vector) {
            element.polyw1_pack(buf);
        }
    }
    /**
     * @brief Fill for testing purpose
     * 
     * @param value Value will be filled for all of the vector
     */
    void fill(int32_t value) {
        for(auto& poly : this->_poly_vector) {
            poly.fill(value);
        }
    }

    friend std::ostream& operator<< <Mm>(std::ostream& os, const PolyVector<Mm>& poly_vector);
};

/**
 * @brief Vector
 * 
 * @tparam Mm This shall be equal to L
 * @tparam Nm 
 */
template <size_t Mm, size_t Nm>
class PolyMatrix {
private:
    std::array<PolyVector<Mm>, Nm> _poly_matrix;
    size_t size_row{0};
    size_t size_colum{0};
public:

    explicit PolyMatrix() : size_row{Mm}, size_colum{Nm} {}

    /**
     * @brief Expand the matrix based on the seed
     * @status: not test est
     * @param rho Seed for generating
     */
    void expand(const std::array<uint8_t, SEEDBYTES>& rho) {

        // For nonce bytes
        uint16_t i{0};

        for(PolyVector<Mm>& vector : this->_poly_matrix) {
            // for nonce byte
            uint16_t j{0};

            for(Polynomial& poly : vector) {
                // Uniform based on the Shake128 and the nonce byte
                poly.polynomial_poly_uniform(rho, ((i << 8) + j));
                j++;
            }
            i++;
        }
    }

    // void expand(const std::array<uint8_t, SEEDBYTES>& rho) {
    //     #pragma omp parallel for
    //     for(size_t i = 0; i < Nm; i++) {
    //         #pragma omp simd
    //         for(size_t j = 0; j < Mm; j++) {
    //             // Uniform based on the Shake128 and the nonce byte
    //             this->_poly_matrix[i]._poly_vector[j].polynomial_poly_uniform(rho, ((i << 8) + j));
    //         }
    //     }
    // }

    /**
     * @brief Matrix multiplication [Nm, Mm] * [Mm, 1]= [Nm, 1]
     * @status: not tested
     * @param matrix 
     * @param vector 
     * @return PolyVector<Nm> 
     */
    friend PolyVector<Nm> matrix_multiply(const PolyMatrix<Mm, Nm>& matrix, const PolyVector<Mm>& vector) { 
        PolyVector<Nm> result;
        #pragma omp parallel for simd
        for(size_t i = 0; i < Nm; i++) {
            result._poly_vector.at(i) = vector_pointwise_multiply_accumulate(
                matrix._poly_matrix.at(i), vector
            );
        }
        return result;  // You were missing the return statement
    }
};


#include "poly_vector.tpp"