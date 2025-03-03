#include "mldsa.h"

int main() {
    MLDSA data{true};
    std::array<uint8_t, CRYPTO_PUBLICKEYBYTES> public_key; 
    std::array<uint8_t, CRYPTO_SECRETKEYBYTES> secret_key;
    
    data.KeyGen(public_key, secret_key);


    uint8_t SignMessage[5000] = {0};
    std::string Mesage = "Hello World";
    std::string Mesage1 = "Hellu World";
    size_t SignMessageLength;
    size_t MessageLength = Mesage.length();

    data.Sign(&SignMessage[0], &SignMessageLength, reinterpret_cast<const uint8_t*>(Mesage.c_str()), MessageLength, NULL, 0, secret_key);

    for(size_t i = 0; i <SignMessageLength; i++) {
        std::cout << (int)SignMessage[i] << " ";
    } std::cout << std::endl;
    int SIZE_OF_ARRAY = 0;
    

}