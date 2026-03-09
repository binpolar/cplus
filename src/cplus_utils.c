#include "cplus_utils.h"

// Simple 32-bit hash function (MurmurHash3 finalizer style)
uint32_t cplus_hash_func(uint32_t input)
{
    uint32_t h = input;
    
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    
    return h;
}