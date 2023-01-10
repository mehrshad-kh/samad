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
    
    while (ptr != NULL) {
        printf("%d: %s (%s)\n", ptr->lunchroom->index, ptr->lunchroom->name,
               ptr->lunchroom->meal_types);
        ptr = ptr->next;
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
