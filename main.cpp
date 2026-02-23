#include <stdio.h>
#include <iostream>
#include <filesystem>

#include "Validation.h"
#include "Random.h"
#include "Greedy.h"


int main(int argc, char* argv[]){

    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <ruta_instancia>\n";
        return 1;
    }

    std::string filename = std::string(argv[1]);
    instance jobs;

    int total_jobs, total_machines;

    int err = read_instance(filename, jobs, &total_jobs, &total_machines);
    printf("read_instance out \n");



    if (err) {
        free_instance(jobs);
        return err;
    }

    // Solution sol = random_solution(jobs, total_machines);
    // printf("random_solution out \n");

    // err = evaluate_solution(sol);
    // if (err != 0) {
    //     printf("evaluate_solution: infeasible\n");
    // }

    // err = validate_solution(sol);
    // if (err != 0) {
    //     printf("validate_solution: %d errors\n", err);
    // } else {
    //     printf("solution OK\n");
    // }

    // write_solution("random_solution.txt", sol);
    for(int i = 0; i < (int) jobs.size(); i++){
        Solution sol = greedy_solution(jobs, i, total_machines);
        printf("greedy_solution out \n");

        err = validate_solution(sol);
        if (err != 0) {
            printf("validate_solution: %d errors\n", err);
        } else {
            printf("solution OK\n");
        }
        std::filesystem::path outDir = "solutions";  
        std::filesystem::create_directories(outDir); 

        std::filesystem::path outFile = outDir / ("greedy_" + std::to_string(i) + ".sol");

        write_solution(outFile.string(), sol);
    }


    for(int i = 0; i < 100; i++){
        Solution sol = random_solution(jobs, total_machines);
        printf("random_solution out \n");

        err = evaluate_solution(sol);
        if (err != 0) {
            printf("evaluate_solution: infeasible\n");
        }

        err = validate_solution(sol);
        if (err != 0) {
            printf("validate_solution: %d errors\n", err);
        } else {
            printf("solution OK\n");
        }

        std::filesystem::path outDir = "solutions";  
        std::filesystem::create_directories(outDir); 

        std::filesystem::path outFile = outDir / ("random_" + std::to_string(i) + ".sol");

        write_solution(outFile.string(), sol);
    }

    // delete jobs;
    free_instance(jobs);

    return 0;
}