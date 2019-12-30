#include <assert.h>
#include <stdio.h>

#define Assert assert

#define InvalidCodePath Assert(!"InvalidCodePath")

#define ArrayCount(Array) (sizeof((Array)) / sizeof((Array)[0]))

#define PRINT_VAR(Variable) printf(#Variable ": %d\n", Variable)

struct pair
{
    const char *Key;
    int Value;
};

struct edge
{
    // B orbits A directly

    int A;
    int B;
};

struct graph
{
    int PairCount;
    pair Pairs[4096];

    int EdgeCount;
    edge Edges[4096];
};

struct queue
{
    int ReadCursor;
    int WriteCursor;
    int Nodes[4096];
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

static graph *
AllocateGraph(void)
{
    graph *Graph = (graph *)malloc(sizeof(graph));
    Graph->PairCount = 0;
    Graph->EdgeCount = 0;

    return(Graph);
}

inline bool
StringsAreEqual(const char *A, const char *B)
{
    while(*A &&
          *B)
    {
        if(*A++ != *B++)
        {
            return(false);
        }
    }

    bool Result = (*A == *B);

    return(Result);
}

static bool
Lookup(graph *Graph, const char *Key, int *Value)
{
    int FoundCount = 0;
    bool Result = false;

    for(int PairIndex = 0;
        PairIndex < Graph->PairCount;
        PairIndex++)
    {
        pair *Pair = (Graph->Pairs + PairIndex);
        if(StringsAreEqual(Pair->Key, Key))
        {
            *Value = Pair->Value;
            FoundCount++;
            Result = true;
        }
    }

    Assert(!Result || ((Result) && (FoundCount == 1)));

    return(Result);
}

static int
InsertFirst(graph *Graph, const char *Key)
{
    Assert(Graph->PairCount < ArrayCount(Graph->Pairs));
    int Value = Graph->PairCount++;
    pair *Pair = (Graph->Pairs + Value);
    Pair->Key = Key;
    Pair->Value = Value;

    return(Value);
}

static char *
Copy(char *Buffer, int Count)
{
    char *Result = (char *)malloc(Count + 1);
    for(int Index = 0;
        Index < Count;
        Index++)
    {
        Result[Index] = Buffer[Index];
    }
    Result[Count] = '\0';
    return(Result);
}

static void
AddEdge(graph *Graph, const char *AKey, const char *BKey)
{
    int A;
    int B;
    if(!Lookup(Graph, AKey, &A))
    {
        A = InsertFirst(Graph, AKey);
    }
    if(!Lookup(Graph, BKey, &B))
    {
        B = InsertFirst(Graph, BKey);
    }

    edge Edge;
    Edge.A = A;
    Edge.B = B;

    Graph->Edges[Graph->EdgeCount++] = Edge;
}

static void
Enqueue(queue *Queue, int Node)
{
    Queue->Nodes[Queue->WriteCursor++] = Node;
    if(Queue->WriteCursor >= ArrayCount(Queue->Nodes))
    {
        Queue->WriteCursor &= (ArrayCount(Queue->Nodes) - 1);
    }
}

static int
Dequeue(queue *Queue)
{
    int Result = Queue->Nodes[Queue->ReadCursor++];
    if(Queue->ReadCursor >= ArrayCount(Queue->Nodes))
    {
        Queue->ReadCursor &= (ArrayCount(Queue->Nodes) - 1);
    }
    return(Result);
}

inline bool
IsEmpty(queue *Queue)
{
    bool Result = (Queue->ReadCursor == Queue->WriteCursor);

    return(Result);
}

static queue *
AllocateQueue(void)
{
    queue *Queue = (queue *)calloc(1, sizeof(queue));

    return(Queue);
}

static int
BreadthFirstSearch(graph *Graph, int Start, int End)
{
    queue *Queue = AllocateQueue();

    bool *Visited = (bool *)calloc(1, (sizeof(bool)*Graph->PairCount));
    int *Distances = (int *)calloc(1, (sizeof(int)*Graph->PairCount));

    Enqueue(Queue, Start);
    Visited[Start] = true;

    while(!IsEmpty(Queue))
    {
        int Node = Dequeue(Queue);
//        printf("Node: %d\n", Node);
        for(int EdgeIndex = 0;
            EdgeIndex < Graph->EdgeCount;
            EdgeIndex++)
        {
            edge *Edge = (Graph->Edges + EdgeIndex);
            if((Edge->A == Node) ||
               (Edge->B == Node))
            {
                int Neighbor;
                if(Edge->A == Node)
                {
                    Neighbor = Edge->B;
                }
                else if(Edge->B == Node)
                {
                    Neighbor = Edge->A;
                }
                else
                {
                    InvalidCodePath;
                }

                if(!Visited[Neighbor])
                {
                    Enqueue(Queue, Neighbor);
                    Visited[Neighbor] = true;
                    Distances[Neighbor] = (Distances[Node] + 1);
                }
            }
        }
    }

    free(Visited);
    free(Distances);

    int Distance = Distances[End];

    return(Distance);
}

static void
PrintPairs(graph *Graph)
{
    for(int PairIndex = 0;
        PairIndex < Graph->PairCount;
        PairIndex++)
    {
        pair *Pair = (Graph->Pairs + PairIndex);
        printf("Pair: (%s, %d)\n", Pair->Key, Pair->Value);
    }
}

static void
PrintEdges(graph *Graph)
{
    for(int EdgeIndex = 0;
        EdgeIndex < Graph->EdgeCount;
        EdgeIndex++)
    {
        edge *Edge = (Graph->Edges + EdgeIndex);
        printf("Edge: (%d - %d)\n", Edge->A, Edge->B);
    }
}

int
main(int ArgCount, char **Args)
{
    char *Program = ReadEntireFileAndNullTerminate("day6_input.txt");
    if(Program)
    {
        graph *Graph = AllocateGraph();

        int OrbitBufferCount = 0;
        char OrbitBuffer[256];
        char *ProgramAt = Program;
        while(*ProgramAt != '\0')
        {
            while(*ProgramAt == '\n')
            {   
                ProgramAt++;
            }

            if(*ProgramAt != '\0')
            {
                for(int Index = 0;
                    Index < 7;
                    Index++)
                {
                    OrbitBuffer[OrbitBufferCount++] = *ProgramAt++;
                }

                char *A = Copy(OrbitBuffer, 3);
                char *B = Copy((OrbitBuffer + 4), 3);

                AddEdge(Graph, A, B);

                OrbitBufferCount = 0;
            }
            else
            {
                break;
            }
        }

//        PrintPairs(Graph);

        int TotalOrbits = 0;
        for(int PairIndex = 0;
            PairIndex < Graph->PairCount;
            PairIndex++)
        {
            int Distance = BreadthFirstSearch(Graph, 619, PairIndex);
            TotalOrbits += Distance;
        }
        PRINT_VAR(TotalOrbits);
    }

    return(0);
}
