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
void ChargeAccountAsStudent(sqlite3 *, struct User *);
void SendCharge(sqlite3 *, struct User *);
void ListReservations(sqlite3 *, struct User *);
void ListTransactions(sqlite3 *, struct User *);

int GetBalance(sqlite3 *db, int user_id);
void TransferBalance(sqlite3 *db, int charge_amount,
                     struct User *user, int recipient_id);
/// Returns 1 if yes
/// Returns 0 if not
/// Returns -1 in case of error
int HasReservedBefore(sqlite3 *, int, int);

#endif /* student_h */
