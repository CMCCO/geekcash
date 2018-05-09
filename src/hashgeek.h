#ifndef GEEKHASH_H
#define GEEKHASH_H

#include "uint256.h"
#include "crypto/ripemd160.h"
#include "crypto/sha256.h"
#include "crypto/sph_blake.h"
#include "crypto/sph_bmw.h"
#include "crypto/sph_groestl.h"
#include "crypto/sph_keccak.h"
#include "crypto/sph_cubehash.h"
#include "crypto/sph_echo.h"
#include "crypto/sph_hamsi.h"
#include "crypto/sph_simd.h"
//#include "crypto/sph_fugue.h"
#include "crypto/sph_shabal.h"
// #include "crypto/sph_whirlpool.h"
// #include "crypto/sph_sha2.h"
// #include "crypto/sph_echo.h"
// #include "crypto/sph_haval.h"

#ifndef QT_NO_DEBUG
#include <string>
#endif

#ifdef GLOBALDEFINED
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL sph_blake512_context     z_blake;
GLOBAL sph_bmw512_context       z_bmw;
GLOBAL sph_groestl512_context   z_groestl;
GLOBAL sph_keccak512_context    z_keccak;
GLOBAL sph_cubehash512_context  z_cubehash;
GLOBAL sph_echo512_context      z_echo;
GLOBAL sph_hamsi512_context     z_hamsi;
GLOBAL sph_shabal512_context    z_shabal;
// GLOBAL sph_sha512_context       z_sha2;
GLOBAL sph_simd512_context      z_simd;

#define fillz() do { \
    sph_blake512_init(&z_blake); \
    sph_bmw512_init(&z_bmw); \
    sph_groestl512_init(&z_groestl); \
    sph_keccak512_init(&z_keccak); \
    sph_cubehash512_init(&z_cubehash); \
    sph_echo512_init(&z_echo); \
    sph_hamsi512_init(&z_hamsi); \
    sph_shabal512_init(&z_shabal); \
    sph_simd512_init(&z_simd); \
} while (0) 


#define ZBLAKE (memcpy(&ctx_blake, &z_blake, sizeof(z_blake)))
#define ZBMW (memcpy(&ctx_bmw, &z_bmw, sizeof(z_bmw)))
#define ZGROESTL (memcpy(&ctx_groestl, &z_groestl, sizeof(z_groestl)))
#define ZKECCAK (memcpy(&ctx_keccak, &z_keccak, sizeof(z_keccak)))
#define ZHAMSI (memcpy(&ctx_hamsi, &z_hamsi, sizeof(z_hamsi)))
#define ZSHABAL (memcpy(&ctx_shabal, &z_shabal, sizeof(z_shabal)))
// #define ZSHA2 (memcpy(&ctx_sha2, &z_sha2, sizeof(z_sha2)))

template<typename T1>
inline uint256 HashGeek(const T1 pbegin, const T1 pend)
{
    sph_blake512_context      ctx_blake;
    sph_bmw512_context        ctx_bmw;
    sph_groestl512_context    ctx_groestl;    
    sph_keccak512_context     ctx_keccak;    
    sph_cubehash512_context   ctx_cubehash;    
    sph_echo512_context       ctx_echo;
    sph_hamsi512_context      ctx_hamsi;    
    sph_shabal512_context     ctx_shabal;    
    // sph_sha512_context        ctx_sha2;
    sph_simd512_context      ctx_simd;
    
    static unsigned char pblank[1];
    
    uint512 hash[9];

    sph_blake512_init(&ctx_blake);
    sph_blake512 (&ctx_blake, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
    sph_blake512_close(&ctx_blake, static_cast<void*>(&hash[0]));
    
    sph_bmw512_init(&ctx_bmw);
    sph_bmw512 (&ctx_bmw, static_cast<const void*>(&hash[0]), 64);
    sph_bmw512_close(&ctx_bmw, static_cast<void*>(&hash[1]));

    sph_echo512_init(&ctx_echo);
    sph_echo512 (&ctx_echo, static_cast<const void*>(&hash[1]), 64);
    sph_echo512_close(&ctx_echo, static_cast<void*>(&hash[2]));

    sph_shabal512_init(&ctx_shabal);
    sph_shabal512 (&ctx_shabal, static_cast<const void*>(&hash[2]), 64);
    sph_shabal512_close(&ctx_shabal, static_cast<void*>(&hash[3]));

    sph_groestl512_init(&ctx_groestl);
    sph_groestl512 (&ctx_groestl, static_cast<const void*>(&hash[3]), 64);
    sph_groestl512_close(&ctx_groestl, static_cast<void*>(&hash[4]));

    sph_cubehash512_init(&ctx_cubehash);
    sph_cubehash512 (&ctx_cubehash, static_cast<const void*>(&hash[4]), 64);
    sph_cubehash512_close(&ctx_cubehash, static_cast<void*>(&hash[5]));

    sph_keccak512_init(&ctx_keccak);
    sph_keccak512 (&ctx_keccak, static_cast<const void*>(&hash[5]), 64);
    sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[6]));

    sph_hamsi512_init(&ctx_hamsi);
    sph_hamsi512 (&ctx_hamsi, static_cast<const void*>(&hash[6]), 64);
    sph_hamsi512_close(&ctx_hamsi, static_cast<void*>(&hash[7]));
    
    sph_simd512_init(&ctx_simd);
    sph_simd512 (&ctx_simd, static_cast<const void*>(&hash[7]), 64);
    sph_simd512_close(&ctx_simd, static_cast<void*>(&hash[8]));


    return hash[8].trim256();
}

#endif // GEEKHASH_H
