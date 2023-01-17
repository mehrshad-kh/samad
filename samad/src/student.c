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
#include "mklib.h"

extern const int min_days_for_reservation;;
extern const int max_days_for_reservation;

extern const char *const kErr;
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
           "4: Transfer balance (!)\n"
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
    // int lunchroom_id = 0;
    // int meal_plan_id = 0;
    int rowid = 0;
    struct Lunchroom *lunchroom_ptr = NULL;
    struct Lunchroom *lunchroom_head = NULL;
    struct IncMealPlan *inc_meal_plan_head = NULL;
    struct MealPlan *head = NULL;
    struct MealPlan *meal_plan_ptr = NULL;
    
    struct tm *min_time = NULL;
    struct tm *max_time = NULL;
    
    printf("\n--FOOD RESERVATION--\n");
    printf("Please select a lunchroom:\n");
    
    rc = asprintf(&sql, "SELECT rowid, name, sex "
                  "FROM lunchrooms "
                  "WHERE sex = %d;", user->sex);
    if (rc == -1) {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }
    
    rc = sqlite3_exec(db, sql, &GetLunchroomsCallback, &lunchroom_head, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }
    
    GetMealTypeForLunchrooms(db, lunchroom_head);
    
    LPrintList(lunchroom_head);
    
    if (lunchroom_head == NULL)
        goto exit;
    
input_generation:
    input = TakeShellInput();
    
    lunchroom_ptr = lunchroom_head;
    while (lunchroom_ptr != NULL) {
        if (input == lunchroom_ptr->data->index) {
            rowid = lunchroom_ptr->data->rowid;
            break;
        }
        lunchroom_ptr = lunchroom_ptr->next;
    }
    
    if (rowid == 0) {
        printf("Invalid input. Please try again.\n");
        goto input_generation;
    }
    
    min_time = GetTimeAdvancedBy(min_days_for_reservation);
    max_time = GetTimeAdvancedBy(max_days_for_reservation);
    
    if (min_time == NULL || max_time == NULL) {
        fprintf(stderr, "%s %s\n", kErr, kAllocationErr);
        goto exit_1;
    }
    
    free(sql);
    rc = asprintf(&sql, "SELECT rowid, * FROM meal_plans "
                  "WHERE lunchroom_id = %d "
                  "AND date >= '%d-%02d-%02d' "
                  "AND date <= '%d-%02d-%02d' "
                  "ORDER BY date;", rowid,
                  min_time->tm_year + 1900, min_time->tm_mon + 1,
                  min_time->tm_mday, max_time->tm_year + 1900,
                  max_time->tm_mon + 1, max_time->tm_mday);
    if (rc == -1) {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit_2;
    }
    
    rc = sqlite3_exec(db, sql, &GetIncMealPlansCallback,
                      &inc_meal_plan_head, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit_2;
    }
    
    head = GetMealPlans(db, inc_meal_plan_head);
    
    MPPrintList(head);
    
    if (head == NULL)
        goto exit_2;
    
input_generation2:
    input = TakeShellInput();
    
    rowid = 0;
    meal_plan_ptr = head;
    while (meal_plan_ptr != NULL) {
        if (input == meal_plan_ptr->data->index) {
            rowid = meal_plan_ptr->data->rowid;
            break;
        }
        meal_plan_ptr = meal_plan_ptr->next;
    }
    
    if (rowid == 0) {
        printf("Invalid input. Please try again.\n");
        goto input_generation2;
    }
    
eligibility_check:
    if (meal_plan_ptr->data->food_quantity == 0) {
        printf("Such a reservation cannot be made.\n");
        goto exit_2;
    } else if (meal_plan_ptr->data->price > user->balance) {
        printf("Your account balance is not enough.\n"
               "Please increase your balance first.\n");
        user->balance = ChargeAccountAsStudent(db, user->id_number);
        goto eligibility_check;
    }
    
    free(sql);
    rc = asprintf(&sql, "INSERT INTO reservations "
             "VALUES (%d, %d);", user->rowid, rowid);
    if (rc == -1) {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit_2;
    }
    
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit_2;
    }
    
    free(sql);
    rc = asprintf(&sql, "UPDATE users "
             "SET balance = balance - %d "
             "WHERE rowid = %d;", meal_plan_ptr->data->price,
             user->rowid);
    if (rc == -1) {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit_2;
    }
    
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit_2;
    }
    
    free(sql);
    rc = asprintf(&sql, "UPDATE meal_plans "
             "SET food_quantity = food_quantity - 1 "
             "WHERE rowid = %d;", meal_plan_ptr->data->rowid);
    if (rc == -1) {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit_2;
    }
    
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit_2;
    }
    
    printf("Your reservation was successfully submitted.\n");
    
exit_2:
    free(min_time);
    free(max_time);
    
exit_1:
    LFreeList(&lunchroom_head);
    
exit:
    free(sql);
}

int ChargeAccountAsStudent(sqlite3 *db, const char *id_number)
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
                  "SET balance = balance + %d "
                  "WHERE id_number = '%s';", charge_amount, id_number);
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
    
    free(sql);
    rc = asprintf(&sql, "SELECT balance "
                  "FROM users "
                  "WHERE id_number = '%s';", id_number);
    if (rc == -1) {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }
    
    rc = sqlite3_exec(db, sql, &GetAccountBalanceCallback, &balance, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }
    
    printf("Your account balance is now %d R.\n", balance);
    
exit:
    free(card_number);
    free(one_time_password);
    free(sql);
    
    return balance;
}

void GetMealTypeForLunchrooms(sqlite3 *db, struct Lunchroom *head)
{
//    int rc = 0;
//    char *err_msg = NULL;
//    char *sql = NULL;
//
//    struct Lunchroom *lunchroom = NULL;
//    struct IncMealType *inc_meal_type_head = NULL;
//
//    lunchroom = head;
//    // Infinite loop
//    while (lunchroom != NULL) {
//        asprintf(&sql, "SELECT meal_type_id "
//                 "FROM lunchroom_meal_types "
//                 "WHERE lunchroom_id = %d;", lunchroom->data->rowid);
//
//        sqlite3_exec(db, sql, &SetMealTypeNameCallback2, &inc_meal_type_head, &err_msg);
//    }
//
//    free(sql);
}
