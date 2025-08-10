import csv
import sys

def main():
    # Check correct number of command-line arguments
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    # Read database file into a list of dicts
    database = []
    with open(sys.argv[1], newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        str_sequences = reader.fieldnames[1:]  # skip 'name'
        for row in reader:
            # Convert STR counts from strings to integers
            for str_seq in str_sequences:
                row[str_seq] = int(row[str_seq])
            database.append(row)

    # Read DNA sequence file into a string
    with open(sys.argv[2], 'r') as f:
        dna_sequence = f.read().strip()

    # Find longest match of each STR in DNA sequence
    str_counts = {}
    for str_seq in str_sequences:
        str_counts[str_seq] = longest_match(dna_sequence, str_seq)

    # Check database for matching profile
    for person in database:
        if all(person[str_seq] == str_counts[str_seq] for str_seq in str_sequences):
            print(person['name'])
            return

    print("No match")


def longest_match(sequence, subsequence):
    """
    Returns length of longest run of subsequence in sequence.
    """
    longest_run = 0
    subseq_len = len(subsequence)
    seq_len = len(sequence)

    for i in range(seq_len):
        count = 0
        while True:
            start = i + count * subseq_len
            end = start + subseq_len
            if sequence[start:end] == subsequence:
                count += 1
            else:
                break
        longest_run = max(longest_run, count)

    return longest_run


if __name__ == "__main__":
    main()
