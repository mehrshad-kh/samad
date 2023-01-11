//
//  shared.c
//  samad
//
//  Created by Mehrshad on 10/10/1401 AP.
//

#include <stdio.h>
#include <stdlib.h>
#include "shared.h"
#include "admin.h"
#include "student.h"
#include "callback.h"
#include "utility.h"

extern const char *const kAllocationErr;
extern const char *const kQueryGenerationErr;
extern const char *const kQueryExecutionErr;
extern const char *const kDatabaseOpenErr;
extern const char *const kDatabaseCloseErr;

sqlite3 *OpenDatabase(const char *filename)
{
    int rc = 0;
    
    sqlite3 *db = NULL;
    
    rc = sqlite3_open(filename, &db);
    
    if (rc == SQLITE_OK) {
        return db;
    } else {
        fprintf(stderr, "ERROR: %s: %s\n",
                kDatabaseOpenErr, sqlite3_errmsg(db));
        return NULL;
    }
}

void CloseDatabase(sqlite3 *db)
{
    int rc = 0;
    
    rc = sqlite3_close(db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "ERROR: %s: %s\n",
                kDatabaseCloseErr, sqlite3_errmsg(db));
    }
}

int CreateUsersTable(sqlite3 *db)
{
    int rc = 0;
    int value = 0;
    char *err_msg = NULL;
    char *sql = "CREATE TABLE IF NOT EXISTS users ("
    "user_type INTEGER, activated BOOLEAN, "
    "first_name VARCHAR(100), last_name VARCHAR(100), "
    "id_number VARCHAR(50), national_id VARCHAR(50), "
    "birthdate TEXT, gender INTEGER, charge INTEGER, "
    "password VARCHAR(200), salt BLOB);";
    
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);

    if (rc == SQLITE_OK) {
        value = 0;
    } else {
        fprintf(stderr, "ERROR: %s: %s\n", kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        value = -1;
    }

    return value;
}

int CreateLunchroomsTable(sqlite3 *db)
{
    int rc = 0;
    int value = 0;
    char *err_msg = NULL;
    char *sql = "CREATE TABLE IF NOT EXISTS lunchrooms ("
    "name VARCHAR(100), address VARCHAR(300), "
    "capacity INTEGER, gender INTEGER, meal_types VARCHAR(100));";

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);

    if (rc == SQLITE_OK) {
        value = 0;
    } else {
        fprintf(stderr, "ERROR: %s: %s\n", kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        value = -1;
    }

    return value;
}

int CreateFoodsTable(sqlite3 *db)
{
    int rc = 0;
    int value = 0;
    char *err_msg = NULL;
    char *sql = "CREATE TABLE IF NOT EXISTS foods ("
    "name VARCHAR(100), food_type VARCHAR(100), price INTEGER);";

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);

    if (rc == SQLITE_OK) {
        value = 0;
    } else {
        fprintf(stderr, "ERROR: %s: %s\n", kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        value = -1;
    }

    return value;
}

int CreateMealPlansTable(sqlite3 *db)
{
    int rc = 0;
    int value = 0;
    char *err_msg = NULL;
    char *sql = "CREATE TABLE IF NOT EXISTS meal_plans ("
    "food_id INTEGER, lunchroom_id INTEGER, food_quantity INTEGER, "
    "date char(10));";

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);

    if (rc == SQLITE_OK) {
        value = 0;
    } else {
        fprintf(stderr, "ERROR: %s: %s\n", kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        value = -1;
    }

    return value;
}

bool IsFirstLaunch(sqlite3 *db)
{
    bool value = false;
    int rc = 0;
    int user_count = 0;
    char *err_msg = NULL;
    char *sql = "SELECT COUNT(*) FROM users;";

    rc = sqlite3_exec(db, sql, &CountCallback, &user_count, &err_msg);

    if (rc == SQLITE_OK) {
        if (user_count == 0)
            value = true;
        else
            value = false;
    } else {
        fprintf(stderr, "ERROR: %s: %s\n", kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
    }

    return value;
}

void PerformAccountCreation(sqlite3 *db, int user_type)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

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
    if ((user_type != 0 && user_type != 1)
        || (gender != 0 && gender != 1)) {
        printf("Invalid information. Please try again later.\n");
        goto exit2;
    }
    
    rc = asprintf(&sql, "INSERT INTO users "
                  "VALUES (%d, %d, '%s', '%s', '%s', "
                  "'%s', '%s', %d, %d, '%s', %d);",
                  user_type, activated, first_name, last_name, id_number,
                  national_id, birthdate, gender, 0, password, 0);
    
    if (rc == -1) {
        fprintf(stderr, "ERROR: %s\n", kQueryGenerationErr);
        goto exit;
    }
    
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "ERROR: %s: %s\n", kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }
    
    printf("The account was successfully created.\n");
exit:
    free(sql);
exit2:
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

    bool is_activated = false;
    char *username = NULL;
    char *password = NULL;
    struct User *user = NULL;

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
        
        if (rc == -1) {
            fprintf(stderr, "ERROR: %s\n", kQueryGenerationErr);
            goto exit;
        }
        
        rc = sqlite3_exec(db, sql, &CheckActivationCallback,
                          &is_activated, &err_msg);
        
        if (rc != SQLITE_OK) {
            fprintf(stderr, "ERROR: %s: %s\n", kQueryExecutionErr, err_msg);
            sqlite3_free(err_msg);
            goto exit;
        }
        
        if (!is_activated) {
            printf("Your account is not activated. "
                   "Please contact the administrator.\n");
            user = NULL;
            goto exit;
        }
        
        rc = asprintf(&sql, "SELECT rowid, * FROM users "
                      "WHERE id_number = '%s' "
                      "AND password = '%s';",
                      username, password);
        
        if (rc == -1) {
            fprintf(stderr, "ERROR: %s\n", kQueryGenerationErr);
            goto exit;
        }
            
        rc = sqlite3_exec(db, sql, &LoginCallback, &user, &err_msg);
                    
        if (rc != SQLITE_OK) {
            fprintf(stderr, "ERROR: %s: %s\n", kQueryExecutionErr, err_msg);
            sqlite3_free(err_msg);
            goto exit;
        }
                    
        if (user == NULL) {
            printf("Your username or password is incorrect.\n"
                   "Please try again later.\n");
        } else {
            printf("Logged in as %s %s\n", user->first_name, user->last_name);
            break;
        }
    }

exit:
    free(username);
    free(password);
    free(sql);

    return user;
}

void PerformLogout(struct User **user)
{
    free((*user)->first_name);
    free((*user)->last_name);
    free((*user)->id_number);
    free((*user)->national_id);
    free((*user)->birthdate);
    free(*user);
    
    *user = NULL;
}

void DisplayLoginMenu(sqlite3 *db)
{
    int input = 0;
    int is_first_launch = 0;
    int user_type = 0;
    struct User *user = NULL;
    
    is_first_launch = IsFirstLaunch(db);
    
    printf("\n--SAMAD--\n");
    printf("0: Quit\n"
           "1: Log in\n"
           "2: Sign up\n");

input_generation:
    input = TakeShellInput();

    switch (input) {
        case 0:
            printf("Have a great day!\n");
            goto exit;
            break;
        case 1:
            if (is_first_launch) {
                printf("No admins on file.\n");
                goto input_generation;
            } else {
                user = PerformLogin(db);
                if (user == NULL) {
                    DisplayLoginMenu(db);
                } else {
                    if (user->user_type == kAdmin) {
                        DisplayAdminMenu(db, &user);
                    } else {
                        DisplayStudentMenu(db, &user);
                    }
                }
            }
            break;
        case 2:
            user_type = is_first_launch ? kAdmin : kStudent;
            PerformAccountCreation(db, user_type);
            DisplayLoginMenu(db);
            break;
        default:
            printf("Invalid input. Please try again.\n");
            goto input_generation;
    }
    
exit:
    input = 0;
}