import argparse
import filecmp
from difflib import Differ

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-f1", type=str, required=True, help="File 1")
    parser.add_argument("-f2", type=str, required=True, help="File 2")
    parser.add_argument("-d", type=int, default=0, help="Debug mode, 0: false, 1: true")
    return parser.parse_args()

def cmp_files(f1, f2):
    same = filecmp.cmp(f1, f2)
    if(same):
        print("Correct!")
    else:
        print("Wrong!")
    return same

def diff_files(f1, f2):
    same = cmp_files(f1, f2)
    if(same):
        return
    else:
        with open(f1) as file_1, open(f2) as file_2:
            differ = Differ()
            for line in differ.compare(file_1.readlines(), file_2.readlines()):
                print(line)

if __name__ == "__main__":
    args = parse_args()
    f1 = args.f1
    f2 = args.f2
    debug = args.d
    print(f"Comparing {f1} and {f2}, debug mode: {debug}")
    
    if(debug):
        diff_files(f1, f2)
    else:
        cmp_files(f1, f2)
