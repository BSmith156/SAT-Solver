# SAT-Solver
Command line program to determine satisfiability of k-clause sets.

## Setup
### 1. Clone Repo
`git clone https://github.com/BSmith156/SAT-Solver.git`

### 2. Compile
`g++ sat_solver.cpp -o sat_solver`

## Usage
`./sat_solver [-v] clauses...`

### Arguments
| Argument | Description |
| -------- | ----------- |
| -v | Optional. Enable verbose mode. |
| clauses | Use numbers for literals, minus sign represents negation. 0 indicates the end of a clause. |

## Example
### (1 ∨ ¬2) ∧ (¬2 ∨ 3)
`./sat_solver 1 -2 0 -2 3 0`

## Authors
* [Ben Smith](https://github.com/BSmith156)
