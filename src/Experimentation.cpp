#include <iostream>
#include "time.h"

#include "HashMapConcurrente.hpp"
#include "CargarArchivos.hpp"

#define TOTAL_THREADS 10

void experiment_1(std::string file_path){
    // Run functions with:
    // 1, 2, 4, 8, 16, 26, 32 and 64 threads
    int threads[TOTAL_THREADS] = { 1, 2, 4, 8, 12, 16, 26, 32, 48, 64 };

    std::cout << "thread_count,id,time,function,file_count" << std::endl;

    HashMapConcurrente hashMap{};
    std::vector<std::string> file_paths;
    file_paths.push_back(file_path);
    cargarMultiplesArchivos(hashMap, 2, file_paths);

    clockid_t clk_id = CLOCK_REALTIME;
    for (int i = 0; i < TOTAL_THREADS; ++i){
        
        for (int t = 0; t < 50; ++t){
            // Using threads to calculate maximum
            struct timespec start_max, end_max;
            clock_gettime(clk_id, &start_max);
            auto maximo = hashMap.maximoParalelo(threads[i]);
            clock_gettime(clk_id, &end_max);
            
            std::cout << threads[i] << "," << t << "," << (unsigned long) end_max.tv_nsec - start_max.tv_nsec << "," << "max" << "," << "-1" << std::endl;
        }
    }
}


void experiment_2(std::string file_path){
    clockid_t clk_id = CLOCK_REALTIME;
    std::cout << "thread_count,id,time,function,file_count" << std::endl;
    for (int j = 0; j < 50; ++j){
        for (int i = 1; i < 21; i+=2){
            HashMapConcurrente hashMap{};
            std::vector<std::string> file_paths;
            for (int n = 1; n < 11; ++n){
                file_paths.push_back(file_path);
                
                struct timespec start_load, end_load;
                clock_gettime(clk_id, &start_load);
                cargarMultiplesArchivos(hashMap, i, file_paths);
                clock_gettime(clk_id, &end_load);

                long time_diff = (long) (end_load.tv_sec * 1000000000 + end_load.tv_nsec) - (start_load.tv_sec * 1000000000 + start_load.tv_nsec);
                std::cout << i << "," << j << "," << time_diff << "," << "load" << "," << n << std::endl;
            }
        }
    }
}

int main(int argc, char **argv) {
    if (argc < 2){
        std::cout << "Error: faltan argumentos." << std::endl;
        std::cout << std::endl;
        std::cout << "Modo de uso: " << argv[0] << " <archivo1> <loadTimes>" << std::endl;
        return 1;
    }

    // experiment_1(argv[1]);
    experiment_2(argv[1]);
    
    return 0;
}