//
//  shared.h
//  samad
//
//  Created by Mehrshad on 10/10/1401 AP.
//

#ifndef shared_h
#define shared_h

#include <stdbool.h>
#include <sqlite3.h>
#include "types.h"

sqlite3 *OpenDatabase(const char *);
void CloseDatabase(sqlite3 *);

bool IsFirstLaunch(sqlite3 *);
int CreateUsersTable(sqlite3 *);
int CreateLunchroomsTable(sqlite3 *);
int CreateFoodsTable(sqlite3 *);
int CreateMealPlansTable(sqlite3 *);

// Check if not exists
void PerformAccountCreation(sqlite3 *, int);
struct User *PerformLogin(sqlite3 *);
void PerformLogout(struct User **);

void DisplayLoginMenu(sqlite3 *);

#endif /* shared_h */
