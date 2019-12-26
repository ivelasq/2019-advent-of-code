#include <assert.h>
#include <stdio.h>

#define Assert assert

#define InvalidCodePath Assert(!"InvalidCodePath")
#define InvalidDefaultCase default: {InvalidCodePath;}

struct stream
{
    char *At;
};

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

static bool
IsEOF(stream *Stream)
{
    bool Result = (Stream->At == '\0');

    return(Result);
}

static bool
IsEOL(char Char)
{
    bool Result = ((Char == '\n') ||
                   (Char == '\r'));
    return(Result);
}

static void
EatEOL(stream *Stream)
{
    while(Stream->At[0])
    {
        if(IsEOL(Stream->At[0]))
        {
            Stream->At++;
        }
        else
        {
            break;
        }
    }
}

static bool
IsNumeric(char Char)
{
    bool Result = (('0' <= Char) && (Char <= '9'));

    return(Result);
}

static int
ParseInt(stream *Stream)
{
    int IntBufferCursor = 0;
    char IntBuffer[256];
    while(Stream->At[0] &&
          IsNumeric(Stream->At[0]))
    {
        IntBuffer[IntBufferCursor++] = *Stream->At++;
    }
    IntBuffer[IntBufferCursor++] = '\0';
    int Result = atoi(IntBuffer);

    return(Result);
}

int
main(int ArgCount, char **Args)
{
    char *Program = ReadEntireFile("day2_input.txt");
    if(Program)
    {
        int OpcodeCursor = 0;
        int OpcodeCount = 1024;
        int *Opcodes = (int *)malloc(sizeof(int)*OpcodeCount);

        stream Stream_;
        stream *Stream = &Stream_;
        Stream->At = Program;
        for(;;)
        {
            if(IsNumeric(Stream->At[0]))
            {
                int Opcode = ParseInt(Stream);
                Assert(OpcodeCursor < OpcodeCount);
                Opcodes[OpcodeCursor++] = Opcode;
                printf("Opcode: %d\n", Opcode);
                if(Stream->At[0] == ',')
                {
                    Stream->At++;
                }
            }
            else
            {
                break;
            }
        }

        for(int OpcodeIndex = 0;
            OpcodeIndex < OpcodeCursor;
            OpcodeIndex++)
        {
            bool Exit = false;

            int Opcode = Opcodes[OpcodeIndex];
            switch(Opcode)
            {
                case 1:
                {
                    // @Assert: Enough opcodes remain
                    int AddendPos0 = Opcodes[(OpcodeIndex + 1)];
                    int AddendPos1 = Opcodes[(OpcodeIndex + 2)];
                    int ResultPos = Opcodes[(OpcodeIndex + 3)];

                    int Addend0 = Opcodes[AddendPos0];
                    int Addend1 = Opcodes[AddendPos1];
                    int Result = (Addend0 + Addend1);

                    Opcodes[ResultPos] = Result;

                    // @Check
                    OpcodeIndex += 3;
                } break;

                case 2:
                {
                    // @Assert: Enough opcodes remain
                    int FactorPos0 = Opcodes[(OpcodeIndex + 1)];
                    int FactorPos1 = Opcodes[(OpcodeIndex + 2)];
                    int ResultPos = Opcodes[(OpcodeIndex + 3)];

                    int Factor0 = Opcodes[FactorPos0];
                    int Factor1 = Opcodes[FactorPos1];
                    int Result = (Factor0*Factor1);

                    Opcodes[ResultPos] = Result;

                    // @Check
                    OpcodeIndex += 3;
                } break;

                case 99:
                {
                    Exit = true;
                } break;

                InvalidDefaultCase;
            }

            if(Exit)
            {
                break;
            }
        }

        printf("Final state:\n");
        for(int OpcodeIndex = 0;
            OpcodeIndex < OpcodeCursor;
            OpcodeIndex++)
        {
            printf("%d\n", Opcodes[OpcodeIndex]);
        }
    }

    return(0);
}
