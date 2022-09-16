import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py DATABASE.csv sequence.txt")

    # TODO: Read database file into a variable
    database = []
    db = sys.argv[1]
    with open(db) as file:
        reader = csv.reader(file)
        for person in reader:
            database.append(person)

    # TODO: Read DNA sequence file into a variable
    DNA = ''
    seq = sys.argv[2]
    with open(seq) as sequence:
        for i in sequence:
            DNA += i

    # TODO: Find longest match of each STR in DNA sequence
    STRs = []
    for i in database[0]:
        STRs.append(f'{longest_match(DNA, i)}')
    database.append(STRs)
    # TODO: Check database for matching profiles
    # go to database, for each row(name) check if is the same as STR
    # check AGATC, who matches?

    for row in database:
        counter = 0
        for i in range(len(STRs)):
            if row[i] == database[-1][i]:
                counter += 1
        if counter == (len(STRs) - 1):
            print(row[0])
            return
    print('No match')
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
