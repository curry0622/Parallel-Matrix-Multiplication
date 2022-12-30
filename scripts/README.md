# Scripts

## Generate Test Cases

It will randomly generate matrix A with dimension MxN and matrix B with dimension NxL.

The output content will be stored in [testcases/M_N_L.in](../testcases/).

```shell
python3 gen.py -m [M] -n [N] -l [L]
```

e.g.

```shell
python3 gen.py -m 5 -n 4 -l 3
```

In [testcases/5_4_3.in](../testcases/), you will see below content:

```txt
5 4 3
73 30 71 83 
73 97 67 96 
25 69 88 49 
67 1 18 19 
52 25 53 9 
42 52 12 
27 47 61 
22 27 89 
2 92 75 
```
