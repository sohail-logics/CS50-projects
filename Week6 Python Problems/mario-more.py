from cs50 import get_int

# Prompt until valid height (1-8)
while True:
    height = get_int("Height: ")
    if 1 <= height <= 8:
        break

# Build pyramid
for row in range(1, height + 1):
    # Left pyramid: spaces then hashes
    print(" " * (height - row) + "#" * row, end="")

    # Gap
    print("  ", end="")

    # Right pyramid: hashes (same as left pyramid’s hashes)
    print("#" * row)
