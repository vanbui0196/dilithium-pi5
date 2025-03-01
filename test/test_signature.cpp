#include "mldsa.h"

int main() {
    MLDSA data(true);
    data.KeyGen();
    std::array<uint8_t,SEEDBYTES> rho; 
    std::array<uint8_t, TRBYTES> tr; 
    std::array<uint8_t,SEEDBYTES> key;
    PolyVector<K> t0; 
    PolyVector<L> s1; 
    PolyVector<K> s2;
    data.skDecode(rho,tr,key, t0 ,s1,s2, data.secret_key);
}