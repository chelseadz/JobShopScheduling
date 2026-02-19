#ifndef UTILS_H
#define UTILS_H

#include <queue>
#include <vector>

typedef struct OperationSol{
    int machine;
    int processing_time;
    int job;
    int op_idx;
    int start_time;
    int end_time;
    OperationSol* requiredOp;
}OperationSol;

typedef std::deque<OperationSol* > Job;

typedef std::vector< Job >  instance;

typedef std::vector<std::vector<OperationSol* > >  Solution;

OperationSol* create_OperationSol(
    int machine,
    int processing_time,
    int job,
    int op_idx,
    int start_time, 
    int end_time, 
    OperationSol* requiredOp);


void free_instance(instance& jobs);

int write_solution(const std::string& out_filename, const Solution& sol);

int read_instance(std::string filename, instance& jobs, int *total_jobs, int* total_machines);


#endif 