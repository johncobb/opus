import os
import csv
import sys
import pandas as pd

def pandas_open(file1, filter=None):

    # load csv file1
    return pd.read_csv(file1, sep='\t', header=None, names=['vin'], usecols=['vin'])
    # print("csv file1 head: ", file1)



"""
running: 
activate virtual environment
. /env/bin/activate

python3 run_sample.py output > sample_100
python3 run_sample.py output
"""
if __name__ == "__main__":

    file = sys.argv[1]

    frame = pandas_open(file, None)
    
    """ print unique wmi values for sample """
    # print(frame['vin'].str[0:3].unique())

    # print(frame['vin'][:500])

    df_sample = frame['vin'][:500]

    """ first n records """
    df_sch = df_sample[:400]
    """ last n records """
    df_scan = df_sample[400:]

    # print(df_sample.describe())
    print(df_sch.describe())
    print(df_scan.describe())

    df_merge = pd.merge(df_sch, df_scan, on=['vin'], how='inner')
    print(df_merge.describe())
    # print(df_merge)


