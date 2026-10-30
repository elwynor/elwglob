/* Build 26.10.29.3 07:15PM */
/*****************************************************************************
 *   TVARS.H                                    Globals - Text variables    *
 *                                                                           *
 *   Copyright (C) 2026 Elwynor Technologies.                                *
 *   Created by Mark Laudenbach.                                             *
 *                                                                           *
 *   The named text variables used in ELWGLOB.MSG.  Each name in            *
 *   GLOTVARS becomes the variable GLO_<name> on the board and the index     *
 *   TV_<name> here; the caller sets it just before printing.  Each name in  *
 *   GLOLIVECMD and GLOLIVE is a setting: GLO_<setting> reads its live value *
 *   when the text is printed, so a change made in =gset shows at once.      *
 *   Names are global to the whole BBS: GLO_ plus at most 11 characters.     *
 *                                                                           *
 *   Licensed under the GNU Affero General Public License v3.0.              *
 *****************************************************************************/

#ifndef TVARS_H
#define TVARS_H

#define GLOTVARS \
     X(SETNAME)   /* a setting's name (its 3.43 CNF option name)  */ \
     X(SETVAL)    /* a setting's value                            */ \
     X(SETSHORT)  /* a setting's value, cut to fit a list row     */ \
     X(SETDEF)    /* a setting's default                          */ \
     X(SETDESC)   /* a setting's label                            */ \
     X(SETLIM)    /* the values a setting allows                  */ \
     X(ITEM)      /* a setting's number in the editor (2.14)      */ \
     X(GROUP)     /* a settings group's title                     */ \
     X(COUNT)     /* a count                                      */ \
     X(CMD)       /* a command string                             */ \
     X(OTHER)     /* another setting's name                       */ \
     X(ARG)       /* what the user typed                          */ \
     X(PAGE)      /* a page number                                */ \
     X(PAGES)     /* a number of pages                            */ \
     X(MIN)       /* the smallest value allowed                   */ \
     X(MAX)       /* the largest value allowed, or a length       */ \
     X(VERSION)   /* the module version                           */ \
     X(USERID)    /* the user-id the message is about             */ \
     X(NUMBER)    /* a macro number, or a row number in a list    */ \
     X(DATE)      /* a date                                       */ \
     X(TIME)      /* a time                                       */ \
     X(CREDITS)   /* a number of credits                          */ \
     X(CRTYPE)    /* FREE or PAID credits                         */ \
     X(DAYS)      /* days in class, or days suspended             */ \
     X(MINUTES)   /* minutes on-line, or minutes added or taken   */ \
     X(CHANGE)    /* shortened or extended (time on-line)         */ \
     X(CLASS)     /* a class name                                 */ \
     X(KEY)       /* a key name                                   */ \
     X(MODULE)    /* where a user is on the board                 */ \
     X(CHANNEL)   /* a user's channel (hex)                       */ \
     X(SEX)       /* a user's sex (M or F)                        */ \
     X(AGE)       /* a user's age, or ? if hidden                 */ \
     X(CONN)      /* how a user is connected                      */ \
     X(FLAGS)     /* a user's Who's Online flags                  */ \
     X(FROM)      /* where a user calls from                      */ \
     X(PADDING)   /* spaces that line up Who's Online display 3   */ \
     X(USERS)     /* user or users                                */ \
     X(CHATCHAN)  /* a chat channel number, or OFF               */ \
     X(TOPIC)     /* a chat channel topic                         */ \
     X(MACRO)     /* a macro                                      */ \
     X(MACROCMD)  /* a macro's command string                     */ \
     X(MAXCMD)    /* the longest macro command string             */ \
     X(LOGONMSG)  /* a log-on message                             */ \
     X(LOGOFFMSG) /* a log-off message                            */ \
     X(LOGONOFF)  /* on or off: which friend notice               */ \
     X(FRNDON)    /* the log-on friend notice user-id             */ \
     X(FRNDOFF)   /* the log-off friend notice user-id            */ \
     X(BUSYNOTE)  /* the busy-notify user-id                      */ \
     X(PGBLOCK)   /* a user blocked from paging                   */ \
     X(TELBLOCK)  /* a user blocked from sending telegrams        */ \
     X(HELPTEXT)  /* a global command's label in the help list    */ \
     X(NOWNOT)    /* now or not (invisible)                       */ \
     X(WILL)      /* will or won't (show your age)                */ \
     X(CALLLIMIT) /* minutes allowed per call                     */ \
     X(DAYLIMIT)  /* minutes allowed per day                      */ \
     X(CALLLEFT)  /* minutes left this call                       */ \
     X(DAYLEFT)   /* minutes left today                           */ \
     X(MINCALL)   /* minutes on-line this call                    */ \
     X(MINTODAY)  /* minutes on-line today                        */ \
     X(CALLCHG)   /* shortened or extended: this call             */ \
     X(CALLMINS)  /* minutes added to or taken from this call     */ \
     X(DAYCHG)    /* shortened or extended: today                 */ \
     X(DAYMINS)   /* minutes added to or taken from today         */ \
     X(WIDTH)     /* screen width                                 */ \
     X(LENGTH)    /* screen length, or CONTINUOUS                 */ \
     X(ANSI)      /* ANSI ON or OFF                               */ \
     X(IBM)       /* IBM or NON-IBM                               */ \
     X(EDITOR)    /* LINE or FULL SCREEN editor                   */ \
     X(DUPLEX)    /* HALF or FULL duplex                          */ \
     X(OUTPUT)    /* continuous output ON or OFF / CONTINUOUS     */ \
     X(BUSY)      /* the "Busy" flag                              */ \
     X(PESTER)    /* the Editor pester setting                    */ \
     X(PAGING)    /* the page setting: ON, OFF or OK              */ \
     X(INVIS)     /* invisibility                                 */ \
     X(INVPERM)   /* invisible by Globals: YES or NO              */ \
     X(NOTICES)   /* log-on notices                               */ \
     X(ECHO)      /* chat echo                                    */ \
     X(CHANNOTE)  /* chat join/leave notices                      */ \
     X(TELENOTE)  /* telegram notification                        */ \
     X(SHOWAGE)   /* show your age in Who's Online                */ \
     X(WHODISP)   /* the default Who's Online display             */

/* Command settings: GLO_<name> is "" while the command is turned OFF.     */
#define GLOLIVECMD \
     X(HLPSTG)   X(OFFSTG)   X(CNTSTG)   X(BPSTG)    X(BPSTPSTG) \
     X(ANSSTG)   X(BSYSTG)   X(BNTSTG)   X(DUPSTG)   X(EDTSTG)   \
     X(RLGSTG)   X(MBRSTG)   X(IBMSTG)   X(SWSTG)    X(SLSTG)    \
     X(CRDSTG)   X(WHOSTG)   X(EPSTSTG)  X(DNTSTG)   X(PSTSTG)   \
     X(ZAPSTG)   X(RPTSTG)   X(NEWSTG)   X(LOGSTG)   X(LONSTR)   \
     X(LOFFSTR)  X(LCLRSTR)  X(SHOWSTR)  X(CHATSTG)  X(GTLSTG)   \
     X(TALTSTG)  X(MACSTG)   X(MCSET)    X(MMSET)    X(MCLIST)   \
     X(MMLIST)   X(GLBSTG)   X(TRNSTG)   X(TRDSTG)   X(TEXTSTG)  \
     X(PDYSTG)   X(CSTSTG)   X(SWCSTG)   X(UKSTG)    X(INVSTG)   \
     X(ASUSSTG)  X(SUSSTG)   X(DLUSTG)   X(PGUSTG)   X(FPUSTG)   \
     X(RPGSTG)   X(TCLRSTG)  X(TDASTG)   X(TBKSTG)   X(ALRMSTG)  \
     X(FRNDSTG)

/* Other settings shown in text: chat words, characters, names, numbers.   */
#define GLOLIVE \
     X(ECHSTG)   X(SETSTG)   X(SHOWSTG)  X(DISSTG)   X(TPCSTG)   \
     X(NOTSTG)   X(CLRSTG)   X(TLCCHR)   X(ANSCHR)   X(BCKCHR)   \
     X(FRECHR)   X(SPCCHR)   X(EDTNAM)   X(PGDLY)

#define X(n) TV_##n,
enum tvindex { GLOTVARS TV_COUNT_OF };
#undef X

VOID tv_init(VOID);                  /* register all of our text variables   */
VOID tv_set(INT which, const CHAR *value);     /* set a text value           */
VOID tv_setn(INT which, LONG value);           /* set a numeric value        */
VOID tv_setc(INT which, CHAR value);           /* set a one-character value  */
VOID tv_raw(GBOOL raw);              /* TRUE: values not escaped for prfmsg  */

#endif /* TVARS_H */
