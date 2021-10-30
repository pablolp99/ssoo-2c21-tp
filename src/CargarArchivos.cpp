#ifndef CHM_CPP
#define CHM_CPP

#include <vector>
#include <iostream>
#include <fstream>
// #include <pthread.h>
#include <thread>

#include "CargarArchivos.hpp"

int cargarArchivo(
    HashMapConcurrente &hashMap,
    std::string filePath
) {
    std::fstream file;
    int cant = 0;
    std::string palabraActual;

    // Abro el archivo.
    file.open(filePath, file.in);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo '" << filePath << "'" << std::endl;
        return -1;
    }
    while (file >> palabraActual) {
        hashMap.incrementar(palabraActual);
        cant++;
    }
    // Cierro el archivo.
    if (!file.eof()) {
        std::cerr << "Error al leer el archivo" << std::endl;
        file.close();
        return -1;
    }
    file.close();
    return cant;
}

void cargarArchivoThread(
    HashMapConcurrente &hashMap,
    std::vector<std::string> filePaths,
    std::atomic<int> *idx,
    unsigned int cantThreads
) {
    int current_idx;
    while ((unsigned int) (current_idx = (*idx)++) < cantThreads) {
        cargarArchivo(hashMap, filePaths[current_idx]);   
    }
}

void cargarMultiplesArchivos(
    HashMapConcurrente &hashMap,
    unsigned int cantThreads,
    std::vector<std::string> filePaths
) {
    std::thread threads[cantThreads];
    std::atomic<int> next_file_idx{0};

    for (unsigned int i = 0; i < cantThreads; ++i) {
        threads[i] = (std::thread(
            cargarArchivoThread,
            std::ref(hashMap),
            filePaths,
            &next_file_idx,
            cantThreads
        ));
    }

    for (unsigned int i = 0; i < cantThreads; ++i) {
        threads[i].join();
    }
}

#endif
