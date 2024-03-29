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

enum UserType
{
    kOptional = 0,
    kEmployee = 1,
    kStudent = 2,
};

// Conforming to ISO/IEC 5218 Standard
enum Sex
{
    kNotKnown = 0,
    kMale = 1,
    kFemale = 2,
    kNotApplicable = 9,
};

struct User
{
    int id;
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

struct FoodAndPrice
{
    char *food_name;
    int price;
};

struct LunchroomData
{
    int index;
    int lunchroom_id;
    char *lunchroom_name;
    char *address;
    int capacity;
    int sex;
    int meal_type_id;
    char *meal_type_name;
};

struct Lunchroom
{
    struct LunchroomData *data;
    struct Lunchroom *prev;
    struct Lunchroom *next;
};

struct MealPlan
{
    int index;
    int id;
    char *food_name;
    char *lunchroom_name;
    char *meal_type_name;
    int price;
    int current_quantity;
    char *date;
};

struct LunchroomData *GenerateLunchroomData(char **);
void FreeLunchroomData(struct LunchroomData *);
void LInsertAtEnd(struct LunchroomData *, struct Lunchroom **);
void LPrintList(struct Lunchroom *);
void LFreeList(struct Lunchroom **);

struct MealPlan *GenerateMealPlan(char **);
/// Can be used for both MealPlan and TakingMealPlan
void FreeMealPlan(void *);
void PrintMealPlan(void *);

struct MealPlan *GenerateTakingMealPlan(char **);
void PrintTakingMealPlan(void *);

#endif /* types_h */
