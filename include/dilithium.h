// #pragma once

// #include <array>
// #include <cstdint>
// #include <vector>
// #include <string>
// #include <memory>
// #include <stdexcept>
// #include "constant.h"
// namespace dilithium {

// // Parameters from params.h
// constexpr int SEEDBYTES = 32;
// constexpr int CRHBYTES = 64;
// constexpr int TRBYTES = 64;
// constexpr int RNDBYTES = 32;
// constexpr int N = 256;
// constexpr int Q = 8380417;
// constexpr int D = 13;
// constexpr int ZETA = 1753;

// // Mode-specific parameters
// enum class Mode {
//     DILITHIUM2 = 2,
//     DILITHIUM3 = 3,
//     DILITHIUM5 = 5
// };

// // Base polynomial class
// class Polynomial {
// public:
//     explicit Polynomial(size_t size = N) : coeffs(size) {}
    
//     std::vector<int32_t>& data() { return coeffs; }
//     const std::vector<int32_t>& data() const { return coeffs; }
    
//     void reduce();
//     void ntt();
//     void invntt_tomont();
    
// private:
//     std::vector<int32_t> coeffs;
// };

// // Key pair structure
// struct KeyPair {
//     std::vector<uint8_t> public_key;
//     std::vector<uint8_t> secret_key;
// };

// // Main Dilithium class
// class Dilithium {
// public:
//     explicit Dilithium(Mode mode);
    
//     // Key generation
//     KeyPair generate_keypair();
    
//     // Signing
//     std::vector<uint8_t> sign(const std::vector<uint8_t>& message,
//                              const std::vector<uint8_t>& secret_key,
//                              const std::vector<uint8_t>& context = {});
    
//     // Verification
//     bool verify(const std::vector<uint8_t>& signature,
//                 const std::vector<uint8_t>& message,
//                 const std::vector<uint8_t>& public_key,
//                 const std::vector<uint8_t>& context = {});

// private:
//     Mode mode_;
    
//     // Internal helper functions
//     std::vector<uint8_t> generate_seed() const;
//     void expand_matrix(std::vector<Polynomial>& matrix, const std::vector<uint8_t>& seed) const;
//     void sample_in_ball(Polynomial& poly, const std::vector<uint8_t>& seed) const;
// };

// // Exception classes
// class DilithiumError : public std::runtime_error {
//     using std::runtime_error::runtime_error;
// };

// class VerificationError : public DilithiumError {
// public:
//     VerificationError() : DilithiumError("Signature verification failed") {}
// };

// } // namespace dilithium