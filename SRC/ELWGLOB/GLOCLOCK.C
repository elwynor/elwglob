/* Build 26.10.12.1 09:00AM */
/*****************************************************************************
 *   GLOCLOCK.C                                 Globals - User's clock      *
 *                                                                           *
 *   Copyright (C) 2026 Elwynor Technologies.                                *
 *   Created by Mark Laudenbach.                                             *
 *                                                                           *
 *   Each user chooses 12-hour (4:45 PM) or 24-hour (16:45:00) times for     *
 *   everything Globals shows: =dt, the Who's Online heading, telegrams,    *
 *   alarms and last log-on times.  A user who has not chosen gets the      *
 *   board's default, the DEF12H setting, read when the time is shown, so   *
 *   a change to it reaches them at once.                                   *
 *                                                                           *
 *   The choice is kept in the engine's generic user database (genbb)       *
 *   under the user-id and "ELWGLOB", as the engine keeps its own per-user  *
 *   data (WG33 BBSGEN.C getgen/setgen).  ELWGLOBU.DAT is not touched: its  *
 *   record layout stays as boards have it.  The engine removes the record  *
 *   when the account is deleted (WG33 ACCOUNT.C lowdel -> delgen).         *
 *                                                                           *
 *   Licensed under the GNU Affero General Public License v3.0.              *
 *****************************************************************************/

#include <stdio.h>
#include "GLOBROUS.H"
#include "GLOSET.H"
#include "GLOCLOCK.H"

#define CK_MODNAM "ELWGLOB"          /* our key in the generic database      */

struct ckrec {                       /* our generic database record          */
     CHAR userid[UIDSIZ];            /*   key: user-id                       */
     CHAR modnam[MNMSIZ];            /*   key: always CK_MODNAM              */
     CHAR clock;                     /*   CK_12 or CK_24 (0: not chosen)     */
     UCHAR lastchan[2];              /*   chat channel left by "=cc off",    */
                                     /*   low byte first (0: none); bytes,   */
                                     /*   so the layout has no padding       */
     CHAR resv[29];                  /*   room for more preferences          */
};

static CHAR ckpref[MAXTERMS];        /* CK_xxx, per channel                  */

static GBOOL
ck_read(struct ckrec *rec, const CHAR *userid)  /* TRUE if the user has one  */
{
     GBOOL found;

     /* rec is the lookup key, so it is writable stack memory, never a    */
     /* constant (WG33 DFAAPI.C dfaAcqLock copies a key only from pages it */
     /* can write).                                                       */
     setmem(rec, sizeof(*rec), 0);
     stzcpy(rec->userid, userid, UIDSIZ);
     stzcpy(rec->modnam, CK_MODNAM, MNMSIZ);
     dfaSetBlk(genbb);
     found = dfaAcqEQ(rec, rec, 0);
     dfaRstBlk();
     return found;
}

VOID
ck_load(VOID)                        /* log-on: read the user's choice       */
{
     struct ckrec rec;

     ckpref[usrnum] = CK_DEFAULT;
     if (ck_read(&rec, usaptr->userid)
      && (rec.clock == CK_12 || rec.clock == CK_24)) {
          ckpref[usrnum] = rec.clock;
     }
}

VOID
ck_reset(INT unum)                   /* hang-up: back to the board default   */
{
     if (unum >= 0 && unum < MAXTERMS) {
          ckpref[unum] = CK_DEFAULT;
     }
}

GBOOL
ck_is12(INT unum)                    /* does channel unum see 12-hour times? */
{
     if (unum < 0 || unum >= MAXTERMS || ckpref[unum] == CK_DEFAULT) {
          return GSNUM(DEF12H) != 0;
     }
     return ckpref[unum] == CK_12;
}

VOID
ck_set(INT clock)                    /* the user chooses CK_12 or CK_24      */
{
     struct ckrec rec;
     GBOOL found;

     found = ck_read(&rec, usaptr->userid);
     rec.clock = (CHAR)clock;
     dfaSetBlk(genbb);
     if (found) {
          dfaUpdateV(&rec, sizeof(rec));
     }
     else {
          dfaInsertV(&rec, sizeof(rec));
     }
     dfaRstBlk();
     ckpref[usrnum] = (CHAR)clock;
}

USHORT
ck_lastchan(VOID)                    /* channel "=cc off" left (0: none)     */
{
     struct ckrec rec;

     if (!ck_read(&rec, usaptr->userid)) {
          return 0;
     }
     return (USHORT)(rec.lastchan[0] | (rec.lastchan[1] << 8));
}

VOID
ck_setlastchan(USHORT chan)          /* remember the channel "=cc off" left  */
{
     struct ckrec rec;
     GBOOL found;

     found = ck_read(&rec, usaptr->userid);   /* a new record: clock not  */
     rec.lastchan[0] = (UCHAR)(chan & 0xFF);  /*   chosen (CK_DEFAULT)    */
     rec.lastchan[1] = (UCHAR)(chan >> 8);
     dfaSetBlk(genbb);
     if (found) {
          dfaUpdateV(&rec, sizeof(rec));
     }
     else {
          dfaInsertV(&rec, sizeof(rec));
     }
     dfaRstBlk();
}

VOID
ck_fmt12(USHORT t, CHAR *buf)        /* a now() time as "4:45 PM"            */
{
     /* now() packs the time as hours << 11, minutes << 5, seconds / 2.   */
     INT hour = (t >> 11) & 0x1F;
     INT min = (t >> 5) & 0x3F;

     sprintf(buf, "%d:%02d %s", hour % 12 == 0 ? 12 : hour % 12, min,
             hour < 12 ? "AM" : "PM");
}

const CHAR *
ck_time(INT unum, USHORT t)          /* a now() time as channel unum sees it */
{
     static CHAR buf[4][12];         /* a few per message are fine           */
     static INT next = 0;
     CHAR *b;

     if (!ck_is12(unum)) {
          return nctime(t);
     }
     b = buf[next];
     next = (next + 1) % 4;
     ck_fmt12(t, b);
     return b;
}
