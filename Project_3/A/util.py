import argparse

def get_args(args):
    parser = argparse.ArgumentParser(description = 'Process arguments')

    parser.add_argument("--d", help = "dataset")
    parser.add_argument("--n", help = "number of random time series selected")

    args = parser.parse_args()

    path = args.d
    n = int(args.n)
    return path, n