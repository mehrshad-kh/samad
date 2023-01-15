//
//  student.h
//  samad
//
//  Created by Mehrshad on 17/10/1401 AP.
//

#ifndef student_h
#define student_h

#include <sqlite3.h>
#include "shared.h"
#include "types.h"
#if DEBUG
#include "leak_detector_c.h"
#endif

void DisplayStudentMenu(sqlite3 *, struct User **);

void ReserveFood(sqlite3 *, struct User *);
void ChargeAccountAsStudent(sqlite3 *, const char *);

#endif /* student_h */
