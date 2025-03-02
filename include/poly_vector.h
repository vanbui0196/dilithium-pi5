#pragma once
#include <vector>
#include <iostream>
#include "poly_algo.h"

// Forward declaration of PolyMatrix
template<size_t M, size_t N> class PolyMatrix;

template <size_t Mm>
class PolyVector {
private:
    std::array<Polynomial, Mm> _poly_vector;
    size_t _vector_size;
    template<size_t M, size_t N> friend class PolyMatrix; // for making matrix can reed the internal properties
    template<size_t M, size_t N> friend PolyVector<N> matrix_multiply(const PolyMatrix<M, N>&, const PolyVector<M>&);
public:

    /**
     * @brief Construct a new Poly Vector object
     * 
     */
    explicit PolyVector() : _vector_size{Mm} {

    }

    /**
     * @brief This BRIDGE will be removed soon. Un-wanted method
     * 
     * @param index 
     * @return Polynomial& 
     */
    Polynomial& access_poly_at(size_t index) {
        return _poly_vector.at(index);
    }

    /**
     * @brief Copy constructor for deep copying
     * 
     * @param other The PolyVector to copy from
     */
    PolyVector(const PolyVector<Mm>& other) : _vector_size(other._vector_size) {
        // Deep copy each polynomial in the vector
        for (size_t i = 0; i < Mm; i++) {
            this->_poly_vector[i] = other._poly_vector[i];
        }
    }

    /**
     * @brief Copy assignment operator for deep copying
     * 
     * @param other The PolyVector to copy from
     * @return PolyVector<Mm>& Reference to this object
     */
    PolyVector<Mm>& operator=(const PolyVector<Mm>& other) {
        // Self-assignment check
        if (this == &other) {
            return *this;
        }
        
        // Copy the size
        this->_vector_size = other._vector_size;
        
        // Deep copy each polynomial in the vector
        for (size_t i = 0; i < Mm; i++) {
            this->_poly_vector[i] = other._poly_vector[i];
        }
        
        return *this;
    }

    // Non-const iterators
    // auto begin() { return _vector_size.begin(); }
    // auto end() { return _vector_size.end(); }
    
    // // Const iterators
    // auto begin() const { return _vector_size.begin(); }
    // auto end() const { return _vector_size.end(); }
    
    // // Optional but recommended: const iterators via non-const object
    // auto cbegin() const { return _vector_size.cbegin(); }
    // auto cend() const { return _vector_size.cend(); }

    /**
     * @brief Uniform the vector with the value of
     * @status tested
     * @param seed 
     * @param nonce 
     */
    void vector_uniform_eta(const std::array<uint8_t, CRHBYTES>& seed, uint16_t nonce) {
        for(auto ctr = 0; ctr < Mm; ctr++) {
            this->_poly_vector.at(ctr).polynomial_uniform_eta(seed, nonce++);
        }
    }
    
    /**
     * @brief Uniform vector in the gamma range
     * @status tested
     * @param seed Seed for Shake function
     * @param nonce Nonce byte
     */
    void vector_uniform_gamma1(const std::array<uint8_t, CRHBYTES>& seed, uint16_t nonce) {
        for(auto ctr = 0; ctr < Mm; ctr++) {
            this->_poly_vector.at(ctr).polynomial_uniform_gamma1(seed, Mm * nonce + ctr);
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
     * @status TESTED
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
     * @status TESTED
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
     * @ref polyveck_pointwise_poly_montgomery
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
     * @status TESTED
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
     * @brief Perform the pointwise polynomial in the NTT domain betwen the poly and vector. Return by value

     * @status: NOT-TESTED 
     * @param a First operand
     * @param b Second operand
     * @return PolyVector<Mm> Poly that contain the pointwise multiplication
     */
    friend PolyVector<Mm> vector_and_poly_pointwise_multiply(const Polynomial& a, const PolyVector<Mm>& b) {
        PolyVector<Mm> result;
        for(size_t i = 0; i < Mm; i++) {
            result._poly_vector.at(i) = ntt_domain_multiply(a, b._poly_vector.at(i));
        }
        return result;
    }

    /**
     * @brief Multiply in the NTT domain between the poly and vector. Return by the reference
     * @status NOT-TESTED
     * @param result Reference return
     * @param a Poly a
     * @param b Poly b
     */
    friend void vector_and_poly_pointwise_multiply(PolyVector<Mm>& result,const Polynomial& a, const PolyVector<Mm>& b) {
        for(size_t i = 0; i < Mm; i++) {
            result._poly_vector.at(i) = ntt_domain_multiply(a, b._poly_vector.at(i));
        }
    }

    /**
     * @brief Vector multiply in the NTT domain (return by value)
     * status: TESTED
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
     * @status TESTED
     * @param bound Bound for the validation
     * @return true Bound constraint is not valid
     * @return false Good coefficient
     */
    bool vector_checknorm(int32_t bound) const {
        bool retVal = false;
        for(const Polynomial& element : this->_poly_vector) {
            if(element.norm_check(bound)) {
                retVal = true;
                break;
            }
        }
        return retVal;
    }

    /**
     * @brief Add the value of the Q in case of the negative
     * @status tested
     */
    void vector_caddq(void) {
        for(Polynomial& element : this->_poly_vector) {
            element.negative_add_Q();
        }
    }

    /**
     * @brief Shift all the coefficient in the vector to left 2^D element
     * @status tested
     */
    void vector_shiftl(void) {
        for(Polynomial& element : this->_poly_vector) {
            element.shift_toleft_2D();
        }
    }

    /**
     * @brief Split the vector of Power2Round (2^D)
     * @status TESTED
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
     * @ status tested
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
     * @status TESTED
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
     * @status TESTED
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
     * @status TESTED
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
     * @status tested
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
     * @status TESTED
     * @param buf Buffer for packaging
     */
    void vector_packw1(uint8_t* buf){
        for(size_t i = 0; i < Mm; ++i) {
            this->_poly_vector.at(i).polyw1_pack(&buf[i*POLYW1_PACKEDBYTES]);
        }
    }

    /**
     * @brief Pack t1 into the buffer
     * 
     * @param buf Pointer to the buffer
     */
    void vector_packt1(uint8_t* buf){
        for(size_t i = 0; i < Mm; ++i) {
            this->_poly_vector.at(i).polyt1_pack(&buf[i*POLYT1_PACKEDBYTES]);
        }
    }

    void vector_packt0(uint8_t* buf){
        for(size_t i = 0; i < Mm; ++i) {
            this->_poly_vector.at(i).polyt0_pack(&buf[i*POLYT0_PACKEDBYTES]);
        }
    }

    void vector_unpackt0(const uint8_t* buf){
        for(size_t i = 0; i < Mm; ++i) {
            this->_poly_vector.at(i).polyt0_unpack(&buf[i*POLYT0_PACKEDBYTES]);
        }
    }

    void vector_unpackt1(const uint8_t* buf){
        for(size_t i = 0; i < Mm; ++i) {
            this->_poly_vector.at(i).polyt1_unpack(&buf[i*POLYT1_PACKEDBYTES]);
        }
    }

    /**
     * @brief Pack the vector with the eta size
     * 
     * @param buf Pointer to the buffer
     */
    void vector_packeta(uint8_t* buf){
        for(size_t i = 0; i < Mm; ++i) {
            this->_poly_vector.at(i).polyeta_pack(&buf[i*POLYETA_PACKEDBYTES]);
        }
    }

    void vector_unpacketa(const uint8_t* buf){
        for(size_t i = 0; i < Mm; ++i) {
            this->_poly_vector.at(i).polyeta_unpack(&buf[i*POLYETA_PACKEDBYTES]);
        }
    }

    void vector_packz(uint8_t* buf){
        for(size_t i = 0; i < Mm; ++i) {
            this->_poly_vector.at(i).polyz_pack(&buf[i*POLYZ_PACKEDBYTES]);
        }
    }

    void vector_unpackz(const uint8_t* buf){
        for(size_t i = 0; i < Mm; ++i) {
            this->_poly_vector.at(i).polyz_unpack(&buf[i*POLYZ_PACKEDBYTES]);
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
 * @brief MATRIX with the size of !!!Important: --->[Nm,Mm]<--- !!!Important | Not: ---> [Mm, Nm] <---
 * 
 * @tparam Mm This euqal to L in the matrix context
 * @tparam Nm This equal to K in the matrix context
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
    * @brief Copy constructor for deep copying
    * 
    * @param other The PolyMatrix to copy from
    */
    PolyMatrix(const PolyMatrix<Mm, Nm>& other) : 
    size_row(other.size_row), size_colum(other.size_colum) {
        // Deep copy each PolyVector in the matrix
        for (size_t i = 0; i < Nm; i++) {
            this->_poly_matrix[i] = other._poly_matrix[i];
        }
    }

    /**
    * @brief Copy assignment operator for deep copying
    * 
    * @param other The PolyMatrix to copy from
    * @return PolyMatrix<Mm, Nm>& Reference to this object
    */
    PolyMatrix<Mm, Nm>& operator=(const PolyMatrix<Mm, Nm>& other) {
        // Self-assignment check
        if (this == &other) {
            return *this;
        }
        
        // Copy the dimensions
        this->size_row = other.size_row;
        this->size_colum = other.size_colum;
        
        // Deep copy each PolyVector in the matrix
        for (size_t i = 0; i < Nm; i++) {
            this->_poly_matrix[i] = other._poly_matrix[i];
        }
        
        return *this;
    }
    /**
     * @brief Expand the matrix based on the seed
     * @status TESTED
     * @param rho Seed for generating
     */
    void expand(const std::array<uint8_t, SEEDBYTES>& rho) {
        #pragma omp parallel for
        for(size_t i = 0; i < Nm; i++) {
            #pragma omp simd
            for(size_t j = 0; j < Mm; j++) {
                // Uniform based on the Shake128 and the nonce byte
                this->_poly_matrix[i]._poly_vector[j].polynomial_poly_uniform(rho, ((i << 8) + j));
            }
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
     * @status: TESTED
     * @param matrix 
     * @param vector 
     * @return PolyVector<Nm> 
     */
    friend PolyVector<Nm> matrix_multiply(const PolyMatrix<Mm, Nm>& matrix, const PolyVector<Mm>& vector) { 
        PolyVector<Nm> result;
        #pragma omp parallel for simd
        for(size_t i = 0; i < Nm; i++) {
            result.access_poly_at(i) = vector_pointwise_multiply_accumulate(
                matrix._poly_matrix.at(i), vector
            );
        }
        return result;  // You were missing the return statement
    }

    friend std::ostream& operator<< (std::ostream& os, const PolyMatrix<Mm, Nm>& matrix) {
        size_t i = 0;
        for(const auto& vector : matrix._poly_matrix) { 
            std::cout << "\n========Rows[" << i << "]============\n";
            std::cout << vector;
            i++;
        }
        return os;
    }
    void fill(int32_t value) {
        for(PolyVector<Mm> vector : this->_poly_matrix) {
            vector.fill(value);
        }
    }
};


#include "poly_vector.tpp"