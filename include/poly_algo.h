#pragma once
#include <vector>
#include <array>
#include <cstdint>
#include "utils.h"
#include "stream.h"
#include "constant.h"

class Polynomial {
private:
    std::array<int32_t, N> _coeffs; // coefficient of the data
    bool ntt_status;   // current status of the ntt transform of polynomial
    
public:
    // Constructor 
    explicit Polynomial();

    // Set and the get method for handling the data
    int32_t get_value(size_t index);
    void set_value(size_t index, int32_t value);

    void ntt_transform();
    void intt_transform();
    void reduced();
    void negative_add_Q();
};
