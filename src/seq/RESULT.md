# Result

## Naive Implementation

| Testcase    | -N | -n | -c | runtime    |
|-------------|----|----|----|------------|
| 1.in        | 1  | 1  | 1  | 0.130      |
| 4.in        | 1  | 1  | 1  | 0.055      |
| 100.in      | 1  | 1  | 1  | 0.051      |
| 10000.in    | 1  | 1  | 1  | 0.056      |
| 1000000.in  | 1  | 1  | 1  | 8.584      |
| 4000000.in  | 1  | 1  | 1  | 71.298     |
| 9000000.in  | 1  | 1  | 1  | 255.268    |
| 16000000.in | 1  | 1  | 1  | 619.415    |
| 25000000.in | 1  | 1  | 1  | TLE killed |

Note: **TLE killed** means that the program got killed because of the runtime limit on the apollo server.

## Cache Friendly Implementation

| Testcase    | -N | -n | -c | runtime |
|-------------|----|----|----|---------|
| 1.in        | 1  | 1  | 1  | 0.145   |
| 4.in        | 1  | 1  | 1  | 0.153   |
| 100.in      | 1  | 1  | 1  | 0.054   |
| 10000.in    | 1  | 1  | 1  | 0.052   |
| 1000000.in  | 1  | 1  | 1  | 0.727   |
| 4000000.in  | 1  | 1  | 1  | 4.342   |
| 9000000.in  | 1  | 1  | 1  | 13.870  |
| 16000000.in | 1  | 1  | 1  | 31.741  |
| 25000000.in | 1  | 1  | 1  | 65.889  |
