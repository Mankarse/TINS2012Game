#include "Primes.h"
#include "AllPrimes.h"

template<typename T, std::size_t N>
std::size_t length(T(&)[N]) {
    return N;
}

//Factorize by trial division
std::vector<unsigned> factorize(unsigned n) {
    std::vector<unsigned> primeFactors;
    
    if (n == 1) {
        primeFactors.push_back(1);
        return primeFactors;
    }
    
    unsigned const* it(allPrimes);
    unsigned const* end(allPrimes + length(allPrimes));
    for (; it != end; ++it) {
        unsigned p(*it);
        if (p*p > n) break;
        while (n % p == 0) {
            primeFactors.push_back(p);
            n /= p;
        }
    }
    if (n > 1) primeFactors.push_back(n);
 
    return primeFactors;
}
