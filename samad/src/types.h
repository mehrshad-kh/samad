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

struct LunchroomData
{
    int index;
    int rowid;
    char *name;
    char *address;
    int capacity;
    int gender;
    char *meal_types;
};

struct Lunchroom
{
    struct LunchroomData *data;
    struct Lunchroom *prev;
    struct Lunchroom *next;
};

struct FoodAndPrice
{
    char *food_name;
    int price;
};

struct IncompleteMealPlanData
{
    int index;
    int rowid;
    int food_id;
    int lunchroom_id;
    int meal_type_id;
    int quantity;
    char *date;
};

struct IncompleteMealPlan
{
    struct IncompleteMealPlanData *data;
    struct IncompleteMealPlan *prev;
    struct IncompleteMealPlan *next;
};

struct MealPlanData
{
    int index;
    int rowid;
    char *food_name;
    char *lunchroom_name;
    char *meal_type_name;
    int price;
    int quantity;
    char *date;
};

struct MealPlan
{
    struct MealPlanData *data;
    struct MealPlan *prev;
    struct MealPlan *next;
};

struct LunchroomData *GenerateLunchroomData(char **);
void FreeLunchromData(struct LunchroomData *);
void LInsertAtEnd(struct LunchroomData *, struct Lunchroom **);
void LPrintList(struct Lunchroom *);
void LFreeList(struct Lunchroom **);

struct IncompleteMealPlanData *GenerateIncompleteMealPlanData(char **);
void ImpnInsertAtEnd(struct IncompleteMealPlanData *,
                     struct IncompleteMealPlan **);

struct MealPlanData *GenerateMealPlanData(int index, int rowid,
                                          char *food_name,
                                          char *lunchroom_name,
                                          char *meal_type_name,
                                          int price, int quantity, char *date);
void MPInsertAtEnd(struct MealPlanData *, struct MealPlan **);
void MPPrintList(struct MealPlan *);

struct MealPlan *GetMealPlans(sqlite3 *, struct IncompleteMealPlan *);


#endif /* types_h */
