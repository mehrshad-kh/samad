//
//  shared.h
//  samad
//
//  Created by Mehrshad on 10/10/1401 AP.
//

#ifndef shared_h
#define shared_h

#define DEBUG 0

#include <stdio.h>
#include <stdbool.h>
#include <sqlite3.h>
#include <stdlib.h>
#include "shared.h"
#include "admin.h"
#include "student.h"
#include "callback.h"
#include "types.h"
#include "utility.h"
#if DEBUG
#include "leak_detector_c.h"
#endif

sqlite3 *OpenDatabase(const char *);
void CloseDatabase(sqlite3 *);

bool IsFirstLaunch(sqlite3 *);
int CreateTables(sqlite3 *);
int CreateUsersTable(sqlite3 *);
int CreateLunchroomsTable(sqlite3 *);
int CreateMealTypesTable(sqlite3 *);
int CreateLunchroomMealTypesTable(sqlite3 *);
int CreateFoodsTable(sqlite3 *);
int CreateMealPlansTable(sqlite3 *);
int CreateReservationsTable(sqlite3 *);

// Check if not exists
void PerformAccountCreation(sqlite3 *, int);
struct User *PerformLogin(sqlite3 *);
void PerformLogout(struct User **);

void DisplayLoginMenu(sqlite3 *);

#endif /* shared_h */
