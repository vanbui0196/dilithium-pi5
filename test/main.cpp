 #include <vector>
 #include <algorithm>
 #include <iostream>
 #include <fstream>
 #include <iomanip>
 #include <sstream>
 #include <string>
 #include <cstring>
 #include "utils.h"
 #include "mldsa.h"
 
 // Convert binary data to hex string
 std::string to_hex_string(const uint8_t* data, size_t length) {
     std::stringstream ss;
     ss << std::hex << std::setfill('0');
     
     for (size_t i = 0; i < length; i++) {
         ss << std::setw(2) << static_cast<int>(data[i]);
     }
     
     return ss.str();
 }
 
 void print_usage() {
     std::cout << "Usage: mldsa [OPTIONS]\n\n"
               << "Options:\n"
               << "  -g, --keygen    Generation of  ML-DSA keypair\n"
               << "  -h, --help      Help\n"
               << std::endl;
 }
 
 void generate_keypair() {
     std::cout << "Generating ML-DSA keypair..." << std::endl;
     
     // Create ML-DSA object and generate keys
     MLDSA mldsa;
     std::array<uint8_t, CRYPTO_PUBLICKEYBYTES> public_key;
     std::array<uint8_t, CRYPTO_SECRETKEYBYTES> secret_key;
     
     mldsa.KeyGen(public_key, secret_key);
     
     // Convert the binary keys to hex strings
     std::string hex_public = to_hex_string(public_key.data(), public_key.size());
     std::string hex_private = to_hex_string(secret_key.data(), secret_key.size());
     
     // Output the keys
     std::cout << "Public key size: " << public_key.size() << " bytes" << std::endl;
     std::cout << "Private key size: " << secret_key.size() << " bytes" << std::endl;
     
     // Save the keys to files
     std::ofstream public_file("mldsa_public.txt");
     public_file << hex_public;
     public_file.close();
     
     std::ofstream private_file("mldsa_private.txt");
     private_file << hex_private;
     private_file.close();
     
     std::cout << "Keys have been saved in hex format to 'mldsa_public.txt' and 'mldsa_private.txt'" << std::endl;
 }
 
 int main(int argc, char* argv[]) {
     // If no arguments provided, show usage
     if (argc < 2) {
         print_usage();
         return 1;
     }
     
     // Process command-line arguments
     for (int i = 1; i < argc; i++) {
         if (strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--keygen") == 0) {
             generate_keypair();
             return 0;
         }
         else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
             print_usage();
             return 0;
         }
         else {
             std::cerr << "Unknown option: " << argv[i] << std::endl;
             print_usage();
             return 1;
         }
     }
     
     return 0;
 }