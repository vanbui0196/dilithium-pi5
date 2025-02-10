#pragma once
#include "config.h"
#include "constant.h"
#include <cstdint>
#include <vector>
#include <array>
#include <utility>


namespace mldsa {
namespace utils {
    int32_t montgomery_reduce(int64_t a);
    int32_t reduce32(int32_t a);
    int32_t caddq(int32_t a);
    int32_t freeze(int32_t a);
}
}