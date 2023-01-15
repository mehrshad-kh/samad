//
//  types.c
//  samad
//
//  Created by Mehrshad on 17/10/1401 AP.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "callback.h"
#include "types.h"

const int min_days_for_reservation = 3;
const int max_days_for_reservation = 14;

const char *const kAllocationErr = "Cannot allocate memory";
const char *const kQueryGenerationErr = "Cannot generate query";
const char *const kQueryExecutionErr = "Cannot execute query";
const char *const kDatabaseOpenErr = "Cannot open database";
const char *const kDatabaseCloseErr = "Cannot close database";

struct LunchroomData *GenerateLunchroomData(char **row_data)
{
    char *end_ptr = NULL;
    
    struct LunchroomData *lunchroom = NULL;
    
    lunchroom = (struct LunchroomData *)calloc(1, sizeof(struct LunchroomData));
    if (lunchroom == NULL) {
        fprintf(stderr, "ERROR: %s\n", kAllocationErr);
        goto exit;
    }
    lunchroom->index = 1;
    lunchroom->rowid = (int)strtol(row_data[0], &end_ptr, 10);
    lunchroom->name = strdup(row_data[1]);
    lunchroom->meal_types = strdup(row_data[2]);
    lunchroom->sex = (int)strtol(row_data[3], &end_ptr, 10);
    lunchroom->address = NULL;
    lunchroom->capacity = 0;
    
exit:
    return lunchroom;
}

void FreeLunchromData(struct LunchroomData *lunchroom)
{
    free(lunchroom->name);
    free(lunchroom->meal_types);
    free(lunchroom);
}

struct IncompleteMealPlanData *GenerateIncompleteMealPlanData(char **row_data)
{
    char *end_ptr = NULL;
    
    int i = 0;
    struct IncompleteMealPlanData *meal_plan = NULL;
    
    i = 1;
    meal_plan = (struct IncompleteMealPlanData *)calloc(1, sizeof(struct IncompleteMealPlanData));
    if (meal_plan == NULL) {
        fprintf(stderr, "ERROR: %s\n", kAllocationErr);
        goto exit;
    }
    meal_plan->index = i;
    meal_plan->rowid = (int)strtol(row_data[0], &end_ptr, 10);
    meal_plan->food_id = (int)strtol(row_data[1], &end_ptr, 10);
    meal_plan->lunchroom_id = (int)strtol(row_data[2], &end_ptr, 10);
    meal_plan->meal_type_id = (int)strtol(row_data[3], &end_ptr, 10);
    meal_plan->quantity = (int)strtol(row_data[4], &end_ptr, 10);
    meal_plan->date = strdup(row_data[5]);
    
exit:
    return meal_plan;
}

void LInsertAtEnd(struct LunchroomData *data, struct Lunchroom **head)
{
    struct Lunchroom *ptr = NULL;
    struct Lunchroom *new_ptr = NULL;
    
    if (*head == NULL) {
        new_ptr = (struct Lunchroom *)malloc(sizeof(struct Lunchroom));
        
        if (new_ptr != NULL) {
            *head = new_ptr;
            new_ptr->next = NULL;
            new_ptr->prev = NULL;
            new_ptr->data = data;
        } else {
            perror("Insufficient memory at LInsertAtEnd");
        }
    } else {
        ptr = *head;
        
        while (ptr->next != NULL)
            ptr = ptr->next;
        
        new_ptr = (struct Lunchroom *)malloc(sizeof(struct Lunchroom));
        
        if (new_ptr != NULL) {
            ptr->next = new_ptr;
            new_ptr->prev = ptr;
            new_ptr->next = NULL;
            new_ptr->data = data;
            new_ptr->data->index = new_ptr->prev->data->index + 1;
        } else {
            perror("Insufficient memory at LInsertAtEnd");
        }
    }
}

void LPrintList(struct Lunchroom *head)
{
    struct Lunchroom *ptr = head;
    
    if (ptr == NULL) {
        printf("No lunchrooms.\n");
    } else {
        while (ptr != NULL) {
            printf("%d: %s (%s)\n", ptr->data->index, ptr->data->name,
                   ptr->data->meal_types);
            ptr = ptr->next;
        }
    }
}

void MPPrintList(struct MealPlan *head)
{
    struct MealPlan *ptr = head;
    
    if (ptr == NULL) {
        printf("No meal plans.\n");
    } else {
        while (ptr != NULL) {
            printf("%d: (%s) %s (%s) %d R\n", ptr->data->index,
                   ptr->data->date, ptr->data->food_name,
                   ptr->data->meal_type_name, ptr->data->price);
            ptr = ptr->next;
        }
    }
}

void LFreeList(struct Lunchroom **head)
{
    struct Lunchroom *ptr = NULL;
    struct Lunchroom *one_to_last_ptr = NULL;
    
    if (*head != NULL) {
        ptr = *head;
        *head = NULL;
        
        while (ptr != NULL) {
            one_to_last_ptr = ptr;
            ptr = ptr->next;
            FreeLunchromData(one_to_last_ptr->data);
            free(one_to_last_ptr);
        }
    }
}

void ImpnInsertAtEnd(struct IncompleteMealPlanData *data, struct IncompleteMealPlan **head)
{
    struct IncompleteMealPlan *ptr = NULL;
    struct IncompleteMealPlan *new_ptr = NULL;
    
    if (*head == NULL) {
        new_ptr = (struct IncompleteMealPlan *)malloc(sizeof(struct IncompleteMealPlan));
        
        if (new_ptr != NULL) {
            *head = new_ptr;
            new_ptr->next = NULL;
            new_ptr->prev = NULL;
            new_ptr->data = data;
        } else {
            perror("Insufficient memory at ImpnInsertAtEnd");
        }
    } else {
        ptr = *head;
        
        while (ptr->next != NULL)
            ptr = ptr->next;
        
        new_ptr = (struct IncompleteMealPlan *)malloc(sizeof(struct IncompleteMealPlan));
        
        if (new_ptr != NULL) {
            ptr->next = new_ptr;
            new_ptr->prev = ptr;
            new_ptr->next = NULL;
            new_ptr->data = data;
            new_ptr->data->index = new_ptr->prev->data->index + 1;
        } else {
            perror("Insufficient memory at ImpnInsertAtEnd");
        }
    }
}

struct MealPlanData *GenerateMealPlanData(int index,
                                          int rowid,
                                          char *food_name,
                                          char *lunchroom_name,
                                          char *meal_type_name,
                                          int price,
                                          int quantity,
                                          char *date)
{
    struct MealPlanData *data = NULL;
    
    data = (struct MealPlanData *)calloc(1, sizeof(struct MealPlanData));
    if (data == NULL) {
        fprintf(stderr, "ERROR: %s\n", kAllocationErr);
        goto exit;
    }
    data->index = index;
    data->rowid = rowid;
    data->food_name = strdup(food_name);
    data->lunchroom_name = strdup(lunchroom_name);
    data->meal_type_name = strdup(meal_type_name);
    data->price = price;
    data->quantity = quantity;
    data->date = strdup(date);
    
exit:
    return data;
}

void MPInsertAtEnd(struct MealPlanData *data, struct MealPlan **head)
{
    struct MealPlan *ptr = NULL;
    struct MealPlan *new_node = NULL;
    
    if (*head == NULL) {
        new_node = (struct MealPlan *)malloc(sizeof(struct MealPlan));
        
        if (new_node != NULL) {
            *head = new_node;
            new_node->next = NULL;
            new_node->prev = NULL;
            new_node->data = data;
        } else {
            perror("Insufficient memory at MPInsertAtEnd");
        }
    } else {
        ptr = *head;
        
        while (ptr->next != NULL)
            ptr = ptr->next;
        
        new_node = (struct MealPlan *)malloc(sizeof(struct MealPlan));
        
        if (new_node != NULL) {
            ptr->next = new_node;
            new_node->prev = ptr;
            new_node->next = NULL;
            new_node->data = data;
        } else {
            perror("Insufficient memory at MPInsertAtEnd");
        }
    }
}

struct MealPlan *GetMealPlans(sqlite3 *db,
                              struct IncompleteMealPlan *incomplete_head)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;
    
    char *lunchroom_name = NULL;
    char *meal_type_name = NULL;
    
    struct IncompleteMealPlan *ptr = NULL;
    
    struct FoodAndPrice food_and_price = {0};
    struct MealPlan *head = NULL;
    struct MealPlanData *data = NULL;
    
    if (incomplete_head == NULL)
        goto exit;
    
    ptr = incomplete_head;
    while (ptr != NULL) {
        rc = asprintf(&sql, "SELECT name, price "
                      "FROM foods "
                      "WHERE rowid = %d;", ptr->data->food_id);
        if (rc == -1) {
            fprintf(stderr, "ERROR: %s\n", kQueryGenerationErr);
            goto exit;
        }
        
        rc = sqlite3_exec(db, sql, &SetFoodAndPriceCallback,
                          &food_and_price, &err_msg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "ERROR: %s: %s\n", kQueryExecutionErr, err_msg);
            sqlite3_free(err_msg);
            goto exit;
        }
        
        rc = asprintf(&sql, "SELECT name "
                      "FROM lunchrooms "
                      "WHERE rowid = %d;", ptr->data->lunchroom_id);
        if (rc == -1) {
            fprintf(stderr, "ERROR: %s\n", kQueryGenerationErr);
            free(food_and_price.food_name);
            return NULL;
        }
        
        rc = sqlite3_exec(db, sql, &SetLunchroomNameCallback,
                          &lunchroom_name, &err_msg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "ERROR: %s: %s\n", kQueryExecutionErr, err_msg);
            sqlite3_free(err_msg);
            free(food_and_price.food_name);
            return NULL;
        }
        
        rc = asprintf(&sql, "SELECT name "
                      "FROM meal_types "
                      "WHERE rowid = %d;", ptr->data->meal_type_id);
        if (rc == -1) {
            fprintf(stderr, "ERROR: %s\n", kQueryGenerationErr);
            free(food_and_price.food_name);
            free(lunchroom_name);
            return NULL;
        }
        
        rc = sqlite3_exec(db, sql, &SetMealTypeNameCallback,
                          &meal_type_name, &err_msg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "ERROR: %s: %s\n", kQueryExecutionErr, err_msg);
            sqlite3_free(err_msg);
            free(food_and_price.food_name);
            free(lunchroom_name);
            return NULL;
        }
        
        data = GenerateMealPlanData(ptr->data->index,
                                    ptr->data->rowid,
                                    food_and_price.food_name,
                                    lunchroom_name, meal_type_name,
                                    food_and_price.price,
                                    ptr->data->quantity, ptr->data->date);
        MPInsertAtEnd(data, &head);
        
        ptr = ptr->next;
        
        free(food_and_price.food_name);
        free(lunchroom_name);
        free(meal_type_name);
    }
    
exit:
    return head;
}
