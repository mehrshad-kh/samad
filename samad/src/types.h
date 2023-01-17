//
//  types.h
//  samad
//
//  Created by Mehrshad on 17/10/1401 AP.
//

#ifndef types_h
#define types_h

#include <stdbool.h>
#include "shared.h"
#if DEBUG
#include "leak_detector_c.h"
#endif

enum UserType
{
    kAdmin = 0,
    kStudent = 1,
    kOptional = 2,
};

struct FoodAndPrice
{
    char *food_name;
    int price;
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
    int sex;
    int balance;
};

struct LunchroomData
{
    int index;
    int rowid;
    char *name;
    char *address;
    int capacity;
    int sex;
    char *meal_type;
};

struct Lunchroom
{
    struct LunchroomData *data;
    struct Lunchroom *prev;
    struct Lunchroom *next;
};

struct IncMealTypeData
{
    int meal_type_id;
};

struct IncMealType
{
    struct IncMealTypeData *data;
    struct IncMealType *prev;
    struct IncMealType *next;
};

struct MealTypeData
{
    int rowid;
    char *name;
};

struct MealType
{
    struct MealTypeData *data;
    struct MealType *prev;
    struct MealType *next;
};

struct IncMealPlanData
{
    int index;
    int rowid;
    int food_id;
    int lunchroom_id;
    int meal_type_id;
    int food_quantity;
    char *date;
};

struct IncMealPlan
{
    struct IncMealPlanData *data;
    struct IncMealPlan *prev;
    struct IncMealPlan *next;
};

struct MealPlanData
{
    int index;
    int rowid;
    char *food_name;
    char *lunchroom_name;
    char *meal_type_name;
    int price;
    int food_quantity;
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

struct IncMealPlanData *GenerateIncMealPlanData(char **);
void ImpnInsertAtEnd(struct IncMealPlanData *,
                     struct IncMealPlan **);

struct MealPlanData *GenerateMealPlanData(int index, int rowid,
                                          char *food_name,
                                          char *lunchroom_name,
                                          char *meal_type_name,
                                          int price, int quantity, char *date);
void MPInsertAtEnd(struct MealPlanData *, struct MealPlan **);
void MPPrintList(struct MealPlan *);

struct MealPlan *GetMealPlans(sqlite3 *, struct IncMealPlan *);

#endif /* types_h */
