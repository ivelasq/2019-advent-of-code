#include <assert.h>
#include <stdio.h>

#define Assert assert

#define InvalidCodePath Assert(!"InvalidCodePath")
#define InvalidDefaultCase default:{InvalidCodePath;} break

#define CODE_BUFFER_LENGTH 2048

// Because I don't have a debugger :(
#define PRINT_VAR(Variable) printf(#Variable ": %d\n", Variable)

struct stream
{
    const char *At;
};

static char *
ReadEntireFileAndNullTerminate(const char *FileName)
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

inline bool
IsNumeric(char C)
{
    bool Result = (('0' <= C) && (C <= '9'));

    return(Result);
}

static int
ParseInt(stream *Stream)
{
    int BufferCount = 0;
    char Buffer[256];
    while(*Stream->At &&
          ((*Stream->At == '-') ||
           IsNumeric(*Stream->At)))
    {
        Assert(BufferCount < sizeof(Buffer));
        Buffer[BufferCount++] = *Stream->At++;
    }
    Buffer[BufferCount++] = '\0';
    int Result = atoi(Buffer);

    return(Result);
}

inline int
PlaceValue(int I, int Place)
{
    int Result = ((I / Place) % 10);

    return(Result);
}

inline bool
IsPositionMode(int Mode)
{
    bool Result = (Mode == 0);

    return(Result);
}

inline bool
IsImmediateMode(int Mode)
{
    bool Result = (Mode == 1);

    return(Result);
}

inline bool
IsValidParamMode(int Mode)
{
    bool Result = (IsPositionMode(Mode) ||
                   IsImmediateMode(Mode));
    return(Result);
}

static int
LoadParam(int *Codes, int Index, int Mode)
{
    int Result = 0;

    if(IsPositionMode(Mode))
    {
        int ParamIndex = Codes[Index];
        Result = Codes[ParamIndex];
    }
    else if(IsImmediateMode(Mode))
    {
        Result = Codes[Index];
    }
    else
    {
        InvalidCodePath;
    }

    return(Result);
}

int
main(int ArgCount, char **Args)
{
    char *Program = ReadEntireFileAndNullTerminate("day5_input.txt");
//    const char *Program = "1101,100,-1,4,0";
    if(Program)
    {
        int CodeCount = 0;
        int *Codes = (int *)malloc(sizeof(int)*CODE_BUFFER_LENGTH);

        stream Stream_;
        stream *Stream = &Stream_;
        Stream->At = Program;
        for(;;)
        {
            if((*Stream->At == '-') ||
               IsNumeric(*Stream->At))
            {
                int Code = ParseInt(Stream);
                Assert(CodeCount < CODE_BUFFER_LENGTH);
                Codes[CodeCount++] = Code;
                if(*Stream->At == ',')
                {
                    Stream->At++;
                }
            }
            else
            {
                break;
            }
        }

#if 0
        for(int CodeIndex = 0;
            CodeIndex < CodeCount;
            CodeIndex++)
        {
            printf("%d ", Codes[CodeIndex]);
        }
        printf("\n");
#endif

#if 1
        int CodeIndex = 0;
        while(CodeIndex < CodeCount)
        {
            bool Halt = false;

            int Code = Codes[CodeIndex];
            int Instruction = (Code % 100);
            switch(Instruction)
            {
                // @Assert: Enough opcodes remain for each case

                case 1:
                {
                    int Param0Mode = PlaceValue(Code, 100);
                    int Param1Mode = PlaceValue(Code, 1000);
                    int Param2Mode = PlaceValue(Code, 10000);
                    Assert(IsPositionMode(Param2Mode));

                    int Param0 = LoadParam(Codes, (CodeIndex + 1), Param0Mode);
                    int Param1 = LoadParam(Codes, (CodeIndex + 2), Param1Mode);
                    int Param2 = Codes[(CodeIndex + 3)];

                    int Result = (Param0 + Param1);
                    Codes[Param2] = Result;

                    CodeIndex += 4;
                } break;

                case 2:
                {
                    int Param0Mode = PlaceValue(Code, 100);
                    int Param1Mode = PlaceValue(Code, 1000);
                    int Param2Mode = PlaceValue(Code, 10000);
                    Assert(IsPositionMode(Param2Mode));

                    int Param0 = LoadParam(Codes, (CodeIndex + 1), Param0Mode);
                    int Param1 = LoadParam(Codes, (CodeIndex + 2), Param1Mode);
                    int Param2 = Codes[(CodeIndex + 3)];

                    int Result = (Param0*Param1);
                    Codes[Param2] = Result;

                    CodeIndex += 4;
                } break;

                case 3:
                {
                    int Param0Mode = PlaceValue(Code, 100);

                    printf("Input: ");
                    int Int;
                    fscanf(stdin, "%d", &Int);

                    int Param0 = Codes[(CodeIndex + 1)];
                    Codes[Param0] = Int;

                    CodeIndex += 2;
                } break;

                case 4:
                {
                    int Param0Mode = PlaceValue(Code, 100);

                    int Param0 = LoadParam(Codes, (CodeIndex + 1), Param0Mode);
                    printf("Output: %d\n", Param0);

                    CodeIndex += 2;
                } break;

                case 99:
                {
                    Halt = true;
                    printf("Halted successfully (99)\n");
                } break;

                InvalidDefaultCase;
            }

            if(Halt)
            {
                break;
            }
        }

#if 0
        for(int CodeIndex = 0;
            CodeIndex < CodeCount;
            CodeIndex++)
        {
            printf("%d ", Codes[CodeIndex]);
        }
        printf("\n");
#endif
#endif
    }

    return(0);
}
