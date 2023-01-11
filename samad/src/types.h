//
//  types.h
//  samad
//
//  Created by Mehrshad on 17/10/1401 AP.
//

#ifndef types_h
#define types_h

#include <stdbool.h>

enum UserType
{
    kAdmin = 0,
    kStudent = 1,
    kOptional = 2,
};

struct User
{
    int rowid;
    int user_type;
    bool activated;
    char *first_name;
    char *last_name;
    char *id_number;
    char *national_id;
    char *birthdate;
    int gender;
};

struct Lunchroom
{
    int index;
    int rowid;
    char *name;
    char *address;
    int capacity;
    int gender;
    char *meal_types;
};

struct LunchroomNode
{
    struct Lunchroom *lunchroom;
    struct LunchroomNode *next;
    struct LunchroomNode *prev;
};

struct Lunchroom *CreateLunchroom(char **);
void FreeLunchrom(struct Lunchroom *);

void LNInsertAtEnd(struct Lunchroom *, struct LunchroomNode **);
void LNPrintList(struct LunchroomNode *);
void LNFreeList(struct LunchroomNode **);

#endif /* types_h */
