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
    struct LunchroomNode *prev;
    struct LunchroomNode *next;
};

struct IncompleteMealPlan
{
    int index;
    int rowid;
    int food_id;
    int lunchroom_id;
    int quantity;
    char *date;
};

struct IncompleteMealPlanNode
{
    struct IncompleteMealPlan *meal_plan;
    struct IncompleteMealPlanNode *prev;
    struct IncompleteMealPlanNode *next;
};

struct MealPlan
{
    int index;
    int rowid;
    char *food_name;
    char *lunchroom_name;
    int quantity;
    char *date;
    
    struct MealPlan *prev;
    struct MealPlan *next;
};

struct Lunchroom *CreateLunchroom(char **);
void FreeLunchrom(struct Lunchroom *);

struct IncompleteMealPlan *CreateIncompleteMealPlan(char **);

void LNInsertAtEnd(struct Lunchroom *, struct LunchroomNode **);
void LNPrintList(struct LunchroomNode *);
void LNFreeList(struct LunchroomNode **);

void ImpnInsertAtEnd(struct IncompleteMealPlan *,
                     struct IncompleteMealPlanNode **);
void MPInsertAtEnd(int index, int rowid, char *food_name,
                   char *lunchroom_name, int quantity, char *date,
                   struct MealPlan **head);

#endif /* types_h */
