
#include "Utils.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


OperationSol* create_OperationSol(
    int machine,
    int processing_time,
    int job,
    int op_idx,
    int start_time, 
    int end_time, 
    OperationSol* requiredOp){

    OperationSol* newOp = new OperationSol();
    
    newOp->machine = machine;
    newOp->processing_time = processing_time;
    newOp->job=job;
    newOp->op_idx = op_idx;
    newOp->start_time = start_time;
    newOp->end_time = end_time;
    newOp->requiredOp = requiredOp;

    return newOp;

}

void free_instance(instance& jobs) {
    for (auto& job : jobs) {
        for (auto* op : job) {
            delete op;          // delete each OperationSol exactly once
        }
        job.clear();
    }
    jobs.clear();
}

int write_solution(const std::string& out_filename, const Solution& sol) {
    std::ofstream out(out_filename);
    if (!out) {
        std::cerr << "Could not open output file: " << out_filename << "\n";
        return -1;
    }

    out << calculate_max_time(sol) << "\n";
    for (const auto& machine : sol) {
        // out << "M" << i << ": ";

        for (const auto* op : machine) {
            out << "(" << op->job << ", " << op->op_idx << ", " << op->processing_time << ", " << op->start_time << ", " << op->end_time << "), ";
        }

        out << "\n";
    }

    return 0;
}

int read_instance(const std::string& filename,
                  instance& jobs, 
                  JobsAllOps& jobs_all_ops,
                  int* total_jobs,
                  int* total_machines)
{
    std::fstream input(filename);
    if (!input) {
        std::cerr << "No se encontro el archivo \"" << filename << "\"\n";
        return -1;
    }

    jobs.clear();

    std::string word;
    input >> word; *total_jobs = std::stoi(word);
    input >> word; *total_machines = std::stoi(word);

    std::cout << "total_jobs: " << *total_jobs
              << ", total_machines: " << *total_machines << "\n";

    jobs.reserve(*total_jobs);
    jobs_all_ops.assign(*total_jobs, {}); 

    std::string line;
    std::getline(input, line); // consumir fin de línea

    for (int i = 0; i < *total_jobs; i++) {
        OperationSol* req_op = nullptr;

        if (!std::getline(input, line)) {
            std::cerr << "Falta la linea del job " << i << "\n";
            return 1;
        }

        std::stringstream ss(line);

        Job aux;     
        int j = 0;

        while (ss >> word) {
            int machine = std::stoi(word);
            if (!(ss >> word)) {
                std::cerr << "Formato invalido en job " << i << " (par incompleto)\n";
                return 2;
            }
            int processing_time = std::stoi(word);

            OperationSol* op_sol = create_OperationSol(
                machine,
                processing_time,
                i,
                j,
                0,
                0,
                req_op
            );

            if (!op_sol) {
                std::cerr << "create_OperationSol devolvio nullptr (job " << i << ", op " << j << ")\n";
                return 3;
            }

            aux.push_back(op_sol);
            jobs_all_ops[i].push_back(op_sol); 

            req_op = op_sol;
            ++j;
        }

        jobs.push_back(std::move(aux));
    }

    return 0;
}

int calculate_max_time(const Solution& sol) {
    int max = 0;
    OperationSol* lastOp = nullptr;

    for (auto& machine : sol) {
        lastOp = machine.back();
        if(lastOp->end_time > max){
            max = lastOp->end_time;
        }
    }

    return max;
}


int write_times(const std::string& out_filename, const Solution& sol) {
    std::ofstream out(out_filename, std::ios::app);
    if (!out) {
        std::cerr << "Could not open output file: " << out_filename << "\n";
        return -1;
    }

    out << calculate_max_time(sol) << "\n";

    return 0;
}