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
    unsigned int index = hashIndex(clave);
    std::mutex &mtx = mutex_per_bucket[index];
    mtx.lock();
    
    ListaAtomica<hashMapPair> *list = tabla[index];
    bool found = false;

    for (auto it = list->begin(); it != list->end(); it++){
        auto obj = *it;
        if (obj.first == clave){
            hashMapPair new_pair = { clave, obj.second + 1 };
            *it = new_pair;
            found = true;
        }
    }

    if (!found){
        list->insertar(hashMapPair(clave, 1));
    }

    mtx.unlock();
}

std::vector<std::string> HashMapConcurrente::claves() {
    std::vector<std::string> result;
    for (int i = 0; i < 26; i++){
        ListaAtomica<hashMapPair> *list = tabla[i];
        for (auto it = list->begin(); it != list->end(); it++){
            auto obj = *it;
            result.push_back(obj.first);
        }
    }
    return result;
}

unsigned int HashMapConcurrente::valor(std::string clave) {
    unsigned int index = hashIndex(clave);
    ListaAtomica<hashMapPair> *list = tabla[index];
    for (auto it = list->begin(); it != list->end(); it++){
        auto obj = *it;
        if (obj.first == clave){
            return obj.second;
        }
    }
    return 0;
}

hashMapPair HashMapConcurrente::maximo() {
    hashMapPair *max = new hashMapPair();
    max->second = 0;
    
    // bloqueo inserción todas las filas (toda la tabla)
    for (unsigned int i = 0; i < 26; ++i) {
        mutex_per_bucket[i].lock();
    }

    for (unsigned int index = 0; index < HashMapConcurrente::cantLetras; index++) {
        for (auto &p : *tabla[index]) {
            if (p.second > max->second) {
                max->first = p.first;
                max->second = p.second;
            }
        }
    }
    
    // desbloqueo inserción todas las filas (toda la tabla)
    for (unsigned int i = 0; i < 26; ++i) {
        mutex_per_bucket[i].unlock();
    }

    return *max;
}



hashMapPair HashMapConcurrente::maximoParalelo(unsigned int cant_threads) {
    // Completar (Ejercicio 3)
    // no tiene sentido tener mas threads que filas en el hashmap?
    std::thread threads[cant_threads];
    std::atomic<int> next_row_max{0};
    std::mutex mtx_global_max;
    hashMapPair *max = new hashMapPair();
    max->second = 0;

    for (unsigned int i = 0; i < cant_threads; ++i) {
        threads[i] = std::thread(HashMapConcurrente::maximoThread, this, &next_row_max, max, &mtx_global_max);
    }

    for (unsigned int i = 0; i < cant_threads; ++i) {
        threads[i].join();
    }
    
    return *max;
}

void HashMapConcurrente::maximoThread(void *hash_map, std::atomic<int> *row, hashMapPair *global_max, std::mutex *mtx_global_max) {
    // this
    HashMapConcurrente * hashMap = (HashMapConcurrente*)hash_map;

    hashMapPair *thread_max = new hashMapPair();
    thread_max->second = 0;

    // *row al ser atomic int devuelve valor actual e incrementa 1
    // se comparte con los otros threads y nunca dos threads van a obtener 
    // el mismo valor
    int current_row;  
    while ((current_row = (*row)++) < 26) {
        // calculo maximo de la fila
        for (auto &p : *hashMap->tabla[current_row]) {
            if (p.second > thread_max->second) {
                thread_max->first = p.first;
                thread_max->second = p.second;
            }
        }
    }

    // piso el maximo calculado por el thread en las filas qque proceso
    // en la variable compartida con los demas threads
    (*mtx_global_max).lock();
    if (global_max->second < thread_max->second) {
        global_max->first = thread_max->first;
        global_max->second = thread_max->second;
    }
    (*mtx_global_max).unlock();
}

#endif
