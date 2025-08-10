# credit.py
from cs50 import get_string

def luhn_checksum(card_number):
    total = 0
    reverse_digits = card_number[::-1]

    for i, digit in enumerate(reverse_digits):
        n = int(digit)
        if i % 2 == 1:  # every second digit from the right (0-indexed from reversed)
            n *= 2
            if n > 9:
                n -= 9
        total += n
    return total % 10 == 0

def main():
    card_number = get_string("Number: ")

    # Validate using Luhn’s Algorithm
    if not luhn_checksum(card_number):
        print("INVALID")
        return

    length = len(card_number)
    first_two = int(card_number[:2])
    first_one = int(card_number[0])

    # Check card type
    if length == 15 and first_two in [34, 37]:
        print("AMEX")
    elif length == 16 and 51 <= first_two <= 55:
        print("MASTERCARD")
    elif length in [13, 16] and first_one == 4:
        print("VISA")
    else:
        print("INVALID")

if __name__ == "__main__":
    main()
