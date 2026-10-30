/* Build 26.10.29.3 07:15PM */
/*****************************************************************************
 *   TVARS.C                                    Globals - Text variables    *
 *                                                                           *
 *   Copyright (C) 2026 Elwynor Technologies.                                *
 *   Created by Mark Laudenbach.                                             *
 *                                                                           *
 *   Every changing value in ELWGLOB.MSG is a named text variable - the      *
 *   bytes 0x01 <justify> <width+32> NAME 0x01 - rather than a printf-style  *
 *   %s, so sysops can reword, reorder or drop values when they edit the     *
 *   text.  The engine replaces each variable with the string returned by    *
 *   the routine registered for its name; we fill those strings with         *
 *   tv_set() just before printing a message.  A setting shown in text       *
 *   (GLOLIVECMD, GLOLIVE) is not set by the caller: its routine returns     *
 *   the live value, so a change made in =gset shows in every message.       *
 *                                                                           *
 *   prfmsg() expands the variables FIRST and then runs the result through   *
 *   vsprintf() as a format, so a '%' in a value would be read as a format   *
 *   code.  Values are therefore returned with every '%' doubled, except     *
 *   while tv_raw(TRUE) is in force for text expanded with getmsg(), which   *
 *   never goes through vsprintf().                                          *
 *                                                                           *
 *   The engine handles at most 79 characters per variable (WG33 MENUING.C   *
 *   grbtxv), so values are cut to 79 here; longer text is printed between   *
 *   two messages instead.                                                   *
 *                                                                           *
 *   Licensed under the GNU Affero General Public License v3.0.              *
 *****************************************************************************/

#include <stdio.h>
#include "gcomm.h"
#include "majorbbs.h"
#include "GLOSET.H"
#include "TVARS.H"

#define TVBSIZ 80                    /* 79 characters and the NUL            */

extern CHAR *GLO_VERSION;            /* ELWGLOB.C                            */

static CHAR tvval[TV_COUNT_OF][TVBSIZ];        /* values as set              */
static CHAR tvout[2 * TVBSIZ];                 /* value as returned          */
static GBOOL tvraw = FALSE;                    /* TRUE: return values as set */

static CHAR *
tvesc(const CHAR *s)                 /* return a value, escaped for prfmsg() */
{
     CHAR *d = tvout;
     INT n;

     for (n = 0; *s != '\0' && n < TVBSIZ - 1; n++) {
          if (*s == '%' && !tvraw) {
               *d++ = '%';
          }
          *d++ = *s++;
     }
     *d = '\0';
     return tvout;
}

/* The engine needs a separate routine for each registered name.            */
#define X(n) static CHAR *tv##n(VOID) { return tvesc(tvval[TV_##n]); }
GLOTVARS
#undef X
#define X(n) static CHAR *tl##n(VOID) { return tvesc(GSCMD(n)); }
GLOLIVECMD
#undef X
#define X(n) static CHAR *tl##n(VOID) { return tvesc(GSSTR(n)); }
GLOLIVE
#undef X

VOID
tv_init(VOID)                        /* register all of our text variables   */
{
     tv_set(TV_VERSION, GLO_VERSION);
#define X(n) register_textvar("GLO_" #n, tv##n);
     GLOTVARS
#undef X
#define X(n) register_textvar("GLO_" #n, tl##n);
     GLOLIVECMD
     GLOLIVE
#undef X
}

VOID
tv_set(INT which, const CHAR *value)           /* set a text value           */
{
     stzcpy(tvval[which], value == NULL ? "" : value, TVBSIZ);
}

VOID
tv_setn(INT which, LONG value)                 /* set a numeric value        */
{
     sprintf(tvval[which], "%ld", value);
}

VOID
tv_setc(INT which, CHAR value)                 /* set a one-character value  */
{
     tvval[which][0] = value;
     tvval[which][1] = '\0';
}

VOID
tv_raw(GBOOL raw)                    /* TRUE: values not escaped for prfmsg  */
{
     tvraw = raw;
}
