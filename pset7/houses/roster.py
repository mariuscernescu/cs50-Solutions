from cs50 import SQL
from sys import argv


db = SQL("sqlite:///students.db")
# Check command-line arguments
if len(argv) != 2:
    print("Usage: python roster.py house")
    exit(1)

# Query database for all students in house
# Use db.execute to SELECT rows from t
results = db.execute("SELECT * FROM students WHERE house = ? ORDER BY last, first", argv[1])
for row in results:
    if not row['middle']:
        print(f"{row['first']} {row['last']}, born {row['birth']}")
    else:
        print(f"{row['first']} {row['middle']} {row['last']}, born {row['birth']}")

# Return value will be a list of Python dicts, where each dict represents a row in the table
# Remember to check for NULL values for middle names
# Print out each student's full name and birth year
# Students sould be sorted alphabetically bu last name, then first name
