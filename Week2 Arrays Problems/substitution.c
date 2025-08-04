#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

bool validate_key(string key);
string substitute(string key, string plaintext);

int main(int argc, string argv[])
{
    // Check for exactly one command-line argument
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string key = argv[1];

    // Validate the key
    if (!validate_key(key))
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // Get plaintext from user
    string plaintext = get_string("plaintext: ");

    // Encrypt using substitution
    string ciphertext = substitute(key, plaintext);

    // Print result
    printf("ciphertext: %s\n", ciphertext);
    return 0;
}

bool validate_key(string key)
{
    if (strlen(key) != 26)
    {
        return false;
    }

    bool seen[26] = {false};

    for (int i = 0; i < 26; i++)
    {
        if (!isalpha(key[i]))
        {
            return false;
        }

        int index = toupper(key[i]) - 'A';

        if (seen[index])
        {
            return false; // Duplicate letter
        }

        seen[index] = true;
    }
    return true;
}

string substitute(string key, string plaintext)
{
    int n = strlen(plaintext);
    static char ciphertext[1000]; // Enough buffer

    for (int i = 0; i < n; i++)
    {
        char c = plaintext[i];

        if (isupper(c))
        {
            ciphertext[i] = toupper(key[c - 'A']);
        }
        else if (islower(c))
        {
            ciphertext[i] = tolower(key[c - 'a']);
        }
        else
        {
            ciphertext[i] = c; // Leave unchanged
        }
    }

    ciphertext[n] = '\0'; // Null-terminate
    return ciphertext;
}
