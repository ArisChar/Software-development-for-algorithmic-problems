import argparse

def get_args(args):
    parser = argparse.ArgumentParser(description = 'Process arguments')

    parser.add_argument("--d", help = "dataset")
    parser.add_argument("--q", help = "queryset")
    parser.add_argument("--od", help = "output_dataset_file")
    parser.add_argument("--oq", help = "output_query_file")

    args = parser.parse_args()

    path = args.d
    query = args.q
    outd = args.od
    outq = args.oq
    return path, query, outd, outq