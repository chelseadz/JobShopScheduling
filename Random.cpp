#include <chrono>
#include <random>
#include <ctime>

#include "Random.h"

Solution random_solution(instance jobs, int total_machines){
    printf("Generating random_solution...  \n");
    Solution sol(total_machines);

    auto seed = static_cast<unsigned long long>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()
    );

    std::mt19937_64 gen(seed);
    std::uniform_int_distribution<int> dist_int(0, jobs.size()-1);

    bool not_finished = true;
    int actual_job, current_machine;
    OperationSol* actual_op;

    while(not_finished){

        // elegimos un job al azar
        actual_job = dist_int(gen);
        // printf("actual_job: %d\n", actual_job);

        if (jobs.at(actual_job).empty()) {
            continue; //
        }

        // desencolamos la primera operacion del trabajo actual, y se elimina
        actual_op = jobs.at(actual_job).front();
        jobs.at(actual_job).pop_front();

        // printf("pop front: (%d, %d)\n", actual_op->job, actual_op->op_idx);
        
        current_machine = actual_op->machine;

        // agregamos la operacion actual en la maquina que corresponda
        sol.at(current_machine).push_back(actual_op);

        for(Job job : jobs){
            if(!job.empty()) {
                not_finished = true;
                break;
            }
            not_finished = false;
        }
    }

    // int i = 0;
    // for(auto machine : sol){
    //     printf("M%d: ", i);

    //     for(auto op : machine){
    //         printf("(%d, %d, %d), ", op->job, op->op_idx, op->processing_time); // job
    //     }

    //     printf("\n");
    //     i++;
    // }

    return sol;
}
