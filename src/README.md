# Source Code

## How to Compile

First, enter the directory representing that parallel implementation.

Second, type in `make`.

e.g.

```shell
cd pthread
make
```

## How to run

First, enter the directory representing that parallel implementation.

Second, type in the following command:

```shell
./mm_[PARALLEL_TYPE] [INPUT_FILE] [OUTPUT_FILE]
```

e.g.

The following command will run your code and read [testcase 100.in](../testcases/100.in) as input and output the result to **tmp.out**.

```shell
cd pthread
./mm_pthread ../../testcases/100.in tmp.out
```

## How to Verify

First, enter the directory representing that parallel implementation.

Second, type in the following command:

```shell
make verify [TESTCASE]
```

e.g.

The following commands will verify your pthread version implementation on [testcase 100.in](../testcases/100.in).

```shell
cd pthread
make verify 100
```

## How to Judge

First, enter the directory representing that parallel implementation.

Second, type in the following command:

```shell
make judge
```

e.g.

The following commands will verify your pthread version implementation on all testcases.

```shell
cd pthread
make judge
```
