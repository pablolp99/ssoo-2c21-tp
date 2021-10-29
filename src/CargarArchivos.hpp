#ifndef HMC_ARCHIVOS_HPP
#define HMC_ARCHIVOS_HPP

#include <ctype.h>
#include <vector>
#include <string>
// #include <pthread.h>
#include <thread>

#include "HashMapConcurrente.hpp"

int cargarArchivo(
    HashMapConcurrente &hashMap,
    std::string filePath
);

void cargarArchivoThread(
    HashMapConcurrente &hashMap,
    std::vector<std::string> filePaths,
    std::atomic<int> *idx,
    unsigned int cantThreads
);

void cargarMultiplesArchivos(
    HashMapConcurrente &hashMap,
    unsigned int cantThreads,
    std::vector<std::string> filePaths
);


#endif /* HMC_ARCHIVOS_HPP */
