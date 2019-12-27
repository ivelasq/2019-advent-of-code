#include <stdio.h>

#include "aoc_math.h"

#define PASSWORD_DIGITS 6

static void
Expand(int *Digits, int Password)
{
    int DigitIndex = 0;
    for(int Place = (int)Pow(10, (PASSWORD_DIGITS - 1));
        Place >= 1;
        Place /= 10)
    {
        int Digit = ((Password / Place) % 10);
        Digits[DigitIndex++] = Digit;
    }
}

static void
PrintDigits(int *Digits)
{
    for(int DigitIndex = 0;
        DigitIndex < PASSWORD_DIGITS;
        DigitIndex++)
    {
        int Digit = Digits[DigitIndex];
        printf("%d ", Digit);
    }
    printf("\n");
}

static bool
ContainsEqualCouple(int *Digits)
{
    int CouplesFound = 0;
    int LongerRunsFound = 0;

    int Outer = 0;
    while(Outer < (PASSWORD_DIGITS - 1))
    {
        int StretchCount = 1;

        int DigitA = Digits[Outer];
        for(int Inner = (Outer + 1);
            Inner < PASSWORD_DIGITS;
            Inner++)
        {
            int DigitB = Digits[Inner];
            if(DigitA == DigitB)
            {
                StretchCount++;
            }
        }

        if(StretchCount == 2)
        {
            CouplesFound++;
        }
        else if(StretchCount > 2)
        {
            LongerRunsFound++;
        }

        Outer += StretchCount;
    }

    bool Result = (CouplesFound > 0);

    return(Result);
}

static bool
IsPositivelyMonotonic(int *Digits)
{
    bool Result = true;

    for(int DigitIndex = 0;
        DigitIndex < (PASSWORD_DIGITS - 1);
        DigitIndex++)
    {
        int Digit = Digits[DigitIndex];
        int NextDigit = Digits[(DigitIndex + 1)];
        if(Digit > NextDigit)
        {
            Result = false;
            break;
        }
    }    

    return(Result);
}

int
main(int ArgCount, char **Args)
{
    int ValidPasswordCount = 0;
    int PasswordDigits[PASSWORD_DIGITS];
    for(int Password = 357253;
        Password <= 892942;
        Password++)
    {
        Expand(PasswordDigits, Password);
//        PrintDigits(PasswordDigits);
        if(ContainsEqualCouple(PasswordDigits))
        {
            if(IsPositivelyMonotonic(PasswordDigits))
            {
                printf("%d\n", Password);
                ValidPasswordCount++;
            }
        }
    }
    printf("Found %d valid passwords\n", ValidPasswordCount);

    return(0);
}
