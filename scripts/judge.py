import os
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
        subprocess.run([f"{args.e} {args.t}/{test} tmp.out"], shell=True)
        cmp_files(f"{args.t}/{test[:-3]}.out", "tmp.out")
        print()
        
    subprocess.run(["rm -rf tmp.out"], shell=True)
