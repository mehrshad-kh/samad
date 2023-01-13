//
//  types.c
//  samad
//
//  Created by Mehrshad on 17/10/1401 AP.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

const int min_days_for_reservation = 3;
const int max_days_for_reservation = 14;

const char *const kAllocationErr = "Cannot allocate memory";
const char *const kQueryGenerationErr = "Cannot generate query";
const char *const kQueryExecutionErr = "Cannot execute query";
const char *const kDatabaseOpenErr = "Cannot open database";
const char *const kDatabaseCloseErr = "Cannot close database";

struct Lunchroom *CreateLunchroom(char **row_data)
{
    char *end_ptr = NULL;
    
    int i = 0;
    struct Lunchroom *lunchroom = NULL;
    
    i = 1;
    lunchroom = (struct Lunchroom *)calloc(1, sizeof(struct Lunchroom));
    if (lunchroom == NULL) {
        fprintf(stderr, "ERROR: %s\n", kAllocationErr);
        goto exit;
    }
    lunchroom->index = i;
    lunchroom->rowid = (int)strtol(row_data[0], &end_ptr, 10);
    lunchroom->name = strdup(row_data[1]);
    lunchroom->meal_types = strdup(row_data[2]);
    lunchroom->gender = (int)strtol(row_data[3], &end_ptr, 10);
    lunchroom->address = NULL;
    lunchroom->capacity = 0;
    
exit:
    return lunchroom;
}

void FreeLunchrom(struct Lunchroom *lunchroom)
{
    free(lunchroom->name);
    free(lunchroom->meal_types);
    free(lunchroom);
}

struct IncompleteMealPlan *CreateIncompleteMealPlan(char **row_data)
{
    char *end_ptr = NULL;
    
    int i = 0;
    struct IncompleteMealPlan *meal_plan = NULL;
    
    i = 1;
    meal_plan = (struct IncompleteMealPlan *)calloc(1, sizeof(struct IncompleteMealPlan));
    if (meal_plan == NULL) {
        fprintf(stderr, "ERROR: %s\n", kAllocationErr);
        goto exit;
    }
    meal_plan->index = i;
    meal_plan->rowid = (int)strtol(row_data[0], &end_ptr, 10);
    meal_plan->food_id = (int)strtol(row_data[1], &end_ptr, 10);
    meal_plan->lunchroom_id = (int)strtol(row_data[2], &end_ptr, 10);
    meal_plan->quantity = (int)strtol(row_data[3], &end_ptr, 10);
    meal_plan->date = strdup(row_data[4]);
    
exit:
    return meal_plan;
}

void LNInsertAtEnd(struct Lunchroom *value, struct LunchroomNode **head)
{
    struct LunchroomNode *ptr = NULL;
    struct LunchroomNode *new_ptr = NULL;
    
    if (*head == NULL) {
        new_ptr = (struct LunchroomNode *)malloc(sizeof(struct LunchroomNode));
        
        if (new_ptr != NULL) {
            *head = new_ptr;
            new_ptr->next = NULL;
            new_ptr->prev = NULL;
            new_ptr->lunchroom = value;
        } else {
            perror("Insufficient memory at LNInsertAtEnd");
        }
    } else {
        ptr = *head;
        
        while (ptr->next != NULL)
            ptr = ptr->next;
        
        new_ptr = (struct LunchroomNode *)malloc(sizeof(struct LunchroomNode));
        
        if (new_ptr != NULL) {
            ptr->next = new_ptr;
            new_ptr->prev = ptr;
            new_ptr->next = NULL;
            new_ptr->lunchroom = value;
            new_ptr->lunchroom->index = new_ptr->prev->lunchroom->index + 1;
        } else {
            perror("Insufficient memory at LNInsertAtEnd");
        }
    }
}

void LNPrintList(struct LunchroomNode *head)
{
    struct LunchroomNode *ptr = head;
    
    if (ptr == NULL) {
        printf("No lunchrooms.\n");
    } else {
        while (ptr != NULL) {
            printf("%d: %s (%s)\n", ptr->lunchroom->index, ptr->lunchroom->name,
                   ptr->lunchroom->meal_types);
            ptr = ptr->next;
        }
    }
}

void LNFreeList(struct LunchroomNode **head)
{
    struct LunchroomNode *ptr = NULL;
    struct LunchroomNode *one_to_last_ptr = NULL;
    
    if (*head != NULL) {
        ptr = *head;
        *head = NULL;
        
        while (ptr != NULL) {
            one_to_last_ptr = ptr;
            ptr = ptr->next;
            FreeLunchrom(one_to_last_ptr->lunchroom);
            free(one_to_last_ptr);
        }
    }
}

void ImpnInsertAtEnd(struct IncompleteMealPlan *value, struct IncompleteMealPlanNode **head)
{
    struct IncompleteMealPlanNode *ptr = NULL;
    struct IncompleteMealPlanNode *new_ptr = NULL;
    
    if (*head == NULL) {
        new_ptr = (struct IncompleteMealPlanNode *)malloc(sizeof(struct IncompleteMealPlanNode));
        
        if (new_ptr != NULL) {
            *head = new_ptr;
            new_ptr->next = NULL;
            new_ptr->prev = NULL;
            new_ptr->meal_plan = value;
        } else {
            perror("Insufficient memory at ImpnInsertAtEnd");
        }
    } else {
        ptr = *head;
        
        while (ptr->next != NULL)
            ptr = ptr->next;
        
        new_ptr = (struct IncompleteMealPlanNode *)malloc(sizeof(struct IncompleteMealPlanNode));
        
        if (new_ptr != NULL) {
            ptr->next = new_ptr;
            new_ptr->prev = ptr;
            new_ptr->next = NULL;
            new_ptr->meal_plan = value;
            new_ptr->meal_plan->index = new_ptr->prev->meal_plan->index + 1;
        } else {
            perror("Insufficient memory at ImpnInsertAtEnd");
        }
    }
}

void MPInsertAtEnd(int index, int rowid, char *food_name,
                   char *lunchroom_name, int quantity, char *date,
                   struct MealPlan **head)
{
    struct MealPlan *ptr = NULL;
    struct MealPlan *new_node = NULL;
    
    if (*head == NULL) {
        new_node = (struct MealPlan *)malloc(sizeof(struct MealPlan));
        
        if (new_node != NULL) {
            *head = new_node;
            new_node->next = NULL;
            new_node->prev = NULL;
            new_node->index = index;
            new_node->rowid = rowid;
            new_node->food_name = strdup(food_name);
            new_node->lunchroom_name = strdup(lunchroom_name);
            new_node->quantity = quantity;
            new_node->date = strdup(date);
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
            new_node->index = index;
            new_node->rowid = rowid;
            new_node->food_name = strdup(food_name);
            new_node->lunchroom_name = strdup(lunchroom_name);
            new_node->quantity = quantity;
            new_node->date = strdup(date);
            // new_node->lunchroom->index = new_node->prev->lunchroom->index + 1;
        } else {
            perror("Insufficient memory at MPInsertAtEnd");
        }
    }
}
