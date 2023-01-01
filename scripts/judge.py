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
    return parser.parse_args()

if __name__ == "__main__":
    args = parse_args()

    testcases = os.listdir(args.t)
    testcases = [file for file in testcases if file.endswith(".in")]
    testcases.sort(key=lambda x: int(x[:-3]))

    for test in testcases:
        print(colored(f"Testcase: {test}", "yellow"))
        start = time.time()
        subprocess.run([f"{args.e} {args.t}/{test} tmp.out"], shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        diff = time.time() - start
        print(colored(f"Time: {diff:.3f} seconds", "white"))
        cmp_files(f"{args.t}/{test[:-3]}.out", "tmp.out")
        print()
        
    subprocess.run(["rm -rf tmp.out"], shell=True)
