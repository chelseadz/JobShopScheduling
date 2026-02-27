#include "Validation.h"

#include <queue>
#include <algorithm>
#include <cstdio>
#include <algorithm>


int validate_solution(const Solution& sol) {
    int errors = 0;

    for (int m = 0; m < (int)sol.size(); ++m) {
        const auto& machine = sol[m];

        // machine order constraint
        for (int k = 0; k < (int)machine.size(); ++k) {
            const OperationSol* op = machine[k];

            // duration consistency
            if (op->end_time != op->start_time + op->processing_time) {
                printf("[ERR] M%d pos %d (J%d,O%d): end(%d) != start(%d)+p(%d)\n",
                            m, k, op->job, op->op_idx, op->end_time, op->start_time, op->processing_time);
                errors++;
            }

            // machine precedence
            if (k > 0) {
                const OperationSol* prev = machine[k - 1];
                if (op->start_time < prev->end_time) {
                    printf("[ERR] Machine overlap M%d: (J%d,O%d) start=%d < prev end=%d\n",
                                m, op->job, op->op_idx, op->start_time, prev->end_time);
                    errors++;
                }
            }

            // job precedence
            if (op->requiredOp != nullptr) {
                const OperationSol* req = op->requiredOp;
                if (op->start_time < req->end_time) {
                    printf("[ERR] Job precedence: (J%d,O%d) start=%d < required (J%d,O%d) end=%d\n",
                                op->job, op->op_idx, op->start_time,
                                req->job, req->op_idx, req->end_time);
                    errors++;
                }
            }
        }
    }

    return errors; // 0 = OK
}
