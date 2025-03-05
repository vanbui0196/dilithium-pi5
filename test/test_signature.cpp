#include "mldsa.h"
#include <chrono>
int main() {
    MLDSA data{true};
    std::array<uint8_t, CRYPTO_PUBLICKEYBYTES> public_key; 
    std::array<uint8_t, CRYPTO_SECRETKEYBYTES> secret_key;


    // Start timing
    // auto start = std::chrono::high_resolution_clock::now();
    // data.KeyGen(public_key, secret_key);
    // auto end = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> diff = end - start;
    // //std::cout << "Keygen time taken: " << diff.count() << " seconds" << "\n";
    // std::cout <<diff.count() << ",";
    // uint8_t SignMessage[5000] = {0};
    // std::string Mesage = "Hello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello World";
    // std::string Mesage1 = "Hello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello WorldHello World";
    // size_t SignMessageLength;
    // size_t MessageLength = Mesage.length();

    // start = std::chrono::high_resolution_clock::now();
    // data.Sign(&SignMessage[0], &SignMessageLength, reinterpret_cast<const uint8_t*>(Mesage.c_str()), MessageLength, NULL, 0, secret_key);
    // end = std::chrono::high_resolution_clock::now();
    // diff = end - start;
    // //std::cout << "Signing time taken: " << diff.count() << " seconds" << "\n";
    // std::cout <<diff.count() << ",";
    // start = std::chrono::high_resolution_clock::now();
    // data.Verify(SignMessage,SignMessageLength,reinterpret_cast<const uint8_t*>(Mesage1.c_str()),MessageLength,NULL,0,public_key); 
    // //std::cout << "Signing time taken: " << diff.count() << " seconds" << "\n";
    // std::cout <<diff.count() << "\n";


    std::string Mesage = "This is the string";
    data.KeyGen(public_key, secret_key);
    size_t SignMessageLength;
    uint8_t SignMessage[CRYPTO_BYTES] = {0};
    size_t MessageLength = Mesage.length();
    data.Sign(&SignMessage[0], &SignMessageLength, reinterpret_cast<const uint8_t*>(Mesage.c_str()), MessageLength, NULL, 0, secret_key);

    for(auto i = 0; i < CRYPTO_BYTES; i++) {
        std::cout << (int)SignMessage[i] << " ";
    }
    std::cout << "\n";
    return 0;

}