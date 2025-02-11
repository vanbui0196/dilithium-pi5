#include "poly_algo.h"

/**
 * @brief Construct a new Polynomial:: Polynomial object
 * 
 */
Polynomial::Polynomial() : ntt_status{false} {
    // No feature at the moment
    // Only init the status of the ntt transform to false
}

/**
 * @brief Get the value at the position of the polynomial
 * 
 * @param index Index value
 * @return int32_t The coefficient of the polynomials
 */
int32_t Polynomial::get_value(size_t index) {
    return this->_coeffs.at(index);
}

/**
 * @brief Set the value at the index of the coefficient
 * 
 * @param index Index of coefficient 0 < index < 255
 * @param value Value for the index
 */
void Polynomial::set_value(size_t index, int32_t value) {
    this->_coeffs.at(index) = value;
}



void Polynomial::reduced() {
    for(auto element : this->_coeffs) {
        element = mldsa::utils::reduce32(element);
    }
}