#ifndef HMC_HPP
#define HMC_HPP

#include <mutex>
#include <array>
#include <atomic>
#include <string>
#include <vector>

#include "ListaAtomica.hpp"

typedef std::pair<std::string, unsigned int> hashMapPair;

class HashMapConcurrente {
 public:
    static constexpr int cantLetras = 26;

    HashMapConcurrente();

    void incrementar(std::string clave);
    std::vector<std::string> claves();
    unsigned int valor(std::string clave);

    hashMapPair maximo();
    hashMapPair maximoParalelo(unsigned int cantThreads);

 private:
    ListaAtomica<hashMapPair> *tabla[HashMapConcurrente::cantLetras];

    unsigned int hashIndex(std::string clave);
    void maximoThread();
    std::array<std::mutex, 26> mutex_per_bucket;

    // Para maximo
    std::atomic<int> next_row_max{0};
    std::mutex mtx_global_max;
    hashMapPair global_max;
};

#endif  /* HMC_HPP */
