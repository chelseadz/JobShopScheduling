#include <stdio.h>
#include <vector>
#include <iostream>
#include <queue>

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

    Solution sol = greedy_solution(jobs, 0, total_machines);
    printf("greedy_solution out \n");


    err = validate_solution(sol);
    if (err != 0) {
        printf("validate_solution: %d errors\n", err);
    } else {
        printf("solution OK\n");
    }

    write_solution("greedy_solution.txt", sol);


    // int i =0;
    // for (auto job : jobs){
    //     printf("job %d: ", i);
    //     for(std::pair<int, int> operation : job){
    //         printf("(%d, %d), ", operation.first, operation.second);
    //     }        

    //     printf("\b\b");
    //     printf("\n");
    //     i++;
    // }

    // delete sol;
    free_instance(jobs);

    return 0;
}