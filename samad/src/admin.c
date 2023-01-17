//
//  admin.c
//  samad
//
//  Created by Mehrshad on 17/10/1401 AP.
//

#include <stdio.h>
#include <stdlib.h>
#include "shared.h"
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
           "2: Change student password (!)\n"
           "3: Activate student\n"
           "4: Deactivate student (!)\n"
           "5: Remove student (!)\n"
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
    case 3:
        ActivateStudent(db);
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
           "1: Lunchroom\n"
           "2: Meal type\n"
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
        DisplayLunchroomMenu(db, user);
        break;
    case 2:
        DisplayMealTypeMenu(db, user);
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

exit:
    input = 0;
}

void ChangeMyPassword(sqlite3 *db, const struct User *user)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

    size_t n = 0;
    bool password_is_correct = false;
    char *current_password = NULL;
    char *new_password = NULL;

    current_password = (char *)calloc(64, sizeof(char));
    new_password = (char *)calloc(64, sizeof(char));

    printf("\n--CHANGE MY PASSWORD--\n");

    printf("Current password: ");
    getline(&current_password, &n, stdin);
    RemoveTrailingNewline(current_password);

    printf("New password: ");
    getline(&new_password, &n, stdin);
    RemoveTrailingNewline(new_password);

    rc = asprintf(&sql, "SELECT rowid FROM users "
                        "WHERE rowid = %d AND password = '%s';",
                  user->rowid, current_password);

    if (rc != -1)
    {
        rc = sqlite3_exec(db, sql, &CheckPasswordCallback,
                          &password_is_correct, &err_msg);

        if (rc == SQLITE_OK)
        {
            if (password_is_correct)
            {
                rc = asprintf(&sql, "UPDATE users "
                                    "SET password = '%s' "
                                    "WHERE rowid = %d;",
                              new_password, user->rowid);

                if (rc != -1)
                {
                    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);

                    if (rc == SQLITE_OK)
                    {
                        printf("Your password was successfully changed.\n");
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
                    fprintf(stderr, "%s %s\n", kErr, kQueryExecutionErr);
                }
            }
            else
            {
                printf("Your password is incorrect. "
                       "Please try again later.\n");
            }
        }
        else
        {
            fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
            sqlite3_free(err_msg);
        }
    }
    else
    {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
    }

    free(sql);
    free(current_password);
    free(new_password);
}

void ActivateStudent(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

    char *id_number = NULL;

    printf("\n--STUDENT ACTIVATION--\n");
    printf("Please enter a student ID: ");
    TakeStringInput(&id_number);

    // If valid id_number
    // Perhaps better to check if student
    rc = asprintf(&sql, "UPDATE users "
                        "SET activated = 1 "
                        "WHERE id_number = '%s';",
                  id_number);
    if (rc == -1) {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }

    printf("The account was successfully activated.\n");

exit:
    free(id_number);
    free(sql);
}

void ChargeAccountAsAdmin(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

    char *id_number = NULL;
    int balance_amount = 0;

    printf("\n--CHARGE ACCOUNT--\n");
    printf("Please enter a student ID: ");
    TakeStringInput(&id_number);

    printf("Please enter the amount: ");
    balance_amount = TakeIntInput();

    // Check if student not admin
    // Check if activated
    // Check if valid id_number
    // Perhaps better to retrieve the rowid first
    rc = asprintf(&sql, "UPDATE users "
                        "SET balance = balance + %d "
                        "WHERE id_number = '%s';",
                  balance_amount, id_number);

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

    printf("The balance was successfully updated.\n");

exit:
    free(id_number);
    free(sql);
}

void ListStudents(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

    printf("\n--STUDENTS--");

    rc = asprintf(&sql, "SELECT rowid, first_name, last_name, "
                        "id_number, balance FROM users;");

    if (rc == -1)
    {
        printf("%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }

    rc = sqlite3_exec(db, sql, &PrintRecordCallback, "users", &err_msg);

    if (rc != SQLITE_OK)
    {
        printf("%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }

exit:
    free(sql);
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

    rc = asprintf(&sql, "INSERT INTO lunchrooms "
                        "VALUES ('%s', '%s', %d, %d);",
                  name, address, capacity, sex);
    if (rc == -1) {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }

    rc = asprintf(&sql, "INSERT INTO lunchroom_meal_types "
                        "VALUES (%lld, %d);", 
                        sqlite3_last_insert_rowid(db), meal_type_id);
    if (rc == -1) {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
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

        rc = asprintf(&sql, "INSERT INTO foods "
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

    if (rc == 0)
    {
        printf("\n--DEFINE MEAL TYPE--\n");
        printf("Please complete the following form.\n");

        printf("Name: ");
        TakeStringInput(&name);

        // Check if such meal type exists
        rc = asprintf(&sql, "INSERT INTO meal_types "
                            "VALUES ('%s');",
                      name);

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
    }
}

void ListMealTypes(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

    printf("\n--MEAL TYPES--\n");

    rc = asprintf(&sql, "SELECT rowid, name "
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
    int meal_plan_id = 0;
    int food_quantity = 0;
    char *date = NULL;

    if (rc == 0)
    {
        printf("\n--DEFINE MEAL PLAN--\n");
        printf("Please complete the following form.\n");

        printf("Food ID: ");
        food_id = TakeIntInput();

        printf("Lunchroom ID: ");
        lunchroom_id = TakeIntInput();

        printf("Meal type ID: ");
        meal_plan_id = TakeIntInput();

        printf("Food quantity: ");
        food_quantity = TakeIntInput();

        // Check date format
        printf("Date (YYYY-MM-DD): ");
        TakeStringInput(&date);

        // Check if such ids exist
        rc = asprintf(&sql, "INSERT INTO meal_plans "
                            "VALUES (%d, %d, %d, %d, '%s');",
                            food_id, lunchroom_id, meal_plan_id,
                            food_quantity, date);

        if (rc != -1)
        {
            rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);

            if (rc == SQLITE_OK)
            {
                printf("The meal plan was succesfully defined.\n");
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
        free(date);
    }
}

void ListLunchrooms(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

    printf("\n--LUNCHROOMS--");

    rc = asprintf(&sql, "SELECT rowid, name, capacity, sex "
                        "FROM lunchrooms;");
    if (rc == -1) {
        printf("%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }

    rc = sqlite3_exec(db, sql, &PrintRecordCallback, "lunchrooms", &err_msg);
    if (rc != SQLITE_OK) {
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

    rc = asprintf(&sql, "SELECT rowid, name, food_type, price "
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
        goto exit;
    }

exit:
    free(sql);
}

void ListMealPlans(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

    printf("\n--MEAL PLANS--\n");

    rc = asprintf(&sql, "SELECT rowid, food_id, lunchroom_id, "
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
