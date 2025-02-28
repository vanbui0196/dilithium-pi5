#pragma once
#include <vector>
#include <array>
#include <cstdint>
#include <iostream>
#include "utils.h"
#include "stream.h"
#include "constant.h"
#include <stdexcept>

class Polynomial {
private:
    std::array<int32_t, N> _coeffs; // coefficient of the data
    bool ntt_status;   // current status of the ntt transform of polynomial
    uint32_t _coefficient_from_3bytes(int32_t* coeff, uint32_t length, const uint8_t* buf, uint32_t buflen);
    uint32_t _coefficient_from_halfbyte(int32_t* coeff, uint32_t length, const uint8_t* buf, uint32_t buflen);
    
    // constant for handling the buffer block of the stream function
    static constexpr uint32_t POLY_UNIFORM_NBLOCKS = 
    ((768 + mldsa::stream_function::STREAM128_BLOCKBYTES - 1)/mldsa::stream_function::STREAM128_BLOCKBYTES);

    static constexpr uint32_t POLY_UNIFORM_ETA_NBLOCKS = 
    (ETA == 2) ? ((136 + mldsa::stream_function::STREAM256_BLOCKBYTES - 1)/ mldsa::stream_function::STREAM256_BLOCKBYTES) :
    ((227 + mldsa::stream_function::STREAM256_BLOCKBYTES - 1)/ mldsa::stream_function::STREAM256_BLOCKBYTES);

    static constexpr uint32_t POLY_UNIFORM_GAMMA1_NBLOCKS = 
        ((POLYZ_PACKEDBYTES + mldsa::stream_function::STREAM256_BLOCKBYTES - 1)/mldsa::stream_function::STREAM256_BLOCKBYTES);
    
public:
    // Constructor 
    explicit Polynomial();
    bool get_ntt_status(void);

    // Set and the get method for handling the data
    int32_t get_value(size_t index);
    void set_value(size_t index, int32_t value);
    void fill(int32_t value);

    // Finite field operator
    void NTT(void);
    void invNTT(void);
    void reduced(void);
    void negative_add_Q(void);
    void shift_toleft_2D(void);
    void poly_add(const Polynomial& poly);
    // decompose funtion
    void power2round(Polynomial& lowbits_poly, Polynomial& highbits_poly);
    void decompose(Polynomial& lowbits_poly, Polynomial& highbits_poly);

    //hint related methods
    uint32_t make_hint(const Polynomial& lowbits_poly,const Polynomial& highbits_poly);
    void use_hint(const Polynomial& highbits_poly, const Polynomial& hints_poly);
    //Polynomial ntt_domain_multiply(const Polynomial& poly_left, const Polynomial& poly_right); -> implement with the friend function
    bool norm_check(int32_t bound) const;

    // polynomial uniform (from SHAKE function)
    void polynomial_poly_uniform(const std::array<uint8_t, SEEDBYTES>& seed, uint16_t nonce);
    void polynomial_uniform_eta(const std::array<uint8_t, CRHBYTES>& seed, uint16_t nonce);
    void polynomial_uniform_gamma1(const std::array<uint8_t, CRHBYTES>& seed, uint16_t nonce);
    void polynomial_sample_in_ball(const std::array<uint8_t, CTILDEBYTES>& seed);

    // pack + unpack function
    void polyeta_pack(uint8_t* buf);
    void polyt1_pack(uint8_t* buf); // not tested
    void polyt0_pack(uint8_t* buf); // not tested
    void polyz_pack(uint8_t* buf);  // not tested
    void polyw1_pack(uint8_t* buf); // not tested


    void polyeta_unpack(const uint8_t* a);
    void polyt1_unpack(const uint8_t* a); // not tested
    void polyt0_unpack(const uint8_t* a); // not tested
    void polyz_unpack(const uint8_t *buf);



    // Operator overloading
    Polynomial& operator+=(const Polynomial& poly); // adding 2 polynomials (!!! no Q reduction)
    Polynomial& operator-=(const Polynomial& poly); // substracting 2 polynomials
    Polynomial& operator*=(const Polynomial& poly); // multiplting 2 polynomial in case of the monetegry

    // copy constructor
    Polynomial(const Polynomial& other); // Copy constructor
    Polynomial& operator=(const Polynomial& other); // Copy assignment operator
    
    // friend operator
    friend Polynomial operator+(const Polynomial& poly_left, const Polynomial& poly_right);
    friend Polynomial operator-(const Polynomial& poly_left, const Polynomial& poly_right);
    friend Polynomial operator*(Polynomial poly_left, const Polynomial& poly_right);

    //friend hint function
    friend uint32_t poly_make_hint(Polynomial& hints_poly, const Polynomial& lowbits_poly,const Polynomial& highbits_poly);
    friend void poly_use_hint(Polynomial& corrected_poly, const Polynomial& highbits_poly, const Polynomial& hints_poly);
    // test feature
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& poly);
    friend Polynomial ntt_domain_multiply(const Polynomial& poly_left, const Polynomial& poly_right);
};
