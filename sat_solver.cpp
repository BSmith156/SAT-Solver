#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

using namespace std;

bool verbose;

void print_clauses(vector<vector<int>>*);
bool dpll(vector<vector<int>>);
vector<vector<int>> unit_propagation(vector<vector<int>>);

int main(int argc, char** argv) {
    // Check arguments
    if(argc == 1 || !strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
        printf("Usage: ./sat_solver [-v] clauses...\n\nOptions:\n  -v  --verbose: enable verbose mode\n  -clauses:      use numbers for literals, minus sign represents negation\n                 0 indicates the end of a clause\n\nExample, (1 ∨ ¬2) ∧ (¬2 ∨ 3):\n  ./sat_solver 1 -2 0 -2 3 0\n");
        return -1;
    }

    int clause_begin = 1;
    verbose = false;
    if(!strcmp(argv[1], "-v") || !strcmp(argv[1], "--verbose")) {
        clause_begin++;
        verbose = true;
    }

    // Create vector of clauses from arguments
    vector<vector<int>> clauses {vector<int>()};
    for(int i = clause_begin; i < argc; i++) {
        int literal = atoi(argv[i]);
        if(literal == 0) {
            if(i != argc - 1) {
                clauses.push_back(vector<int>());
            }
        } else {
            clauses[clauses.size() - 1].push_back(literal);
        }
    }

    // Check satisfiability
    clock_t start = clock();
    if(dpll(clauses)) {
        printf("Satisfiable!\n");
    } else {
        printf("Unsatisfiable!\n");
    }
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("CPU time taken: %.6lf\n", time_spent);

    return 0;
}

// Print clauses using mathematical notation
void print_clauses(vector<vector<int>>* clauses) {
    if(clauses->size() == 0) {
        printf("∅");
    }
    for(int i = 0; i < clauses->size(); i++) {
        printf("(");
        if((*clauses)[i].size() == 0) {
            printf("□");
        } else {
            for(int j = 0; j < (*clauses)[i].size(); j++) {
                if((*clauses)[i][j] < 0) {
                    printf("¬");
                }
                printf("%d", abs((*clauses)[i][j]));
                if(j < (*clauses)[i].size() - 1) {
                    printf(" ∨ ");
                }
            }
        }
        printf(")");
        if(i < clauses->size() - 1) {
            printf(" ∧ ");
        }
    }
    printf("\n");
}

// DPLL algorithm
bool dpll(vector<vector<int>> clauses) {
    clauses = unit_propagation(clauses);

    if(clauses.size() == 0) {
        return true;
    }
    for(int i = 0; i < clauses.size(); i++) {
        if(clauses[i].size() == 0) {
            return false;
        }
    }

    clauses.push_back(vector<int>());
    clauses[clauses.size() - 1].push_back(clauses[0][0]);
    if(verbose) {
        printf("Add unit clause ");
        if(clauses[0][0] < 0) {
            printf("¬");
            printf("%d:\n", abs(clauses[0][0]));
        }
    }
    bool sat = dpll(clauses);
    if(sat) {
        return true;
    } else {
        if(verbose) {
            printf("Backtrack, add unit clause ");
            if(clauses[0][0] > 0) {
                printf("¬");
                printf("%d:\n", abs(clauses[0][0]));
            }
        }
        clauses[clauses.size() - 1][0] *= -1;
        return dpll(clauses);
    }
}

// Unit propagation algorithm, halts propagation and returns current clauses if clause set contains empty clause
vector<vector<int>> unit_propagation(vector<vector<int>> clauses) {
    bool unsat = false;
    bool propagated;
    do {
        if(verbose) {
            print_clauses(&clauses);
        }
        if(unsat) {
            return clauses;
        }
        propagated = false;
        for(int i = clauses.size() - 1; i >= 0; i--) {
            if(clauses[i].size() == 1) {
                int literal = clauses[i][0];
                if(verbose) {
                    printf("Propagate ");
                    if(literal < 0) {
                        printf("¬");
                    }
                    printf("%d:\n", abs(literal));
                }
                for(int j = 0; j < clauses.size(); j++) {
                    for(int k = 0; k < clauses[j].size(); k++) {
                        if(clauses[j][k] == literal) {
                            clauses.erase(clauses.begin() + j--);
                            break;
                        } else if (clauses[j][k] == -literal) {
                            clauses[j].erase(clauses[j].begin() + k--);
                            if(clauses[j].size() == 0) {
                                if(!verbose) {
                                    return clauses;
                                }
                                unsat = true;
                            }
                        }
                    }
                }
                propagated = true;
                break;
            }
        }
    } while(propagated);
    return clauses;
}