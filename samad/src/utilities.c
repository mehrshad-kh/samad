//
//  utilities.c
//  samad
//
//  Created by Mehrshad on 11/10/1401 AP.
//

#include <stdlib.h>
#include <string.h>
#include "utilities.h"

void RemoveTrailingNewline(char *str)
{
    size_t len = strlen(str);

    if (len != 0)
        if (*(str + len - 1) == '\n')
            *(str + len - 1) = '\0';
}

void ClearScreen(void)
{
    printf("\e[1;1H\e[2J");
}

int TakeShellInput(void)
{
    size_t n = 0;
    char *end_ptr = NULL;
    
    int entry = 0;
    char *entry_str = NULL;
    
    printf(">>> ");
    getline(&entry_str, &n, stdin);
    entry = (int)strtol(entry_str, &end_ptr, 10);
    
    free(entry_str);
    
    return entry;
}

int TakeIntInput(void)
{
    size_t n = 0;
    char *end_ptr = NULL;
    char *str = NULL;
    int return_value = 0;
    
    getline(&str, &n, stdin);
    return_value = (int)strtol(str, &end_ptr, 10);
    
    free(str);
    
    return return_value;
}

void TakeStringInput(char **str)
{
    size_t n = 0;
    
    getline(str, &n, stdin);
    RemoveTrailingNewline(*str);
}
