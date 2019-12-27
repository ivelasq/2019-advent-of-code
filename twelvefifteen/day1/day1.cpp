#include <stdio.h>
#include <stdlib.h>

#define EatEOL(At) while(*(At) == '\n') {*(At)++;}

static char *
ReadEntireFile(const char *FileName)
{
    char *Result = 0;

    FILE *File = fopen(FileName, "r");
    if(File)
    {
        fseek(File, 0, SEEK_END);
        long FileSize = ftell(File);
        fseek(File, 0, SEEK_SET);

        Result = (char *)malloc(FileSize + 1);
        fread(Result, FileSize, 1, File);
        Result[FileSize] = '\0';

        fclose(File);
    }

    return(Result);
}

static int
ToFuel(int Mass)
{
    int Result = ((Mass / 3) - 2);

    return(Result);
}

int
main(int ArgCount, char **Args)
{
    char *Masses = ReadEntireFile("day1_input.txt");
    if(Masses)
    {
        int TotalFuel = 0;

        char *MassesAt = Masses;
        while(*MassesAt)
        {
            EatEOL(MassesAt);

            int MassBufferCursor = 0;
            char MassBuffer[256];

            while(*MassesAt &&
                  (*MassesAt != '\n'))
            {
                MassBuffer[MassBufferCursor++] = *MassesAt++;
            }
            MassBuffer[MassBufferCursor++] = '\0';

            int FuelForModule = 0;

            int Mass = atoi(MassBuffer);
            for(;;)
            {
                int RequiredFuel = ToFuel(Mass);
                if(RequiredFuel > 0)
                {
                    FuelForModule += RequiredFuel;
                    Mass = RequiredFuel;
                }
                else
                {
                    break;
                }
            }

            TotalFuel += FuelForModule;
        }

        printf("Total fuel (part 2): %d\n", TotalFuel);
    }

    return(0);
}
