import argparse

from numpy import double

def get_args(args):
    parser = argparse.ArgumentParser(description = 'Process arguments')

    parser.add_argument("--d", help = "dataset")
    parser.add_argument("--n", help = "number of random time series selected")
    parser.add_argument("--mae", help = "error value as double")
    

    args = parser.parse_args()

    path = args.d
    n = int(args.n)
    nu_mae = double(args.mae)
    return path, n ,nu_mae