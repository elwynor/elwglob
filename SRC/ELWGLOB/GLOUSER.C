/* Build 26.10.12.2 05:41AM */
/*****************************************************************************
 *   GLOUSER.C                                  Globals - User menu         *
 *                                                                           *
 *   Copyright (C) 2026 Elwynor Technologies.                                *
 *   Created by Mark Laudenbach.                                             *
 *                                                                           *
 *   The menu a user gets on choosing Globals from the BBS menu: their own   *
 *   Globals settings (chat channel, echo, page setting, notices, log-on     *
 *   messages, macros, clock, screen and terminal settings), each shown     *
 *   only if they have the key for the global command that changes it and   *
 *   the sysop has not turned that command off.  A user who may use =gset   *
 *   also gets S, the system settings menu.  The =config command opens the  *
 *   same menu from anywhere on the board.                                  *
 *                                                                           *
 *   The settings are in sections (chat, pages, log-on, macros, terminal),  *
 *   so each screen fits a 24-line terminal: the first screen lists the     *
 *   sections, a section lists its settings, X goes back one step.          *
 *                                                                           *
 *   Each change runs the global command itself (for example "=cc echo"),   *
 *   built from the live command strings, so the menu checks, saves and      *
 *   answers exactly as the command does and can never drift from it.       *
 *                                                                           *
 *   Like the settings menu (GLOSET.C), the menu takes every line at the    *
 *   top of the global handler while it is open (so a reply such as a        *
 *   log-on message starting with the telegram character is not sent as a   *
 *   telegram) and returns -1; X returns 1, the engine then passes a blank  *
 *   line to the module (WG33 MAJORBBS.C injacr/hdlcri), and globinput()     *
 *   takes the user back to the BBS menu.                                    *
 *                                                                           *
 *   Licensed under the GNU Affero General Public License v3.0.              *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "GLOBROUS.H"
#include "GLOSET.H"
#include "GLOUSER.H"
#include "TVARS.H"
#include "GLOCLOCK.H"

extern HMCVFILE glomb;               /* ELWGLOB.C                            */
extern DFAFILE *userglobdata;
extern struct flddef userdataFDA[];
extern userhelp far help[HELPSIZE];
extern lastcommand usercommand[MAXTERMS];
extern USHORT globalchatchannel[MAXTERMS];
extern UCHAR chatecho[MAXTERMS];
extern UCHAR channotice[MAXTERMS];
extern pagedata userpage[MAXTERMS];
extern UCHAR notifytelegrams[MAXTERMS];
extern UCHAR usrchannel[MAXTERMS];
extern UCHAR showages[MAXTERMS];
extern UCHAR whonumber[MAXTERMS];
extern UCHAR duplex[MAXTERMS];
extern UCHAR bugem[MAXTERMS];
extern INT numchatchannels;
extern CHAR *syschankey;
extern INT elwglobals(VOID);

enum {                               /* kinds of item                        */
     GK_TOGGLE,                      /*   run the command, no question       */
     GK_NUM,                         /*   ask for a number                   */
     GK_PAGE,                        /*   ask ON, OK or OFF                  */
     GK_TEXT,                        /*   ask for text ("-" clears)          */
     GK_MACRO,                       /*   ask "number value"                 */
     GK_CLOCK                        /*   flip 12/24-hour (GLOCLOCK.C)       */
};

enum {                               /* the items, in menu order             */
     GI_CHON, GI_CHAN, GI_ECHO, GI_CHNOT,
     GI_PAGE, GI_TELN, GI_PEST, GI_BUSY,
     GI_LOGN, GI_LONM, GI_LOFM, GI_AGE, GI_WHO, GI_CLK,
     GI_MACC, GI_MACT,
     GI_DUP, GI_SCNW, GI_SCNL, GI_ANSI, GI_IBM, GI_CONT, GI_EDIT,
     GI_COUNT_OF
};

#define GU_SECTS 5                   /* sections, 1 to GU_SECTS              */

static const INT secttitle[GU_SECTS + 1] = {
     0, GUSEC1, GUSEC2, GUSEC3, GUSEC4, GUSEC5
};

static const struct {
     INT label;                      /* its label in ELWGLOB.MSG             */
     INT kind;                       /* GK_xxx                               */
     INT help;                       /* help[] entry whose key it needs      */
     INT cmd;                        /* GS_xxx of the command it runs        */
     INT sect;                       /* its section, 1 to GU_SECTS           */
} items[GI_COUNT_OF] = {             /* (-1, -1: everyone, no command)       */
     { GUCHON,  GK_TOGGLE, 6, GS_CHATSTG, 1 }, /* =cc on|off                 */
     { GUCHAN,  GK_NUM,    6, GS_CHATSTG, 1 }, /* =cc set <n>                */
     { GUECHO,  GK_TOGGLE, 6, GS_CHATSTG, 1 }, /* =cc echo                   */
     { GUCHNOT, GK_TOGGLE, 6, GS_CHATSTG, 1 }, /* =cc notice                 */
     { GUPAGE,  GK_PAGE,  19, GS_PGUSTG,  2 }, /* =p on|ok|off               */
     { GUTELN,  GK_TOGGLE,39, GS_TALTSTG, 2 }, /* =ta                        */
     { GUPEST,  GK_TOGGLE,22, GS_EPSTSTG, 2 }, /* =pest                      */
     { GUBUSY,  GK_TOGGLE, 3, GS_BSYSTG,  2 }, /* =b                         */
     { GULOGN,  GK_TOGGLE,17, GS_LOGSTG,  3 }, /* =log                       */
     { GULONM,  GK_TEXT,  17, GS_LONSTR,  3 }, /* =lon [message]             */
     { GULOFM,  GK_TEXT,  17, GS_LOFFSTR, 3 }, /* =loff [message]            */
     { GUAGE,   GK_TOGGLE,37, GS_WHOSTG,  3 }, /* # on|off                   */
     { GUWHO,   GK_NUM,   37, GS_WHOSTG,  3 }, /* # set <n>                  */
     { GUCLK,   GK_CLOCK, -1, -1,         3 }, /* (no command: GLOCLOCK.C)   */
     { GUMACC,  GK_MACRO, 18, GS_MCSET,   4 }, /* =mc <n> <command>          */
     { GUMACT,  GK_MACRO, 18, GS_MMSET,   4 }, /* =mm <n> <macro>            */
     { GUDUP,   GK_TOGGLE,10, GS_DUPSTG,  5 }, /* =du                        */
     { GUSCNW,  GK_NUM,   29, GS_SWSTG,   5 }, /* =sw <n>                    */
     { GUSCNL,  GK_NUM,   25, GS_SLSTG,   5 }, /* =s <n>                     */
     { GUANSI,  GK_TOGGLE, 2, GS_ANSSTG,  5 }, /* =a                         */
     { GUIBM,   GK_TOGGLE,13, GS_IBMSTG,  5 }, /* =g                         */
     { GUCONT,  GK_TOGGLE, 8, GS_CNTSTG,  5 }, /* =ct                        */
     { GUEDIT,  GK_TOGGLE,12, GS_EDTSTG,  5 }  /* =ed                        */
};

enum {                               /* where a user is in the menu          */
     GUS_OFF = 0,                    /*   not in it                          */
     GUS_MENU,                       /*   choosing a section                 */
     GUS_SECT,                       /*   choosing an item of a section      */
     GUS_VALUE,                      /*   answering an item's question       */
     GUS_SYSOP                       /*   in the system settings menu        */
};

static UCHAR gust[MAXTERMS];         /* GUS_xxx, per channel                 */
static UCHAR gusect[MAXTERMS];       /* section shown                        */
static UCHAR guitem[MAXTERMS];       /* item being answered                  */
static GBOOL gurunning = FALSE;      /* running a command for the menu       */

static GBOOL
gu_allowed(INT it)                   /* may this user change item it?        */
{
     if (items[it].help < 0) {
          return TRUE;
     }
     return haskey(help[items[it].help].key) != 0 && gs_cmdon(items[it].cmd);
}

static GBOOL
gu_sysop(VOID)                       /* may this user open =gset?            */
{
     return hasmaster() || haskey(help[44].key);
}

static VOID
gu_word(INT msgnum)                  /* a value word from ELWGLOB.MSG        */
{
     tv_set(TV_SETVAL, getmsg(msgnum));
}

static VOID
gu_onoff(GBOOL on)
{
     gu_word(on ? GUVON : GUVOFF);
}

static VOID
gu_value(INT it)                     /* GLO_SETVAL: item it's current value  */
{
     struct userdata ud;

     switch (it) {
     case GI_CHON:  gu_onoff(globalchatchannel[usrnum] != 0);    break;
     case GI_CHAN:
          if (globalchatchannel[usrnum] == 0) {
               gu_word(GUVOFF);
          }
          else {
               tv_setn(TV_SETVAL, globalchatchannel[usrnum]);
          }
          break;
     case GI_ECHO:  gu_onoff(chatecho[usrnum] != 0);            break;
     case GI_CHNOT: gu_onoff(channotice[usrnum] == 1);          break;
     case GI_PAGE:
          gu_word(userpage[usrnum].pagesetting == POFF ? GUVOFF
                : userpage[usrnum].pagesetting == POK ? GUVOK : GUVON);
          break;
     case GI_TELN:  gu_onoff(notifytelegrams[usrnum] == 1);     break;
     case GI_LOGN:  gu_onoff(usrchannel[usrnum] == 1);          break;
     case GI_LONM:
     case GI_LOFM:
          setmem(&ud, sizeof(ud), 0);
          dfaSetBlk(userglobdata);
          if (dfaAcqEQ(&ud, usaptr->userid, 0)) {
               cvtDataIP(&ud, sizeof(struct userdata), sizeof(struct userdata),
                         userdataFDA, CVTPACKED, CVTSERVER, usrnum);
          }
          dfaRstBlk();
          ud.logon[MESSSIZE - 1] = ud.logoff[MESSSIZE - 1] = '\0';
          if (*(it == GI_LONM ? ud.logon : ud.logoff) == '\0') {
               gu_word(GSBLANK);
          }
          else {
               tv_set(TV_SETVAL, it == GI_LONM ? ud.logon : ud.logoff);
          }
          break;
     case GI_AGE:   gu_onoff(showages[usrnum] == 1);            break;
     case GI_WHO:   tv_setn(TV_SETVAL, whonumber[usrnum]);       break;
     case GI_CLK:   gu_word(ck_is12(usrnum) ? GUV12 : GUV24);    break;
     case GI_DUP:   gu_word(duplex[usrnum] == 0 ? GUVHALF : GUVFULL); break;
     case GI_PEST:  gu_onoff(bugem[usrnum] == 1);               break;
     case GI_BUSY:  gu_onoff(usrbusy(usrptr) != 0);             break;
     case GI_SCNW:  tv_setn(TV_SETVAL, usaptr->scnwid);          break;
     case GI_SCNL:
          if (usaptr->scnbrk == CTNUOS) {
               gu_word(GUVCONT);
          }
          else {
               tv_setn(TV_SETVAL, usaptr->scnbrk);
          }
          break;
     case GI_ANSI:  gu_onoff((usaptr->ansifl & ANSON) == ANSON);  break;
     case GI_IBM:   gu_word(usaptr->systyp == 1 ? GUVIBM : GUVNIBM); break;
     case GI_CONT:  gu_word(usaptr->scnbrk == CTNUOS ? GUVCONT : GUVPAUS); break;
     case GI_EDIT:  gu_word(usaptr->usrprf == PRFLIN ? GUVLINE : GUVFSCR); break;
     default:       tv_set(TV_SETVAL, "");                      break;
     }
}

static GBOOL
gu_sectshown(INT sect)               /* has section sect an item for them?   */
{
     INT it;

     for (it = 0 ; it < GI_COUNT_OF ; it++) {
          if (items[it].sect == sect && gu_allowed(it)) {
               return TRUE;
          }
     }
     return FALSE;
}

static VOID
gu_menu(VOID)                        /* show the first screen: the sections  */
{
     INT sect, n = 0;

     gust[usrnum] = GUS_MENU;
     prfmsg(GUMENU);
     for (sect = 1 ; sect <= GU_SECTS ; sect++) {
          if (gu_sectshown(sect)) {
               tv_setn(TV_ITEM, ++n);
               tv_set(TV_GROUP, getmsg(secttitle[sect]));
               prfmsg(GUSROW);
          }
     }
     if (gu_sysop()) {
          prfmsg(GUSYS);
     }
     prfmsg(GUPRMT);
}

static INT
gu_picksect(INT n)                   /* the section numbered n, or -1        */
{
     INT sect;

     for (sect = 1 ; sect <= GU_SECTS ; sect++) {
          if (gu_sectshown(sect) && --n == 0) {
               return sect;
          }
     }
     return -1;
}

static VOID
gu_section(VOID)                     /* show the section's settings          */
{
     INT it, n = 0;

     gust[usrnum] = GUS_SECT;
     tv_set(TV_GROUP, getmsg(secttitle[gusect[usrnum]]));
     prfmsg(GUSHDR);
     outprf(usrnum);
     for (it = 0 ; it < GI_COUNT_OF ; it++) {
          if (items[it].sect == gusect[usrnum] && gu_allowed(it)) {
               tv_setn(TV_ITEM, ++n);
               tv_set(TV_SETDESC, getmsg(items[it].label));
               gu_value(it);
               prfmsg(GUROW);
               outprf(usrnum);
          }
     }
     prfmsg(GUSPRMT);
}

static INT
gu_pick(INT n)                       /* the section's item numbered n, or -1 */
{
     INT it;

     for (it = 0 ; it < GI_COUNT_OF ; it++) {
          if (items[it].sect == gusect[usrnum] && gu_allowed(it) && --n == 0) {
               return it;
          }
     }
     return -1;
}

static VOID
gu_run(const CHAR *line)             /* run a global command for the user    */
{
     CHAR save[INPSIZ];

     /* The command must not become the user's "repeat last command".     */
     /* The message block stack (WG33 MCVAPI.C setmbk/rstmbk): give back   */
     /* the menu's level while the command runs with its own, then take it */
     /* again, so the stack balances whatever the command does.            */
     stzcpy(save, usercommand[usrnum].thecommand, INPSIZ);
     stzcpy(input, line, INPSIZ);
     parsin();
     rstmbk();
     gurunning = TRUE;
     elwglobals();
     gurunning = FALSE;
     setmbk(glomb);
     stzcpy(usercommand[usrnum].thecommand, save, INPSIZ);
}

static VOID
gu_toggle(INT it)                    /* run a toggle item's command          */
{
     CHAR line[INPSIZ];

     switch (it) {
     case GI_CHON:
          sprintf(line, "%s %s", GSSTR(CHATSTG), globalchatchannel[usrnum] != 0
                  ? GSSTR(CHOFSTG) : GSSTR(CHONSTG));
          break;
     case GI_ECHO:
          sprintf(line, "%s %s", GSSTR(CHATSTG), GSSTR(ECHSTG));
          break;
     case GI_CHNOT:
          sprintf(line, "%s %s", GSSTR(CHATSTG), GSSTR(NOTSTG));
          break;
     case GI_AGE:
          sprintf(line, "%s %s", GSSTR(WHOSTG), showages[usrnum] == 1 ? "OFF" : "ON");
          break;
     default:
          stzcpy(line, help[items[it].help].command, INPSIZ);
          if (it == GI_TELN) {
               stzcpy(line, GSSTR(TALTSTG), INPSIZ);
          }
          break;
     }
     gu_run(line);
}

static VOID
gu_ask(INT it)                       /* ask an item's question               */
{
     gust[usrnum] = GUS_VALUE;
     guitem[usrnum] = (UCHAR)it;
     tv_set(TV_SETDESC, getmsg(items[it].label));
     switch (it) {
     case GI_CHAN:
          tv_setn(TV_MIN, 0);
          tv_setn(TV_MAX, numchatchannels - (haskey(syschankey) ? 0 : 1));
          prfmsg(GUPNUM);
          break;
     case GI_WHO:
          tv_setn(TV_MIN, 1);
          tv_setn(TV_MAX, NUMWHODIS);
          prfmsg(GUPNUM);
          break;
     case GI_SCNW:
          tv_setn(TV_MIN, MIN_SCREEN_WIDTH);
          tv_setn(TV_MAX, MAX_SCREEN_WIDTH);
          prfmsg(GUPNUM);
          break;
     case GI_SCNL:
          tv_setn(TV_MIN, MIN_SCREEN_LENGTH);
          tv_setn(TV_MAX, MAX_SCREEN_LENGTH);
          prfmsg(GUPNUM);
          break;
     case GI_PAGE:
          prfmsg(GUPPAGE);
          break;
     case GI_LONM:
     case GI_LOFM:
          tv_setn(TV_MAX, MESSSIZE - 1);
          prfmsg(GUPTEXT);
          break;
     case GI_MACC:
          tv_setn(TV_MAX, CMSIZ - 1);
          prfmsg(GUPMACC);
          break;
     case GI_MACT:
          tv_setn(TV_MAX, MACSIZ - 1);
          prfmsg(GUPMACT);
          break;
     }
}

static VOID
gu_answer(CHAR *text)                /* the answer to an item's question     */
{
     INT it = guitem[usrnum];
     CHAR line[INPSIZ];

     if (*text == '\0') {            /* Enter keeps it                       */
          gu_section();
          return;
     }
     switch (it) {
     case GI_CHAN:
          sprintf(line, "%s %s %.6s", GSSTR(CHATSTG), GSSTR(SETSTG), text);
          break;
     case GI_WHO:
          sprintf(line, "%s SET %.6s", GSSTR(WHOSTG), text);
          break;
     case GI_LONM:
     case GI_LOFM:
          /* "-" clears: the command with nothing after it.                 */
          sprintf(line, "%s %.*s", it == GI_LONM ? GSSTR(LONSTR) : GSSTR(LOFFSTR),
                  INPSIZ - 20, strcmp(text, "-") == 0 ? "" : text);
          break;
     case GI_MACC:
          sprintf(line, "%s %.*s", GSSTR(MCSET), INPSIZ - 20, text);
          break;
     case GI_MACT:
          sprintf(line, "%s %.*s", GSSTR(MMSET), INPSIZ - 20, text);
          break;
     default:                        /* GI_PAGE, GI_SCNW, GI_SCNL            */
          sprintf(line, "%s %.10s", help[items[it].help].command, text);
          break;
     }
     gu_run(line);
     gu_section();
}

static VOID
gu_trimmed(CHAR **text)              /* the whole line, spaces kept inside   */
{
     CHAR *s, *e;

     rstrin();
     for (s = input ; *s == ' ' ; s++) {
     }
     for (e = s + strlen(s) ; e > s && e[-1] == ' ' ; e--) {
     }
     *e = '\0';
     *text = s;
}

VOID
gu_begin(VOID)                       /* the user chose Globals from the menu */
{
     setmbk(glomb);
     gu_menu();
     outprf(usrnum);
     rstmbk();
}

INT                                  /*   0 not ours; -1 taken; 1 left       */
gu_input(VOID)                       /* a line typed while in the menu       */
{
     CHAR *text;
     CHAR *end;
     INT n, it, rc = -1;
     CHAR c = (CHAR)(margc > 0 ? toupper((UCHAR)margv[0][0]) : '\0');
     GBOOL word = (margc == 1 && margv[0][1] == '\0');

     if (gust[usrnum] == GUS_OFF || gurunning) {
          return 0;
     }
     setmbk(glomb);
     switch (gust[usrnum]) {
     case GUS_SYSOP:                 /* back from the settings menu          */
          gu_menu();
          break;
     case GUS_MENU:
          if (margc == 0) {
               gu_menu();
          }
          else if (word && c == 'X') {
               gust[usrnum] = GUS_OFF;
               prfmsg(GUBYE);
               rc = 1;
          }
          else if (word && c == 'S' && gu_sysop()) {
               gust[usrnum] = GUS_SYSOP;
               tv_set(TV_CMD, GSSTR(GSETSTG));
               gs_command();         /* pops our setmbk; its X brings the    */
               return -1;            /*   user back here                     */
          }
          else {
               n = (INT)strtol(margv[0], &end, 10);
               n = (margc == 1 && *end == '\0') ? gu_picksect(n) : -1;
               if (n < 0) {
                    prfmsg(GSNOPICK);
                    gu_menu();
               }
               else {
                    gusect[usrnum] = (UCHAR)n;
                    gu_section();
               }
          }
          break;
     case GUS_SECT:
          if (margc == 0) {
               gu_section();
          }
          else if (word && c == 'X') {
               gu_menu();
          }
          else {
               n = (INT)strtol(margv[0], &end, 10);
               it = (margc == 1 && *end == '\0') ? gu_pick(n) : -1;
               if (it < 0) {
                    prfmsg(GSNOPICK);
                    gu_section();
               }
               else if (items[it].kind == GK_TOGGLE) {
                    gu_toggle(it);
                    gu_section();
               }
               else if (items[it].kind == GK_CLOCK) {
                    ck_set(ck_is12(usrnum) ? CK_24 : CK_12);
                    gu_value(it);
                    prfmsg(GUCLKSET);
                    gu_section();
               }
               else {
                    gu_ask(it);
               }
          }
          break;
     case GUS_VALUE:
          gu_trimmed(&text);
          gu_answer(text);
          break;
     }
     outprf(usrnum);
     rstmbk();
     return rc;
}

GBOOL
gu_active(INT unum)                  /* is channel unum in the user menu?    */
{
     return unum >= 0 && unum < MAXTERMS && gust[unum] != GUS_OFF;
}

VOID
gu_userreset(INT unum)               /* log-on/hang-up: not in the menu      */
{
     if (unum >= 0 && unum < MAXTERMS) {
          gust[unum] = GUS_OFF;
     }
}
