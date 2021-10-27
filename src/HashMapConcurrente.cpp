#ifndef CHM_CPP
#define CHM_CPP

#include <thread>
// alternativamente #include <pthread.h>
#include <iostream>
#include <fstream>

#include "HashMapConcurrente.hpp"

HashMapConcurrente::HashMapConcurrente() {
    for (unsigned int i = 0; i < HashMapConcurrente::cantLetras; i++) {
        tabla[i] = new ListaAtomica<hashMapPair>();
    }
}

unsigned int HashMapConcurrente::hashIndex(std::string clave) {
    return (unsigned int)(clave[0] - 'a');
}

void HashMapConcurrente::incrementar(std::string clave) {
    /*
    unsigned int index = hashIndex(clave);
    std::mutex &mtx = mutex_per_bucket[index];
    mtx.lock();
    
    ListaAtomica<hashMapPair> *list = tabla[index];
    
    bool found = false;
    for (int i = 0; i < (int)list->longitud(); ++i){
        hashMapPair par = list[i];

        if (list[i]->first == clave){
            list[i].second++;
            found = true;
        }
    }

    if (!found){
        list.insertar(hashMapPair(clave, 1));
    }

    mtx.unlock();
    */
}

std::vector<std::string> HashMapConcurrente::claves() {
    // Completar (Ejercicio 2)
}

unsigned int HashMapConcurrente::valor(std::string clave) {
    // Completar (Ejercicio 2)
}

hashMapPair HashMapConcurrente::maximo() {
    hashMapPair *max = new hashMapPair();
    max->second = 0;

    for (unsigned int index = 0; index < HashMapConcurrente::cantLetras; index++) {
        for (auto &p : *tabla[index]) {
            if (p.second > max->second) {
                max->first = p.first;
                max->second = p.second;
            }
        }
    }

    return *max;
}



hashMapPair HashMapConcurrente::maximoParalelo(unsigned int cant_threads) {
    // Completar (Ejercicio 3)
}

#endif
