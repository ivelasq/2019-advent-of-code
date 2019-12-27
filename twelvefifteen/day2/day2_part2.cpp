#include <assert.h>
#include <stdio.h>

#define Assert assert

#define InvalidCodePath Assert(!"InvalidCodePath")
#define InvalidDefaultCase default: {InvalidCodePath;}

typedef unsigned char u8;

typedef intptr_t umm;

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

static void
Copy(umm Bytes, void *SourceInit, void *DestInit)
{
    u8 *Source = (u8 *)SourceInit;
    u8 *Dest = (u8 *)DestInit;
    while(Bytes--)
    {
        *Dest++ = *Source++;
    }
}

static void
CopyOpcodes(int Count, void *Source, void *Dest)
{
    Copy((Count*sizeof(int)), Source, Dest);
}

static void
SetNounAndVerb(int *Opcodes, int Noun, int Verb)
{
    Opcodes[1] = Noun;
    Opcodes[2] = Verb;
}

static void
Execute(int *Opcodes, int Count)
{
    for(int OpcodeIndex = 0;
    OpcodeIndex < Count;
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
}

int
main(int ArgCount, char **Args)
{
    char *Program = ReadEntireFile("day2_input.txt");
    if(Program)
    {
        int OpcodeCursor = 0;
        int OpcodeCount = 1024;
        int *OpcodesInitial = (int *)malloc(sizeof(int)*OpcodeCount);
        int *OpcodesWorking = (int *)malloc(sizeof(int)*OpcodeCount);

        // Parsing input into Opcodes buffers
        stream Stream_;
        stream *Stream = &Stream_;
        Stream->At = Program;
        for(;;)
        {
            if(IsNumeric(Stream->At[0]))
            {
                int Opcode = ParseInt(Stream);
                Assert(OpcodeCursor < OpcodeCount);
                OpcodesInitial[OpcodeCursor++] = Opcode;
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
        CopyOpcodes(OpcodeCursor, OpcodesInitial, OpcodesWorking);

        // Determining result from target noun and verb
        bool Found = false;
        int TargetNoun = 0;
        int TargetVerb = 0;

        for(int Noun = 0;
            Noun <= 99;
            Noun++)
        {
            for(int Verb = 0;
                Verb <= 99;
                Verb++)
            {
                CopyOpcodes(OpcodeCursor, OpcodesInitial, OpcodesWorking);
                SetNounAndVerb(OpcodesWorking, Noun, Verb);
                Execute(OpcodesWorking, OpcodeCursor);
                int Output = OpcodesWorking[0];
                if(Output == 19690720)
                {
                    TargetNoun = Noun;
                    TargetVerb = Verb;
                    Found = true;
                    break;
                }
            }

            if(Found)
            {
                break;
            }
        }

        int Result = ((100*TargetNoun) + TargetVerb);
        printf("Result: %d\n", Result);
    }

    return(0);
}
