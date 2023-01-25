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
#include "student.h"
#include "callback.h"
#include "utility.h"
#include "mklib.h"

extern const int kMinDaysForReservation;
extern const int kMaxDaysForReservation;

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
           "4: Send charge\n"
           "5: List reservations\n"
           "6: List transactions\n"
           "7: Change my password\n");
    
input_generation:
    input = TakeShellInput();
    
    switch (input) {
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
        case 5:
            ListReservations(db, *user);
            DisplayStudentMenu(db, user);
            break;
        case 6:
            ListTransactions(db, *user);
            DisplayStudentMenu(db, user);
            break;
        case 7:
            ChangeMyPassword(db, *user);
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
    int meal_type_id = 0;
    int meal_plan_id = 0;
    struct Lunchroom *lunchroom_ptr = NULL;
    struct Lunchroom *lunchroom_head = NULL;
    struct GNode *meal_plan_head = NULL;
    struct GNode *meal_plan_ptr = NULL;
    
    printf("\n--FOOD RESERVATION--\n");
    printf("Please select a lunchroom:\n");
    
    rc = asprintf(&sql, "SELECT ROW_NUMBER() OVER() AS row_num, "
                  "l.id AS lunchroom_id, "
                  "l.name AS lunchroom_name, "
                  "mt.id AS meal_type_id, "
                  "mt.name AS meal_type_name "
                  "FROM lunchrooms l "
                  "INNER JOIN lunchroom_meal_types lmt "
                  "ON l.id = lmt.lunchroom_id "
                  "INNER JOIN meal_types mt "
                  "ON lmt.meal_type_id = mt.id "
                  "WHERE l.sex = %d "
                  "ORDER BY lunchroom_name, meal_type_id;", user->sex);
    if (rc == -1) {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit1;
    }
    
    rc = sqlite3_exec(db, sql, &GetLunchroomsCallback, &lunchroom_head, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }
    
    LPrintList(lunchroom_head);
    
    if (lunchroom_head == NULL)
        goto exit;
    
input_generation:
    input = TakeShellInput();
    
    lunchroom_ptr = lunchroom_head;
    while (lunchroom_ptr != NULL) {
        if (input == lunchroom_ptr->data->index) {
            lunchroom_id = lunchroom_ptr->data->lunchroom_id;
            meal_type_id = lunchroom_ptr->data->meal_type_id;
            break;
        }
        lunchroom_ptr = lunchroom_ptr->next;
    }
    
    if (lunchroom_id == 0) {
        printf("Invalid input. Please try again.\n");
        goto input_generation;
    }
    
    free(sql);
    rc = asprintf(&sql, "SELECT ROW_NUMBER() OVER() AS row_num, "
                  "mp.id AS meal_plan_id, f.name AS food_name, "
                  "l.name AS lunchroom_name, mt.name AS meal_type_name, "
                  "f.price AS food_price, mp.food_quantity AS food_quantity, "
                  "mp.date AS date "
                  "FROM meal_plans mp "
                  "INNER JOIN foods f "
                  "ON mp.food_id = f.id "
                  "INNER JOIN lunchrooms l "
                  "ON mp.lunchroom_id = l.id "
                  "INNER JOIN meal_types mt "
                  "ON mp.meal_type_id = mt.id "
                  "WHERE mp.lunchroom_id = %d "
                  "AND mp.meal_type_id = %d "
                  "AND mp.date >= DATE('now', 'localtime', '+%d days') "
                  "AND mp.date <= DATE('now', 'localtime', '+%d days') "
                  "ORDER BY mp.date;", lunchroom_id, meal_type_id,
                  kMinDaysForReservation, kMaxDaysForReservation);
    if (rc == -1) {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit_1;
    }
    
    rc = sqlite3_exec(db, sql, &GetMealPlansCallback, &meal_plan_head, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit_1;
    }
    
    GPrintList(meal_plan_head, &PrintMealPlanData);
    
    if (meal_plan_head == NULL)
        goto exit_2;
    
input_generation2:
    input = TakeShellInput();
    
    meal_plan_ptr = meal_plan_head;
    while (meal_plan_ptr != NULL) {
        if (input == ((struct MealPlanData *)meal_plan_ptr->data)->index) {
            meal_plan_id = ((struct MealPlanData *)meal_plan_ptr->data)->id;
            break;
        }
        meal_plan_ptr = meal_plan_ptr->next;
    }
    
    if (meal_plan_id == 0) {
        printf("Invalid input. Please try again.\n");
        goto input_generation2;
    }
    
    if (HasReservedBefore(db, user->id, meal_plan_id) == 1) {
        printf("Such a reservation has already been made.\n");
        goto exit_2;
    }
    
eligibility_check:
    if (((struct MealPlanData *)meal_plan_ptr->data)->food_quantity == 0) {
        printf("Such a reservation cannot be made.\n");
        goto exit_2;
    } else if (((struct MealPlanData *)meal_plan_ptr->data)->price > user->balance) {
        printf("Your account balance is not enough.\n"
               "Please increase your balance first.\n");
        ChargeAccountAsStudent(db, user);
        goto eligibility_check;
    }
    
    free(sql);
    rc = asprintf(&sql, "INSERT INTO reservations ("
                  "user_id, meal_plan_id, datetime) "
                  "VALUES (%d, %d, datetime('now', 'localtime'));",
                  user->id, meal_plan_id);
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
                  "WHERE id = %d;",
                  ((struct MealPlanData *)meal_plan_ptr->data)->price,
                  user->id);
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
    
    user->balance = GetBalance(db, user->id);
    
    free(sql);
    rc = asprintf(&sql, "UPDATE meal_plans "
             "SET food_quantity = food_quantity - 1 "
             "WHERE id = %d;", ((struct MealPlanData *)meal_plan_ptr->data)->id);
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
    GFreeList(meal_plan_head, &FreeMealPlanData);
exit_1:
    LFreeList(&lunchroom_head);
exit:
    free(sql);
exit1:
    rc = 0;
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
    
    if (charge_amount <= 0) {
        printf("Invalid amount.\n");
        goto exit;
    }

    rc = asprintf(&sql, "UPDATE users "
                  "SET balance = balance + %d "
                  "WHERE id = %d;", charge_amount, user->id);
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
    
    balance = GetBalance(db, user->id);
    user->balance = balance;
    
    printf("Your account balance is now %d R.\n", user->balance);
    
exit_1:
    free(sql);
exit:
    free(card_number);
    free(one_time_password);
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
    
    rc = asprintf(&sql, "SELECT id, first_name, last_name "
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
           "Would you like to proceed? [N/y] ",
           recipient_user->first_name, recipient_user->last_name);
    
input_generation:
    input = TakeCharInput();
    
    switch (input) {
        case (int)'y':
            break;
        case (int)'N':
        case (int)'\n':
            goto exit_2;
            break;
        default:
            printf("Invalid input. Please try again.\n");
            goto input_generation;
    }
    
    if (charge_amount <= 0) {
        printf("Invalid amount.\n");
    } else if (GetBalance(db, user->id) >= charge_amount) {
        TransferBalance(db, charge_amount, user, recipient_user->id);
        user->balance = GetBalance(db, user->id);
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

void ListReservations(sqlite3 *db, struct User *user)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;
    
    printf("\n--LIST RESERVATIONS--\n");
    
    rc = asprintf(&sql, "SELECT mp.date AS date, "
             "f.name AS food_name, "
             "mt.name AS meal_type_name "
             "FROM reservations r "
             "INNER JOIN meal_plans mp "
             "ON r.meal_plan_id = mp.id "
             "INNER JOIN foods f "
             "ON f.id = mp.food_id "
             "INNER JOIN meal_types mt "
             "ON mt.id = mp.meal_type_id "
             "WHERE r.user_id = %d "
             "AND mp.date >= DATE('now', 'localtime', 'weekday 6') "
             "ORDER BY mp.date;", user->id);
    if (rc == -1) {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }
    
    rc = sqlite3_exec(db, sql, &PrintReservationCallback, NULL, &err_msg);
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

void ListTransactions(sqlite3 *db, struct User *user)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;
    
    printf("\n--LIST TRANSACTIONS--\n");
    
    rc = asprintf(&sql, "SELECT t.datetime AS date_and_time, "
             "tt.name AS transaction_type, "
             "t.amount AS amount "
             "FROM transactions t "
             "INNER JOIN transaction_types tt "
             "ON t.transaction_type_id = tt.id "
             "WHERE t.user_id = %d;", user->id);
    if (rc == -1) {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit1;
    }
    
    rc = sqlite3_exec(db, sql, &PrintRecordCallback, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }
    
exit:
    free(sql);
exit1:
    rc = 0;
}

int GetBalance(sqlite3 *db, int user_id)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;
    
    int balance = 0;
    
    rc = asprintf(&sql, "SELECT balance FROM users "
             "WHERE id = %d;", user_id);
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
             "WHERE id = %d;", charge_amount, user->id);
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
             "WHERE id = %d;", charge_amount, recipient_id);
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
    
    rc = asprintf(&sql, "SELECT id FROM reservations "
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
