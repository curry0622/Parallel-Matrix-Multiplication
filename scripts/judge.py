import os
import time
import argparse
import subprocess
from utils import colored_text
from verify import cmp_files

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-e", type=str, required=True, help="Path of the executable file")
    parser.add_argument("-t", type=str, required=True, help="Path of the testcases folder")
    parser.add_argument("-N", type=int, default=1, help="srun -N")
    parser.add_argument("-n", type=int, default=1, help="srun -n")
    parser.add_argument("-c", type=int, default=1, help="srun -c")
    parser.add_argument("-gpu", type=int, default=0, help="default 0, 1 for using gpu")
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
    file.write("testcase,-N,-n,-c,io_t,mul_t,total_t,correct\n")

    # Run testcases
    for test in testcases:
        colored_text(f"Testcase: {test}", "yellow")
        if(args.gpu == 0):
            subprocess.run(
                [f"srun -N{args.N} -n{args.n} -c{args.c} {args.e} {args.t}/{test} tmp.out"],
                shell=True,
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL
            )
        else:
            subprocess.run(
                [f"{args.e} {args.t}/{test} tmp.out"],
                shell=True,
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL
            )
        # Read time profile from time.txt
        time_prof = open("time.txt", "r")
        io_t = float(time_prof.readline())
        mul_t = float(time_prof.readline())
        total_t = float(time_prof.readline())
        colored_text(f"io: {io_t} sec", "white")
        colored_text(f"mul: {mul_t} sec", "white")
        colored_text(f"total: {total_t} sec", "white")
        correct = cmp_files(f"{args.t}/{test[:-3]}.out", "tmp.out")
        print()
        file.write(f"{test},{args.N},{args.n},{args.c},{io_t:.3f},{mul_t:.3f},{total_t:.3f},{correct}\n")
    
    file.close()
    subprocess.run(["rm -rf tmp.out"], shell=True)
