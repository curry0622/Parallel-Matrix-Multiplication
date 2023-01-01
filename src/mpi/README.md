# Source Code

## How to Compile

In this directory, enter the following command:

```shell
make
```

It will generate the executable file in the current directory.

If you want to remove it, please enter the following command:

```shell
make clean
```

## How to run

In this directory, enter the following command, where

* **[N]** is the number of nodes to run this program
* **[n]** is the number of total processes to launch
* **[C]** is the number of cpus available to each process
* **[INPUT_FILE]** is the input file's path
* **[OUTPUT_FILE]** is the output file's path

```shell
srun -N[N] -n[n] -c[C] ./mm_mpi [INPUT_FILE] [OUTPUT_FILE]
```

**Sample execution:**

The following command will run your code using 2 nodes, 4 processes per node, 8 cores per process and read [testcase 100.in](../testcases/100.in) as input and output the result to **tmp.out**.

```shell
srun -N2 -n4 -c8 ./mm_mpi ../../testcases/100.in tmp.out
```

## How to Verify

In this directory, enter the following command, where [TESTCASE] specifies which testcase to run and other parameters are same with the above mentioned.

```shell
make verify t=[TESTCASE] N=[N] n=[n] c=[C]
```

**Sample execution:**

The following command will verify your code on [testcase 100.in](../testcases/100.in) using 2 nodes, 4 processes per node, and 8 cores per process.

```shell
make verify t=100.in N=2 n=4 c=8
```

Note 1: If you encounter permission error, please go to [scripts directory](../../scripts/) and enter the following command:

```shell
chmod 700 verify.py
```

Note 2: If you want to use local test cases, you should modify `TESTCASE_PATH` in [Makefile](./Makefile).

## How to Judge

In this directory, enter the following command, where each parameter is same with the above mentioned.

```shell
make judge N=[N] n=[n] c=[C]
```

**Sample execution:**

The following command will verify your code on all testcases using 2 nodes, 4 processes per node, and 8 cores per process.

```shell
make judge N=2 n=4 c=8
```

Note 1: If you encounter permission error, please go to [scripts directory](../../scripts/) and enter the following command:

```shell
chmod 700 judge.py
```

Note 2: If you want to use local test cases, you should modify `TESTCASE_PATH` in [Makefile](./Makefile).
