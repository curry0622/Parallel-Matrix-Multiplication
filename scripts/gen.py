import argparse
import random

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-m", default=5, type=int, help="# rows of matrix A")
    parser.add_argument("-n", default=5, type=int, help="# columns of matrix A")
    parser.add_argument("-l", default=5, type=int, help="# columns of matrix B")
    parser.add_argument("-p", default=".", type=str, help="where to output the test case")
    return parser.parse_args()

def print_info(args):
    print(f"Test case: {args.p}/{args.m * args.l}.in")
    print(f"Dim of A: {args.m}x{args.n}")
    print(f"Dim of B: {args.n}x{args.l}")

def gen_random_mtx(m, n):
    return [[random.randint(0, 100) for _ in range(n)] for _ in range(m)]

def output(args, A, B):
    # Output m, n, l
    fout = open(f"{args.p}/{args.m * args.l}.in", "w")
    fout.write(f"{args.m} {args.n} {args.l}\n")

    # Output mtx A
    for row in A:
        for el in row:
            fout.write(f"{el} ")
        fout.write("\n")
        
    # Output mtx B
    for row in B:
        for el in row:
            fout.write(f"{el} ")
        fout.write("\n")

if __name__ == "__main__":
    args = parse_args()
    print_info(args)
    
    A = gen_random_mtx(args.m, args.n)
    B = gen_random_mtx(args.n, args.l)
    
    output(args, A, B)
    print("Done")
