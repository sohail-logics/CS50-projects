

from cs50 import get_int

# Prompt user for height until it is between 1 and 8
while True:
    height = get_int("Height: ")
    if 1 <= height <= 8:
        break

# Build half-pyramid
for row in range(1, height + 1):
    # Print spaces first
    print(" " * (height - row), end="")
    # Then print hashes
    print("#" * row)
