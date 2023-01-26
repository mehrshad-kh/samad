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

extern const char *const kErr;
extern const char *const kQueryExecutionErr;

int main(int argc, const char *argv[])
{
	int rc = 0;
	
	sqlite3 *db = NULL;
	char *path = "/Users/mehrshadkh./Desktop/programming2/uni/sem1/samad/samad/samad2.db";
	
	printf("Copyright (C) 2022-2023 MK Indutries, Ltd. All rights reserved.\n");
	printf("Welcome to SaMaD, your very best companion of the day!\n");
	
	db = OpenDatabase(path);
	
	if (db != NULL) {
		rc = InitDatabase(db);
		
		if (rc == 0) {
			DisplayLoginMenu(db);
		} else {
			fprintf(stderr, "%s %s: %s\n",
				kErr, kQueryExecutionErr, sqlite3_errmsg(db));
			sqlite3_free((void *)sqlite3_errmsg(db));
		}
		
		CloseDatabase(db);
	}
	
	return 0;
}
