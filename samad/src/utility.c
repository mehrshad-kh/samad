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

extern const char *const kErr;
extern const char *kAllocationErr;

void RemoveTrailingNewline(char *str)
{
    size_t len = strlen(str);

    if (len != 0)
        if (*(str + len - 1) == '\n')
            *(str + len - 1) = '\0';
}

int TakeCharInput(void)
{
    size_t n = 0;
    char *str = NULL;
    int value = 0;

    getline(&str, &n, stdin);

    if (strlen(str) == 2 || strcmp(str, "\n") == 0)
        value = str[0];
    else
        value = -1;

    free(str);

    return value;
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

bool IsDateCorrect(const char *date)
{
    char *end_ptr = NULL;
    int n = 0;

    int result = false;

    if (strlen(date) != 10)
        goto exit;

    if (date[4] != '-' || date[7] != '-')
        goto exit;

    n = (int)strtol(date, &end_ptr, 10);
    if (n < 1500 || n > 9999)
        goto exit;

    n = (int)strtol(date + 5, &end_ptr, 10);
    if (n < 1 || n > 12)
        goto exit;

    n = (int)strtol(date + 8, &end_ptr, 10);
    if (n < 1 || n > 31)
        goto exit;

    result = true;

exit:
    return result;
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

struct tm *GetLastSaturday(void)
{
    time_t t = 0;
    struct tm *current_time = NULL;

    t = time(NULL);
    current_time = (struct tm *)calloc(1, sizeof(struct tm));
    localtime_r(&t, current_time);

    // If not Saturday
    if (current_time->tm_wday != 6)
    {
        t -= (1 + current_time->tm_wday) * 24 * 60 * 60;
        localtime_r(&t, current_time);
    }

    return current_time;
}
