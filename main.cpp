#include <stdio.h>
#include <ctime>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>  //pair
#include <vector>
#include <iostream>

typedef std::vector<std::vector<std::pair<int, int> > >  instance;

int read_instance(std::string filename, instance& jobs){

    std::fstream input(filename);
    

    if(!input) {
        printf("No se encontro el archivo \"%s\"", filename.c_str());
        return -1;
    }

    std::string word;
    int total_jobs, total_machines;


    input >> word;
    total_jobs =  std::stoi(word);
    input >> word;
    total_machines =  std::stoi(word);

    printf("total_jobs: %d, total_machines: %d\n", total_jobs, total_machines);

    std::string line;

    std::getline(input, line);

    int machine, processing_time;
    std::vector<std::pair<int, int> > aux;

    for (int i = 0; i < total_jobs; i++){

        // leer una linea que corresponde a un job
        if (!std::getline(input, line)) {
            std::cerr << "Falta la linea del job " << i << "\n";
            return 1;
        }

        std::stringstream ss(line);

        while(ss >> word){
            machine = stoi(word); // guardar primera parte del par
            ss >> word;
            processing_time = stoi(word); // guardar la segunda parte del par

            aux.push_back(std::make_pair(machine, processing_time)); 
        }

        jobs.push_back(aux); // se guarda el job i

        // printf("inserted job %d: ", i);
        // for(std::pair<int, int> operation : jobs.at(i)){
        //     printf("(%d, %d), ", operation.first, operation.second);
        // } 
        // printf("\n");
        aux.clear();
    }

    return 0;

}

int main(int argc, char* argv[]){

    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <ruta_instancia>\n";
        return 1;
    }

    std::string filename = std::string(argv[1]);
    instance jobs;

    int err = read_instance(filename, jobs);

    int i =0;
    for (auto job : jobs){
        printf("job %d: ", i);
        for(std::pair<int, int> operation : job){
            printf("(%d, %d), ", operation.first, operation.second);
        }        

        printf("\r\r\n");
        i++;
    }

    return 0;
}