/* Build 26.10.12.3 07:42AM */
/*****************************************************************************
 *   TEST_GLOSET.C                       Globals - Settings unit tests       *
 *                                                                           *
 *   Copyright (C) 2026 Elwynor Technologies.                                *
 *   Created by Mark Laudenbach.                                             *
 *                                                                           *
 *   Tests GLOSET.C away from a BBS, against the stand-in SDK in STUB\:     *
 *   - a new install gets every default, and every default passes the       *
 *     checks when it is read back;                                          *
 *   - values survive a restart.  The stand-in Btrieve copies a lookup key   *
 *     only from writable memory, like the engine (WG33 DFAAPI.C dfaAcqLock  *
 *     -> ISGOODPT.C goodblk), so a string-constant key fails here as it     *
 *     does on a board;                                                      *
 *   - the =gset menu: groups, pages, picking, checks, duplicate command     *
 *     strings, clearing, values with spaces, resets, the command prefix,    *
 *     leaving;                                                              *
 *   - importing a 3.43 board's values from its ELWGLOB.MSG (DATA\,          *
 *     ELWGLOB.343 is the 3.43 file as released), with edits;                *
 *   - a stored value that is no longer allowed, and a setting missing from  *
 *     an older file;                                                        *
 *   - creating a missing data file from its template (GLOFILES.C), and      *
 *     never touching one that exists;                                       *
 *   - commands on/off: an OFF command reads as "", keeps its string and     *
 *     its place in the duplicate check, and stays off after a restart;      *
 *   - the user's clock (GLOCLOCK.C): 12-hour times, and the board default   *
 *     reaching users who have not chosen.                                   *
 *                                                                           *
 *   Build and run: TESTS\RUN_TESTS.CMD.  Exit code 0 = all passed.          *
 *                                                                           *
 *   Licensed under the GNU Affero General Public License v3.0.              *
 *****************************************************************************/

#include <stdarg.h>
#include <stdlib.h>
#include "GLOBROUS.H"
#include "GLOSET.H"
#include "TVARS.H"
#include "GLOFILES.H"
#include "GLOCLOCK.H"

static int fails = 0;
static int checks = 0;

static void
check(int ok, const char *what)
{
     checks++;
     if (!ok) {
          fails++;
          printf("FAIL: %s\n", what);
     }
}

/* ------------------------------------------------------------------------ *
 * The stand-in SDK.                                                        *
 * ------------------------------------------------------------------------ */

HMCVFILE glomb;
INT margc;
CHAR *margv[64];
INT usrnum;
static struct usracc sysop = { "Sysop" };
struct usracc *usaptr = &sysop;

CHAR input[256];                     /* the input line, split as parsin()    */
static INT inlen;

static void
cmd(const char *line)                /* a user types a line                  */
{
     INT i;

     strcpy(input, line);
     inlen = (INT)strlen(input);
     margc = 0;
     for (i = 0 ; i < inlen ; i++) {
          if (input[i] == ' ') {
               input[i] = '\0';
          }
          else if (i == 0 || input[i - 1] == '\0') {
               margv[margc++] = &input[i];
          }
     }
}

VOID
rstrin(VOID)                         /* put the spaces back                  */
{
     INT i;

     for (i = 0 ; i < inlen ; i++) {
          if (input[i] == '\0') {
               input[i] = ' ';
          }
     }
}

INT
sameas(const CHAR *a, const CHAR *b)
{
     return _stricmp(a, b) == 0;
}

CHAR *
stzcpy(CHAR *dst, const CHAR *src, INT len)
{
     strncpy(dst, src, len - 1);
     dst[len - 1] = '\0';
     return dst;
}

VOID
setmem(VOID *p, UINT n, CHAR c)
{
     memset(p, c, n);
}

static CHAR audit[400][160];
static INT naudit;

VOID
shocst(const CHAR *summary, const CHAR *fmt, ...)
{
     va_list ap;
     CHAR detail[128];

     va_start(ap, fmt);
     vsprintf(detail, fmt, ap);
     va_end(ap);
     if (naudit < 400) {
          sprintf(audit[naudit++], "%s|%s", summary, detail);
     }
}

static int
audited(const char *text)            /* was text in any Audit Trail entry?   */
{
     INT i;

     for (i = 0 ; i < naudit ; i++) {
          if (strstr(audit[i], text) != NULL) {
               return 1;
          }
     }
     return 0;
}

/* In-memory Btrieve file, one key (a zero-terminated string at offset 0). */
#define NREC 400
static CHAR recs[NREC][sizeof(struct gsrec)];
static INT nrecs, recpos = -1;
static int dbexists;
static int keyrefused;               /* lookups given a read-only key        */

GBOOL
isfile(const CHAR *name)
{
     if (sameas(name, GS_FILE)) {
          return dbexists;
     }
     return GetFileAttributesA(name) != INVALID_FILE_ATTRIBUTES;
}

VOID
dfaCreateSpec(const CHAR *name, GBOOL overwrite, size_t reclen,
              size_t pagesize, INT flags, INT nprealloc, INT nkeys,
              struct dfaKeySpec *keys, const CHAR *alt)
{
     (void)name; (void)overwrite; (void)pagesize; (void)flags;
     (void)nprealloc; (void)alt;
     check(reclen == sizeof(struct gsrec), "file created with the record size");
     check(nkeys == 1 && keys->nSegments == 1 && keys->segs[0].position == 0
           && keys->segs[0].length == GS_NAMSIZ
           && keys->segs[0].type == DFAST_ZSTRING, "file created with the name key");
     dbexists = 1;
     nrecs = 0;
}

DFAFILE *
dfaOpen(const CHAR *name, UINT reclen, const CHAR *owner)
{
     static DFAFILE f;

     (void)owner;
     check(sameas(name, GS_FILE) && reclen == sizeof(struct gsrec),
           "settings file opened with the record size");
     return &f;
}

VOID dfaClose(DFAFILE *f) { (void)f; }

/* The generic user database (GLOCLOCK.C): its lookups find nothing in the */
/* stand-in file, and its writes are dropped; the clock tests use the     */
/* per-channel choice.                                                     */
static DFAFILE genfile;
DFAFILE *genbb = &genfile;
VOID dfaUpdateV(VOID *rec, UINT len) { (void)rec; (void)len; }
VOID dfaInsertV(VOID *rec, UINT len) { (void)rec; (void)len; }
const CHAR *nctime(USHORT t) { (void)t; return "nctime"; }
VOID dfaSetBlk(DFAFILE *f) { (void)f; }
VOID dfaRstBlk(VOID) { }

static int
writable(const VOID *p)              /* the engine's goodblk() test          */
{
     MEMORY_BASIC_INFORMATION mbi;

     if (VirtualQuery(p, &mbi, sizeof(mbi)) == 0) {
          return 0;
     }
     return (mbi.Protect & (PAGE_READWRITE | PAGE_WRITECOPY
                            | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) != 0;
}

GBOOL
dfaAcqEQ(VOID *rec, const VOID *key, INT keynum)
{
     CHAR k[GS_NAMSIZ];
     INT i;

     (void)keynum;
     if (writable(key)) {
          stzcpy(k, (const CHAR *)key, GS_NAMSIZ);
     }
     else {
          k[0] = '\0';               /* the engine copies nothing            */
          keyrefused++;
     }
     for (i = 0 ; i < nrecs ; i++) {
          if (strcmp(recs[i], k) == 0) {
               recpos = i;
               if (rec != NULL) {
                    memcpy(rec, recs[i], sizeof(struct gsrec));
               }
               return TRUE;
          }
     }
     return FALSE;
}

VOID
dfaUpdate(VOID *rec)
{
     check(recpos >= 0 && strcmp(recs[recpos], (CHAR *)rec) == 0,
           "dfaUpdate on the record just found");
     memcpy(recs[recpos], rec, sizeof(struct gsrec));
}

VOID
dfaInsert(VOID *rec)
{
     INT i;

     for (i = 0 ; i < nrecs ; i++) {
          if (strcmp(recs[i], (CHAR *)rec) == 0) {
               check(0, "dfaInsert of a key already in the file");
               return;
          }
     }
     memcpy(recs[nrecs++], rec, sizeof(struct gsrec));
}

static INT msgs[400];                /* messages printed since cmd()         */
static INT nmsgs;
static CHAR tv[TV_COUNT_OF][80];
static CHAR saved[TV_COUNT_OF][80];  /* the variables as GSSAVED printed     */
                                     /*   them (a page redraw follows)       */

VOID
prfmsg(INT msgnum, ...)
{
     if (nmsgs < 400) {
          msgs[nmsgs++] = msgnum;
     }
     if (msgnum == GSSAVED || msgnum == GSPFXLEN) {
          memcpy(saved, tv, sizeof(saved));
     }
}

VOID outprf(INT unum) { (void)unum; }

CHAR *
getmsg(INT msgnum)
{
     static CHAR blank[] = "(blank)";

     check(msgnum == GSBLANK, "getmsg only for GSBLANK");
     return blank;
}

CHAR *
spr(const CHAR *fmt, ...)
{
     static CHAR buf[4][256];
     static INT n;
     va_list ap;

     n = (n + 1) % 4;
     va_start(ap, fmt);
     vsprintf(buf[n], fmt, ap);
     va_end(ap);
     return buf[n];
}

VOID tv_set(INT which, const CHAR *value) { stzcpy(tv[which], value, 80); }
VOID tv_setn(INT which, LONG value) { sprintf(tv[which], "%ld", value); }

static int applied;
VOID applysettings(VOID) { applied++; }
VOID setmbk(HMCVFILE mb) { (void)mb; }
VOID doprintoutput(VOID) { }

/* ------------------------------------------------------------------------ *
 * Helpers.                                                                 *
 * ------------------------------------------------------------------------ */

static void
run(const char *line)                /* type the =gset command               */
{
     nmsgs = 0;
     cmd(line);
     gs_command();
}

static INT
type(const char *line)               /* type a line: the menu's answer       */
{
     nmsgs = 0;
     cmd(line);
     return gs_input();
}

static int
printed(INT msgnum)                  /* was msgnum printed by the last run()?*/
{
     INT i;

     for (i = 0 ; i < nmsgs ; i++) {
          if (msgs[i] == msgnum) {
               return 1;
          }
     }
     return 0;
}

static int
count(INT msgnum)
{
     INT i, n = 0;

     for (i = 0 ; i < nmsgs ; i++) {
          n += (msgs[i] == msgnum);
     }
     return n;
}

static void
restart(void)                        /* the board restarts                   */
{
     naudit = 0;
     applied = 0;
     gs_close();
     gs_open();
}

static int
replace(char *buf, const char *old, const char *new_)  /* exactly once      */
{
     char *p = strstr(buf, old);
     size_t ol = strlen(old), nl = strlen(new_);

     if (p == NULL || strstr(p + 1, old) != NULL) {
          return 0;
     }
     memmove(p + nl, p + ol, strlen(p + ol) + 1);
     memcpy(p, new_, nl);
     return 1;
}

/* ------------------------------------------------------------------------ *
 * The tests.                                                               *
 * ------------------------------------------------------------------------ */

static void
test_new_install(void)
{
     INT i, all = 1;

     dbexists = 0;
     naudit = 0;
     gs_open();
     check(audited("GLOBALS SETTINGS CREATED"), "new install: CREATED in the Audit Trail");
     check(nrecs == GS_COUNT_OF, "new install: one row per setting");
     check(applied == 1, "new install: settings applied");
     check(strcmp(GSSTR(HLPKEY), "DEMO") == 0, "default HLPKEY = DEMO");
     check(strcmp(GSSTR(CHATSTG), "=cc") == 0, "default CHATSTG = =cc");
     check(strcmp(GSSTR(CMDPFX), "=") == 0 && strcmp(GSSTR(GTLSTG), "==") == 0
           && strcmp(GSSTR(MACCM0), "=m0") == 0, "default prefix =; == and macros");
     check(GSNUM(NUMCHAT) == 10000, "default NUMCHAT = 10000");
     check(GSNUM(PGALL) == 1, "default PGALL = YES");
     check(GSNUM(MXPST) == 3600, "default MXPST = 3600");
     check(GSCHR(TLCCHR) == '=' && GSCHR(ANSCHR) == '\\', "default characters");
     check(strcmp(GSSTR(GSETSTG), "=gset") == 0 && strcmp(GSSTR(GSETKEY), "SYSOP") == 0,
           "default editor command and key");

     /* Read the defaults back: each must pass the checks (no RESET), and   */
     /* each row must be found by its key (no ADDED, no read-only keys).    */
     keyrefused = 0;
     restart();
     check(keyrefused == 0, "restart: every lookup key is in writable memory");
     check(!audited("RESET") && !audited("ADDED") && !audited("CREATED"),
           "restart: every default is allowed and every row is found");
     check(nrecs == GS_COUNT_OF, "restart: no rows added");
     for (i = 0 ; i < GS_COUNT_OF ; i++) {
          all &= (strlen(recs[i]) > 0 && strlen(recs[i]) < GS_NAMSIZ);
     }
     check(all, "every setting name fits the key");
}

static void
test_editor(void)
{
     INT before;

     run("=gset");
     check(printed(GSMENU) && count(GSGRP) == GSG_COUNT_OF - 1 && printed(GSMPRMT),
           "=gset opens the menu with every group");
     check(type("") == -1 && printed(GSMENU) && count(GSGRP) == GSG_COUNT_OF - 1
           && printed(GSMPRMT), "a blank line in the menu shows the whole menu again");
     check(atoi(tv[TV_ITEM]) == GSG_COUNT_OF - 1, "the prompt's range is every group");
     check(type("99") == -1 && printed(GSNOPICK) && printed(GSMENU)
           && count(GSGRP) == GSG_COUNT_OF - 1, "no group 99, and the whole menu again");
     check(type("2 3") == -1 && printed(GSNOPICK), "one choice at a time");

     type("2");
     check(printed(GSGHDR) && count(GSROW) == 15 && printed(GSGPRMT)
           && strcmp(tv[TV_PAGE], "1") == 0 && strcmp(tv[TV_PAGES], "4") == 0,
           "group 2 shows 15 of its 59 settings, page 1 of 4");
     check(type("") == -1 && printed(GSGHDR) && count(GSROW) == 15 && printed(GSGPRMT),
           "a blank line on a page shows the whole page again");
     check(type("999") == -1 && printed(GSNOPICK) && printed(GSGHDR) && count(GSROW) == 15
           && strcmp(tv[TV_PAGE], "1") == 0, "no item 999, and the same page again");
     type("n");
     check(strcmp(tv[TV_PAGE], "2") == 0 && count(GSROW) == 15, "N: the next page");
     type("P");
     type("p");
     check(strcmp(tv[TV_PAGE], "4") == 0 && count(GSROW) == 14,
           "P from page 1 goes round to the last page, which has 14");
     type("n");
     check(strcmp(tv[TV_PAGE], "1") == 0, "N from the last page goes round to 1");

     type("30");
     check(printed(GSINFO) && printed(GSVPRMT) && strcmp(tv[TV_SETNAME], "CHATSTG") == 0,
           "item 30 of group 2 is CHATSTG, picked from any page");
     before = applied;
     type("=?");
     check(printed(GSDUP) && strcmp(tv[TV_OTHER], "HLPSTG") == 0 && printed(GSGHDR),
           "a command string in use is refused, and the page comes back");
     type("30");
     type("=X");
     check(printed(GSDUP) && strcmp(tv[TV_OTHER], "OFFSTG") == 0,
           "the duplicate check ignores case");
     check(strcmp(GSSTR(CHATSTG), "=cc") == 0 && applied == before,
           "a refused change changes nothing");
     type("30");
     type("=chat");
     check(printed(GSSAVED) && strcmp(GSSTR(CHATSTG), "=chat") == 0 && applied == before + 1,
           "CHATSTG changed, and applied at once");
     check(audited("GLOBALS SETTING CHANGED|Sysop: CHATSTG = =chat"),
           "a change goes in the Audit Trail");
     type("30");
     type("");
     check(printed(GSGHDR) && strcmp(GSSTR(CHATSTG), "=chat") == 0, "Enter keeps the value");
     type("30");
     type("-");
     check(printed(GSBAD), "a command string cannot be blanked");

     type("x");
     check(printed(GSMENU), "X on a group page goes back to the menu");
     type("4");
     type("1");
     type("set");
     check(printed(GSDUP) && strcmp(tv[TV_OTHER], "SETSTG") == 0,
           "chat channel words must differ from each other");
     type("1");
     type("=x");
     check(printed(GSSAVED), "a chat channel word may equal a command string");
     type("1");
     type("echo");
     type("12");
     check(strcmp(tv[TV_SETNAME], "NUMCHAT") == 0, "item 12 of group 4 is NUMCHAT");
     type("50");
     check(printed(GSBAD) && GSNUM(NUMCHAT) == 10000, "a number out of range is refused");
     type("12");
     type("12x");
     check(printed(GSBAD), "a number with junk is refused");
     type("12");
     type("2000");
     check(printed(GSSAVED) && GSNUM(NUMCHAT) == 2000, "a number in range");

     type("x");
     type("1");
     type("67");
     check(strcmp(tv[TV_SETNAME], "GSETKEY") == 0, "item 67 of group 1 is GSETKEY");
     type("-");
     check(printed(GSBAD) && strcmp(GSSTR(GSETKEY), "SYSOP") == 0,
           "the editor's own key cannot be blanked");
     type("1");
     type("-");
     check(printed(GSSAVED) && GSSTR(HLPKEY)[0] == '\0', "- clears an ordinary key");
     type("1");
     type("TWO WORDS");
     check(printed(GSBAD), "a key has no spaces");

     type("x");
     type("5");
     type("11");
     check(strcmp(tv[TV_SETNAME], "MACMC0") == 0, "item 11 of group 5 is MACMC0");
     type("   hello  there, world and more  ");
     check(printed(GSSAVED) && strcmp(GSSTR(MACMC0), "hello  there, world and more") == 0,
           "text keeps its inner spaces; outer spaces are trimmed");
     type("11");
     type("0123456789012345678901234567890123456789");
     check(printed(GSBAD), "text longer than its maximum is refused");
     type("11");
     type("0123456789012345678901234567890123456");
     check(printed(GSSAVED), "text of the maximum length");
     type("11");
     check(strcmp(tv[TV_SETSHORT], "0123456789012345678901234567~") == 0,
           "a long value is cut short in lists");
     type("");
     type("12");
     check(strcmp(tv[TV_SETVAL], "(blank)") == 0, "an empty value shows as (blank)");
     type("");

     type("x");
     type("10");
     type("1");
     type("ab");
     check(printed(GSBAD), "a character setting takes one character");
     type("1");
     type("-");
     check(printed(GSSAVED) && GSCHR(TLCCHR) == '-', "- is a character, not a clear");
     type("1");
     type("=");
     type("x");

     /* The values survive a restart. */
     restart();
     check(strcmp(GSSTR(CHATSTG), "=chat") == 0 && GSNUM(NUMCHAT) == 2000
           && GSSTR(HLPKEY)[0] == '\0'
           && strcmp(GSSTR(MACMC0), "0123456789012345678901234567890123456") == 0,
           "changes survive a restart");
     check(!audited("RESET") && !audited("ADDED"), "restart after changes is quiet");

     /* Group resets. */
     type("2");
     type("d");
     check(printed(GSRSTQ) && strcmp(tv[TV_COUNT], "59") == 0, "D asks first");
     type("no");
     check(printed(GSRSTN) && strcmp(GSSTR(CHATSTG), "=chat") == 0, "only YES resets");
     type("d");
     type("yes");
     check(printed(GSRSTD) && strcmp(GSSTR(CHATSTG), "=cc") == 0 && printed(GSGHDR),
           "D then YES resets the group and shows it again");
     check(GSNUM(NUMCHAT) == 2000, "a group reset leaves other groups alone");

     /* The command prefix. */
     type("1");
     check(strcmp(tv[TV_SETNAME], "CMDPFX") == 0, "item 1 of group 2 is the prefix");
     type("a");
     check(printed(GSBAD), "the prefix must be punctuation");
     type("1");
     type("/a");
     check(printed(GSBAD) && strcmp(GSSTR(CMDPFX), "=") == 0,
           "both characters of the prefix must be punctuation");
     type("1");
     type("///");
     check(printed(GSBAD), "the prefix is at most two characters");
     type("1");
     type("/");
     check(printed(GSPFXQ) && strcmp(tv[TV_COUNT], "56") == 0,
           "a new prefix says how many commands change, and asks");
     type("n");
     check(printed(GSRSTN) && strcmp(GSSTR(CHATSTG), "=cc") == 0
           && strcmp(GSSTR(CMDPFX), "=") == 0,
           "N leaves the prefix and the commands alone");
     before = applied;
     type("1");
     type("/");
     type("Y");
     check(printed(GSPFXD) && strcmp(GSSTR(CMDPFX), "/") == 0 && applied == before + 1,
           "Y changes the prefix");
     check(strcmp(GSSTR(CHATSTG), "/cc") == 0 && strcmp(GSSTR(GTLSTG), "==") == 0
           && strcmp(GSSTR(WHOSTG), "#") == 0 && strcmp(GSSTR(MACCM0), "/m0") == 0,
           "commands follow; == (read telegrams), # and others stay");
     check(GSCHR(TLCCHR) == '=' && strcmp(GSSTR(ECHSTG), "echo") == 0,
           "the telegram character and the chat words do not follow");
     check(audited("GLOBALS PREFIX CHANGED|Sysop: = to /, 56 command strings"),
           "the prefix change is in the Audit Trail");
     type("22");
     check(strcmp(tv[TV_SETNAME], "ZAPSTG") == 0, "item 22 of group 2 is ZAPSTG");
     type("!cc");
     check(printed(GSSAVED), "one command changed on its own");
     type("1");
     type("!");
     check(printed(GSPFXDUP) && strcmp(GSSTR(CMDPFX), "/") == 0
           && strcmp(GSSTR(CHATSTG), "/cc") == 0,
           "a prefix that would make two commands the same is refused");
     type("22");
     type("/zap");
     type("30");
     type("/abcdefghi");
     check(printed(GSSAVED) && strcmp(GSSTR(CHATSTG), "/abcdefghi") == 0,
           "a command of the maximum length");
     type("1");
     type("//");
     check(printed(GSPFXLEN) && strcmp(GSSTR(CMDPFX), "/") == 0
           && strcmp(GSSTR(CHATSTG), "/abcdefghi") == 0
           && strcmp(saved[TV_SETNAME], "CHATSTG") == 0,
           "a prefix that would make a command too long is refused");
     type("30");
     type("/cc");
     type("1");
     type("//");
     type("Y");
     check(strcmp(GSSTR(CMDPFX), "//") == 0 && strcmp(GSSTR(CHATSTG), "//cc") == 0
           && strcmp(GSSTR(GTLSTG), "==") == 0 && strcmp(GSSTR(MACCM0), "//m0") == 0,
           "a two-character prefix: / to // changes the commands once");
     type("x");
     type("5");
     type("1");
     type("//zz");
     check(printed(GSSAVED) && strcmp(GSSTR(MACCM0), "//zz") == 0, "a macro command changed");
     type("d");
     type("YES");
     check(strcmp(GSSTR(MACCM0), "//m0") == 0,
           "a group reset gives the defaults with the board's prefix");
     type("x");
     type("2");
     type("1");
     type("=");
     type("Y");
     check(strcmp(GSSTR(CMDPFX), "=") == 0 && strcmp(GSSTR(CHATSTG), "=cc") == 0
           && strcmp(GSSTR(GTLSTG), "==") == 0 && strcmp(GSSTR(ZAPSTG), "=zap") == 0
           && strcmp(GSSTR(MACCM0), "=m0") == 0,
           "// to =: back to the 3.43 strings");
     type("1");
     type("//");
     type("Y");
     check(strcmp(GSSTR(CMDPFX), "//") == 0 && strcmp(GSSTR(CHATSTG), "//cc") == 0
           && strcmp(GSSTR(GTLSTG), "==") == 0 && strcmp(GSSTR(MACCM0), "//m0") == 0,
           "= to //: == (read telegrams) stays with the telegram character");
     type("x");

     /* Reset everything. */
     type("r");
     check(printed(GSRSTQ) && strcmp(tv[TV_COUNT], "265") == 0, "R asks first");
     type("YES");
     check(printed(GSRSTD) && printed(GSMENU) && strcmp(GSSTR(CMDPFX), "=") == 0
           && strcmp(GSSTR(CHATSTG), "=cc") == 0 && GSNUM(NUMCHAT) == 10000,
           "R then YES resets everything");

     /* Leaving. */
     check(type("x") == 1 && printed(GSBYE), "X leaves; the module prompts again");
     check(type("2") == 0, "after X, lines are not the menu's");
     run("=gset");
     gs_userreset(usrnum);
     check(type("2") == 0, "log-on or hang-up takes the user out of the menu");
}

static void
test_onoff(void)
{
     INT before;

     run("=gset");
     type("3");
     check(printed(GSGHDR) && strcmp(tv[TV_GROUP], "Commands on/off") == 0
           && strcmp(tv[TV_PAGES], "4") == 0, "group 3 is Commands on/off, 57 on 4 pages");
     check(gs_cmdon(GS_CHATSTG) && strcmp(gs_cmd(GS_CHATSTG), "=cc") == 0,
           "a command is ON by default");
     before = applied;
     type("29");                     /* ONCHAT: no question, it flips        */
     check(printed(GSSAVED) && !printed(GSVPRMT) && strcmp(saved[TV_SETNAME], "ONCHAT") == 0
           && strcmp(saved[TV_SETVAL], "OFF") == 0 && applied == before + 1
           && printed(GSGHDR), "picking an ON/OFF setting turns it off at once, and applies");
     check(strcmp(saved[TV_SETDESC], "Chat channels") == 0
           && strcmp(saved[TV_SETSHORT], "OFF  =cc") == 0,
           "an ON/OFF setting is labelled with its command's label and string");
     check(audited("GLOBALS SETTING CHANGED|Sysop: ONCHAT = OFF"),
           "turning a command off is in the Audit Trail");
     check(!gs_cmdon(GS_CHATSTG) && gs_cmd(GS_CHATSTG)[0] == '\0',
           "an OFF command reads as \"\", which no typed word matches");
     check(strcmp(GSSTR(CHATSTG), "=cc") == 0, "an OFF command keeps its string");
     check(gs_cmdon(GS_HLPSTG) && gs_cmdon(GS_GSETSTG) && gs_cmdon(-1),
           "other commands, =gset and \"no command\" stay on");
     type("x");
     type("2");
     type("30");
     check(strcmp(tv[TV_SETSHORT], "=cc (off)") == 0 && strcmp(tv[TV_SETVAL], "=cc") == 0,
           "the command strings page marks an OFF command");
     type("");
     type("22");
     type("=cc");
     check(printed(GSDUP) && strcmp(tv[TV_OTHER], "CHATSTG") == 0,
           "an OFF command's string still cannot be given to another");
     type("x");

     restart();
     check(!gs_cmdon(GS_CHATSTG), "OFF survives a restart");
     run("=gset");
     type("3");
     type("29");
     check(gs_cmdon(GS_CHATSTG) && strcmp(gs_cmd(GS_CHATSTG), "=cc") == 0,
           "picking it again turns it back on, string unchanged");
     type("29");
     type("d");
     type("YES");
     check(gs_cmdon(GS_CHATSTG), "D resets every command to ON");
     type("x");
     type("x");
}

static void
test_clock(void)
{
     CHAR buf[16];

     ck_fmt12((USHORT)((0 << 11) | (0 << 5)), buf);
     check(strcmp(buf, "12:00 AM") == 0, "clock: midnight is 12:00 AM");
     ck_fmt12((USHORT)((9 << 11) | (7 << 5) | 15), buf);
     check(strcmp(buf, "9:07 AM") == 0, "clock: 9:07:30 is 9:07 AM");
     ck_fmt12((USHORT)((12 << 11) | (5 << 5)), buf);
     check(strcmp(buf, "12:05 PM") == 0, "clock: noon is 12:05 PM");
     ck_fmt12((USHORT)((16 << 11) | (45 << 5)), buf);
     check(strcmp(buf, "4:45 PM") == 0, "clock: 16:45 is 4:45 PM");
     ck_fmt12((USHORT)((23 << 11) | (59 << 5) | 29), buf);
     check(strcmp(buf, "11:59 PM") == 0, "clock: 23:59:58 is 11:59 PM");

     ck_reset(usrnum);
     check(!ck_is12(usrnum) && strcmp(ck_time(usrnum, (USHORT)(16 << 11)), "nctime") == 0,
           "clock: by default, 24-hour (the engine's own time)");
     run("=gset");
     type("6");                      /* New-user defaults                    */
     type("13");
     check(strcmp(tv[TV_SETNAME], "DEF12H") == 0, "item 13 of group 6 is DEF12H");
     type("YES");
     check(ck_is12(usrnum) && strcmp(ck_time(usrnum, (USHORT)(16 << 11 | 45 << 5)), "4:45 PM") == 0,
           "clock: the board default reaches a user who has not chosen, at once");
     ck_set(CK_24);
     check(!ck_is12(usrnum), "clock: the user's own choice wins over the default");
     ck_reset(usrnum);
     check(ck_is12(usrnum), "clock: hang-up: back to the board default");
     check(ck_is12(-1) && ck_is12(MAXTERMS), "clock: an unknown channel gets the default");
     type("13");
     type("NO");
     type("x");
     type("x");
}

static void
test_import(const char *v3)
{
     FILE *fp;
     long len;
     char *buf;

     fp = fopen(v3, "rb");
     check(fp != NULL, "3.43 ELWGLOB.MSG test file found");
     if (fp == NULL) {
          return;
     }
     fseek(fp, 0, SEEK_END);
     len = ftell(fp);
     fseek(fp, 0, SEEK_SET);
     buf = (char *)malloc(len + 200);
     buf[fread(buf, 1, len, fp)] = '\0';
     fclose(fp);

     check(replace(buf, "HLPKEY {DEMO}", "HLPKEY {SYSOP}")
        && replace(buf, "CHATSTG {=cc}", "CHATSTG {=chat}")
        && replace(buf, "DEFCC {Default chat channel for new users? 0}",
                        "DEFCC {Default chat channel for new users? 5}")
        && replace(buf, "PGALL {Page every Sysop on line with console page? YES}",
                        "PGALL {Page every Sysop on line with console page? NO}")
        && replace(buf, "TLCCHR { =}", "TLCCHR { /}")
        && replace(buf, "NUMCHAT {How many chat channels in use? 10000}",
                        "NUMCHAT {How many chat channels in use? 50}")
        && replace(buf, "MACMC0 {}", "MACMC0 {say ~}hi}")
        && replace(buf, "CLOKKEY {SUPER}", "CLOKKEY {NOBODY}"),
           "edits made to the 3.43 file");
     remove(GS_V3DONE);
     fp = fopen(GS_V3FILE, "wb");
     fwrite(buf, 1, strlen(buf), fp);
     fclose(fp);
     free(buf);

     dbexists = 0;
     naudit = 0;
     gs_open();
     check(strcmp(GSSTR(HLPKEY), "SYSOP") == 0, "import: a key");
     check(strcmp(GSSTR(CHATSTG), "=chat") == 0, "import: a command string");
     check(GSNUM(DEFCC) == 5, "import: a number after its prompt");
     check(GSNUM(PGALL) == 0 && strcmp(GSSTR(PGALL), "NO") == 0, "import: a YES/NO");
     check(GSCHR(TLCCHR) == '/', "import: a character");
     check(strcmp(GSSTR(MACMC0), "say }hi") == 0, "import: ~} is a brace");
     check(GSNUM(NUMCHAT) == 10000, "import: a value out of range keeps the default");
     check(strcmp(GSSTR(CMDPFX), "=") == 0, "import: the prefix is the one the commands use");
     check(strcmp(GSSTR(CFGSTG), "=config") == 0 && strcmp(GSSTR(OFFSTG), "=x") == 0
           && strcmp(GSSTR(GTLSTG), "==") == 0,
           "import: commands 3.43 did not have take the board's prefix");
     check(GSNUM(MXPST) == 3600 && strcmp(GSSTR(GSETSTG), "=gset") == 0,
           "import: everything else is the default");
     check(audited("GLOBALS IMPORT: VALUE REFUSED|NUMCHAT kept at its default"),
           "import: the refused value is in the Audit Trail");
     check(audited("GLOBALS IMPORT: SETTING|CHATSTG = =chat"),
           "import: each changed value is in the Audit Trail");
     check(audited("GLOBALS SETTINGS IMPORTED|From ELWGLOB.V3: 6 changed, 1 refused"),
           "import: summary in the Audit Trail");
     check(!audited("CREATED"), "import: no CREATED line");
     check(!isfile(GS_V3FILE) && isfile(GS_V3DONE), "import: the file is renamed");

     restart();
     check(strcmp(GSSTR(CHATSTG), "=chat") == 0 && !audited("IMPORTED"),
           "import: happens once; the values are saved");
     remove(GS_V3DONE);
}

static void
test_bad_rows(void)
{
     INT i;

     /* A row whose value is no longer allowed, and a row missing (a file   */
     /* from before a setting was added).                                   */
     for (i = 0 ; i < nrecs ; i++) {
          if (strcmp(recs[i], "NUMCHAT") == 0) {
               strcpy(recs[i] + GS_NAMSIZ, "5");
          }
     }
     for (i = 0 ; i < nrecs ; i++) {
          if (strcmp(recs[i], "GSETSTG") == 0) {
               memmove(recs[i], recs[i + 1], (nrecs - i - 1) * sizeof(recs[0]));
               nrecs--;
               break;
          }
     }
     restart();
     check(GSNUM(NUMCHAT) == 10000 && audited("GLOBALS SETTING RESET|NUMCHAT"),
           "a stored value no longer allowed goes back to its default");
     check(strcmp(GSSTR(GSETSTG), "=gset") == 0
           && audited("GLOBALS SETTINGS ADDED|1 new settings"),
           "a setting missing from the file is added with its default");
     check(nrecs == GS_COUNT_OF, "the missing row is written");
}

static int
samefile(const char *a, const char *b)
{
     FILE *fa = fopen(a, "rb"), *fb = fopen(b, "rb");
     int ca, cb, same = (fa != NULL && fb != NULL);

     while (same) {
          ca = fgetc(fa);
          cb = fgetc(fb);
          if (ca != cb) {
               same = 0;
          }
          if (ca == EOF) {
               break;
          }
     }
     if (fa != NULL) {
          fclose(fa);
     }
     if (fb != NULL) {
          fclose(fb);
     }
     return same;
}

static void
writefile(const char *name, int size, int seed)
{
     FILE *fp = fopen(name, "wb");
     int i;

     for (i = 0 ; i < size ; i++) {
          fputc((i * 7 + seed) & 0xFF, fp);  /* binary, NULs included       */
     }
     fclose(fp);
}

static void
test_data_files(void)
{
     remove("ELWGLOBX.DAT");
     remove("ELWGLOBY.DAT");
     remove("ELWGLOBZ.DAT");
     writefile("ELWGLOBX.VIR", 10000, 1);    /* more than one 4K buffer      */
     naudit = 0;
     makedat("ELWGLOBX");
     check(samefile("ELWGLOBX.VIR", "ELWGLOBX.DAT"),
           "data file: a missing .DAT is created from its .VIR, byte for byte");
     check(audited("GLOBALS DATA FILE CREATED|ELWGLOBX.DAT from ELWGLOBX.VIR"),
           "data file: creation is in the Audit Trail");

     writefile("ELWGLOBX.DAT", 500, 2);      /* a board's live data          */
     naudit = 0;
     makedat("ELWGLOBX");
     check(!samefile("ELWGLOBX.VIR", "ELWGLOBX.DAT") && naudit == 0,
           "data file: an existing .DAT is never touched");

     CreateDirectoryA("GCVIRDAT", NULL);
     writefile("GCVIRDAT\\ELWGLOBY.VIR", 3000, 3);
     makedat("ELWGLOBY");
     check(samefile("GCVIRDAT\\ELWGLOBY.VIR", "ELWGLOBY.DAT"),
           "data file: the template may be in GCVIRDAT");

     naudit = 0;
     makedat("ELWGLOBZ");
     check(!isfile("ELWGLOBZ.DAT") && audited("GLOBALS DATA FILE MISSING"),
           "data file: no template - nothing created, and it says so");
}

int
main(int argc, char **argv)
{
     if (argc < 3) {
          printf("usage: TEST_GLOSET <3.43 ELWGLOB.MSG> <scratch folder>\n");
          return 2;
     }
     if (!SetCurrentDirectoryA(argv[2])) {
          printf("cannot use folder %s\n", argv[2]);
          return 2;
     }
     remove(GS_V3FILE);
     test_new_install();
     test_editor();
     test_onoff();
     test_clock();
     test_import(argv[1]);
     test_bad_rows();
     test_data_files();
     printf("TEST_GLOSET: %d checks, %d failed\n", checks, fails);
     return fails ? 1 : 0;
}
