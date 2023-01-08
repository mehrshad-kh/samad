//
//  mklib.h
//
//
//  Created by Mehrshad on 4/9/1401 AP.
//

#ifndef mklib_h
#define mklib_h

#include <stdbool.h>
#include <stdint.h>

struct Node
{
    int id;
    struct Node *next;
};

struct String
{
    char *value;
    struct String *next;
    struct String *prev;
};

struct List
{
    struct String *head;
    struct List *next;
    struct List *prev;
};

struct MKPoint {
    double x;
    double y;
};

static const double kPI = 3.14159265358979323846;

// Gravitational acceleration
static const double kG = 9.80665;

bool Xor(bool, bool);
void swapc(char *, char *);
void swapi(int *, int *);
bool issym(char, int, ...);

// Both elements are populated
void SolveQuadratic(double, double, double, double [2]);

static inline uint8_t todgt(char);
static inline char tochar(uint8_t);
char *GetGreater(const char *, const char *);
void RemoveLeadingZero(char **);

// "123456" * '7'
void MultiplyNByOne(const char *, char, int, char **);

// "123" + "456"
void AddNToN(const char *, const char *, char **);

// "+123" * "-456"
void MultiplyNByN(const char *, const char *, char **);

double GetDeterminant(int order, double [order][order]);
double GetCofactor(int order, double [order][order], int, int);
double GetMinor(int order, double [order][order], int, int);

void FindAndReplace(const char *, const char *, const char *, char **);
void FindAndReplace2(const char *, const char *, const char *, char **);
bool Contains(const char *, const char *);
int GetNumberOfOccurrences(const char *, const char *);
void GetIndicesOfOccurrence(const char *, const char *, int **);

// String functions
void SInsertAtEnd(const char *, struct String **);
// void SInsertAfterAddress(const char *, struct String *, struct String **);
// void SDeleteWithAddress(struct String *, struct String **);
void SFreeList(struct String **);
void SPrintList(struct String *);

// List functions
void LInsertAtEnd(struct String *, struct List **);
// void LInsertAfterAddress(struct String *, struct List *, struct List **);
// void LDeleteWithAddress(struct List *, struct List **);
void LFreeList(struct List **);
void LPrintList(struct List *);

// General linked list functions
int GetListLength(struct Node *);
void DeleteFirstInList(struct Node **);
void DeleteLastInList(struct Node **);
void PrintList(struct Node *);

#endif /* mklib_h */
