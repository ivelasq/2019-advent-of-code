#ifndef AOC_MATH_H
#define AOC_MATH_H

#include <math.h>

struct v2
{
    int x, y;
};

inline float
Abs(float S)
{
    float Result = fabsf(S);

    return(Result);
}

inline int
Absi(int I)
{
    int Result = (int)Abs((float)I);

    return(Result);
}

//
// v2 operations
//

inline v2
V2(int X, int Y)
{
    v2 Result;
    Result.x = X;
    Result.y = Y;

    return(Result);
}

inline bool
operator!=(v2 A, v2 B)
{
    bool Result = ((A.x != B.x) ||
                   (A.y != B.y));
    return(Result);
}

inline int
ManhattanDistance(v2 A, v2 B)
{
    int Result = (Absi(B.y - A.y) +
                  Absi(B.x - A.x));
    return(Result);
}

#endif
