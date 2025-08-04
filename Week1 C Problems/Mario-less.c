#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int h;
    do
    {
        h = get_int("enter number between 1 to 9: ");
    }
    while (h < 1 || h > 8);

    for (int row = 0; row < h; row++)
    {
        for (int space = h - 1; space > row; space--)
        {
            printf(" ");
        }
        for (int hash = 0; hash <= row; hash++)
        {
            printf("#");
        }
        printf("\n");
    }

    return 0;
}
