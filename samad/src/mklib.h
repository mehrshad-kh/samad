//
//  mklib.h
//
//
//  Created by Mehrshad on 4/9/1401 AP.
//

#ifndef mklib_h
#define mklib_h

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

struct Node
{
    int id;
    struct Node *next;
};

struct DNode
{
    int id;
    struct DNode *next;
    struct DNode *prev;
};

struct GNode
{
    void *data;
    struct GNode *prev;
    struct GNode *next;
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
// Possible problem with strcpy()
void MultiplyNByN(const char *, const char *, char **);

double GetDeterminant(int order, double [order][order]);
double GetCofactor(int order, double [order][order], int, int);
double GetMinor(int order, double [order][order], int, int);

void FindAndReplace(const char *, const char *, const char *, char **);
// More optimized
// Free return value
char *FindAndReplace2(const char *, const char *, const char *);
bool Contains(const char *, const char *);
int GetNumberOfOccurrences(const char *, const char *);
void GetIndicesOfOccurrence(const char *, const char *, int **);

// Singly linked list functions
void InsertAtFirst(int, struct Node **);
void InsertAtEnd(int, struct Node **);
void DeleteAtLocation(int, struct Node **);

// Doubly linked list functions
void DLInsertAtEnd(int, struct DNode **);
void DLInsertAfterAddress(int, struct DNode *, struct DNode **);
void DLPrintBackwards(struct DNode *);
void DLPerformBubbleSortDsc(struct DNode **);
void DLDeleteWithAddress(struct DNode *, struct DNode **);
void DLDeleteRepeatsInSorted(struct DNode **);

// Linked list functions in general
int GetListLength(struct Node *);
void DeleteFirstInList(struct Node **);
void DeleteLastInList(struct Node **);
void FreeList(struct Node **);
void PrintList(struct Node *);

int GInsertAtEnd(struct GNode **head, void *data);
/// Free members with FreeData()
int GFreeList(struct GNode *head, void (*FreeData)(void *));
void GPrintList(struct GNode *ptr, void (*PrintNode)(void *));

#endif /* mklib_h */
