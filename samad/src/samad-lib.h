//
//  samad-lib.h
//  samad
//
//  Created by Mehrshad on 10/10/1401 AP.
//

#ifndef samad_lib_h
#define samad_lib_h

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

sqlite3 *OpenDatabase(const char *);
void CloseDatabase(sqlite3 *);

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
void DisplayAccountManagement(sqlite3 *, struct User **);
void DisplayFoodManagement(sqlite3 *, struct User **);

void ChangeMyPassword(sqlite3 *, const struct User *);
void ActivateStudent(sqlite3 *);
void ChargeStudentAccount(sqlite3 *);

void DisplayLunchroomMenu(sqlite3 *, struct User **);
void DefineLunchroom(sqlite3 *);
void ListLunchrooms(sqlite3 *);
void DefineFood(sqlite3 *);
void DefineMealPlan(sqlite3 *);

#endif /* samad_lib_h */
