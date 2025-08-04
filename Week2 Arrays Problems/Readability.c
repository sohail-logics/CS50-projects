#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(void)
{
    string sen = get_string("enter sentence: ");

    int letters = 0;
    for(int i = 0; i < strlen(sen); i++)
    {
        if(isalpha(sen[i]))
        {
            letters++;
        }
    }

    int words = 0;
    for(int i = 0; i < strlen(sen); i++)
    {
        if(sen[i] == ' ')
        {
            words++;
        }

    }
    words++;


    int tsen = 0;
     for(int i = 0; i < strlen(sen); i++)
    {
        if(sen[i] == '!' || sen[i] == '.' || sen[i] == '?')
        {
            tsen++;
        }
    }

   float l = ((float)letters/words) * 100;
   float s = ((float)tsen/words) * 100;

   float grade = round(0.0588 * l - 0.296 * s - 15.8);
   int index = (int) grade;

   if(index < 1)
   {
    printf("Before Grade 1\n");
   }
   else if(index == 1)
   {
    printf("Grade 1\n");
   }
   else if(index == 2)
   {
    printf("Grade 2\n");
   }
   else if(index == 3)
   {
    printf("Grade 3\n");
   }
   else if(index == 4)
   {
    printf("Grade 4\n");
   }
   else if(index == 5)
   {
    printf("Grade 5\n");
   }
   else if(index == 6)
   {
    printf("Grade 6\n");
   }
   else if(index == 7)
   {
    printf("Grade 7\n");
   }
   else if(index == 8)
   {
    printf("Grade 8\n");
   }
   else if(index == 9)
   {
    printf("Grade 9\n");
   }
   else if(index == 10)
   {
    printf("Grade 10\n");
   }
   else if(index == 11)
   {
    printf("Grade 11\n");
   }
   else if(index == 12)
   {
    printf("Grade 12\n");
   }
   else if(index == 13)
   {
    printf("Grade 13\n");
   }
   else if(index == 14)
   {
    printf("Grade 14\n");
   }
   else if(index == 15)
   {
    printf("Grade 15\n");
   }
   else if(index == 16)
   {
    printf("Grade 16\n");
   }
   else
   {
    printf("Grade 16+\n");
   }

}
