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

In this directory, enter the following command:

```shell
./mm_seq [INPUT_FILE] [OUTPUT_FILE]
```

e.g.

The following command will run your code and read [testcase 100.in](../testcases/100.in) as input and output the result to **tmp.out**.

```shell
./mm_seq ../../testcases/100.in tmp.out
```

## How to Verify

In this directory, enter the following command:

```shell
make verify [TESTCASE]
```

e.g.

The following commands will verify your code on [testcase 100.in](../testcases/100.in).

```shell
make verify 100
```

Note: If you encounter permission error, please go to [scripts directory](../../scripts/) and enter the following command:

```shell
chmod 700 verify.py
```

## How to Judge

In this directory, enter the following command, it will verify your code on all testcases.

```shell
make judge
```

Note: If you encounter permission error, please go to [scripts directory](../../scripts/) and enter the following command:

```shell
chmod 700 judge.py
```
