#include "Greedy.h"
#include <cstdio>
#include <limits>

int add_operation(Solution& sol, instance& jobs, int job_ix){
    if (job_ix < 0 || job_ix >= (int)jobs.size()) return -1;
    if (jobs.at(job_ix).empty()) return -2;

    OperationSol* actual_op = jobs.at(job_ix).front();

    jobs.at(job_ix).pop_front();

    // machine index sanity
    if (actual_op == nullptr) return -3;
    if (actual_op->machine < 0 || actual_op->machine >= (int)sol.size()) return -4;

    OperationSol* previous_op = nullptr;
    if (!sol.at(actual_op->machine).empty()) {
        previous_op = sol.at(actual_op->machine).back();
    }

    OperationSol* required_op = actual_op->requiredOp; // may be nullptr

    int machine_ready = previous_op ? previous_op->end_time : 0;
    int job_ready     = required_op ? required_op->end_time : 0;

    actual_op->start_time = (machine_ready > job_ready) ? machine_ready : job_ready;
    actual_op->end_time = actual_op->start_time + actual_op->processing_time;

    // agregamos la operacion actual en la maquina que corresponda
    sol.at(actual_op->machine).push_back(actual_op);

    // printf("6\n");
    return 0;
}

Solution greedy_solution(instance jobs, int start_job, int total_machines){
    printf("Generating greedy_solution...  \n");
    Solution sol(total_machines);

    bool not_finished = true;

    add_operation(sol, jobs, start_job);

    std::vector<OperationSol*> ready_op;

    // fill ready_op
    for(int i = 0; i < (int) jobs.size(); i++){

        // printf("i: %d ", i);
        if (!jobs[i].empty()) {
            ready_op.push_back(jobs.at(i).front()); 
            jobs.at(i).pop_front();
        }
    }

    while(not_finished){
        
        // se elige la op de menor tiempo
        int min = std::numeric_limits<int>::max();
        int min_idx = -1;
        int job_idx = -1;

        for (int i = 0; i < (int)ready_op.size(); i++) {
            auto op = ready_op.at(i);

            if ( op->processing_time < min) {
                min = op->processing_time;
                min_idx = i;
                job_idx = op->job; 
            }
        }


        add_operation(sol, jobs, job_idx);

        ready_op.erase(ready_op.begin() + min_idx);

        OperationSol* nextOp = nullptr;
        if (!jobs.at(job_idx).empty()) 
            nextOp = jobs.at(job_idx).front();
            

        if(nextOp != nullptr){
            ready_op.push_back(nextOp);
            jobs.at(job_idx).pop_front();
        }
        
        if(!ready_op.empty())
            not_finished = true;
        else
            not_finished = false;

        // for(Job job : jobs){
        //     if(!job.empty()) {
        //         not_finished = true;
        //         break;
        //     }
        //     not_finished = false;
        // }
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
