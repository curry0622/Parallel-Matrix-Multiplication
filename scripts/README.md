# Scripts

## Generate Test Cases

It will randomly generate matrix A with dimension **[M]x[N]** and matrix B with dimension **[N]x[L]**.

The output content will be stored in **[P]/[M*L].in**. **[P]** has a default value, which is the path of the current directory.

```shell
python3 gen.py -m [M] -n [N] -l [L] [Optional: -p [P]]
```

Use the following command to see each parameter's info.

```shell
python3 gen.py -h
```

**Sample execution:**

```shell
python3 gen.py -m 5 -n 4 -l 3 -p ../testcases
```

In [../testcases/15.in](../testcases/), you will see something like:

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

## Verify Output

It will check whether file_1 **[F1]** is the same as file_2 **[F2]** or not.

The default value of **[D]** is 0. Set **[D]** to 1 if you want to see more debug messages.

```shell
python3 verify.py -f1 [F1] -f2 [F2] [Optional: -d [D]]
```

Use the following command to see each parameter's info.

```shell
python3 verify.py -h
```

**Sample execution:**

```shell
python3 verify.py -f1 testcase.out -f2 yours.out -d 1
```

It will output "Correct!" if two files are exactly the same.

Otherwise, it will output "Wrong!" and some debug messages.

## Judge All Outputs

It will run your executable file **[E]** and check whether your outputs are same with all the testcases in the specified folder **[T]** or not.

```shell
python3 judge.py -e [E] -t [-T]
```

Use the following command to see each parameter's info.

```shell
python3 judge.py -h
```

**Sample execution:**

```shell
python3 judge.py -e ../src/seq/mm_seq -t ../testcases
```
