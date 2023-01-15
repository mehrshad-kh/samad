//
//  utility.h
//  samad
//
//  Created by Mehrshad on 11/10/1401 AP.
//

#ifndef utility_h
#define utility_h

#include <time.h>
#include "shared.h"
#if DEBUG
#include "leak_detector_c.h"
#endif

// Replace trailing newline with null terminator
void RemoveTrailingNewline(char *);

int TakeShellInput(void);
int TakeIntInput(void);
/// Free output
void TakeStringInput(char **);

/// Free output
struct tm *GetTimeAdvancedBy(int day_offset);

#endif /* utility_h */
