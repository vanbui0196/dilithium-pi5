#include "mldsa.h"

int main() {
    MLDSA data(true);
    data.KeyGen();
    uint8_t SignMessage[5000] = {0};
    std::string Mesage = "Hello World";
    size_t SignMessageLength;
    size_t MessageLength = Mesage.length();

    data.Sign(&SignMessage[0], &SignMessageLength, reinterpret_cast<const uint8_t*>(Mesage.c_str()), MessageLength, NULL, 0, data.secret_key);

}