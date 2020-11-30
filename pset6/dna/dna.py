import csv
from sys import argv
from collections import defaultdict


# Open the CSV file and DNA sequence, read contents into memory

def main():
    if len(argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        exit(1)

    fileC = open(argv[1], 'r')
    readCsv = csv.DictReader(fileC)  # readCsv is now a dict containing the data from the csv file.

    f = open(argv[2], 'r')
    txtFile = f.read()  # txtFile is not the variable containing the DNA sequence.

    # For each STR, compute the longest run of consecutive repeats in the DNA sequence

    computeRepeats(txtFile)

    # Compare the STR counts against each row in the CSV file.

    compareStr(readCsv)


# For each STR, compute the longest run of consecutive repeats in the DNA sequence

def computeRepeats(text):
    global sequences

    if 'sequences' not in globals():
        sequences = {
            'AGATC': 0,
            'TTTTTTCT': 0,
            'AATG': 0,
            'TCTAG': 0,
            'GATA': 0,
            'TATC': 0,
            'GAAA': 0,
            'TCTG': 0
        }

    counter = 0

    for s in sequences:  # for every sequence
        count = 0
        pattern = s
        while pattern in text:
            count += 1
            pattern += s
        sequences[s] = count


# Compare the STR counts against each row in the CSV file.


def compareStr(csvFile):
    for row in csvFile:
        k1 = set(row.keys())
        k2 = set(sequences.keys())
        common_keys = set(k1).intersection(set(k2))
        counter = 0
        for key in common_keys:
            if not int(row[key]) == int(sequences[key]):
                counter += 1
        if counter == 0:
            print(row['name'])
            exit(0)
    print('No match')
    exit(1)


main()