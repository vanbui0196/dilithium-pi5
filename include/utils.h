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
    std::pair<int32_t, int32_t> power2round(int32_t a);
    std::pair<int32_t, int32_t> decompose(int32_t a);
    uint32_t make_hint(int32_t a1, int32_t a0);
    int32_t use_hint(int32_t a, uint32_t hint);
}
}