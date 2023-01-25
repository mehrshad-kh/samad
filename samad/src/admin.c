//
//  admin.c
//  samad
//
//  Created by Mehrshad on 17/10/1401 AP.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "admin.h"
#include "callback.h"
#include "utility.h"

extern const char *const kErr;
extern const char *const kAllocationErr;
extern const char *const kQueryGenerationErr;
extern const char *const kQueryExecutionErr;
extern const char *const kDatabaseOpenErr;
extern const char *const kDatabaseCloseErr;

void DisplayAdminMenu(sqlite3 *db, struct User **user)
{
    int input = 0;

    printf("\n--ADMIN--\n");
    printf("What would you like to do?\n");
    printf("0: Log out\n"
           "1: Account management\n"
           "2: Food management\n");

input_generation:
    input = TakeShellInput();

    switch (input)
    {
    case 0:
        PerformLogout(user);
        DisplayLoginMenu(db);
        break;
    case 1:
        DisplayAccountManagementMenu(db, user);
        break;
    case 2:
        DisplayFoodManagementMenu(db, user);
        break;
    default:
        printf("Invalid input. Please try again.\n");
        goto input_generation;
    }
}

void DisplayAccountManagementMenu(sqlite3 *db, struct User **user)
{
    int input = 0;

    printf("\n--ACCOUNT MANAGEMENT--\n");
    printf("What would you like to do?\n");
    printf("0: Return\n"
           "1: Change my password\n"
           "2: Change student password\n"
           "3: Activate student\n"
           "4: Deactivate student\n"
           "5: Remove student\n"
           "6: Register new user\n"
           "7: Charge an account\n"
           "8: List students\n");

input_generation:
    input = TakeShellInput();

    switch (input)
    {
    case 0:
        DisplayAdminMenu(db, user);
        break;
    case 1:
        ChangeMyPassword(db, *user);
        DisplayAccountManagementMenu(db, user);
        break;
    case 2:
        ChangeStudentPassword(db, *user);
        DisplayAccountManagementMenu(db, user);
        break;
    case 3:
        ActivateStudent(db);
        DisplayAccountManagementMenu(db, user);
        break;
    case 4:
        DeactivateStudent(db);
        DisplayAccountManagementMenu(db, user);
        break;
    case 5:
        RemoveStudent(db);
        DisplayAccountManagementMenu(db, user);
        break;
    case 6:
        PerformAccountCreation(db, kOptional);
        DisplayAccountManagementMenu(db, user);
        break;
    case 7:
        ChargeAccountAsAdmin(db);
        DisplayAccountManagementMenu(db, user);
        break;
    case 8:
        ListStudents(db);
        DisplayAccountManagementMenu(db, user);
        break;
    default:
        printf("Invalid input. Please try again.\n");
        goto input_generation;
    }
}

void DisplayFoodManagementMenu(sqlite3 *db, struct User **user)
{
    int input = 0;

    printf("\n--FOOD MANAGEMENT--\n");
    printf("0: Return\n"
           "1: Meal type\n"
           "2: Lunchroom\n"
           "3: Food\n"
           "4: Meal plan\n");

input_generation:
    input = TakeShellInput();

    switch (input)
    {
    case 0:
        DisplayAdminMenu(db, user);
        break;
    case 1:
        DisplayMealTypeMenu(db, user);
        break;
    case 2:
        DisplayLunchroomMenu(db, user);
        break;
    case 3:
        DisplayFoodMenu(db, user);
        break;
    case 4:
        DisplayMealPlanMenu(db, user);
        break;
    default:
        printf("Invalid input. Please try again.\n");
        goto input_generation;
    }
}

void ChangeMyPassword(sqlite3 *db, const struct User *user)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

    bool is_correct = false;
    char *current_password = NULL;
    char *new_password = NULL;

    printf("\n--CHANGE MY PASSWORD--\n");

    current_password = strdup(getpass("Current password: "));
    new_password = strdup(getpass("New password: "));

    rc = asprintf(&sql, "SELECT id FROM users "
                        "WHERE id = %d AND password = '%s';",
                  user->id, current_password);
    if (rc == -1)
    {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }

    rc = sqlite3_exec(db, sql, &CheckPasswordCallback,
                      &is_correct, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }

    if (!is_correct)
    {
        printf("Your password is incorrect. "
               "Please try again later.\n");
        goto exit;
    }

    free(sql);
    rc = asprintf(&sql, "UPDATE users "
                        "SET password = '%s' "
                        "WHERE id = %d;",
                  new_password, user->id);
    if (rc == -1)
    {
        fprintf(stderr, "%s %s\n", kErr, kQueryExecutionErr);
        goto exit;
    }

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }

    printf("Your password was successfully changed.\n");

exit:
    free(current_password);
    free(new_password);
    free(sql);
}

void ChangeStudentPassword(sqlite3 *db, const struct User *user)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

    bool exists = false;
    char *id_number = NULL;
    char *password = NULL;

    printf("\n--CHANGE STUDENT PASSWORD--\n");

    printf("ID number: ");
    TakeStringInput(&id_number);

    password = getpass("New password: ");

    rc = asprintf(&sql, "SELECT id FROM users "
                        "WHERE user_type = %d "
                        "AND id_number = '%s';",
                  kStudent, id_number);
    if (rc == -1)
    {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit1;
    }

    rc = sqlite3_exec(db, sql, &CheckIDNumberCallback, &exists, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }

    if (!exists)
    {
        printf("No such student.\n");
        goto exit;
    }

    free(sql);
    rc = asprintf(&sql, "UPDATE users "
                        "SET password = '%s' "
                        "WHERE id_number = '%s';",
                  password, id_number);
    if (rc == -1)
    {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit1;
    }

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }

    printf("The password was successfully changed.\n");

exit:
    free(sql);
exit1:
    free(id_number);
}

void ActivateStudent(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

    char *id_number = NULL;

    printf("\n--STUDENT ACTIVATION--\n");
    printf("Student ID: ");
    TakeStringInput(&id_number);

    // If valid id_number
    // Perhaps better to check if student
    rc = asprintf(&sql, "UPDATE users "
                        "SET activated = 1 "
                        "WHERE id_number = '%s';",
                  id_number);
    if (rc == -1)
    {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }

    printf("The account was successfully activated.\n");

exit:
    free(id_number);
    free(sql);
}

void DeactivateStudent(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

    bool exists = false;
    char *id_number = NULL;

    printf("\n--STUDENT DEACTIVATION--\n");

    printf("ID number: ");
    TakeStringInput(&id_number);

    rc = asprintf(&sql, "SELECT id FROM users "
                        "WHERE user_type = %d "
                        "AND id_number = '%s'",
                  kStudent, id_number);
    if (rc == -1)
    {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit1;
    }

    rc = sqlite3_exec(db, sql, &CheckIDNumberCallback, &exists, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }

    if (!exists)
    {
        printf("No such student.\n");
        goto exit;
    }

    free(sql);
    rc = asprintf(&sql, "UPDATE users "
                        "SET activated = 0 "
                        "WHERE id_number = '%s';",
                  id_number);
    if (rc == -1)
    {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit1;
    }

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }

    printf("The student was successfully deactivated.\n");

exit:
    free(sql);
exit1:
    free(id_number);
}

void RemoveStudent(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

    int input = 0;
    bool exists = false;
    char *id_number = NULL;

    printf("\n--STUDENT REMOVAL--\n");

    printf("ID number: ");
    TakeStringInput(&id_number);

    rc = asprintf(&sql, "SELECT id FROM users "
                        "WHERE user_type = %d "
                        "AND id_number = '%s'",
                  kStudent, id_number);
    if (rc == -1) {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit1;
    }

    rc = sqlite3_exec(db, sql, &CheckIDNumberCallback, &exists, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }

    if (!exists) {
        printf("No such student.\n");
        goto exit;
    }

    printf("You are about to remove student ID %s.\n"
           "Are you sure want to proceed? [N/y] ",
           id_number);

input_generation:
    input = TakeCharInput();

    switch (input) {
    case (int)'y':
        break;
    case (int)'N':
    case (int)'\n':
        goto exit;
        break;
    default:
        printf("Invalid input. Please try again.\n");
        goto input_generation;
    }

    free(sql);
    rc = asprintf(&sql, "UPDATE users "
                  "SET effective_end_date = DATE('now', 'localtime', '+1 days') "
                  "WHERE id_number = '%s';", id_number);
    if (rc == -1) {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit1;
    }

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }

    printf("The student was successfully removed.\n");

exit:
    free(sql);
exit1:
    free(id_number);
}

void ChargeAccountAsAdmin(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

    char *id_number = NULL;
    int charge_amount = 0;

    printf("\n--CHARGE ACCOUNT--\n");
    printf("Please enter a student ID: ");
    TakeStringInput(&id_number);

    printf("Please enter the amount: ");
    charge_amount = TakeIntInput();

    if (charge_amount <= 0)
    {
        printf("Invalid amount.\n");
        goto exit;
    }

    // Check if activated
    // Perhaps better to retrieve the id first
    rc = asprintf(&sql, "UPDATE users "
                        "SET balance = balance + %d "
                        "WHERE id_number = '%s' AND user_type = %d;",
                  charge_amount, id_number, kStudent);
    if (rc == -1)
    {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit_1;
    }

    printf("The balance was successfully updated.\n");

exit_1:
    free(sql);
exit:
    free(id_number);
}

void ListStudents(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

    printf("\n--STUDENTS--\n");

    rc = asprintf(&sql, "SELECT id, first_name, last_name, "
                        "id_number, balance FROM users "
                        "WHERE effective_end_date IS NULL;");
    if (rc == -1) {
        printf("%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }

    rc = sqlite3_exec(db, sql, &PrintRecordCallback, "users", &err_msg);
    if (rc != SQLITE_OK) {
        printf("%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit_1;
    }

exit_1:
    free(sql);
exit:
    rc = 0;
}

void DisplayLunchroomMenu(sqlite3 *db, struct User **user)
{
    int input = 0;

    printf("\n--LUNCHROOM--\n");
    printf("0: Return\n"
           "1: Define\n"
           "2: List\n");

input_generation:
    input = TakeShellInput();

    switch (input)
    {
    case 0:
        DisplayFoodManagementMenu(db, user);
        break;
    case 1:
        DefineLunchroom(db);
        DisplayLunchroomMenu(db, user);
        break;
    case 2:
        ListLunchrooms(db);
        DisplayLunchroomMenu(db, user);
        break;
    default:
        printf("Invalid input. Please try again.\n");
        goto input_generation;
    }
}

void DisplayFoodMenu(sqlite3 *db, struct User **user)
{
    int input = 0;

    printf("\n--FOOD--\n");
    printf("0: Return\n"
           "1: Define\n"
           "2: List\n");

input_generation:
    input = TakeShellInput();

    switch (input)
    {
    case 0:
        DisplayFoodManagementMenu(db, user);
        break;
    case 1:
        DefineFood(db);
        DisplayFoodMenu(db, user);
        break;
    case 2:
        ListFoods(db);
        DisplayFoodMenu(db, user);
        break;
    default:
        printf("Invalid input. Please try again.\n");
        goto input_generation;
    }
}

void DisplayMealTypeMenu(sqlite3 *db, struct User **user)
{
    int input = 0;

    printf("\n--MEAL TYPE--\n");
    printf("0: Return\n"
           "1: Define\n"
           "2: List\n");

input_generation:
    input = TakeShellInput();

    switch (input)
    {
    case 0:
        DisplayFoodManagementMenu(db, user);
        break;
    case 1:
        DefineMealType(db);
        DisplayMealTypeMenu(db, user);
        break;
    case 2:
        ListMealTypes(db);
        DisplayMealTypeMenu(db, user);
        break;
    default:
        printf("Invalid input. Please try again.\n");
        goto input_generation;
    }
}

void DisplayMealPlanMenu(sqlite3 *db, struct User **user)
{
    int input = 0;

    printf("\n--MEAL PLAN--\n");
    printf("0: Return\n"
           "1: Define\n"
           "2: List\n");

input_generation:
    input = TakeShellInput();

    switch (input)
    {
    case 0:
        DisplayFoodManagementMenu(db, user);
        break;
    case 1:
        DefineMealPlan(db);
        DisplayMealPlanMenu(db, user);
        break;
    case 2:
        ListMealPlans(db);
        DisplayMealPlanMenu(db, user);
        break;
    default:
        printf("Invalid input. Please try again.\n");
        goto input_generation;
    }
}

void DefineLunchroom(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

    char *name = NULL;
    char *address = NULL;
    int capacity = 0;
    int sex = 0;
    int meal_type_id = 0;

    printf("\n--DEFINE LUNCHROOM--\n");
    printf("Please complete the following form.\n");

    printf("Name: ");
    TakeStringInput(&name);

    printf("Address: ");
    TakeStringInput(&address);

    printf("Capacity: ");
    capacity = TakeIntInput();

    printf("Sex (1: male, 2: female): ");
    sex = TakeIntInput();

    printf("Meal type ID: ");
    meal_type_id = TakeIntInput();

    rc = asprintf(&sql, "INSERT INTO lunchrooms ("
                        "name, address, capacity, sex) "
                        "VALUES ('%s', '%s', %d, %d);",
                  name, address, capacity, sex);
    if (rc == -1)
    {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }

    rc = asprintf(&sql, "INSERT INTO lunchroom_meal_types ("
                        "lunchroom_id, meal_type_id) "
                        "VALUES (%lld, %d);",
                  sqlite3_last_insert_rowid(db), meal_type_id);
    if (rc == -1)
    {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }

    printf("The lunchroom was successfully defined.\n");

exit:
    free(sql);
    free(name);
    free(address);
}

void DefineFood(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

    char *name = NULL;
    char *food_type = NULL;
    int price = 0;

    if (rc == 0)
    {
        printf("\n--DEFINE FOOD--\n");
        printf("Please complete the following form.\n");

        printf("Name: ");
        TakeStringInput(&name);

        printf("Food type: ");
        TakeStringInput(&food_type);

        printf("Price: ");
        price = TakeIntInput();

        rc = asprintf(&sql, "INSERT INTO foods ("
                            "name, food_type, price) "
                            "VALUES ('%s', '%s', %d);",
                      name, food_type, price);

        if (rc != -1)
        {
            rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);

            if (rc == SQLITE_OK)
            {
                printf("The food was succesfully defined.\n");
            }
            else
            {
                fprintf(stderr, "%s %s: %s\n",
                        kErr, kQueryGenerationErr, err_msg);
                sqlite3_free(err_msg);
            }
        }
        else
        {
            fprintf(stderr, "%s %s\n", kErr, kQueryExecutionErr);
        }

        free(sql);
        free(name);
        free(food_type);
    }
}

void DefineMealType(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

    char *name = NULL;
    char *start_time = NULL;
    char *end_time = NULL;

    if (rc == 0)
    {
        printf("\n--DEFINE MEAL TYPE--\n");
        printf("Please complete the following form.\n");

        printf("Name: ");
        TakeStringInput(&name);

        printf("Start time (HH:MM): ");
        TakeStringInput(&start_time);

        printf("End time: (HH:MM): ");
        TakeStringInput(&end_time);

        rc = asprintf(&sql, "INSERT INTO meal_types ("
                            "name, start_time, end_time) "
                            "VALUES ('%s', '%s', '%s');",
                      name, start_time, end_time);

        if (rc != -1)
        {
            rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);

            if (rc == SQLITE_OK)
            {
                printf("The meal type was succesfully defined.\n");
            }
            else
            {
                fprintf(stderr, "%s %s: %s\n",
                        kErr, kQueryExecutionErr, err_msg);
                sqlite3_free(err_msg);
            }
        }
        else
        {
            fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        }

        free(sql);
        free(name);
        free(start_time);
        free(end_time);
    }
}

void ListMealTypes(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

    printf("\n--MEAL TYPES--\n");

    rc = asprintf(&sql, "SELECT id, name, start_time, end_time "
                        "FROM meal_types;");

    if (rc == -1)
    {
        printf("%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }

    rc = sqlite3_exec(db, sql, &PrintRecordCallback, "meal_types", &err_msg);

    if (rc != SQLITE_OK)
    {
        printf("%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }

exit:
    free(sql);
}

void DefineMealPlan(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

    int food_id = 0;
    int lunchroom_id = 0;
    int meal_type_id = 0;
    int food_quantity = 0;
    char *date = NULL;

    printf("\n--DEFINE MEAL PLAN--\n");
    printf("Please complete the following form.\n");

    printf("Food ID: ");
    food_id = TakeIntInput();

    printf("Lunchroom ID: ");
    lunchroom_id = TakeIntInput();

    printf("Meal type ID: ");
    meal_type_id = TakeIntInput();

    printf("Food quantity: ");
    food_quantity = TakeIntInput();

    printf("Date (YYYY-MM-DD): ");
    TakeStringInput(&date);

    if (!IsDateCorrect(date)) {
        printf("Invalid date.\nDid you enter in Persian calendar?\n");
        goto exit;
    }

    rc = asprintf(&sql, "INSERT INTO meal_plans ("
                        "food_id, lunchroom_id, meal_type_id, "
                        "food_quantity, date) "
                        "VALUES (%d, %d, %d, %d, '%s');",
                  food_id, lunchroom_id, meal_type_id,
                  food_quantity, date);
    if (rc == -1) {
        printf("%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        printf("%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit_1;
    }

    printf("The meal plan was succesfully defined.\n");

exit_1:
    free(sql);
exit:
    free(date);
}

void ListLunchrooms(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

    printf("\n--LUNCHROOMS--");

    rc = asprintf(&sql, "SELECT l.id AS lunchroom_id, "
                        "l.name AS lunchroom_name, "
                        "mt.name AS meal_type_name, "
                        "l.capacity AS capacity, "
                        "l.sex AS sex "
                        "FROM lunchrooms l "
                        "INNER JOIN lunchroom_meal_types lmt "
                        "ON lmt.lunchroom_id = l.id "
                        "INNER JOIN meal_types mt "
                        "ON mt.id = lmt.meal_type_id;");
    if (rc == -1)
    {
        printf("%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }

    rc = sqlite3_exec(db, sql, &PrintRecordCallback, "lunchrooms", &err_msg);
    if (rc != SQLITE_OK)
    {
        printf("%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }

exit:
    free(sql);
}

void ListFoods(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

    printf("\n--FOODS--");

    rc = asprintf(&sql, "SELECT id, name, food_type, price "
                        "FROM foods;");
    if (rc == -1)
    {
        printf("%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }

    rc = sqlite3_exec(db, sql, &PrintRecordCallback, "foods", &err_msg);
    if (rc != SQLITE_OK)
    {
        printf("%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit_1;
    }

exit_1:
    free(sql);
exit:
    rc = 0;
}

void ListMealPlans(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

    printf("\n--MEAL PLANS--\n");

    rc = asprintf(&sql, "SELECT id, food_id, lunchroom_id, "
                        "meal_type_id, food_quantity, date "
                        "FROM meal_plans;");

    if (rc == -1)
    {
        printf("%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }

    rc = sqlite3_exec(db, sql, &PrintRecordCallback, "meal_plans", &err_msg);

    if (rc != SQLITE_OK)
    {
        printf("%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }

exit:
    free(sql);
}
