//
//  samad-lib.c
//  samad
//
//  Created by Mehrshad on 10/10/1401 AP.
//

#include <stdio.h>
#include <stdlib.h>
// #include <string.h>
#include "samad-lib.h"
#include "callback.h"
#include "utilities.h"

const char *const kAllocationErr = "Cannot allocate memory";
const char *const kQueryGenerationErr = "Cannot generate query";
const char *const kQueryExecutionErr = "Cannot execute query";
const char *const kDatabaseOpenErr = "Cannot open database";
const char *const kDatabaseCloseErr = "Cannot close database";

int CreateUsersTable(sqlite3 *db)
{
    int rc = 0;
    int return_value = 0;
    char *err_msg = NULL;
    char *sql = "CREATE TABLE IF NOT EXISTS users ("
    "user_type INTEGER, activated BOOLEAN, "
    "first_name varchar(100), last_name varchar(100), "
    "id_number varchar(50), national_id varchar(50), "
    "birthdate char(10), gender INTEGER, charge INTEGER, "
    "password varchar(200), salt BLOB);";
    
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);

    if (rc == SQLITE_OK)
    {
        return_value = 0;
    }
    else
    {
        fprintf(stderr, "ERROR: %s: %s\n", kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        return_value = -1;
    }

    return return_value;
}

int CreateLunchroomsTable(sqlite3 *db)
{
    int rc = 0;
    int return_value = 0;
    char *err_msg = NULL;
    char *sql = "CREATE TABLE IF NOT EXISTS lunchrooms ("
    "name varchar(100), address varchar(300), "
    "capacity INTEGER, gender INTEGER, meal_types varchar(100));";

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);

    if (rc == SQLITE_OK) {
        return_value = 0;
    } else {
        fprintf(stderr, "ERROR: %s: %s\n", kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        return_value = -1;
    }

    return return_value;
}

int CreateFoodsTable(sqlite3 *db)
{
    int rc = 0;
    int return_value = 0;
    char *err_msg = NULL;
    char *sql = "CREATE TABLE IF NOT EXISTS foods ("
    "name varchar(100), food_type varchar(100), price INTEGER);";

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);

    if (rc == SQLITE_OK) {
        return_value = 0;
    } else {
        fprintf(stderr, "ERROR: %s: %s\n", kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        return_value = -1;
    }

    return return_value;
}

int CreateMealPlansTable(sqlite3 *db)
{
    int rc = 0;
    int return_value = 0;
    char *err_msg = NULL;
    char *sql = "CREATE TABLE IF NOT EXISTS meal_plans ("
    "food_id INTEGER, lunchroom_id INTEGER, food_quantity INTEGER, "
    "date char(10));";

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);

    if (rc == SQLITE_OK) {
        return_value = 0;
    } else {
        fprintf(stderr, "ERROR: %s: %s\n", kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        return_value = -1;
    }

    return return_value;
}

bool IsFirstLaunch(sqlite3 *db)
{
    bool return_value = false;
    int rc = 0;
    int user_count = 0;
    char *err_msg = NULL;
    char *sql = "SELECT COUNT(*) FROM users;";

    rc = sqlite3_exec(db, sql, &CountCallback, &user_count, &err_msg);

    if (rc == SQLITE_OK)
    {
        if (user_count == 0)
            return_value = true;
        else
            return_value = false;
    }
    else
    {
        fprintf(stderr, "ERROR: %s: %s\n", kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
    }

    return return_value;
}

void PerformAccountCreation(sqlite3 *db, int user_type)
{
    int rc = 0;
    char *err_msg = NULL;
    char *end_ptr = NULL;
    char *sql = NULL;

    size_t n = 0;
    int activated = (user_type == kStudent ? 0 : 1);
    char *first_name = NULL;
    char *last_name = NULL;
    char *id_number = NULL;
    char *national_id = NULL;
    char *birthdate = NULL;
    int gender = 0;
    char *password = NULL;

    printf("\n--ACCOUNT CREATION--\n");
    printf("Please complete the following form to create a new account.\n");

    // Perhaps better to remove while
    while (true) {
        if (user_type == kOptional) {
            printf("Account type (0: admin, 1: student): ");
            user_type = TakeIntInput();
        }

        printf("First name: ");
        TakeStringInput(&first_name);

        printf("Last name: ");
        TakeStringInput(&last_name);

        printf("ID number: ");
        TakeStringInput(&id_number);

        printf("National ID: ");
        TakeStringInput(&national_id);

        printf("Birthdate (YYYY-MM-DD): ");
        TakeStringInput(&birthdate);

        printf("Gender (0: male, 1: female): ");
        gender = TakeIntInput();

        printf("Password: ");
        TakeStringInput(&password);

        // Better input validation
        if ((user_type == 0 || user_type == 1)
            && (gender == 0 || gender == 1)) {
            rc = asprintf(&sql, "INSERT INTO users "
                          "VALUES (%d, %d, '%s', '%s', '%s', "
                          "'%s', '%s', %d, %d, '%s', %d);",
                          user_type, activated, first_name, last_name, id_number,
                          national_id, birthdate, gender, 0, password, 0);

            if (rc != -1) {
                rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);

                if (rc == SQLITE_OK) {
                    printf("The account was successfully created.\n");
                    break;
                } else {
                    fprintf(stderr, "ERROR: %s: %s\n",
                            kQueryExecutionErr, err_msg);
                    sqlite3_free(err_msg);
                }
            } else {
                fprintf(stderr, "ERROR: %s\n", kQueryGenerationErr);
            }
        } else {
            printf("The information was incorrect. Please try again.\n\n");
        }
    }

    free(sql);
    free(first_name);
    free(last_name);
    free(id_number);
    free(national_id);
    free(birthdate);
    free(password);
}

struct User *PerformLogin(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

    size_t n = 0;
    bool is_activated = false;
    char *username = NULL;
    char *password = NULL;
    struct User *user_ptr = NULL;

    printf("\n--LOGIN--\n");

    while (true) {
        printf("Username: ");
        TakeStringInput(&username);

        // Mask password
        // Compute hash
        printf("Password: ");
        TakeStringInput(&password);
        
        rc = asprintf(&sql, "SELECT activated FROM users "
                      "WHERE id_number = '%s' AND password = '%s';",
                      username, password);
        
        if (rc != -1) {
            rc = sqlite3_exec(db, sql, &CheckActivationCallback,
                              &is_activated, &err_msg);
            
            if (rc == SQLITE_OK) {
                if (is_activated) {
                    rc = asprintf(&sql, "SELECT rowid, * FROM users "
                                  "WHERE id_number = '%s' "
                                  "AND password = '%s';",
                                  username, password);

                    if (rc != -1) {
                        rc = sqlite3_exec(db, sql, &LoginCallback,
                                          &user_ptr, &err_msg);

                        if (rc == SQLITE_OK) {
                            if (user_ptr != NULL) {
                                break;
                            } else {
                                printf("Your username or password is incorrect.\n"
                                       "Please try again.\n\n");
                            }
                        } else {
                            fprintf(stderr, "ERROR: %s: %s\n",
                                    kQueryExecutionErr, err_msg);
                            sqlite3_free(err_msg);
                        }
                    } else {
                        fprintf(stderr, "ERROR: %s\n", kQueryGenerationErr);
                    }
                } else {
                    printf("Your account is not activated."
                           "Please contact the administrator.\n\n");
                    user_ptr = NULL;
                }
            } else {
                fprintf(stderr, "ERROR: %s: %s\n",
                        kQueryExecutionErr, err_msg);
                sqlite3_free(err_msg);
            }
        } else {
            fprintf(stderr, "ERROR: %s\n", kQueryGenerationErr);
        }
    }

    free(username);
    free(password);
    free(sql);

exit:
    return user_ptr;
}

void PerformLogout(struct User **user)
{
    free((*user)->first_name);
    free((*user)->last_name);
    free((*user)->id_number);
    free((*user)->national_id);
    free((*user)->birthdate);
    free(*user);
    // Not quite what I want
    *user = NULL;
}

void DisplayLoginMenu(sqlite3 *db)
{
    int entry = 0;
    int user_type = 0;
    struct User *user = NULL;
    
    printf("\n--SAMAD--\n");
    printf("0: Exit\n"
           "1: Log in\n"
           "2: Sign up\n");

input_generation:
    entry = TakeShellInput();

    switch (entry)
    {
        case 0:
            printf("Have a great day!\n");
            goto exit;
            break;
        case 1:
            break;
        case 2:
            user_type = IsFirstLaunch(db) ? kAdmin : kStudent;
            PerformAccountCreation(db, user_type);
            break;
        default:
            printf("Invalid input. Please try again.\n");
            goto input_generation;
    }

    user = PerformLogin(db);
    printf("Login successful.\n");

    if (user->user_type == kAdmin)
    {
        DisplayAdminMenu(db, &user);
    }
    else
    {
        // DisplayStudentMenu
    }
    
exit:
    entry = 0;
}

void DisplayAdminMenu(sqlite3 *db, struct User **user)
{
    int entry = 0;

    printf("\n--ADMIN-- %s (%s)\n", (*user)->first_name, (*user)->id_number);
    printf("What would you like to do?\n");
    printf("0: Log out\n"
           "1: Account management\n"
           "2: Food management\n");

input_generation:
    entry = TakeShellInput();

    switch (entry)
    {
        case 0:
            PerformLogout(user);
            DisplayLoginMenu(db);
            break;
        case 1:
            PerformAccountManagement(db, user);
            break;
        case 2:
            PerformFoodManagement(db, user);
            break;
        default:
            printf("Invalid input. Please try again.\n");
            goto input_generation;
    }
}

void PerformAccountManagement(sqlite3 *db, struct User **user)
{
    int entry = 0;

    printf("\n--ACCOUNT MANAGEMENT--\n");
    printf("What would you like to do?\n");
    printf("0: Return\n"
           "1: Change my password\n"
           "2: Change a student password\n"
           "3: Activate a student\n"
           "4: Deactivate a student\n"
           "5: Remove a student\n"
           "6: Register a new user\n"
           "7: Charge a student account\n");

input_generation:
    entry = TakeShellInput();

    switch (entry) {
        case 0:
            DisplayAdminMenu(db, user);
            break;
        case 1:
            ChangeMyPassword(db, *user);
            PerformAccountManagement(db, user);
            break;
        case 6:
            PerformAccountCreation(db, kOptional);
            PerformAccountManagement(db, user);
            break;
        case 7:
            // ChargeStudentAccount(db);
            PerformFoodManagement(db, user);
            break;
        default:
            printf("Invalid input. Please try again.\n");
            goto input_generation;
    }
}

void PerformFoodManagement(sqlite3 *db, struct User **user)
{
    int input = 0;
    
    printf("\n--FOOD MANAGEMENT--\n");
    printf("0: Return\n"
           "1: Define lunchroom\n"
           "2: Define food\n"
           "3: Define meal plan\n");
    
input_generation:
    input = TakeShellInput();
    
    switch (input) {
        case 0:
            DisplayAdminMenu(db, user);
            break;
        case 1:
            DefineLunchroom(db);
            break;
        case 2:
            DefineFood(db);
            break;
        case 3:
            DefineMealPlan(db);
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

    if (rc != -1) {
        rc = sqlite3_exec(db, sql, &CheckPasswordCallback,
                          &password_is_correct, &err_msg);

        if (rc == SQLITE_OK) {
            if (password_is_correct) {
                rc = asprintf(&sql, "UPDATE users "
                                    "SET password = '%s' "
                                    "WHERE rowid = %d;",
                              new_password, user->rowid);

                if (rc != -1) {
                    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);

                    if (rc == SQLITE_OK) {
                        printf("Your password was successfully changed.\n");
                    } else {
                        fprintf(stderr, "ERROR: %s: %s\n",
                                kQueryExecutionErr, err_msg);
                        sqlite3_free(err_msg);
                    }
                } else {
                    fprintf(stderr, "ERROR: %s\n", kQueryExecutionErr);
                }
            } else {
                printf("Your password is incorrect. "
                       "Please try again later.\n");
            }
        } else {
            fprintf(stderr, "ERROR: %s: %s\n", kQueryExecutionErr, err_msg);
            sqlite3_free(err_msg);
        }
    } else {
        fprintf(stderr, "ERROR: %s\n", kQueryGenerationErr);
    }

    free(sql);
    free(current_password);
    free(new_password);
}

void DefineLunchroom(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;
    
    char *name = NULL;
    char *address = NULL;
    int capacity = 0;
    int gender = 0;
    char *meal_types = NULL;
    
    rc = CreateLunchroomsTable(db);
    
    if (rc == 0) {
        printf("\n--DEFINE LUNCHROOM--\n");
        printf("Please complete the following form.\n");
        
        printf("Name: ");
        TakeStringInput(&name);
        
        printf("Address: ");
        TakeStringInput(&address);
        
        printf("Capacity: ");
        capacity = TakeIntInput();
        
        printf("Gender (0: male, 1: female): ");
        gender = TakeIntInput();
        
        printf("Meal types (lunch-dinner): ");
        TakeStringInput(&meal_types);
        
        rc = asprintf(&sql, "INSERT INTO lunchrooms "
                      "VALUES ('%s', '%s', %d, %d, '%s');",
                      name, address, capacity, gender, meal_types);
        
        if (rc != -1) {
            rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
            
            if (rc == SQLITE_OK) {
                printf("The lunchroom was successfully defined.\n");
            } else {
                fprintf(stderr, "ERROR: %s: %s\n",
                        kQueryExecutionErr, err_msg);
                sqlite3_free(err_msg);
            }
        } else {
            fprintf(stderr, "ERROR: %s\n", kQueryExecutionErr);
        }
        
        free(sql);
        free(name);
        free(address);
        free(meal_types);
    }
}

void DefineFood(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;
    
    char *name = NULL;
    char *food_type = NULL;
    int price = 0;
    
    rc = CreateFoodsTable(db);
    
    if (rc == 0) {
        printf("\n--DEFINE FOOD--\n");
        printf("Please complete the following form.\n");
        
        printf("Name: ");
        TakeStringInput(&name);
        
        printf("Food type: ");
        TakeStringInput(&food_type);
        
        printf("Price: ");
        price = TakeIntInput();

        rc = asprintf(&sql, "INSERT INTO foods "
                      "VALUES ('%s', '%s', %d);", name, food_type, price);
        
        if (rc != -1) {
            rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
            
            if (rc == SQLITE_OK) {
                printf("The food was succesfully defined.\n");
            } else {
                fprintf(stderr, "ERROR: %s: %s\n",
                        kQueryExecutionErr, err_msg);
                sqlite3_free(err_msg);
            }
        } else {
            fprintf(stderr, "ERROR: %s\n", kQueryExecutionErr);
        }
        
        free(sql);
        free(name);
        free(food_type);
    }
}

void DefineMealPlan(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;
    
    int food_id = 0;
    int lunchroom_id = 0;
    int food_quantity = 0;
    char *date = NULL;
    
    rc = CreateMealPlansTable(db);
    
    if (rc == 0) {
        printf("\n--DEFINE MEAL PLAN--\n");
        printf("Please complete the following form.\n");
        
        printf("Food ID: ");
        food_id = TakeIntInput();
        
        printf("Lunchroom ID: ");
        lunchroom_id = TakeIntInput();
        
        printf("Food quantity: ");
        food_quantity = TakeIntInput();
        
        printf("Date: ");
        TakeStringInput(&date);
        
        rc = asprintf(&sql, "INSERT INTO meal_plans "
                      "VALUES (%d, %d, %d, '%s');",
                      food_id, lunchroom_id, food_quantity, date);
        
        if (rc != -1) {
            rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
            
            if (rc == SQLITE_OK) {
                printf("The meal plan was succesfully defined.\n");
            } else {
                fprintf(stderr, "ERROR: %s: %s\n",
                        kQueryExecutionErr, err_msg);
                sqlite3_free(err_msg);
            }
        } else {
            fprintf(stderr, "ERROR: %s\n", kQueryExecutionErr);
        }
        
        free(sql);
        free(date);
    }
}
