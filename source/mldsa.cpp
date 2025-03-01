#include "mldsa.h"


MLDSA::MLDSA() : test_mode{false} {
}

MLDSA::MLDSA(bool mode) : test_mode{mode} {
}

void MLDSA::KeyGen() {

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


    if(true == this->test_mode) {
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

    // for(auto i = 0; i < rho.size(); i++) {
    //     std::cout << (int)rho.at(i);
    // } std::cout << std::endl;
    // for(auto i = 0; i < tr.size(); i++) {
    //     std::cout << (int)tr.at(i);
    // } std::cout << std::endl;
    // for(auto i = 0; i < key.size(); i++) {
    //     std::cout << (int)key.at(i);
    // } std::cout << std::endl;
    // std::cout << "Vector s1: \n" << this->_s1;
    // std::cout << "Vector s2: \n" << this->_s2;
    // std::cout << "Vector t0: \n" << this->_t0;

}

/**
 * @brief Encode the public key
 * TESTED
 * @param rho 
 */
void MLDSA::pkEncode(const std::array<uint8_t, SEEDBYTES>& rho) {
    
    // Copy rho into the public key
    for(size_t i = 0; i < SEEDBYTES; i++) {
        this->public_key[i] = rho.at(i);
    }

    // pack the vector to the buffer
    this->_t1.vector_packt1(this->public_key.data() + SEEDBYTES);
}

/**
 * @brief Encode secret key
 * TESTED
 * @param rho Seed
 * @param tr Hash public key
 * @param key 
 */
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

/**
 * @brief Decode SK
 * TESTED
 * @param rho rho for Matrix
 * @param tr hash of PK
 * @param key 
 * @param t0 t0 vector
 * @param s1 secret key
 * @param s2 secret key
 * @param secret_key array of secret key
 */
void MLDSA::skDecode(std::array<uint8_t,SEEDBYTES>& rho, std::array<uint8_t, TRBYTES>& tr, std::array<uint8_t,SEEDBYTES>& key,
              PolyVector<K>& t0, PolyVector<L>& s1, PolyVector<K>& s2, const std::array<uint8_t, CRYPTO_SECRETKEYBYTES>& secret_key) 
{
    // Position tracking variable
    size_t tracking_pos{0};

    // Copy data from secrekey in to the rho
    for(size_t i = 0; i < SEEDBYTES; i++) {
        rho.at(i) = secret_key.at(tracking_pos + i);
    }
    tracking_pos += SEEDBYTES;

    // Copy data from secrekey in to the KEY
    for(size_t i = 0; i < SEEDBYTES; i++) {
        key.at(i) = secret_key.at(tracking_pos + i);
    }
    tracking_pos += SEEDBYTES;

    // Copy data from secrekey in to the TR
    for(size_t i = 0; i < TRBYTES; i++) {
        tr.at(i) = secret_key.at(tracking_pos + i);
    }
    tracking_pos += TRBYTES;

    // Unpack the data in to s1 vector
    s1.vector_unpacketa(secret_key.data() + tracking_pos);
    tracking_pos += L*POLYETA_PACKEDBYTES;

    // Upack the data into s2 vector
    s2.vector_unpacketa(secret_key.data() + tracking_pos);
    tracking_pos += K*POLYETA_PACKEDBYTES;

    //Unpack the data into t0 vector
    t0.vector_unpackt0(secret_key.data() + tracking_pos);
    tracking_pos += K * POLYT0_PACKEDBYTES; // not needed, just for the fullfilment

    // for(auto i = 0; i < rho.size(); i++) {
    //     std::cout << (int)rho.at(i);
    // } std::cout << std::endl;
    // for(auto i = 0; i < tr.size(); i++) {
    //     std::cout << (int)tr.at(i);
    // } std::cout << std::endl;
    // for(auto i = 0; i < key.size(); i++) {
    //     std::cout << (int)key.at(i);
    // } std::cout << std::endl;
    // std::cout << "Vector s1: \n" << this->_s1;
    // std::cout << "Vector s2: \n" << this->_s2;
    // std::cout << "Vector t0: \n" << this->_t0;
}


int MLDSA::Sign(uint8_t* SignMessage, size_t* SignMessageLength,
    const uint8_t* Mesage, size_t MessageLength,
    const uint8_t* ctx, size_t ctxlen, const std::array<uint8_t, CRYPTO_SECRETKEYBYTES>& secret_key) 
{
    uint8_t Pre[257] = {0};

    // Error case
    if(ctxlen > 255) {
        return -1;
    }

    // CTX handling
    Pre[0] = 0; Pre[1] = ctxlen;
    for(size_t i = 0; i < ctxlen; i++)
    {
        Pre[2 + i] = ctx[i];
    }

    // Handling the buffer mode
    std::array<uint8_t, RNDBYTES> random_buffer = {0};

    if(this->test_mode == false) {
        mldsa::utils::get_random_bytes(random_buffer.data(), RNDBYTES);
    }

    // Call the internal method for handling the Signing
    SignInternal(SignMessage, SignMessageLength, Mesage, MessageLength, Pre, ctxlen + 2, random_buffer, secret_key);

    return 0;
}

void MLDSA::SignInternal(uint8_t* SignMessage, size_t* SignMessageLength,
    const uint8_t* Mesage, size_t MessageLength,
    const uint8_t* Pre, size_t PreLenth, const std::array<uint8_t, RNDBYTES> randombuf, 
    const std::array<uint8_t, CRYPTO_SECRETKEYBYTES>& secret_key) 
{
    // Local variable (unpack key)
    std::array<uint8_t,SEEDBYTES> rho = {0}; 
    std::array<uint8_t, TRBYTES> tr = {0}; 
    std::array<uint8_t,SEEDBYTES> key = {0};
    PolyVector<K> t0; 
    PolyVector<L> s1; 
    PolyVector<K> s2;

    // Local variable (Seed of Shake)
    std::array<uint8_t, CRHBYTES> mu{0};        // µ
    std::array<uint8_t, CRHBYTES> rhoprime{0};  // ϱ"

    // Local variable (sample in ball)
    Polynomial challengePoly;                   // c̃

    // Local variable (shake)
    keccak_state hashState;

    // Decode the secret key
    this->skDecode(rho,tr,key,t0,s1,s2,secret_key);

    /* Compute mu */
    shake256_init(&hashState);
    shake256_absorb(&hashState, tr.data(), TRBYTES);
    shake256_absorb(&hashState, Pre, PreLenth);
    shake256_absorb(&hashState, Mesage, MessageLength);
    shake256_finalize(&hashState);
    shake256_squeeze(mu.data(), CRHBYTES, &hashState);

    // Line 7 of the FIPS204
    shake256_squeeze(rhoprime.data(), CRHBYTES, &hashState);
}