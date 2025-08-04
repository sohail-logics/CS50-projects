#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    if(argc != 2)
    {
        printf("Usage: ./caeser key \n");
        return 1;
    }

    for(int i = 0; i < strlen(argv[1]); i++)
    {
        if(!isdigit(argv[1][i]))
        {
            printf("Usage: ./caeser key\n");
            return 1;
        }

    }
     int k = atoi(argv[1]);
       if (k < 0)
    {
        printf("Key must be a non-negative integer.\n");
        return 1;
    }


    string plaintext = get_string("plaintext: ");
    printf("ciphertext: ");


    for (int i = 0; i < strlen(plaintext); i++)
    {
        char c = plaintext[i];

        if (isupper(c))
        {
            printf("%c", ((c - 'A' + k) % 26) + 'A');
        }
        else if (islower(c))
        {
            printf("%c", ((c - 'a' + k) % 26) + 'a');
        }
        else
        {
            printf("%c", c);
        }
    }

    printf("\n");
    return 0;
}

