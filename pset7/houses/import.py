from cs50 import SQL
from sys import argv
import csv


db = SQL("sqlite:///students.db")

# Check command-line arguments
if len(argv) != 2:
    print("Usage: python import.py file.csv")
    exit(1)

# Open CSV file given by command-line argument

with open(argv[1]) as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        fullName = row['name'].split()
        if len(fullName) < 3:
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)",  # If there is no middle name, add None instead
                       fullName[0], None, fullName[1], row["house"], row["birth"])
        else:
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)",  # IF there is a middle name add it
                       fullName[0], fullName[1], fullName[2], row["house"], row["birth"])