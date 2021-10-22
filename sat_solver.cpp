#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

using namespace std;

void print_clauses(vector<vector<int>>*);

int main(int argc, char** argv) {
    // Check arguments
    if(argc == 1 || !strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
        printf("Usage: ./sat_solver clauses...\n\nOptions:\n  -clauses: use numbers for literals, minus sign represents negation\n            0 indicates the end of a clause\n\nExample, (1 ∨ ¬2) ∧ (¬2 ∨ 3):\n  ./sat_solver 1 -2 0 -2 3 0\n");
        return -1;
    }

    // Create vector of clauses from arguments
    vector<vector<int>> clauses {vector<int>()};
    for(int i = 1; i < argc; i++) {
        int literal = atoi(argv[i]);
        if(literal == 0) {
            if(i != argc - 1) {
                clauses.push_back(vector<int>());
            }
        } else {
            clauses[clauses.size() - 1].push_back(literal);
        }
    }

    print_clauses(&clauses);

    return 0;
}

// Print clauses using mathematical notation
void print_clauses(vector<vector<int>>* clauses) {
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