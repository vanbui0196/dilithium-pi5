#include <vector>
#include <algorithm>
#include <iostream>
#include "ntt.h"
#include "utils.h"

int main() {
  // check for the function of the file is the main thing that we check over
  // chekc on the main 
  auto a = mldsa::utils::montgomery_reduce(64);
  std::cout << a << std::endl;
  
}