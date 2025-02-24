/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-02-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <vector>
#include <algorithm>
#include <iostream>
#include "utils.h"

extern "C" {
  #include "fips202.h"
}

int main() {
  // check for the function of the file is the main thing that we check over
  // chekc on the main 
  keccak_state state;
  shake128_init(&state);
  
}