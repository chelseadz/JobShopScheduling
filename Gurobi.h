#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

#include "Utils.h"

// Variable names
static std::string Svar(int j, int k) {
    return "S_" + std::to_string(j) + "_" + std::to_string(k);
}
static std::string Yvar(int m, int a, int b) {
    return "Y_m" + std::to_string(m) + "_" + std::to_string(a) + "_" + std::to_string(b);
}

static void sort_jobs_by_opidx(JobsAllOps& jobs_all_ops) {
    for (auto& ops : jobs_all_ops) {
        std::sort(ops.begin(), ops.end(),
                  [](OperationSol* a, OperationSol* b){ return a->op_idx < b->op_idx; });
    }
}

static long long sum_processing_times(const JobsAllOps& jobs_all_ops) {
    long long s = 0;
    for (auto& job : jobs_all_ops)
        for (auto* op : job)
            if (op) s += op->processing_time;
    return s;
}

// Optional validation for Taillard classic: each job has M ops and uses each machine once.
static void validate_taillard_classic(const JobsAllOps& jobs_all_ops, int total_machines) {
    const int J = (int)jobs_all_ops.size();
    const int M = total_machines;

    for (int j = 0; j < J; ++j) {
        if ((int)jobs_all_ops[j].size() != M) {
            throw std::runtime_error("Job " + std::to_string(j) + " does not have exactly M operations.");
        }
        std::vector<int> seen(M, 0);
        for (auto* op : jobs_all_ops[j]) {
            if (!op) throw std::runtime_error("Null OperationSol* in jobs_all_ops");
            if (op->machine < 0 || op->machine >= M)
                throw std::runtime_error("Machine out of range in job " + std::to_string(j));
            if (op->processing_time < 0)
                throw std::runtime_error("Negative processing time in job " + std::to_string(j));
            seen[op->machine]++;
        }
        for (int m = 0; m < M; ++m) {
            if (seen[m] != 1) {
                throw std::runtime_error("Taillard classic violated: job " + std::to_string(j) +
                                         " uses machine " + std::to_string(m) +
                                         " " + std::to_string(seen[m]) + " times.");
            }
        }
    }
}

// Writes a valid MILP in LP format (Big-M).
// - M_value: if <= 0, uses sum of processing times as safe M.
int write_jssp_lp_bigM(const std::string& lp_filename,
                       JobsAllOps jobs_all_ops,   // pass by value to sort safely
                       int total_machines,
                       long long M_value)
{
    sort_jobs_by_opidx(jobs_all_ops);
    validate_taillard_classic(jobs_all_ops, total_machines);

    const int J = (int)jobs_all_ops.size();
    const int M = total_machines;

    long long Mbig = M_value;
    if (Mbig <= 0) Mbig = sum_processing_times(jobs_all_ops);
    if (Mbig <= 0) return -3;

    // Build ops per machine: list of (j,k,p)
    struct JK { int j; int k; int p; };
    std::vector<std::vector<JK>> byMachine(M);

    for (int j = 0; j < J; ++j) {
        for (int k = 0; k < M; ++k) {
            OperationSol* op = jobs_all_ops[j][k];
            byMachine[op->machine].push_back({j, k, op->processing_time});
        }
    }

    // Optional: stable ordering for naming Y variables
    for (int m = 0; m < M; ++m) {
        std::sort(byMachine[m].begin(), byMachine[m].end(),
                  [](const JK& a, const JK& b){
                      if (a.j != b.j) return a.j < b.j;
                      return a.k < b.k;
                  });
        if ((int)byMachine[m].size() != J) return -4;
    }

    std::ofstream out(lp_filename);
    if (!out) {
        std::cerr << "Could not open output file: " << lp_filename << "\n";
        return -1;
    }

    // ===== Objective =====
    out << "Minimize\n";
    out << "  Cmax\n\n";

    // ===== Constraints =====
    out << "Subject To\n";

    // Job precedence: S[j,k+1] - S[j,k] >= p[j,k]
    for (int j = 0; j < J; ++j) {
        for (int k = 0; k + 1 < M; ++k) {
            int pjk = jobs_all_ops[j][k]->processing_time;
            out << "  prec_" << j << "_" << k << ": "
                << Svar(j, k+1) << " - " << Svar(j, k)
                << " >= " << pjk << "\n";
        }
    }

    // Makespan: Cmax - S[j,last] >= p[j,last]
    for (int j = 0; j < J; ++j) {
        int plast = jobs_all_ops[j][M-1]->processing_time;
        out << "  cmax_" << j << ": "
            << "Cmax - " << Svar(j, M-1)
            << " >= " << plast << "\n";
    }

    // Machine non-overlap (Big-M):
    // Let y=1 => A before B:
    //   S_A + pA <= S_B + M(1-y)
    // <=> S_A - S_B + M y <= M - pA
    //
    // y=0 => B before A:
    //   S_B + pB <= S_A + M y
    // <=> S_B - S_A - M y <= -pB
    for (int m = 0; m < M; ++m) {
        const auto& L = byMachine[m];
        const int n = (int)L.size(); // == J

        for (int a = 0; a < n; ++a) {
            for (int b = a + 1; b < n; ++b) {
                const JK& A = L[a];
                const JK& B = L[b];
                std::string y = Yvar(m, a, b);

                // A before B when y=1 (relaxed when y=0)
                out << "  noov1_" << m << "_" << a << "_" << b << ": "
                    << Svar(A.j, A.k) << " - " << Svar(B.j, B.k)
                    << " + " << Mbig << " " << y
                    << " <= " << (Mbig - A.p) << "\n";

                // B before A when y=0 (enforced when y=0, relaxed when y=1)
                out << "  noov0_" << m << "_" << a << "_" << b << ": "
                    << Svar(B.j, B.k) << " - " << Svar(A.j, A.k)
                    << " - " << Mbig << " " << y
                    << " <= " << (-B.p) << "\n";
            }
        }
    }

    // ===== Bounds =====
    out << "\nBounds\n";
    for (int j = 0; j < J; ++j)
        for (int k = 0; k < M; ++k)
            out << "  " << Svar(j,k) << " >= 0\n";
    out << "  Cmax >= 0\n";

    // ===== Binary vars =====
    out << "\nBinary\n";
    for (int m = 0; m < M; ++m) {
        int n = (int)byMachine[m].size(); // J
        for (int a = 0; a < n; ++a) {
            for (int b = a + 1; b < n; ++b) {
                out << "  " << Yvar(m,a,b) << "\n";
            }
        }
    }

    out << "\nEnd\n";
    return 0;
}