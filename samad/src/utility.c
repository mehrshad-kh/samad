//
//  utility.c
//  samad
//
//  Created by Mehrshad on 11/10/1401 AP.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"

extern const char *kAllocationErr;

void RemoveTrailingNewline(char *str)
{
    size_t len = strlen(str);

    if (len != 0)
        if (*(str + len - 1) == '\n')
            *(str + len - 1) = '\0';
}

int TakeShellInput(void)
{
    size_t n = 0;
    char *end_ptr = NULL;
    
    int input = 0;
    char *entry_str = NULL;
    
    printf(">>> ");
    getline(&entry_str, &n, stdin);
    input = (int)strtol(entry_str, &end_ptr, 10);
    
    free(entry_str);
    
    return input;
}

int TakeIntInput(void)
{
    size_t n = 0;
    char *end_ptr = NULL;
    char *str = NULL;
    int value = 0;
    
    getline(&str, &n, stdin);
    value = (int)strtol(str, &end_ptr, 10);
    
    free(str);
    
    return value;
}

void TakeStringInput(char **str)
{
    size_t n = 0;
    
    getline(str, &n, stdin);
    RemoveTrailingNewline(*str);
}

struct tm *GetTimeAdvancedBy(int day_offset)
{
    time_t t = 0;
    struct tm *date = NULL;
    
    t = time(NULL);
    t += day_offset * 24 * 60 * 60;
    date = (struct tm *)calloc(1, sizeof(struct tm));
    if (date == NULL)
        goto exit;
    
    localtime_r(&t, date);
    
exit:
    return date;
}
