# readability.py
from cs50 import get_string

def count_letters(text):
    return sum(1 for c in text if c.isalpha())

def count_words(text):
    # Words are separated by spaces
    return len(text.split())

def count_sentences(text):
    # Sentences end with ., !, or ?
    return sum(1 for c in text if c in ['.', '!', '?'])

def main():
    text = get_string("Text: ")

    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # Coleman-Liau index
    L = (letters / words) * 100
    S = (sentences / words) * 100
    index = round(0.0588 * L - 0.296 * S - 15.8)

    if index < 1:
        print("Before Grade 1")
    elif index >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {index}")

if __name__ == "__main__":
    main()
