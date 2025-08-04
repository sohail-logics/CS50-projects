#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int cent;
    // it will take only positive value
    do
    {
        cent = get_int("change owed: ");
    }
    while (cent < 0);

    int coin = 0;

    coin += cent / 25;
    cent %= 25;
    coin += cent / 10;
    cent %= 10;
    coin += cent / 5;
    cent %= 5;
    coin += cent;

    printf("coins needed : %i\n", coin);
}
