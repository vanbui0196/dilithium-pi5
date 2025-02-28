#include "mldsa.h"


MLDSA::MLDSA() {
    

}

void MLDSA::KeyGen(bool testmode) {

    // array for get random value
    std::array<uint8_t, SEEDBYTES + CRHBYTES + SEEDBYTES> seedbuf; // buffer for sheed byte
    std::array<uint8_t, TRBYTES> tr; // buffer for the tr part
    std::array<uint8_t, SEEDBYTES> rho;
    std::array<uint8_t, CRHBYTES> rhoprime;
    std::array<uint8_t, SEEDBYTES> key;

    // initalize the variable to zero
    seedbuf.fill(0);
    tr.fill(0);
    rho.fill(0);
    rhoprime.fill(0);
    key.fill(0);


    if(true == testmode) {
        for(size_t i = 0; i < SEEDBYTES; i++) {
            seedbuf.at(i) = i;
        }
    }
    else 
    {
        // Use kernel to get the random values
        bool retVal = false;
        do {
            retVal = mldsa::utils::get_random_bytes(seedbuf.data(), SEEDBYTES);
        } while(retVal == false);
    }

    seedbuf[SEEDBYTES+0] = K;
    seedbuf[SEEDBYTES+1] = L;
    shake256(seedbuf.data(), 2*SEEDBYTES + CRHBYTES, seedbuf.data(), SEEDBYTES+2);

    // Copy the array
    std::copy(seedbuf.begin(), seedbuf.begin() + SEEDBYTES, rho.begin());
    std::copy(seedbuf.begin() + SEEDBYTES, seedbuf.begin() + SEEDBYTES + CRHBYTES, rhoprime.begin());
    std::copy(seedbuf.begin() + SEEDBYTES + CRHBYTES, seedbuf.begin() + SEEDBYTES + CRHBYTES + SEEDBYTES, key.begin());

    // smaple the matrix A based on the rho -> having A -> Â actually
    this->_A.expand(rho); //tested
    
    // sample the vector s1 and s2 -> having s1, s2
    this->_s1.vector_uniform_eta(rhoprime, 0); // tested
    this->_s2.vector_uniform_eta(rhoprime, L); //tested

    PolyVector<L> s1_hat = this->_s1;
    s1_hat.vector_NTT();
    this->_t = matrix_multiply(this->_A, s1_hat); //-> t = A * s1
    this->_t.vector_reduced();
    this->_t.vector_invNTT(); // tested

    this->_t = this->_t + this->_s2; //-> t = A*s1 + s2

    this->_t.vector_caddq(); // -> adding the value to the coefficient in case of negative

    // Extract highbit and low bit part
    this->_t.vector_power2round(this->_t1, this->_t0); // -> decompose the vector (t1, t0) -> tested

    // encode the public key
    this->pkEncode(rho); // tested -> pk

    // compute the tr for the private key
    shake256(tr.data(), TRBYTES, this->public_key.data(), CRYPTO_PUBLICKEYBYTES); // tested

    // pack the secret key
    this->skEncode(rho, tr, key); // tested

}


void MLDSA::pkEncode(const std::array<uint8_t, SEEDBYTES>& rho) {
    
    // Copy rho into the public key
    for(size_t i = 0; i < SEEDBYTES; i++) {
        this->public_key[i] = rho.at(i);
    }

    // pack the vector to the buffer
    this->_t1.vector_packt1(this->public_key.data() + SEEDBYTES);
}

void MLDSA::skEncode(const std::array<uint8_t, SEEDBYTES>& rho, 
    const std::array<uint8_t, TRBYTES>& tr, 
    const std::array<uint8_t, SEEDBYTES>& key) 
{
    size_t adding_pos{0};
    // copy the data from rho in to secret key
    std::copy(rho.begin(),rho.end(), this->secret_key.begin() + adding_pos);
    adding_pos += SEEDBYTES;

    // copy the data from key in secret key buffer
    std::copy(key.begin(),key.end(), this->secret_key.begin() + adding_pos);
    adding_pos+= SEEDBYTES;

    // copy the data from tr (hash of the public key) in to secret key
    std::copy(tr.begin(),tr.end(), this->secret_key.begin() + adding_pos);
    adding_pos+= TRBYTES;

    // pack s1 -> secret key buffer (L size)
    this->_s1.vector_packeta(this->secret_key.begin() + adding_pos);
    adding_pos += L * POLYETA_PACKEDBYTES;

    // pack s2 -> secret key buffer (K size)
    this->_s2.vector_packeta(this->secret_key.begin() + adding_pos);
    adding_pos += K * POLYETA_PACKEDBYTES;

    // pack t0 into buffer
    this->_t0.vector_packt0(this->secret_key.begin() + adding_pos);
    adding_pos += K * POLYT0_PACKEDBYTES; // this does not has any purpose

}