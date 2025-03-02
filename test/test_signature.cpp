#include "mldsa.h"

int main() {
    MLDSA data(true);
    data.KeyGen();
    uint8_t SignMessage[5000] = {0};
    std::string Mesage = "Hello World";
    std::string Mesage1 = "Hellu World";
    size_t SignMessageLength;
    size_t MessageLength = Mesage.length();

    data.Sign(&SignMessage[0], &SignMessageLength, reinterpret_cast<const uint8_t*>(Mesage.c_str()), MessageLength, NULL, 0, data.secret_key);

    // for(size_t i = 0; i <SignMessageLength; i++) {
    //     std::cout << (int)SignMessage[i] << " ";
    // } std::cout << std::endl;

    std::array<uint8_t, CTILDEBYTES> sample_in_ball_seed ={0}; 
    PolyVector<L> z; 
    PolyVector<K> h;
    std::cout <<  data.Verify(SignMessage,SignMessageLength,reinterpret_cast<const uint8_t*>(Mesage1.c_str()),MessageLength,NULL,0,data.public_key); 
    std::cout << std::endl;
}