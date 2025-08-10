from cs50 import get_float

def main():
    # Keep prompting until user enters a valid non-negative amount
    while True:
        try:
            dollars = get_float("Change owed: ")
            if dollars >= 0:
                break
        except ValueError:
            # In case user enters something that's not a number
            continue

    # Convert dollars to cents (avoid floating-point issues)
    cents = round(dollars * 100)

    coins = 0

    # Calculate quarters
    coins += cents // 25
    cents %= 25

    # Calculate dimes
    coins += cents // 10
    cents %= 10

    # Calculate nickels
    coins += cents // 5
    cents %= 5

    # Calculate pennies
    coins += cents

    # Output the minimum number of coins
    print(coins)

if __name__ == "__main__":
    main()

