//
//  utility.h
//  samad
//
//  Created by Mehrshad on 11/10/1401 AP.
//

#ifndef utility_h
#define utility_h

// Replace trailing newline with null terminator
void RemoveTrailingNewline(char *);

// Unnecessary
void ClearScreen(void);
int TakeShellInput(void);
int TakeIntInput(void);
void TakeStringInput(char **);

#endif /* utility_h */
