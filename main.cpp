#include <stdio.h>
#include <iostream>
#include <filesystem>
#include <unordered_set>

#include "Validation.h"
#include "Random.h"
#include "Greedy.h"
#include "Gurobi.h"

int main(int argc, char* argv[]){

    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <ruta_instancia>\n";
        return 1;
    }

    std::string filename = std::string(argv[1]);
    instance jobs;

    int total_jobs, total_machines, best_time;

    std::vector<std::vector<OperationSol* > > jobs_all_ops; 
    int err = read_instance(filename, jobs, jobs_all_ops, &total_jobs, &total_machines);
    printf("read_instance out \n");


    if (err) {
        free_instance(jobs);
        return err;
    }

    // SPT
    best_time = std::numeric_limits<int>::max();
    for(int i = 0; i < (int) jobs.size(); i++){
        Solution sol = greedy_SPT(jobs, i, total_machines);
        // printf("greedy_solution out \n");

        err = validate_solution(sol);
        if (err != 0) {
            printf("validate_solution: %d errors\n", err);
        } else {
            printf("Inicio con %d: %d\n", i, calculate_max_time(sol));
        }
        std::filesystem::path outDir = "solutions";  
        std::filesystem::create_directories(outDir); 

        std::filesystem::path outFile = outDir / ("best_SPT.sol");
        std::filesystem::path outTimes = ("times_SPT.txt");
        
        int time = calculate_max_time(sol);
        if(time < best_time)
        if(time < best_time){
            write_solution(outFile.string(), sol);
            best_time = time;
        }

        write_times(outTimes.string(), sol);
        
    }

    // LPT
    best_time = std::numeric_limits<int>::max();
    for(int i = 0; i < (int) jobs.size(); i++){
        Solution sol = greedy_LPT(jobs, i, total_machines);
        // printf("greedy_solution out \n");

        err = validate_solution(sol);
        if (err != 0) {
            printf("validate_solution: %d errors\n", err);
        } else {
            printf("Inicio con %d: %d\n", i, calculate_max_time(sol));
        }
        std::filesystem::path outDir = "solutions";  
        std::filesystem::create_directories(outDir); 

        std::filesystem::path outFile = outDir / ("best_LPT.sol");
        std::filesystem::path outTimes = ("times_LPT.txt");

        int time = calculate_max_time(sol);
        if(time < best_time){
            write_solution(outFile.string(), sol);
            best_time = time;
        }

        write_times(outTimes.string(), sol);
        
    }

    best_time = std::numeric_limits<int>::max();
    for(int i = 0; i < total_jobs; i++){
        Solution sol = random_solution(jobs, total_machines);
        // printf("random_solution out \n");

        err = evaluate_solution(sol);
        if (err != 0) {
            printf("evaluate_solution: infeasible\n");
        }

        err = validate_solution(sol);
        if (err != 0) {
            printf("validate_solution: %d errors\n", err);
        } else {
            // printf("solution OK\n");
            printf("Random %d: %d\n", i, calculate_max_time(sol));
        }

        std::filesystem::path outDir = "solutions";  
        std::filesystem::create_directories(outDir); 

        std::filesystem::path outFile = outDir / ("best_random.sol");
        std::filesystem::path outTimes = ("times_random.txt");

        int time = calculate_max_time(sol);
        if(time < best_time){
            write_solution(outFile.string(), sol);
            best_time = time;
        }

        write_times(outTimes.string(), sol);
    }


    // int rc = write_jssp_lp_bigM("jssp.lp", jobs_all_ops, total_machines, 0);
    // if (rc != 0) {
    //     std::cerr << "Error escribiendo LP: " << rc << "\n";
    // }

    free_instance(jobs);

    return 0;
}