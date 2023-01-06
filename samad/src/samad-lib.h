//
//  samad-lib.h
//  samad
//
//  Created by Mehrshad on 10/10/1401 AP.
//

#ifndef samad_lib_h
#define samad_lib_h

#include <stdio.h>

#endif /* samad_lib_h */

#include <stdbool.h>
#include <sqlite3.h>

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

int CreateUsersTable(sqlite3 *);
int CreateLunchroomsTable(sqlite3 *);
int CreateFoodsTable(sqlite3 *);
int CreateMealPlansTable(sqlite3 *);
bool IsFirstLaunch(sqlite3 *);
// Check if not exists
void PerformAccountCreation(sqlite3 *, int);
struct User *PerformLogin(sqlite3 *);
void PerformLogout(struct User **);
void DisplayLoginMenu(sqlite3 *);
void DisplayAdminMenu(sqlite3 *, struct User **);
void PerformAccountManagement(sqlite3 *, struct User **);
void PerformFoodManagement(sqlite3 *, struct User **);
void ChangeMyPassword(sqlite3 *, const struct User *);
void ChargeStudentAccount(sqlite3 *);
void DefineLunchroom(sqlite3 *);
void DefineFood(sqlite3 *);
void DefineMealPlan(sqlite3 *);
