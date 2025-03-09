#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstring>
#include <chrono> // For time measurement
#include "utils.h"
#include "mldsa.h"

extern "C" {
  #include "fips202.h"
}

// Convert binary data to hex string
std::string to_hex_string(const uint8_t* data, size_t length) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    
    for (size_t i = 0; i < length; i++) {
        ss << std::setw(2) << static_cast<int>(data[i]);
    }
    
    return ss.str();
}

// Convert hex string to binary data
std::vector<uint8_t> from_hex_string(const std::string& hex) {
    std::vector<uint8_t> bytes;
    
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byte_str = hex.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(std::stoi(byte_str, nullptr, 16));
        bytes.push_back(byte);
    }
    
    return bytes;
}

// Read file contents into a string
std::string read_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void print_usage() {
    std::cout << "Usage: mldsa [OPTIONS]\n\n"
              << "Options:\n"
              << "  -g, --keygen                   Generation of the MLDSA keypair\n"
              << "  -s, --sign                     Sign a message with ML-DSA\n"
              << "      --message=FILE             Message file to sign or verify\n"
              << "      --sk=FILE                  Private key file (in hex format) for signing\n"
              << "      --output=FILE              Output file for signature (default: signature.txt)\n"
              << "  -v, --verify                   Verify a signature with ML-DSA\n"
              << "      --pk=FILE                  Public key file (in hex format) for verification\n"
              << "      --signature=FILE           Signature file (in hex format) to verify\n"
              << "  -t, --time                     Measure and display execution time in microseconds\n"
              << "  -h, --help                     Show this help message\n"
              << std::endl;
}

void generate_keypair(bool measure_time) {
    std::cout << "Generating ML-DSA keypair..." << std::endl;
    
    // Create ML-DSA object and generate keys
    MLDSA mldsa;
    std::array<uint8_t, CRYPTO_PUBLICKEYBYTES> public_key;
    std::array<uint8_t, CRYPTO_SECRETKEYBYTES> secret_key;
    
    // Start time measurement if requested
    auto start_time = std::chrono::high_resolution_clock::now();
    
    mldsa.KeyGen(public_key, secret_key);
    
    // End time measurement if requested
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    
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
    
    // Print execution time if requested
    if (measure_time) {
        std::cout << "Execution time: " << duration.count() << " us" << std::endl;
    }
}

void sign_message(const std::string& message_file, const std::string& sk_file, const std::string& output_file, bool measure_time) {
    std::cout << "Signing message from " << message_file << " with key from " << sk_file << "..." << std::endl;
    
    try {
        // Read message from file
        std::string message = read_file(message_file);
        
        // Read private key from file
        std::string hex_sk = read_file(sk_file);
        std::vector<uint8_t> sk_bytes = from_hex_string(hex_sk);
        
        // Ensure the key has the correct size
        if (sk_bytes.size() != CRYPTO_SECRETKEYBYTES) {
            std::cerr << "Error: Invalid private key size. Expected " << CRYPTO_SECRETKEYBYTES 
                      << " bytes, got " << sk_bytes.size() << " bytes." << std::endl;
            return;
        }
        
        // Create an array to hold the private key
        std::array<uint8_t, CRYPTO_SECRETKEYBYTES> secret_key;
        std::copy_n(sk_bytes.begin(), CRYPTO_SECRETKEYBYTES, secret_key.begin());
        
        // Prepare buffer for signature
        std::vector<uint8_t> signature(CRYPTO_BYTES);
        size_t sig_len;
        
        // Create ML-DSA object
        MLDSA mldsa;
        
        // Start time measurement if requested
        auto start_time = std::chrono::high_resolution_clock::now();
        
        // Sign the message
        int result = mldsa.Sign(
            signature.data(), &sig_len,
            reinterpret_cast<const uint8_t*>(message.data()), message.size(),
            nullptr, 0, secret_key
        );
        
        // End time measurement if requested
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        
        if (result != 0) {
            std::cerr << "Error: Signature generation failed with code " << result << std::endl;
            return;
        }
        
        // Resize signature to actual length
        signature.resize(sig_len);
        
        // Convert signature to hex string
        std::string hex_signature = to_hex_string(signature.data(), signature.size());
        
        // Save signature to file
        std::ofstream sig_file(output_file);
        sig_file << hex_signature;
        sig_file.close();
        
        std::cout << "Signature saved to " << output_file << std::endl;
        std::cout << "Signature size: " << sig_len << " bytes" << std::endl;
        
        // Print execution time if requested
        if (measure_time) {
            std::cout << "Execution time: " << duration.count() << " us" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void verify_signature(const std::string& message_file, const std::string& pk_file, const std::string& signature_file, bool measure_time) {

    // Log out the message
    std::cout << "Verifying signature from " << signature_file << " for message " 
              << message_file << " with public key from " << pk_file << "..." << std::endl;
    
    try {
        // Read message from file
        std::string message = read_file(message_file);
        
        // Read public key from file
        std::string hex_pk = read_file(pk_file);
        std::vector<uint8_t> pk_bytes = from_hex_string(hex_pk);
        
        // Ensure the public key has the correct size
        if (pk_bytes.size() != CRYPTO_PUBLICKEYBYTES) {
            std::cerr << "Error: Invalid public key size. Expected " << CRYPTO_PUBLICKEYBYTES 
                      << " bytes, got " << pk_bytes.size() << " bytes." << std::endl;
            return;
        }
        
        // Read signature from file
        std::string hex_signature = read_file(signature_file);
        std::vector<uint8_t> signature = from_hex_string(hex_signature);
        
        // Create an array to hold the public key
        std::array<uint8_t, CRYPTO_PUBLICKEYBYTES> public_key;
        std::copy_n(pk_bytes.begin(), CRYPTO_PUBLICKEYBYTES, public_key.begin());
        
        // Create ML-DSA object
        MLDSA mldsa;
        
        // Start time measurement if requested
        auto start_time = std::chrono::high_resolution_clock::now();
        
        // Verify the signature
        int result = mldsa.Verify(
            signature.data(), signature.size(),
            reinterpret_cast<const uint8_t*>(message.data()), message.size(),
            nullptr, 0, public_key
        );
        
        // End time measurement if requested
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        
        if (result == 0) {
            std::cout << "Verification SUCCESSFUL! The signature is valid." << std::endl;
        } else {
            std::cout << "Verification FAILED! The signature is invalid." << std::endl;
        }
        
        // Print execution time if requested
        if (measure_time) {
            std::cout << "Execution time: " << duration.count() << " us" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// Helper function to extract the value from arguments like --key=value
std::string get_option_value(const std::string& option) {
    size_t pos = option.find('=');
    if (pos != std::string::npos && pos + 1 < option.length()) {
        return option.substr(pos + 1);
    }
    return "";
}

int main(int argc, char* argv[]) {
    // If no arguments provided, show usage
    if (argc < 2) {
        print_usage();
        return 1;
    }
    
    // Variables for command options
    bool sign_mode = false;
    bool verify_mode = false;
    bool measure_time = false;
    std::string message_file;
    std::string sk_file;
    std::string pk_file;
    std::string signature_file;
    std::string output_file = "signature.txt";
    
    // Process command-line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-g" || arg == "--keygen") {
            generate_keypair(measure_time);
            return 0;
        }
        else if (arg == "-s" || arg == "--sign") {
            sign_mode = true;
        }
        else if (arg == "-v" || arg == "--verify") {
            verify_mode = true;
        }
        else if (arg == "-t" || arg == "--time") {
            measure_time = true;
        }
        else if (arg.find("--message=") == 0) {
            message_file = get_option_value(arg);
        }
        else if (arg.find("--sk=") == 0) {
            sk_file = get_option_value(arg);
        }
        else if (arg.find("--pk=") == 0) {
            pk_file = get_option_value(arg);
        }
        else if (arg.find("--signature=") == 0) {
            signature_file = get_option_value(arg);
        }
        else if (arg.find("--output=") == 0) {
            output_file = get_option_value(arg);
        }
        else if (arg == "-h" || arg == "--help") {
            print_usage();
            return 0;
        }
        else {
            std::cerr << "Unknown option: " << arg << std::endl;
            print_usage();
            return 1;
        }
    }
    
    // Handle sign mode
    if (sign_mode) {
        if (message_file.empty()) {
            std::cerr << "Error: Message file not specified. Use --message=FILE" << std::endl;
            return 1;
        }
        if (sk_file.empty()) {
            std::cerr << "Error: Private key file not specified. Use --sk=FILE" << std::endl;
            return 1;
        }
        
        sign_message(message_file, sk_file, output_file, measure_time);
        return 0;
    }
    
    // Handle verify mode
    if (verify_mode) {
        if (message_file.empty()) {
            std::cerr << "Error: Message file not specified. Use --message=FILE" << std::endl;
            return 1;
        }
        if (pk_file.empty()) {
            std::cerr << "Error: Public key file not specified. Use --pk=FILE" << std::endl;
            return 1;
        }
        if (signature_file.empty()) {
            std::cerr << "Error: Signature file not specified. Use --signature=FILE" << std::endl;
            return 1;
        }
        
        verify_signature(message_file, pk_file, signature_file, measure_time);
        return 0;
    }
    
    // No valid mode is current applicable for the message
    print_usage();
    return 1;
}