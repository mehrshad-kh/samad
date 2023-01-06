//
//  main.c
//  samad
//
//  Created by Mehrshad on 9/10/1401 AP.
//

#include <stdio.h>
#include <sqlite3.h>
#include "samad-lib.h"
#include "callback.h"
#include "utilities.h"

// extern const char *const kAllocationErr;
// extern const char *const kQueryGenerationErr;
extern const char *const kQueryExecutionErr;
extern const char *const kDatabaseOpenErr;
extern const char *const kDatabaseCloseErr;

int main(int argc, const char *argv[])
{
    int rc = 0;

    sqlite3 *db = NULL;

    printf("Copyright (C) 2022-2023 MK Indutries, Ltd.\n");
    printf("Welcome to SaMaD, your very best companion of the day!\n");

    db = OpenDatabase("/Users/mehrshadkh./Desktop/"
                      "programming2/uni/sem1/samad/samad/samad.db");

    if (db != NULL) {
        rc = CreateUsersTable(db);

        if (rc == 0)
            DisplayLoginMenu(db);
        
        CloseDatabase(db);
    }

    return 0;
}
