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
int GetListCallback(void *, int, char **, char **);
int GetIncompleteMealPlansCallback(void *, int, char **, char **);
int SetFoodNameCallback(void *, int, char **, char **);
int SetLunchroomNameCallback(void *, int, char **, char **);

#endif /* callback_h */


