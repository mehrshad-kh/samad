//
//  callback.h
//  samad
//
//  Created by Mehrshad on 11/10/1401 AP.
//

#ifndef callback_h
#define callback_h

#include "shared.h"

int CountCallback(void *, int, char **, char **);
int CheckActivationCallback(void *, int, char **, char **);
int LoginCallback(void *, int, char **, char **);
int CheckPasswordCallback(void *, int, char **, char **);
int CheckIDNumberCallback(void *, int, char **, char **);

int GetBalanceCallback(void *, int, char **, char **);
int PrintRecordCallback(void *, int, char **, char **);
int GetLunchroomsCallback(void *, int, char **, char **);
int GetMealPlansCallback(void *, int, char **, char **);
int GetTakingMealPlansCallback(void *, int, char **, char **);
int SetFoodAndPriceCallback(void *, int, char **, char **);
int SetLunchroomNameCallback(void *, int, char **, char **);
int HasReservedBeforeCallback(void *, int, char **, char **);
int GetFirstAndLastNames(void *, int, char **, char **);
int PrintReservationCallback(void *, int, char **, char **);

#endif /* callback_h */


