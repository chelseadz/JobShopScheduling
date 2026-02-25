#ifndef UTILS_H
#define UTILS_H

#include <queue>
#include <vector>
#include <string>

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

typedef std::vector<std::vector<OperationSol* > > JobsAllOps;

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

int read_instance(const std::string& filename,
                  instance& jobs,
                  std::vector<std::vector<OperationSol*>>& jobs_all_ops,
                  int* total_jobs,
                  int* total_machines);

int calculate_max_time(const Solution& sol);

int write_times(const std::string& out_filename, const Solution& sol);

#endif 