//
//  callback.c
//  samad
//
//  Created by Mehrshad on 11/10/1401 AP.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "shared.h"
#include "callback.h"
#include "mklib.h"

extern const char *const kAllocationErr;
extern const char *const kQueryGenerationErr;

int CountCallback(void *ptr, int column_count,
                  char **row_data, char **column_names)
{
    char *end_ptr = NULL;
    
    int *user_count_ptr = (int *)ptr;
    *user_count_ptr = (int)strtol(*row_data, &end_ptr, 10);
    
    return 0;
}

int LoginCallback(void *ptr, int column_count,
                  char **row_data, char **column_names)
{
    char *end_ptr = NULL;
    struct User **user_ptr = (struct User **)ptr;
    
    if (column_count == 0) {
        *user_ptr = NULL;
    } else {
        *user_ptr = (struct User *)calloc(1, sizeof(struct User));
        
        if (*user_ptr != NULL) {
            (*user_ptr)->rowid = (int)strtol(*row_data, &end_ptr, 10);
            (*user_ptr)->user_type = (int)strtol(*(row_data + 1),
                                                 &end_ptr, 10);
            (*user_ptr)->activated = (int)strtol(*(row_data + 2),
                                                 &end_ptr, 10);
            (*user_ptr)->first_name = strdup(*(row_data + 3));
            (*user_ptr)->last_name = strdup(*(row_data + 4));
            (*user_ptr)->id_number = strdup(*(row_data + 5));
            (*user_ptr)->national_id = strdup(*(row_data + 6));
            (*user_ptr)->birthdate = strdup(*(row_data + 7));
            (*user_ptr)->sex = (int)strtol(*(row_data + 8), &end_ptr, 10);
        } else {
            fprintf(stderr, "\e[31;1mERROR:\e[0m %s\n", kAllocationErr);
        }
    }
    
    return 0;
}

int CheckActivationCallback(void *ptr, int column_count,
                            char **row_data, char **column_names)
{
    char *end_ptr = NULL;
    
    bool *is_activated  = (bool *)ptr;
    
    *is_activated = (int)strtol(*row_data, &end_ptr, 10);
    
    return 0;
}

int CheckPasswordCallback(void *ptr, int column_count,
                          char **row_data, char **column_names)
{
    bool *password_is_correct = (bool *)ptr;
    
    if (column_count == 0)
        *password_is_correct = false;
    else
        *password_is_correct = true;
    
    return 0;
}

int GetAccountBalanceCallback(void *ptr, int column_count,
                              char **row_data, char **column_names)
{
    char *end_ptr = NULL;
    
    int *balance = (int *)ptr;
    
    *balance = (int)strtol(row_data[0], &end_ptr, 10);
    
    return 0;
}

int PrintRecordCallback(void *ptr, int column_count,
                        char **row_data, char **column_names)
{
    char *table_name = (char *)ptr;
    char *column_name = NULL;
    
    printf("\n");
    for (int i = 0; i < column_count; i++) {
        if (strcmp(column_names[i], "rowid") == 0) {
            column_name = (char *)calloc(strlen(table_name) + 2 + 1, sizeof(char));
            strcpy(column_name, table_name);
            column_name[strlen(column_name) - 1] = '\0';
            strcat(column_name, " ID");
        } else {
            column_name = FindAndReplace2(column_names[i], "_", " ");
        }
        
        column_name[0] = toupper(column_name[0]);
        
        printf("%s: %s\n", column_name, row_data[i]);
        
        free(column_name);
    }
    
    return 0;
}

int GetListCallback(void *ptr, int column_count,
                         char **row_data, char **column_names)
{
    struct LunchroomData *lunchroom = NULL;
    struct Lunchroom **head = NULL;
    
    lunchroom = GenerateLunchroomData(row_data);
    head = (struct Lunchroom **)ptr;
    
    LInsertAtEnd(lunchroom, head);
    
    return 0;
}

int GetIncompleteMealPlansCallback(void *ptr, int column_count,
                                   char **row_data, char **column_names)
{
    struct IncompleteMealPlan **head = NULL;
    struct IncompleteMealPlanData *data = NULL;
    
    head = (struct IncompleteMealPlan **)ptr;
    data = GenerateIncompleteMealPlanData(row_data);
    
    ImpnInsertAtEnd(data, head);
    
    return 0;
}

int SetFoodAndPriceCallback(void *ptr, int column_count,
                        char **row_data, char **column_names)
{
    char *end_ptr = NULL;
    
    struct FoodAndPrice *food_and_price = NULL;
    
    food_and_price = (struct FoodAndPrice *)ptr;
    food_and_price->food_name = strdup(row_data[0]);
    food_and_price->price = (int)strtol(row_data[1], &end_ptr, 10);
    
    return 0;
}

int SetLunchroomNameCallback(void *ptr, int column_count,
                             char **row_data, char **column_names)
{
    char **lunchroom_name = NULL;
    
    lunchroom_name = (char **)ptr;
    
    *lunchroom_name = strdup(row_data[0]);
    
    return 0;
}

int SetMealTypeNameCallback(void *ptr, int column_count,
                            char **row_data, char **column_names)
{
    char **meal_type_name = NULL;
    
    meal_type_name = (char **)ptr;
    
    *meal_type_name = strdup(row_data[0]);
    
    return 0;
}
