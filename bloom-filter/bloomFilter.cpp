#include "bloomFilter.h"

BloomFilter::BloomFilter(int32_t size, int32_t functionCount, int32_t p): size(size), functionCount(functionCount), p(p) {
    bitArray = new bool[size];

    for(int32_t i = 0; i < size; i++)
       bitArray[i] = false;

    setFunctions();
}

BloomFilter::~BloomFilter() {
    delete [] bitArray;

    for(int32_t i = 0; i < functionCount; i++)
        delete functionParams[i];

    delete [] functionParams;
}

void BloomFilter::add(const string & text) {
    for(int32_t i = 0; i < functionCount; i++) {
        int32_t index = hash(text, functionParams[i]);
        if(!bitArray[index])
            bitArray[index] = true;
    }
}

bool BloomFilter::find(const string & text) {
    for(int32_t i = 0; i < functionCount; i++) {
        if(!bitArray[hash(text, functionParams[i])])
            return false;
    }
    return true;
}

int32_t BloomFilter::hash(const string & text, FunctionParams * functionParam) {
    int32_t result = 0;

    for(int32_t i = 0; i < (int32_t)text.length(); i++) {
        result += (((int32_t)text[i]) * (pow(functionParam -> a,i)));
    }

    result *= functionParam -> c;
    result += functionParam -> b;
    result = (p + (result % p)) % p;
    return (size + (result % size)) % size;
}

void BloomFilter::setFunctions() {
    functionParams = new FunctionParams*[functionCount];

    // random generator init
    random_device rd;
    default_random_engine gen(rd());
    uniform_int_distribution<int32_t> distribution(1, p - 1);

    for(int32_t i = 0; i < functionCount; i++)
        functionParams[i] = new FunctionParams(distribution(gen), distribution(gen), distribution(gen));
}