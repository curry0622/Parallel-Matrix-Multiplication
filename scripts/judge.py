import os
import time
import argparse
import subprocess
from termcolor import colored
from verify import cmp_files

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-e", type=str, required=True, help="Path of the executable file")
    parser.add_argument("-t", type=str, required=True, help="Path of the testcases folder")
    parser.add_argument("-N", type=int, default=1, help="srun -N")
    parser.add_argument("-n", type=int, default=1, help="srun -n")
    parser.add_argument("-c", type=int, default=1, help="srun -c")
    return parser.parse_args()

if __name__ == "__main__":
    # Parse arguments
    args = parse_args()

    # Sort testcases filenames
    testcases = os.listdir(args.t)
    testcases = [file for file in testcases if file.endswith(".in")]
    testcases.sort(key=lambda x: int(x[:-3]))
    
    # Write header to result.csv
    file = open("result.csv", "w")
    file.write("testcase,-N,-n,-c,runtime,correct\n")

    # Run testcases
    for test in testcases:
        print(colored(f"Testcase: {test}", "yellow"))
        start = time.time()
        subprocess.run(
            [f"srun -N{args.N} -n{args.n} -c{args.c} {args.e} {args.t}/{test} tmp.out"],
            shell=True,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL
        )
        runtime = time.time() - start
        print(colored(f"Runtime: {runtime:.3f} seconds", "white"))
        correct = cmp_files(f"{args.t}/{test[:-3]}.out", "tmp.out")
        print()
        file.write(f"{test},{args.N},{args.n},{args.c},{runtime:.3f},{correct}\n")
    
    file.close()
    subprocess.run(["rm -rf tmp.out"], shell=True)
