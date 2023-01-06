//
//  callback.c
//  samad
//
//  Created by Mehrshad on 11/10/1401 AP.
//

#include <stdlib.h>
#include <string.h>
#include "samad-lib.h"
#include "callback.h"

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
            (*user_ptr)->gender = (int)strtol(*(row_data + 8), &end_ptr, 10);
        } else {
            fprintf(stderr, "ERROR: %s\n", kAllocationErr);
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
