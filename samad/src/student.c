//
//  student.c
//  samad
//
//  Created by Mehrshad on 17/10/1401 AP.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "shared.h"
#include "student.h"
#include "callback.h"
#include "utility.h"
#include "mklib/mklib.h"

extern const char *const kAllocationErr;
extern const char *const kQueryGenerationErr;
extern const char *const kQueryExecutionErr;
extern const char *const kDatabaseOpenErr;
extern const char *const kDatabaseCloseErr;

void DisplayStudentMenu(sqlite3 *db, struct User **user)
{
    int input = 0;
    
    printf("\n--STUDENT--\n");
    printf("What would you like to do?\n");
    printf("0: Log out\n"
           "1: Reserve food\n"
           "2: Take food (!)\n"
           "3: Charge account\n"
           "4: Transfer charge (!)\n"
           "5: Show reserved food (!)\n");
    
input_generation:
    input = TakeShellInput();
    
    switch (input)
    {
        case 0:
            PerformLogout(user);
            DisplayLoginMenu(db);
            break;
        case 1:
            ReserveFood(db, *user);
            DisplayStudentMenu(db, user);
            break;
        case 2:
            break;
        case 3:
            ChargeAccountAsStudent(db, (*user)->id_number);
            DisplayStudentMenu(db, user);
            break;
        default:
            printf("Invalid input. Please try again.\n");
            goto input_generation;
    }
}

void ReserveFood(sqlite3 *db, struct User *user)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;
    
    int input = 0;
    int lunchroom_id = 0;
    struct LunchroomNode *ptr = NULL;
    struct LunchroomNode *head = NULL;
    
    time_t t = 0;
    struct tm *current_time = NULL;
    char *date = NULL;
    struct tm *min_time = NULL;
    char *min_date = NULL;
    struct tm *max_time = NULL;
    char *max_date = NULL;

    printf("\n--FOOD RESERVATION--\n");
    printf("Please select a lunchroom:\n");

    rc = asprintf(&sql, "SELECT rowid, name, meal_types, gender "
                  "FROM lunchrooms "
                  "WHERE gender = %d;", user->gender);
    if (rc == -1) {
        fprintf(stderr, "ERROR: %s\n", kQueryGenerationErr);
        goto exit;
    }
    
    rc = sqlite3_exec(db, sql, &RetrieveListCallback, &head, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "ERROR: %s: %s\n", kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }
    
    LNPrintList(head);
    
input_generation:
    input = TakeShellInput();
    
    // Retrieve data from meal_plans
    ptr = head;
    while (ptr != NULL) {
        if (input == ptr->lunchroom->index)
            lunchroom_id = ptr->lunchroom->rowid;
        ptr = ptr->next;
    }
    
    if (input == 0) {
        printf("Invalid input. Please try again.\n");
        goto input_generation;
    }
    
    t = time(NULL);
    current_time = localtime(&t);
    min_time = AddTimeByDays(current_time, 3);
    max_time = AddTimeByDays(current_time, 14);
    
    min_date = (char *)calloc(10, sizeof(char));
    max_date = (char *)calloc(10, sizeof(char));
    strftime(min_date, 10, "%Y-%m-%d", min_time);
    strftime(max_date, 10, "%Y-%m-%d", max_time);
    
    rc = asprintf(&sql, "SELECT rowid, * FROM meal_plans "
                  "WHERE lunchroom_id = %d "
                  "AND date > '%s' "
                  "AND date < '%s';", lunchroom_id, min_date, max_date);
    if (rc == -1) {
        fprintf(stderr, "ERROR: %s\n", kQueryGenerationErr);
        goto exit_1;
    }
    
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "ERROR: %s: %s\n", kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit_1;
    }
    
    // Returned dates are not correct
    printf("SQL statement: %s\n", sql);
    
exit_1:
    LNFreeList(&head);
    free(min_date);
    free(max_time);
    
exit:
    free(sql);
}

void ChargeAccountAsStudent(sqlite3 *db, const char *id_number)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;
    
    char *card_number = NULL;
    char *one_time_password = NULL;
    int charge_amount = 0;
    int balance = 0;
    
    printf("\n--CHARGE ACCOUNT--\n");
    
    printf("Please enter the amount: ");
    charge_amount = TakeIntInput();
    
    printf("Please enter credit card number: ");
    TakeStringInput(&card_number);
    
    printf("Please enter OTP: ");
    TakeStringInput(&one_time_password);
    
    // Check if student not admin
    // Check if activated
    // Check if valid id_number
    // Perhaps better to retrieve the rowid first
    rc = asprintf(&sql, "UPDATE users "
                  "SET charge = charge + %d "
                  "WHERE id_number = '%s';", charge_amount, id_number);
    
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
    
    rc = asprintf(&sql, "SELECT charge "
                  "FROM users "
                  "WHERE id_number = '%s';", id_number);
    
    if (rc == -1) {
        fprintf(stderr, "ERROR: %s\n", kQueryGenerationErr);
        goto exit;
    }
    
    rc = sqlite3_exec(db, sql, &GetAccountBalanceCallback, &balance, &err_msg);
    
    printf("Your account balance is now %d R.\n", balance);
    
exit:
    free(card_number);
    free(one_time_password);
    free(sql);
}
