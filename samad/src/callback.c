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
#include "callback.h"
#include "mklib.h"

extern const char *const kErr;
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
	struct User **user = (struct User **)ptr;
	
	if (column_count == 0) {
		*user = NULL;
	} else {
		*user = (struct User *)calloc(1, sizeof(struct User));
		
		if (*user != NULL) {
			(*user)->id = (int)strtol(row_data[0], &end_ptr, 10);
			(*user)->user_type = (int)strtol(row_data[1], &end_ptr, 10);
			(*user)->activated = (int)strtol(row_data[2], &end_ptr, 10);
			(*user)->first_name = strdup(row_data[3]);
			(*user)->last_name = strdup(row_data[4]);
			(*user)->id_number = strdup(row_data[5]);
			(*user)->national_id = strdup(row_data[6]);
			(*user)->birthdate = strdup(row_data[7]);
			(*user)->sex = (int)strtol(row_data[8], &end_ptr, 10);
			(*user)->balance = (int)strtol(row_data[9], &end_ptr, 10);
		} else {
			fprintf(stderr, "%s %s\n", kErr, kAllocationErr);
		}
	}
	
	return 0;
}

int CheckActivationCallback(void *ptr, int column_count,
			    char **row_data, char **column_names)
{
	char *end_ptr = NULL;
	
	bool *is_activated  = (bool *)ptr;
	
	*is_activated = (int)strtol(row_data[0], &end_ptr, 10);
	
	return 0;
}

int CheckPasswordCallback(void *ptr, int column_count,
			  char **row_data, char **column_names)
{
	bool *is_correct = (bool *)ptr;
	
	if (column_count == 0)
		*is_correct = false;
	else
		*is_correct = true;
	
	return 0;
}

int CheckIDNumberCallback(void *ptr, int column_count,
			  char **row_data, char **column_names)

{
	bool *exists = (bool *)ptr;
	
	if (column_count == 0)
		*exists = false;
	else
		*exists = true;
	
	return 0;
}

int GetBalanceCallback(void *ptr, int column_count,
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
		if (strcmp(column_names[i], "id") == 0) {
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

int GetLunchroomsCallback(void *ptr, int column_count,
			  char **row_data, char **column_names)
{
	struct LunchroomData *lunchroom = NULL;
	struct Lunchroom **head = NULL;
	
	lunchroom = GenerateLunchroomData(row_data);
	head = (struct Lunchroom **)ptr;
	
	LInsertAtEnd(lunchroom, head);
	
	return 0;
}

int GetMealPlansCallback(void *ptr, int column_count,
			 char **row_data, char **column_names)
{
	struct GNode **head = NULL;
	
	head = (struct GNode **)ptr;
	
	GInsertAtEnd(head, (void *)GenerateMealPlan(row_data));
	
	return 0;
}

int GetTakingMealPlansCallback(void *ptr, int column_count,
			       char **row_data, char **column_names)
{
	struct GNode **head = NULL;
	
	head = (struct GNode **)ptr;
	
	GInsertAtEnd(head, (void *)GenerateTakingMealPlan(row_data));
	
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

int HasReservedBeforeCallback(void *ptr, int column_count,
			      char **row_data, char **column_names)
{
	int *has_reserved = (int *)ptr;
	
	if (column_count == 1)
		*has_reserved = 1;
	else if (column_count == 0)
		*has_reserved = 0;
	
	return 0;
}

int GetFirstAndLastNames(void *ptr, int column_count,
			 char **row_data, char **column_names)
{
	char *end_ptr = NULL;
	
	struct User **recipient_user = (struct User **)ptr;
	
	if (column_count == 0) {
		*recipient_user = NULL;
	}
	else {
		*recipient_user = (struct User *)calloc(1, sizeof(struct User));
		
		(*recipient_user)->id = (int)strtol(row_data[0], &end_ptr, 10);
		(*recipient_user)->first_name = strdup(row_data[1]);
		(*recipient_user)->last_name = strdup(row_data[2]);
		// Initialize the rest perhaps
	}
	
	
	return 0;
}

int PrintReservationCallback(void *ptr, int column_count,
			     char **row_data, char **column_names)
{
	printf("%s: [%s] %s (%s)\n", row_data[0],
	       row_data[1], row_data[2], row_data[3]);
	
	return 0;
}

int PrintTakenReservationCallback(void *ptr, int column_count,
				  char **row_data, char **column_names)
{
	printf("%s: [%s] (%s) %s (%s)\n", row_data[0], row_data[1],
	       row_data[2], row_data[3], row_data[4]);
	
	return 0;
}
