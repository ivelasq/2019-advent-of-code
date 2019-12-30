#include <assert.h>
#include <stdio.h>

#define Assert assert

#define InvalidCodePath Assert(!"InvalidCodePath")
#define InvalidDefaultCase default:{InvalidCodePath;} break

#define ArrayCount(Array) (sizeof((Array)) / sizeof((Array)[0]))

#define CODE_BUFFER_LENGTH 4096

// Because I don't have a debugger :(
#define PRINT_VAR(Variable) printf(#Variable ": %d\n", Variable)

typedef unsigned char u8;

typedef intptr_t smm;

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

inline void
Copy(smm Count, void *SourceInit, void *DestInit)
{
    u8 *Source = (u8 *)SourceInit;
    u8 *Dest = (u8 *)DestInit;
    while(Count--)
    {
        *Dest++ = *Source++;
    }
}

static bool
HasUniqueDigitsBetweenZeroAndFour(int Value)
{
    int Used[10] = {};

    int OnesDigit = ((Value / 1) % 10);
    int TensDigit = ((Value / 10) % 10);
    int HundredsDigit = ((Value / 100) % 10);
    int ThousandsDigit = ((Value / 1000) % 10);
    int TenThousandsDigit = ((Value / 10000) % 10);

    Used[OnesDigit]++;
    Used[TensDigit]++;
    Used[HundredsDigit]++;
    Used[ThousandsDigit]++;
    Used[TenThousandsDigit]++;

    bool Result = ((Used[0] == 1) &&
                   (Used[1] == 1) &&
                   (Used[2] == 1) &&
                   (Used[3] == 1) &&
                   (Used[4] == 1));
    return(Result);
}

int
main(int ArgCount, char **Args)
{
    const char *Program = "3,8,1001,8,10,8,105,1,0,0,21,34,59,76,101,114,195,276,357,438,99999,3,9,1001,9,4,9,1002,9,4,9,4,9,99,3,9,102,4,9,9,101,2,9,9,102,4,9,9,1001,9,3,9,102,2,9,9,4,9,99,3,9,101,4,9,9,102,5,9,9,101,5,9,9,4,9,99,3,9,102,2,9,9,1001,9,4,9,102,4,9,9,1001,9,4,9,1002,9,3,9,4,9,99,3,9,101,2,9,9,1002,9,3,9,4,9,99,3,9,101,2,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,1,9,4,9,3,9,102,2,9,9,4,9,3,9,101,1,9,9,4,9,3,9,1001,9,1,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,2,9,4,9,3,9,102,2,9,9,4,9,99,3,9,101,2,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,1,9,4,9,3,9,101,2,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,101,1,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,101,2,9,9,4,9,3,9,1001,9,1,9,4,9,99,3,9,102,2,9,9,4,9,3,9,1001,9,1,9,4,9,3,9,1001,9,1,9,4,9,3,9,102,2,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,101,1,9,9,4,9,3,9,101,2,9,9,4,9,99,3,9,1002,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,101,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,101,2,9,9,4,9,3,9,101,2,9,9,4,9,99,3,9,1002,9,2,9,4,9,3,9,1001,9,1,9,4,9,3,9,101,2,9,9,4,9,3,9,101,2,9,9,4,9,3,9,101,2,9,9,4,9,3,9,101,1,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,1,9,4,9,3,9,101,2,9,9,4,9,99";

    int CodeCount = 0;
    int *Codes = (int *)malloc(sizeof(int)*CODE_BUFFER_LENGTH);
    int *CodesInit = (int *)malloc(sizeof(int)*CODE_BUFFER_LENGTH);

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
            CodesInit[CodeCount++] = Code;
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

    int HighestSignal = 0;

    for(int Number = 01234;
        Number <= 43210;
        Number++)
    {
        if(HasUniqueDigitsBetweenZeroAndFour(Number))
        {
            Copy((sizeof(int)*CodeCount), CodesInit, Codes);

            int PhaseSettings[5];
            PhaseSettings[0] = ((Number / 10000) % 10);
            PhaseSettings[1] = ((Number / 1000) % 10);
            PhaseSettings[2] = ((Number / 100) % 10);
            PhaseSettings[3] = ((Number / 10) % 10);
            PhaseSettings[4] = ((Number / 1) % 10);

            int PrevOutput = 0;
            for(int PhaseSettingIndex = 0;
                PhaseSettingIndex < ArrayCount(PhaseSettings);
                PhaseSettingIndex++)
            {
                int PhaseSetting = PhaseSettings[PhaseSettingIndex];

                bool PhaseSettingSubmitted = false;
                bool PrevOutputSubmitted = false;

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
                            Assert(IsPositionMode(Param0Mode));

                            int Int;
#if 0
                            printf("Input: ");
                            fscanf(stdin, "%d", &Int);
#else
                            if(!PhaseSettingSubmitted)
                            {
                                Int = PhaseSettings[PhaseSettingIndex];
                                PhaseSettingSubmitted = true;
                            }
                            else if(!PrevOutputSubmitted)
                            {
                                Int = PrevOutput;
                                PrevOutputSubmitted = true;
                            }
                            else
                            {
                                InvalidCodePath;
                            }
#endif

                            int Param0 = Codes[(CodeIndex + 1)];
                            Codes[Param0] = Int;

                            CodeIndex += 2;
                        } break;

                        case 4:
                        {
                            int Param0Mode = PlaceValue(Code, 100);

                            int Param0 = LoadParam(Codes, (CodeIndex + 1), Param0Mode);
#if 0
                            printf("Output: %d\n", Param0);
#else
                            PrevOutput = Param0;
#endif

                            CodeIndex += 2;
                        } break;

                        case 5:
                        {
                            int Param0Mode = PlaceValue(Code, 100);
                            int Param1Mode = PlaceValue(Code, 1000);

                            int Param0 = LoadParam(Codes, (CodeIndex + 1), Param0Mode);
                            int Param1 = LoadParam(Codes, (CodeIndex + 2), Param1Mode);

                            if(Param0 != 0)
                            {
                                CodeIndex = Param1;
                            }
                            else
                            {
                                CodeIndex += 3;
                            }
                        } break;

                        case 6:
                        {
                            int Param0Mode = PlaceValue(Code, 100);
                            int Param1Mode = PlaceValue(Code, 1000);

                            int Param0 = LoadParam(Codes, (CodeIndex + 1), Param0Mode);
                            int Param1 = LoadParam(Codes, (CodeIndex + 2), Param1Mode);

                            if(Param0 == 0)
                            {
                                CodeIndex = Param1;
                            }
                            else
                            {
                                CodeIndex += 3;
                            }
                        } break;

                        case 7:
                        {
                            int Param0Mode = PlaceValue(Code, 100);
                            int Param1Mode = PlaceValue(Code, 1000);
                            int Param2Mode = PlaceValue(Code, 10000);
                            Assert(IsPositionMode(Param2Mode));

                            int Param0 = LoadParam(Codes, (CodeIndex + 1), Param0Mode);
                            int Param1 = LoadParam(Codes, (CodeIndex + 2), Param1Mode);
                            int Param2 = Codes[(CodeIndex + 3)];

                            if(Param0 < Param1)
                            {
                                Codes[Param2] = 1;
                            }
                            else
                            {
                                Codes[Param2] = 0;
                            }

                            CodeIndex += 4;
                        } break;

                        case 8:
                        {
                            int Param0Mode = PlaceValue(Code, 100);
                            int Param1Mode = PlaceValue(Code, 1000);
                            int Param2Mode = PlaceValue(Code, 10000);
                            Assert(IsPositionMode(Param2Mode));

                            int Param0 = LoadParam(Codes, (CodeIndex + 1), Param0Mode);
                            int Param1 = LoadParam(Codes, (CodeIndex + 2), Param1Mode);
                            int Param2 = Codes[(CodeIndex + 3)];

                            if(Param0 == Param1)
                            {
                                Codes[Param2] = 1;
                            }
                            else
                            {
                                Codes[Param2] = 0;
                            }

                            CodeIndex += 4;
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

//                    Assert(PhaseSettingSubmitted && PrevOutputSubmitted);

                    if(PhaseSettingIndex == (ArrayCount(PhaseSettings) - 1))
                    {
                        int Signal = PrevOutput;
                        if(Signal > HighestSignal)
                        {
                            HighestSignal = Signal;
                        }
                    }
                }
            }
        }
    }

    PRINT_VAR(HighestSignal);

    return(0);
}
