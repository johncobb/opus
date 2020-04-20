import os
import csv
import sys
import pandas
from util import ValidateVIN

# python ml.py training/vin_master_origin

def footer():
     print("")


def pandas_build(file1, filter=None):

    # load csv file1
    frame_one = pandas.read_csv(file1, sep='\t', header=None, names=['vin', 'make'], usecols=['vin', 'make'])
    # print("csv file1 head: ", file1)
    # print(frame_one.describe())
    # print(frame_one.head())
    footer()
    
    # sort ascending first by make then by vin
    frame_one.sort_values(by=['vin'], ascending=True)

    
    #export_frame(frame_one, filter)
    filter_frame(frame_one, filter)
    

def filter_frame(frame_one, filter=None):

    index_vin = 1
    index_make = 2

    for row in frame_one.itertuples():
        try:
            vin = row[index_vin]
            make = row[index_make]
            wmi = row[index_vin][0:3]

            vin_result = ValidateVIN(vin)
            if vin_result[0]:
                if filter is None:
                    print(vin)
                else:
                    if wmi in filter:
                        print(vin)
        except csv.Error as e:
            print(e)

# cat vinValidation.txt | grep False > invalidVins.txt
# cat vinValidation.txt | grep True > validVins.txt
# python ml.py > vinValidation.txt

"""
running: 
activate virtual environment
. /env/bin/activate

python3 build_dataset.py vin_dataset_master > output
"""


if __name__ == "__main__":

    file = sys.argv[1]
    # print("Welcome to the build_dataset utility.")
    # print("file: ", file)

    """ example below is a list of all valid lexus wmi identifiers"""
    wmi_filter = ['JTH', 'JT8', '58A', '2T2', '1TH', 'JTJ', 'JLJ', 
                  '1T8', 'JTB', 'JTN', 'JT6', 'JYJ', '2TZ', 'JT1', 
                  '2TS', '1TJ']

    pandas_build(file, wmi_filter)





    
 
