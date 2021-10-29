#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <unordered_set>

using namespace std;

bool verbose;

void print_clauses(vector<unordered_set<int>>*);
bool dpll(vector<unordered_set<int>>);
void unit_propagation(vector<unordered_set<int>>*);
void tautology_elimination(vector<unordered_set<int>>*);

int main(int argc, char** argv) {
    // Check arguments
    if(argc == 1 || !strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
        printf("Usage: ./sat_solver [-v] clauses...\n\nOptions:\n  -v  --verbose: enable verbose mode, will slow down computation\n  -clauses:      use numbers for literals, minus sign represents negation\n                 0 indicates the end of a clause\n\nExample, (1 ∨ ¬2) ∧ (¬2 ∨ 3):\n  ./sat_solver 1 -2 0 -2 3 0\n");
        return -1;
    }

    int clause_begin = 1;
    verbose = false;
    if(!strcmp(argv[1], "-v") || !strcmp(argv[1], "--verbose")) {
        clause_begin++;
        verbose = true;
    }

    // Create vector of clauses from arguments
    vector<unordered_set<int>> clauses {unordered_set<int>()};
    for(int i = clause_begin; i < argc; i++) {
        int literal = atoi(argv[i]);
        if(literal == 0) {
            if(i != argc - 1) {
                clauses.push_back(unordered_set<int>());
            }
        } else {
            clauses[clauses.size() - 1].insert(literal);
        }
    }

    // Check satisfiability
    clock_t start = clock();
    tautology_elimination(&clauses);
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
void print_clauses(vector<unordered_set<int>>* clauses) {
    if(clauses->size() == 0) {
        printf("∅");
    }
    for(int i = 0; i < clauses->size(); i++) {
        printf("(");
        if((*clauses)[i].size() == 0) {
            printf("□");
        } else {
            for(const auto& literal : (*clauses)[i]) {
                if(literal != *(*clauses)[i].begin()) {
                    printf(" ∨ ");
                }
                if(literal < 0) {
                    printf("¬");
                }
                printf("%d", abs(literal));
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
bool dpll(vector<unordered_set<int>> clauses) {
    unit_propagation(&clauses);

    if(clauses.size() == 0) {
        return true;
    }
    for(int i = 0; i < clauses.size(); i++) {
        if(clauses[i].size() == 0) {
            return false;
        }
    }

    clauses.push_back(unordered_set<int>());
    int literal = *clauses[0].begin();
    clauses[clauses.size() - 1].insert(literal);
    if(verbose) {
        printf("Add unit clause ");
        if(literal < 0) {
            printf("¬");
        }
        printf("%d:\n", abs(literal));
    }
    bool sat = dpll(clauses);
    if(sat) {
        return true;
    } else {
        if(verbose) {
            printf("Backtrack, add unit clause ");
            if(literal > 0) {
                printf("¬");
            }
            printf("%d:\n", abs(literal));
        }
        clauses[clauses.size() - 1].erase(literal);
        clauses[clauses.size() - 1].insert(-literal);
        return dpll(clauses);
    }
}

// Unit propagation algorithm, halts propagation and returns current clauses if clause set contains empty clause
void unit_propagation(vector<unordered_set<int>>* clauses) {
    bool unsat = false;
    bool propagated;
    do {
        if(verbose) {
            print_clauses(clauses);
        }
        if(unsat) {
            return;
        }
        propagated = false;
        for(int i = clauses->size() - 1; i >= 0; i--) {
            if((*clauses)[i].size() == 1) {
                int literal = *(*clauses)[i].begin();
                if(verbose) {
                    printf("Propagate ");
                    if(literal < 0) {
                        printf("¬");
                    }
                    printf("%d:\n", abs(literal));
                }
                for(int j = 0; j < clauses->size(); j++) {
                    if((*clauses)[j].count(literal)) {
                        clauses->erase(clauses->begin() + j--);
                        continue;
                    } else if ((*clauses)[j].count(-literal)) {
                        (*clauses)[j].erase(-literal);
                        if((*clauses)[j].size() == 0) {
                            if(!verbose) {
                                return;
                            }
                            unsat = true;
                        }
                    }
                }
                propagated = true;
                break;
            }
        }
    } while(propagated);
}

// Tautology elimination algorithm, returns true if tautology is removed
void tautology_elimination(vector<unordered_set<int>>* clauses) {
    bool removed = false;
    for(int i = 0; i < clauses->size(); i++) {
        for(const auto& literal : (*clauses)[i]) {
            if((*clauses)[i].count(-literal)) {
                clauses->erase(clauses->begin() + i--);
                removed = true;
                break;
            }
        } 
    }
    if(verbose) {
        if(removed) {
            printf("Removed tautologies.\n");
        } else {
            printf("No tautologies found.\n");
        }
    }
}