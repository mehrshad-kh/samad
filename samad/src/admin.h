//
//  admin.h
//  samad
//
//  Created by Mehrshad on 17/10/1401 AP.
//

#ifndef admin_h
#define admin_h

#include <sqlite3.h>
#include "shared.h"
#include "types.h"

void DisplayAdminMenu(sqlite3 *, struct User **);
void DisplayAccountManagementMenu(sqlite3 *, struct User **);
void DisplayFoodManagementMenu(sqlite3 *, struct User **);

void ChangeMyPassword(sqlite3 *, const struct User *);
void ChangeStudentPassword(sqlite3 *, const struct User *);
void ActivateStudent(sqlite3 *);
void DeactivateStudent(sqlite3 *);
void RemoveStudent(sqlite3 *);
void ChargeAccountAsAdmin(sqlite3 *);
void ListStudents(sqlite3 *);

void DisplayLunchroomMenu(sqlite3 *, struct User **);
void DefineLunchroom(sqlite3 *);
void ListLunchrooms(sqlite3 *);
void AddMealTypeToLunchroom(sqlite3 *);

void DisplayFoodMenu(sqlite3 *, struct User **);
void DefineFood(sqlite3 *);
void ListFoods(sqlite3 *);

void DisplayMealTypeMenu(sqlite3 *, struct User **);
void DefineMealType(sqlite3 *);
void ListMealTypes(sqlite3 *);

void DisplayMealPlanMenu(sqlite3 *, struct User **);
void DefineMealPlan(sqlite3 *);
void ListMealPlans(sqlite3 *);

#endif /* admin_h */
