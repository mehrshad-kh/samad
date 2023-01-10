//
//  utility.h
//  samad
//
//  Created by Mehrshad on 11/10/1401 AP.
//

#ifndef utility_h
#define utility_h

#include <time.h>

// Replace trailing newline with null terminator
void RemoveTrailingNewline(char *);

// Unnecessary
void ClearScreen(void);
int TakeShellInput(void);
int TakeIntInput(void);
void TakeStringInput(char **);

struct tm *AddTimeByDays(struct tm *, int);
void AddTimeByDays2(struct tm *, int);

#endif /* utility_h */
