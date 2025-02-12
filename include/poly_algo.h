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
    void power2round(Polynomial& lowbits_poly, Polynomial& highbits_poly);
    void decompose(Polynomial& lowbits_poly, Polynomial& highbits_poly);
    uint32_t make_hint(Polynomial& lowbits_poly, Polynomial& highbits_poly, Polynomial& hints_poly);
    Polynomial ntt_domain_multiply(const Polynomial& poly_left, const Polynomial& poly_right);

    // Operator overloading
    Polynomial& operator+=(const Polynomial& poly); // adding 2 polynomials (!!! no Q reduction)
    Polynomial& operator-=(const Polynomial& poly); // substracting 2 polynomials
    Polynomial& operator*=(const Polynomial& poly); // multiplting 2 polynomial in case of the monetegry
    
    // friend operator
    friend Polynomial operator+(Polynomial poly_left, const Polynomial& poly_right);
    friend Polynomial operator-(Polynomial poly_left, const Polynomial& poly_right);
    friend Polynomial operator*(Polynomial poly_left, const Polynomial& poly_right);

    // test feature
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& poly);
};
