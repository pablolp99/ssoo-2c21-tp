#include <iostream>
#include "time.h"

#include "HashMapConcurrente.hpp"
#include "CargarArchivos.hpp"

void experiment(std::vector<std::string> file_paths){
    // Run functions with:
    // 1, 2, 4, 8, 16, 26, 32 and 64 threads
    int threads[8] = { 1, 2, 4, 8, 16, 26, 32, 64 };

    std::cout << "thread_count,id,load_time,max_time,time" << std::endl;

    for (int i = 0; i < 8; ++i){
        clockid_t clk_id = CLOCK_REALTIME;
        for (int t = 0; t < 50; ++t){
            
            HashMapConcurrente hashMap{};
            // Using threads to load files
            struct timespec start_load, end_load;
            clock_gettime(clk_id, &start_load);
            cargarMultiplesArchivos(hashMap, threads[i], file_paths);
            clock_gettime(clk_id, &end_load);

            // Using threads to calculate maximum
            struct timespec start_max, end_max;
            clock_gettime(clk_id, &start_max);
            auto maximo = hashMap.maximoParalelo(threads[i]);
            clock_gettime(clk_id, &end_max);
            
            std::cout << threads[i] << "," << t << "," << end_load.tv_nsec - start_load.tv_nsec << "," << end_max.tv_nsec - start_max.tv_nsec << std::endl;
        }
    }
}

int main(int argc, char **argv) {
    if (argc < 1){
        std::cout << "Error: faltan argumentos." << std::endl;
        std::cout << std::endl;
        std::cout << "Modo de uso: " << argv[0] << " <archivo1> [<archivo2> ...]" << std::endl;
        return 1;
    }

    std::vector<std::string> file_paths = {};
    for (int i = 3; i < argc; i++) {
        file_paths.push_back(argv[i]);
    }
    experiment(file_paths);
    
    return 0;
}