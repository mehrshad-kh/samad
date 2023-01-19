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
           "4: Transfer balance\n"
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
        case 3:
            ChargeAccountAsStudent(db, *user);
            DisplayStudentMenu(db, user);
            break;
        case 4:
            SendCharge(db, *user);
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
    int rowid = 0;
    struct Lunchroom *lunchroom_ptr = NULL;
    struct Lunchroom *lunchroom_head = NULL;
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
    rc = asprintf(&sql, "SELECT mp.rowid, f.name, l.name, mt.name, "
                  "f.price, mp.food_quantity, mp.date "
                  "FROM meal_plans mp "
                  "INNER JOIN foods f "
                  "ON mp.food_id = f.rowid "
                  "INNER JOIN lunchrooms l "
                  "ON mp.lunchroom_id = l.rowid "
                  "INNER JOIN meal_types mt "
                  "ON mp.meal_type_id = mt.rowid "
                  "WHERE mp.lunchroom_id = %d "
                  "AND mp.date >= '%d-%02d-%02d' "
                  "AND mp.date <= '%d-%02d-%02d' "
                  "ORDER BY mp.date;", rowid,
                  min_time->tm_year + 1900, min_time->tm_mon + 1,
                  min_time->tm_mday, max_time->tm_year + 1900,
                  max_time->tm_mon + 1, max_time->tm_mday);
    if (rc == -1) {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit_2;
    }
    
    rc = sqlite3_exec(db, sql, &GetMealPlansCallback, &head, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit_2;
    }
    
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
    
    if (HasReservedBefore(db, user->rowid, rowid) == 1) {
        printf("Such a reservation has already been made.\n");
        goto exit_2;
    }
    
eligibility_check:
    if (meal_plan_ptr->data->food_quantity == 0) {
        printf("Such a reservation cannot be made.\n");
        goto exit_2;
    } else if (meal_plan_ptr->data->price > user->balance) {
        printf("Your account balance is not enough.\n"
               "Please increase your balance first.\n");
        ChargeAccountAsStudent(db, user);
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
    
    user->balance = GetBalance(db, user->rowid);
    
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

void ChargeAccountAsStudent(sqlite3 *db, struct User *user)
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
                  "WHERE rowid = %d;", charge_amount, user->rowid);
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
    
    balance = GetBalance(db, user->rowid);
    user->balance = balance;
    
    printf("Your account balance is now %d R.\n", user->balance);
    
exit:
    free(card_number);
    free(one_time_password);
    free(sql);
}

void SendCharge(sqlite3 *db, struct User *user)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;
    
    int input = 0;
    int charge_amount = 0;
    char *recipient_student_id = NULL;
    struct User *recipient_user = NULL;
    
    printf("\n--SEND CHARGE--\n");
    
    printf("Please enter the amount: ");
    charge_amount = TakeIntInput();
    
    printf("Please enter the recipient student ID: ");
    TakeStringInput(&recipient_student_id);
    
    rc = asprintf(&sql, "SELECT rowid, first_name, last_name "
             "FROM users "
             "WHERE id_number = '%s' "
             "AND user_type = %d;", recipient_student_id, kStudent);
    if (rc == -1) {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }
    
    rc = sqlite3_exec(db, sql, &GetFirstAndLastNames, &recipient_user, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit_1;
    }
    
    if (recipient_user == NULL) {
        printf("No such student ID.\n"
               "Please try again later.\n");
        goto exit_1;
    }
    
    printf("The recipient is %s %s.\n"
           "Would you like to proceed? (Y/N)\n",
           recipient_user->first_name, recipient_user->last_name);
    
input_generation:
    input = TakeCharInput();
    
    switch (input) {
        case (int)'Y':
            break;
        case (int)'N':
            goto exit_2;
            break;
        default:
            printf("Invalid input. Please try again.\n");
            goto input_generation;
    }
    
    if (charge_amount == 0) {
        printf("You cannot transfer 0 R.\n");
    } else if (GetBalance(db, user->rowid) >= charge_amount) {
        TransferBalance(db, charge_amount, user, recipient_user->rowid);
        user->balance = GetBalance(db, user->rowid);
        printf("The amount has been successfully transferred.\n"
               "Your balance is now %d R.\n", user->balance);
    } else {
        printf("Your account balance is not enough.\n");
    }

exit_2:
    free(recipient_user->first_name);
    free(recipient_user->last_name);
    free(recipient_user);
exit_1:
    free(sql);
exit:
    free(recipient_student_id);
}

int GetBalance(sqlite3 *db, int user_id)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;
    
    int balance = 0;
    
    rc = asprintf(&sql, "SELECT balance FROM users "
             "WHERE rowid = %d;", user_id);
    if (rc == -1) {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }
    
    rc = sqlite3_exec(db, sql, &GetBalanceCallback, &balance, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit_1;
    }
    
exit_1:
    free(sql);
exit:
    return balance;
}

void TransferBalance(sqlite3 *db, int charge_amount,
                     struct User *user, int recipient_id)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;
    
    rc = asprintf(&sql, "UPDATE users "
             "SET balance = balance - %d "
             "WHERE rowid = %d;", charge_amount, user->rowid);
    if (rc == -1) {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }
    
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit_1;
    }
    
    free(sql);
    rc = asprintf(&sql, "UPDATE users "
             "SET balance = balance + %d "
             "WHERE rowid = %d;", charge_amount, recipient_id);
    if (rc == -1) {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }
    
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit_1;
    }
    
exit_1:
    free(sql);
exit:
    rc = 0;
}

int HasReservedBefore(sqlite3 *db, int user_id, int meal_plan_id)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;
    
    int return_value = -1;
    int has_reserved = 0;
    
    rc = asprintf(&sql, "SELECT rowid FROM reservations "
             "WHERE user_id = %d AND meal_plan_id = %d;",
             user_id, meal_plan_id);
    if (rc == -1) {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }
    
    rc = sqlite3_exec(db, sql, &HasReservedBeforeCallback,
                      &has_reserved, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit_1;
    }
    
    if (has_reserved == 1)
        return_value = 1;
    else
        return_value = 0;
    
exit_1:
    free(sql);
exit:
    return return_value;
}
