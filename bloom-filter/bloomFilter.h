#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <cstdint>
#include <string>
#include <iostream>
#include <random>
#include <cstdlib>
#include <cmath>

using namespace std;

/**
 * Structure for function params used for hash functions
 */
struct FunctionParams {
    int32_t a, b, c;
    FunctionParams(int32_t a, int32_t b, int32_t c) : a(a), b(b), c(c) {}
};

/**
 * Bloom Filter: space-efficient probabilistic hashing data structure. False positive matches are possible.
 */
class BloomFilter {
    
    /**
     * Array of bool with already used hashes set as true
     */
    bool * bitArray;
    
    /**
     * Size of bitArray
     */
    int32_t size;
    
    /**
     * Array of FunctionParams used for hash functions
     */
    FunctionParams ** functionParams;
    
    /**
     * Size of functionParams array
     */
    int32_t functionCount;
    
    /**
     * Modulus used in hash functions (should be prime)
     */
    int32_t p;

public:

    /**
     * @param size size of bitArray
     * @param functionCount number of functions to use for hashing
     * @param p modulus of hash functions
     */
    BloomFilter(int32_t size, int32_t functionCount, int32_t p);

    ~BloomFilter();

    /**
     * Adds text to data structure
     * Sets functionCount number of bits in array to true (index are calculated with hash functions)
     * @param text
     */
    void add(const string & text);

    /**
     * @param text string to find in the data structure
     * @return false if the text is not inside, true if there is high chance it is
     */
    bool find(const string & text);

    /**
     * Calculates hash according to given hash function params, uses poly-mod-prime function
     * @param text string to hash
     * @param functionParam params of function
     * @return hash value
     */
    int32_t hash(const string & text, FunctionParams * functionParam);

private:

    /**
     * Generates params of given number ofs hash function from uniform distribution (randomly)
     */
    void setFunctions();
};

#endif
