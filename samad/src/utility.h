//
//  utility.h
//  samad
//
//  Created by Mehrshad on 11/10/1401 AP.
//

#ifndef utility_h
#define utility_h

#include <time.h>
#include <stdbool.h>
#include "shared.h"
#if DEBUG
#include "leak_detector_c.h"
#endif

// Replace trailing newline with null terminator
void RemoveTrailingNewline(char *);

/// Returns -1 on other-than-single-character inputs
int TakeCharInput(void);
int TakeIntInput(void);
/// Free output
void TakeStringInput(char **);
int TakeShellInput(void);

bool IsDateCorrect(const char *);

#endif /* utility_h */
