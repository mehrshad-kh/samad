//
//  student.h
//  samad
//
//  Created by Mehrshad on 17/10/1401 AP.
//

#ifndef student_h
#define student_h

#include <sqlite3.h>
#include "types.h"

void DisplayStudentMenu(sqlite3 *, struct User **);

void ReserveFood(sqlite3 *, struct User *);
void ChargeAccountAsStudent(sqlite3 *, const char *);

#endif /* student_h */
