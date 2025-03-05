#pragma once
#ifndef CONFIG_H
#define CONFIG_H

// Default mode if not specified
#ifndef DILITHIUM_MODE
#define DILITHIUM_MODE 5
#endif


// Configure namespace and algorithm name based on mode
#if DILITHIUM_MODE == 2
    #define CRYPTO_ALGNAME "ML-DSA44"
#elif DILITHIUM_MODE == 3
    #define CRYPTO_ALGNAME "ML-DSA65" 
#elif DILITHIUM_MODE == 5
    #define CRYPTO_ALGNAME "ML-DSA87"
#endif

#endif // CONFIG_H