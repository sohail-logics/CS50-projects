#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    string name = get_string("Enter your name: ");

    printf("hello, %s\n", name);
}
