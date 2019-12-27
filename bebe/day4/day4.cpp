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
ContainsEqualNeighbors(int *Digits)
{
    bool Result = false;

    for(int DigitIndex = 0;
        DigitIndex < (PASSWORD_DIGITS - 1);
        DigitIndex++)
    {
        int Digit = Digits[DigitIndex];
        int NextDigit = Digits[(DigitIndex + 1)];
        if(Digit == NextDigit)
        {
            Result = true;
            break;
        }
    }

    return(Result);
}

static bool
PositivelyMonotonic(int *Digits)
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
        if(ContainsEqualNeighbors(PasswordDigits))
        {
            if(PositivelyMonotonic(PasswordDigits))
            {
                ValidPasswordCount++;
            }
        }
    }
    printf("Found %d valid passwords\n", ValidPasswordCount);

    return(0);
}
