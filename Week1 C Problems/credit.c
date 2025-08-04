#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long number;
    do
    {
        number = get_long("Enter your card number: ");
    }
    while (number < 0);

    int sum = 0;
    int count = 0;
    long n = number;

    while (n > 0)
    {
        int digit = n % 10;

        if (count % 2 == 0)
        {
            sum += digit;
        }
        else
        {
            int doubled = digit * 2;
            if (doubled > 9)
            {
                sum += (doubled % 10) + (doubled / 10);
            }
            else
            {
                sum += doubled;
            }
        }

        n /= 10;
        count++;
    }

    long start = number;
    while (start >= 100)
    {
        start /= 10;
    }

    if (sum % 10 == 0)
    {
        if ((start == 34 || start == 37) && count == 15)
        {
            printf("AMEX\n");
        }
        else if ((start >= 51 && start <= 55) && count == 16)
        {
            printf("MASTERCARD\n");
        }
        else if ((start / 10 == 4) && (count == 13 || count == 16))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }

    return 0;
}
