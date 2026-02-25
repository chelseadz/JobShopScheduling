#include "Greedy.h"
#include <cstdio>
#include <limits>

void printOp(OperationSol* op){

    if(op != nullptr)
        printf("(J: %d, op: %d, M: %d, s: %d, e: %d)\n", op->job, op->op_idx, op->machine, op->start_time, op->end_time);
    else
        printf("null \n");
    return;
}

int add_operation(Solution& sol, OperationSol* actual_op){

    // OperationSol* actual_op = jobs.at(job_ix).front();

    // jobs.at(job_ix).pop_front();
    printf("actual: ");
    printOp(actual_op);
    printf("requiredOp: ");
    printOp(actual_op->requiredOp);

    // machine index sanity
    if (actual_op == nullptr) return -3;
    if (actual_op->machine < 0 || actual_op->machine >= (int)sol.size()) return -4;

    OperationSol* previous_op = nullptr;
    if (!sol.at(actual_op->machine).empty()) {
        previous_op = sol.at(actual_op->machine).back();
    }

    OperationSol* required_op = actual_op->requiredOp; 

    if(required_op && required_op->end_time == 0 && actual_op->op_idx != 0) 
        return -1;  // no se ha procesado required_op y la operacion no es la inicial 

    int machine_ready = previous_op ? previous_op->end_time : 0;
    int job_ready     = required_op ? required_op->end_time : 0;

    actual_op->start_time = (machine_ready > job_ready) ? machine_ready : job_ready;
    actual_op->end_time = actual_op->start_time + actual_op->processing_time;

    printOp(actual_op);
    // agregamos la operacion actual en la maquina que corresponda
    sol.at(actual_op->machine).push_back(actual_op);

    // printf("6\n");
    return 0;
}


Solution greedy_SPT(instance jobs, int start_job, int total_machines){
    // printf("Generating greedy_solution...  \n");
    Solution sol(total_machines);

    bool not_finished = true;

    for(Job job : jobs){
        for(OperationSol* op : job){
            op->end_time = op->start_time = 0;
        }
    }

    // schedule selected start job first
    add_operation(sol, jobs.at(start_job).front());
    jobs.at(start_job).pop_front();

    std::vector<OperationSol*> ready_op;

    // fill ready_op
    for(int i = 0; i < (int) jobs.size(); i++){

        // printf("i: %d ", i);
        if (!jobs[i].empty()) {
            ready_op.push_back(jobs.at(i).front()); 
            printOp(ready_op.back());
            jobs.at(i).pop_front();
        }
    }

    while(not_finished){
        
        // se elige la op de menor tiempo
        int min = std::numeric_limits<int>::max();
        int min_idx = -1;
        int job_idx = -1;

        for (int i = 0; i < (int)ready_op.size(); i++) {
            OperationSol* op = ready_op.at(i);

            if ( op->processing_time < min) {
                min = op->processing_time;
                min_idx = i;
                job_idx = op->job; 
            }
        }

        if (min_idx < 0 || min_idx >= (int)ready_op.size()) {
            printf("[OOR] min_idx=%d ready_op.size=%zu\n", min_idx, ready_op.size());
            break;
        }
        int err = add_operation(sol, ready_op.at(min_idx));
        if(!err){
            ready_op.erase(ready_op.begin() + min_idx); // solo se elimina si no hubo error al agregar op

            OperationSol* nextOp = nullptr;
            if (!jobs.at(job_idx).empty()) 
                nextOp = jobs.at(job_idx).front();
                

            if(nextOp != nullptr){
                ready_op.push_back(nextOp);
                jobs.at(job_idx).pop_front();
            }
        }else if (err == -1){
            // nunca sucedera que entre una operacion a ready cuya requerida no ha sido procesada
            // por la anatomia de cola de cada Job
        }
        
        if(!ready_op.empty())
            not_finished = true;
        else
            not_finished = false;

    }

    return sol;
}


Solution greedy_LPT(instance jobs, int start_job, int total_machines){
    // printf("Generating greedy_solution...  \n");
    Solution sol(total_machines);

    bool not_finished = true;

    for(Job job : jobs){
        for(OperationSol* op : job){
            op->end_time = op->start_time = 0;
        }
    }

    // schedule selected start job first
    add_operation(sol, jobs.at(start_job).front());
    jobs.at(start_job).pop_front();

    std::vector<OperationSol*> ready_op;

    // fill ready_op
    for(int i = 0; i < (int) jobs.size(); i++){

        // printf("i: %d ", i);
        if (!jobs[i].empty()) {
            ready_op.push_back(jobs.at(i).front()); 
            printOp(ready_op.back());
            jobs.at(i).pop_front();
        }
    }

    while(not_finished){
        
        // se elige la op de mayor tiempo
        int max = 0;
        int max_idx = -1;
        int job_idx = -1;

        for (int i = 0; i < (int)ready_op.size(); i++) {
            OperationSol* op = ready_op.at(i);

            if ( op->processing_time > max) {
                max = op->processing_time;
                max_idx = i;
                job_idx = op->job; 
            }
        }

        if (max_idx < 0 || max_idx >= (int)ready_op.size()) {
            printf("[OOR] max_idx=%d ready_op.size=%zu\n", max_idx, ready_op.size());
            break;
        }
        int err = add_operation(sol, ready_op.at(max_idx));
        if(!err){
            ready_op.erase(ready_op.begin() + max_idx); // solo se elimina si no hubo error al agregar op

            OperationSol* nextOp = nullptr;
            if (!jobs.at(job_idx).empty()) 
                nextOp = jobs.at(job_idx).front();
                

            if(nextOp != nullptr){
                ready_op.push_back(nextOp);
                jobs.at(job_idx).pop_front();
            }
        }else if (err == -1){
            // nunca sucedera que entre una operacion a ready cuya requerida no ha sido procesada
            // por la anatomia de cola de cada Job
        }
        
        if(!ready_op.empty())
            not_finished = true;
        else
            not_finished = false;

    }

    return sol;
}
