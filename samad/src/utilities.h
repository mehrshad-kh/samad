//
//  utilities.h
//  samad
//
//  Created by Mehrshad on 11/10/1401 AP.
//

#ifndef utilities_h
#define utilities_h

// Replace trailing newline with null terminator
void RemoveTrailingNewline(char *);

// Unnecessary
void ClearScreen(void);
int TakeShellInput(void);
int TakeIntInput(void);
void TakeStringInput(char **);

void PrintInColumns(void);

#endif /* utilities_h */
