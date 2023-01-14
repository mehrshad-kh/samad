//
//  main.c
//  samad
//
//  Created by Mehrshad on 9/10/1401 AP.
//

#include <stdio.h>
#include <sqlite3.h>
#include "shared.h"
#include "callback.h"
#include "utility.h"

extern const char *const kAllocationErr;
extern const char *const kQueryGenerationErr;
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
        rc = CreateTables(db);

        if (rc == 0) {
            DisplayLoginMenu(db);
        } else {
            fprintf(stderr, "ERROR: %s: %s\n",
                    kQueryExecutionErr, sqlite3_errmsg(db));
            sqlite3_free((void *)sqlite3_errmsg(db));
        }
        
        CloseDatabase(db);
    }

    return 0;
}
