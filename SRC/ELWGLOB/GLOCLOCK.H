/* Build 26.10.12.1 09:00AM */
/*****************************************************************************
 *   GLOCLOCK.H                                 Globals - User's clock      *
 *                                                                           *
 *   Copyright (C) 2026 Elwynor Technologies.                                *
 *   Created by Mark Laudenbach.                                             *
 *                                                                           *
 *   Licensed under the GNU Affero General Public License v3.0.              *
 *****************************************************************************/

#ifndef GLOCLOCK_H
#define GLOCLOCK_H

#define CK_DEFAULT 0                 /* the user has not chosen: board's     */
#define CK_12      12                /*   default (DEF12H) applies           */
#define CK_24      24

VOID  ck_load(VOID);                 /* log-on: read the user's choice       */
VOID  ck_reset(INT unum);            /* hang-up: back to the board default   */
GBOOL ck_is12(INT unum);             /* does channel unum see 12-hour times? */
VOID  ck_set(INT clock);             /* the user chooses CK_12 or CK_24      */
const CHAR *ck_time(INT unum, USHORT t);  /* a now() time for channel unum */
USHORT ck_lastchan(VOID);           /* channel "=cc off" left (0: none)     */
VOID  ck_setlastchan(USHORT chan);  /* remember the channel "=cc off" left  */
VOID  ck_fmt12(USHORT t, CHAR *buf); /* a now() time as "4:45 PM"            */

#endif /* GLOCLOCK_H */
