//
//  student.c
//  samad
//
//  Created by Mehrshad on 17/10/1401 AP.
//

#include <stdio.h>
#include "shared.h"
#include "student.h"
#include "utility.h"

void DisplayStudentMenu(sqlite3 *db, struct User **user)
{
    int input = 0;
    
    printf("\n--STUDENT--\n");
    printf("What would you like to do?\n");
    printf("0: Log out\n");
    
input_generation:
    input = TakeShellInput();
    
    switch (input)
    {
        case 0:
            PerformLogout(user);
            DisplayLoginMenu(db);
            break;
        case 1:
            break;
        case 2:
            break;
        default:
            printf("Invalid input. Please try again.\n");
            goto input_generation;
    }
}
