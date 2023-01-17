//
//  callback.h
//  samad
//
//  Created by Mehrshad on 11/10/1401 AP.
//

#ifndef callback_h
#define callback_h

int CountCallback(void *, int, char **, char **);
int CheckActivationCallback(void *, int, char **, char **);
int LoginCallback(void *, int, char **, char **);
int CheckPasswordCallback(void *, int, char **, char **);

int GetAccountBalanceCallback(void *, int, char **, char **);
int PrintRecordCallback(void *, int, char **, char **);
int GetLunchroomsCallback(void *, int, char **, char **);
int GetIncMealPlansCallback(void *, int, char **, char **);
int SetFoodAndPriceCallback(void *, int, char **, char **);
int SetLunchroomNameCallback(void *, int, char **, char **);
// Sets the name to a string
int SetMealTypeNameCallback(void *, int, char **, char **);
// Sets the names to IncMealType
int SetMealTypeNameCallback2(void *, int, char **, char **);

#endif /* callback_h */


