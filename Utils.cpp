
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

    for (const auto& machine : sol) {
        // out << "M" << i << ": ";

        for (const auto* op : machine) {
            out << "(" << op->job << ", " << op->op_idx << ", " << op->processing_time << ", " << op->start_time << ", " << op->end_time << "), ";
        }

        out << "\n";
    }

    return 0;
}

int read_instance(std::string filename, instance& jobs, int *total_jobs, int* total_machines){

    std::fstream input(filename);
    

    if(!input) {
        printf("No se encontro el archivo \"%s\"", filename.c_str());
        return -1;
    }

    std::string word;

    input >> word;
    *total_jobs =  std::stoi(word);
    input >> word;
    *total_machines =  std::stoi(word);

    printf("total_jobs: %d, total_machines: %d\n", *total_jobs, *total_machines);

    std::string line;

    std::getline(input, line);

    int machine, processing_time;
    Job aux;
    // OperationSol* req_op = nullptr; 
    OperationSol* op_sol = nullptr;

    for (int i = 0; i < *total_jobs; i++){

        OperationSol* req_op = nullptr;   // <-- reset for each job

        // leer una linea que corresponde a un job
        if (!std::getline(input, line)) {
            std::cerr << "Falta la linea del job " << i << "\n";
            return 1;
        }

        std::stringstream ss(line);

        // printf("line %d: %s\n",i, line.c_str());

        int j = 0; // operacion 0 del job i
        while(ss >> word){
            machine = stoi(word); // guardar primera parte del par
            ss >> word;
            processing_time = stoi(word); // guardar la segunda parte del par

            // Operation op = std::make_pair(machine, processing_time);
            op_sol = create_OperationSol(
                machine,
                processing_time,
                i,
                j,
                0,
                0,
                req_op
            );

            aux.push_back(op_sol); 

            req_op = op_sol; // ahora la operacion actual es la requerida para la diguiente operacion
            j++; // pasar a la siguiente operacion
        }

        jobs.push_back(aux); // se guarda el job i

        printf("inserted job %d: \n", i);

        // for(auto operation : jobs.at(i)){
            
        //     int req_idx = operation->requiredOp ? operation->requiredOp->op_idx : -1;

        //     printf("(%d, %d, %d, %d) req(%d), ", 
        //     operation->job,
        //     operation->op_idx,
        //     operation->machine, 
        //     operation->processing_time,
        //     req_idx
        //     );
        // } 
        // printf("\n");

        aux = Job();

    }

    // printf("out loop\n");

    return 0;

}
