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

In this directory, enter the following command, where **[C]** specifies how many cores are used to run this program.

```shell
srun -N1 -n1 -c[C] ./mm_openmp [INPUT_FILE] [OUTPUT_FILE]
```

e.g.

The following command will run your code using 12 cores and read [testcase 100.in](../testcases/100.in) as input and output the result to **tmp.out**.

```shell
srun -N1 -n1 -c12 ./mm_openmp ../../testcases/100.in tmp.out
```

## How to Verify

In this directory, enter the following command, where **[C]** specifies how many cores are used to run this program.

```shell
make verify [TESTCASE] c=[C]
```

e.g.

The following commands will verify your code on [testcase 100.in](../testcases/100.in) using 12 cores.

```shell
make verify 100 c=12
```

Note: If you encounter permission error, please go to [scripts directory](../../scripts/) and enter the following command:

```shell
chmod 700 verify.py
```

## How to Judge

In this directory, enter the following command, where **[C]** specifies how many cores are used to run this program (i.e. `srun -c`).

```shell
make judge c=[C]
```

e.g.

The following commands will verify your code on all testcases using 12 cores.

```shell
make judge c=12
```

Note: If you encounter permission error, please go to [scripts directory](../../scripts/) and enter the following command:

```shell
chmod 700 judge.py
```
