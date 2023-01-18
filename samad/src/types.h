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
void FreeLunchroomData(struct LunchroomData *);
void LInsertAtEnd(struct LunchroomData *, struct Lunchroom **);
void LPrintList(struct Lunchroom *);
void LFreeList(struct Lunchroom **);

struct MealPlanData *GenerateMealPlanData(int index, int rowid,
                                          char *food_name,
                                          char *lunchroom_name,
                                          char *meal_type_name,
                                          int price, int quantity, char *date);
struct MealPlanData *GenerateMealPlanData2(char **);
void MPInsertAtEnd(struct MealPlanData *, struct MealPlan **);
void MPPrintList(struct MealPlan *);

void GetMealTypeForLunchrooms(sqlite3 *, struct Lunchroom *);

#endif /* types_h */
