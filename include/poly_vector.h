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
     * @brief Add two two Poly together (return by value)
     * 
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
     * @brief Convert the poly vector into the NTT domain (Or Frequency domain)
     * 
     */
    void vector_NTT() {
        for(Polynomial& poly : this->_poly_vector) {
            poly.NTT();
        }
    }

    /**
     * @brief Convert the poly back to Polynomial domain (or time domain)
     * 
     */
    void vector_invNTT() {
        for(Polynomial& poly : this->_poly_vector) {
            poly.invNTT();
        }
    }

    /**
     * @brief Perform the pointwise polynomial in the NTT domain
     * 
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

    friend void vector_pointwise_multiply(PolyVector<Mm>& result,const PolyVector<Mm>& a, const PolyVector<Mm>& b) {
        for(size_t i = 0; i < Mm; i++) {
            result._poly_vector.at(i) = ntt_domain_multiply(a._poly_vector.at(i), b._poly_vector.at(i));
        }
    }

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

    friend void vector_pointwise_multiply_accumulate(Polynomial& result, const PolyVector<Mm>& a, const PolyVector<Mm>& b) {
        Polynomial temp;
        // first initial
        result = ntt_domain_multiply(a._poly_vector.at(0), b._poly_vector.at(0));

        for(size_t i = 1; i < Mm; i++) {
            temp = ntt_domain_multiply(a._poly_vector.at(i), b._poly_vector.at(i));
            result += temp;
        }
    }
    
    void fill(int32_t value) {
        for(auto& poly : this->_poly_vector) {
            poly.fill(value);
        }
    }

    friend std::ostream& operator<< <Mm>(std::ostream& os, const PolyVector<Mm>& poly_vector);
};

template <size_t N, size_t Mm>
class PolyMatrix {
private:
    std::array<PolyVector<Mm>, N> _poly_matrix;

public:
    void expand();
};


#include "poly_vector.tpp"