#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n;
    do
    {
        n = get_int("enter number between 1 to 9: ");
    }
    while (n < 1 || n > 8);

    for (int row = 0; row < n; row++)
    {
        for (int space = n; space > row + 1; space--)
        {
            printf(" ");
        }
        for (int hash = 0; hash < row + 1; hash++)
        {
            printf("#");
        }

        printf("  ");

        for (int lhash = 0; lhash < row + 1; lhash++)
        {
            printf("#");
        }

        printf("\n");
    }

    return 0;
}
