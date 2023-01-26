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

const int kMinDaysForReservation = 3;
const int kMaxDaysForReservation = 14;

const char *const kErr = "\e[31;1mERROR:\e[0m";
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
        fprintf(stderr, "%s %s\n", kErr, kAllocationErr);
        goto exit;
    }
    lunchroom->index = (int)strtol(row_data[0], &end_ptr, 10);
    lunchroom->lunchroom_id = (int)strtol(row_data[1], &end_ptr, 10);
    lunchroom->lunchroom_name = strdup(row_data[2]);
    lunchroom->meal_type_id = (int)strtol(row_data[3], &end_ptr, 10);
    lunchroom->meal_type_name = strdup(row_data[4]);
    lunchroom->address = NULL;
    lunchroom->capacity = 0;
    lunchroom->sex = 0;
    
exit:
    return lunchroom;
}

void FreeLunchroomData(struct LunchroomData *lunchroom)
{
    if (lunchroom == NULL) {
        return;
    }
    
    free(lunchroom->lunchroom_name);
    free(lunchroom->meal_type_name);
    free(lunchroom);
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
            printf("%d: %s (%s)\n", ptr->data->index, ptr->data->lunchroom_name,
                   ptr->data->meal_type_name);
            ptr = ptr->next;
        }
    }
}

void PrintMealPlan(void *ptr)
{
    struct MealPlan *data = (struct MealPlan *)ptr;
    
    printf("%d: (%s) %s (%s) %d R (%d left)\n", data->index, data->date,
           data->food_name, data->meal_type_name, data->price,
           data->food_quantity);
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
            FreeLunchroomData(one_to_last_ptr->data);
            free(one_to_last_ptr);
        }
    }
}

struct MealPlan *GenerateMealPlan(char **row_data)
{
    char *end_ptr = NULL;
    
    struct MealPlan *meal_plan = NULL;
    
    meal_plan = (struct MealPlan *)calloc(1, sizeof(struct MealPlan));
    if (meal_plan == NULL) {
        fprintf(stderr, "%s %s\n", kErr, kAllocationErr);
        goto exit;
    }
    meal_plan->index = (int)strtol(row_data[0], &end_ptr, 10);
    meal_plan->id = (int)strtol(row_data[1], &end_ptr, 10);
    meal_plan->food_name = strdup(row_data[2]);
    meal_plan->lunchroom_name = strdup(row_data[3]);
    meal_plan->meal_type_name = strdup(row_data[4]);
    meal_plan->price = (int)strtol(row_data[5], &end_ptr, 10);
    meal_plan->food_quantity = (int)strtol(row_data[6], &end_ptr, 10);
    meal_plan->date = strdup(row_data[7]);
    
exit:
    return meal_plan;
}

void FreeMealPlan(void *ptr)
{
    struct MealPlan *meal_plan = (struct MealPlan *)ptr;

    if (meal_plan == NULL) {
        return;
    }
    
    free(meal_plan->food_name);
    free(meal_plan->lunchroom_name);
    free(meal_plan->meal_type_name);
    free(meal_plan->date);
}
