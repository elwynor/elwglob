/* Build 26.10.29.3 07:15PM */
/*****************************************************************************
 *   GLOSET.H                                   Globals - Live settings     *
 *                                                                           *
 *   Copyright (C) 2026 Elwynor Technologies.                                *
 *   Created by Mark Laudenbach.                                             *
 *                                                                           *
 *   The settings live in ELWGLOBO.DAT, one row per setting, and are         *
 *   changed online with the =gset command; every change applies at once    *
 *   There are no CNF options: a new install starts from the                 *
 *   defaults in GLOSETS.H, or from a 3.43 board's values (ELWGLOB.V3).      *
 *                                                                           *
 *   Code reads the live values - gsval[]/gsnum[] through GSSTR/GSNUM/GSCHR, *
 *   or the module globals that applysettings() (ELWGLOB.C) refreshes after  *
 *   every change - never a copy taken at startup.                           *
 *                                                                           *
 *   Licensed under the GNU Affero General Public License v3.0.              *
 *****************************************************************************/

#ifndef GLOSET_H
#define GLOSET_H

#include "GLOSETS.H"

#define GS_FILE    "ELWGLOBO.DAT"    /* the settings                         */
#define GS_V3FILE  "ELWGLOB.V3"      /* a 3.43 ELWGLOB.MSG kept for import   */
#define GS_V3DONE  "ELWGLOB.V3I"     /*   renamed to this once imported      */
#define GS_NAMSIZ  9                 /* setting name and NUL                 */
#define GS_VALSIZ  40                /* longest value (39) and NUL           */

enum gsgroup {                       /* settings groups, in editor order     */
     GSG_KEYS = 1, GSG_CMDS, GSG_ONOFF, GSG_CHAT, GSG_MACROS, GSG_NEWUSER, GSG_PAGING,
     GSG_TIMERS, GSG_CREDITS, GSG_GENERAL, GSG_MODULES, GSG_COUNT_OF
};

enum gstype {                        /* what a setting holds                 */
     GST_KEY,                        /*   a key; blank lets everyone in      */
     GST_KEYNB,                      /*   a key that must not be blank       */
     GST_CMD,                        /*   a global command string            */
     GST_SUB,                        /*   a word typed after the chat command*/
     GST_WORD,                       /*   one word (default macro command)   */
     GST_STR,                        /*   text                               */
     GST_NUM,                        /*   a number from min to max           */
     GST_BOOL,                       /*   YES or NO                          */
     GST_CHR,                        /*   one character                      */
     GST_PFX,                        /*   the command prefix (punctuation)   */
     GST_ON                          /*   ON or OFF: a command in use?  Its  */
                                     /*   "min" is the command's GS_xxx      */
};

#define X(n, g, t, lo, hi, d, l) GS_##n,
enum gsindex { GLOSETS GS_COUNT_OF };
#undef X

#pragma pack(push, 1)
struct gsrec {                       /* one row of ELWGLOBO.DAT              */
     CHAR name[GS_NAMSIZ];           /*   setting name (key 0)               */
     CHAR value[GS_VALSIZ];          /*   value as text                      */
};
#pragma pack(pop)

extern CHAR gsval[GS_COUNT_OF][GS_VALSIZ];     /* live values, as text       */
extern LONG gsnum[GS_COUNT_OF];                /* numbers; YES = 1, NO = 0   */

#define GSSTR(n) (gsval[GS_##n])
#define GSNUM(n) (gsnum[GS_##n])
#define GSCHR(n) (gsval[GS_##n][0])
#define GSCMD(n) (gs_cmd(GS_##n))    /* a command string; "" while it is OFF */

VOID gs_open(VOID);                  /* open, load (or create/import), apply */
VOID gs_close(VOID);
VOID gs_command(VOID);               /* the settings editor command: menu    */
INT  gs_input(VOID);                 /* a line while in the menu, or 0       */
VOID gs_userreset(INT unum);         /* log-on/hang-up: not in the menu      */
VOID gs_help(VOID);                  /* its help (=? =gset)                  */
GBOOL gs_cmdon(INT idx);             /* is command setting idx turned on?    */
CHAR *gs_cmd(INT idx);               /* its string, or "" (never matches)    */

VOID applysettings(VOID);            /* ELWGLOB.C: push values into globals  */

#endif /* GLOSET_H */
