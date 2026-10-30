/* Build 26.10.29.3 07:15PM */
/*****************************************************************************
 *   GLOSET.C                                   Globals - Live settings     *
 *                                                                           *
 *   Copyright (C) 2026 Elwynor Technologies.                                *
 *   Created by Mark Laudenbach.                                             *
 *                                                                           *
 *   Loads, checks and saves the settings (ELWGLOBO.DAT, one row per         *
 *   setting), imports a 3.43 board's option values once, and runs the      *
 *   online editor, a menu opened with the =gset global command.             *
 *                                                                           *
 *   Licensed under the GNU Affero General Public License v3.0.              *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "GLOBROUS.H"
#include "GLOSET.H"
#include "TVARS.H"

extern HMCVFILE glomb;               /* ELWGLOB.C                            */
extern VOID doprintoutput(VOID);     /* GLOBROUS.C                           */

struct gsdef {                       /* one setting                          */
     const CHAR *name;               /*   its name (the row key)             */
     INT group;                      /*   GSG_xxx                            */
     INT type;                       /*   GST_xxx                            */
     LONG min, max;                  /*   numbers: range; text: max length   */
     const CHAR *def;                /*   default, as text                   */
     const CHAR *label;              /*   what the editor shows              */
};

#define X(n, g, t, lo, hi, d, l) { #n, g, t, lo, hi, d, l },
static const struct gsdef gsdefs[GS_COUNT_OF] = { GLOSETS };
#undef X

static const CHAR *grptitle[GSG_COUNT_OF] = {
     "All settings",
     "Access keys",
     "Command strings",
     "Commands on/off",
     "Chat channels",
     "Default macros for new users",
     "New-user defaults",
     "Paging and beeps",
     "Timers (seconds)",
     "Credits and days",
     "General",
     "Module names"
};

CHAR gsval[GS_COUNT_OF][GS_VALSIZ];  /* live values, as text                 */
LONG gsnum[GS_COUNT_OF];             /* numbers; YES = 1, NO = 0             */
static DFAFILE *gsdat;               /* ELWGLOBO.DAT                         */

#define GSC_OK   0                   /* gs_check() results                   */
#define GSC_BAD  1                   /*   value not allowed                  */
#define GSC_DUP  2                   /*   command string already in use      */

/* ------------------------------------------------------------------------ *
 * Values.                                                                  *
 * ------------------------------------------------------------------------ */

static GBOOL
oneword(const CHAR *s, GBOOL blankok)          /* printable, no spaces       */
{
     if (*s == '\0') {
          return blankok;
     }
     for ( ; *s != '\0' ; s++) {
          if (!isgraph((UCHAR)*s)) {
               return FALSE;
          }
     }
     return TRUE;
}

static GBOOL
printable(const CHAR *s)             /* printable text, spaces allowed       */
{
     for ( ; *s != '\0' ; s++) {
          if (!isprint((UCHAR)*s)) {
               return FALSE;
          }
     }
     return TRUE;
}

static INT                           /*   GSC_xxx                            */
gs_check(                            /* is a value allowed for a setting?    */
INT idx,                             /*   setting                            */
const CHAR *text,                    /*   value as typed or stored           */
CHAR *out,                           /*   receives it as it will be stored   */
GBOOL dupcheck,                      /*   also refuse a command in use?      */
INT *clash)                          /*   receives the clashing setting      */
{
     const struct gsdef *d = &gsdefs[idx];
     LONG n;
     CHAR *end;
     INT i;

     if ((LONG)strlen(text) >= GS_VALSIZ) {
          return GSC_BAD;
     }
     switch (d->type) {
     case GST_NUM:
          n = strtol(text, &end, 10);
          if (*text == '\0' || *end != '\0' || n < d->min || n > d->max) {
               return GSC_BAD;
          }
          sprintf(out, "%ld", n);
          return GSC_OK;
     case GST_BOOL:
          if (sameas(text, "Y") || sameas(text, "YES")) {
               strcpy(out, "YES");
          }
          else if (sameas(text, "N") || sameas(text, "NO")) {
               strcpy(out, "NO");
          }
          else {
               return GSC_BAD;
          }
          return GSC_OK;
     case GST_ON:
          if (sameas(text, "ON") || sameas(text, "Y") || sameas(text, "YES")) {
               strcpy(out, "ON");
          }
          else if (sameas(text, "OFF") || sameas(text, "N") || sameas(text, "NO")) {
               strcpy(out, "OFF");
          }
          else {
               return GSC_BAD;
          }
          return GSC_OK;
     case GST_CHR:
          if (strlen(text) != 1 || !isgraph((UCHAR)text[0])) {
               return GSC_BAD;
          }
          strcpy(out, text);
          return GSC_OK;
     case GST_PFX:                   /* punctuation: a letter or digit here  */
          if (*text == '\0' || (LONG)strlen(text) > d->max  /* would      */
           || !ispunct((UCHAR)text[0])            /*   swallow ordinary words */
           || (text[1] != '\0' && !ispunct((UCHAR)text[1]))) {
               return GSC_BAD;
          }
          strcpy(out, text);
          return GSC_OK;
     case GST_KEY:
     case GST_KEYNB:
          if ((LONG)strlen(text) > d->max
           || !oneword(text, d->type == GST_KEY)) {
               return GSC_BAD;
          }
          strcpy(out, text);
          return GSC_OK;
     case GST_STR:
          if ((LONG)strlen(text) > d->max || !printable(text)) {
               return GSC_BAD;
          }
          strcpy(out, text);
          return GSC_OK;
     default:                        /* GST_CMD, GST_SUB, GST_WORD           */
          if ((LONG)strlen(text) > d->max || !oneword(text, FALSE)) {
               return GSC_BAD;
          }
          strcpy(out, text);
          if (dupcheck && d->type != GST_WORD) {
               /* Commands are matched with sameas(), so case does not    */
               /* make two of them different.  Chat channel words are    */
               /* matched after the chat command and only among          */
               /* themselves.                                            */
               for (i = 0 ; i < GS_COUNT_OF ; i++) {
                    if (i != idx && gsdefs[i].type == d->type
                     && sameas(gsval[i], text)) {
                         *clash = i;
                         return GSC_DUP;
                    }
               }
          }
          return GSC_OK;
     }
}

static VOID
gs_store(INT idx, const CHAR *value) /* set a checked value in memory        */
{
     stzcpy(gsval[idx], value, GS_VALSIZ);
     switch (gsdefs[idx].type) {
     case GST_NUM:
          gsnum[idx] = atol(value);
          break;
     case GST_BOOL:
          gsnum[idx] = sameas(value, "YES");
          break;
     case GST_ON:
          gsnum[idx] = sameas(value, "ON");
          break;
     default:
          gsnum[idx] = 0;
          break;
     }
}

static VOID
gs_save(INT idx)                     /* write one setting's row              */
{
     struct gsrec rec;

     /* rec.name is the lookup key, so it must be writable memory: the    */
     /* engine copies a key only from pages it can write (WG33 DFAAPI.C   */
     /* dfaAcqLock -> ISGOODPT.C goodblk), and a string constant gives an */
     /* empty key - the Farwest Trivia / Auto Validator bug of 2026-10-07. */
     setmem(&rec, sizeof(rec), 0);
     stzcpy(rec.name, gsdefs[idx].name, GS_NAMSIZ);
     dfaSetBlk(gsdat);
     if (dfaAcqEQ(NULL, rec.name, 0)) {
          stzcpy(rec.value, gsval[idx], GS_VALSIZ);
          dfaUpdate(&rec);
     }
     else {
          stzcpy(rec.value, gsval[idx], GS_VALSIZ);
          dfaInsert(&rec);
     }
     dfaRstBlk();
}

static VOID gs_followpfx(INT i);

static VOID
gs_default(INT idx)                  /* one setting to its default, saved    */
{
     /* A command string's default takes the board's prefix: CMDPFX comes */
     /* before every command in GLOSETS.H, so it is already loaded (or    */
     /* reset with them).                                                 */
     gs_store(idx, gsdefs[idx].def);
     gs_followpfx(idx);
     gs_save(idx);
}

/* ------------------------------------------------------------------------ *
 * Commands on/off.  Each command string has an ON/OFF setting (GST_ON)     *
 * whose "min" names it.  An OFF command reads as "", which no typed word   *
 * matches, so Globals passes the line on to the user's module.  Its string *
 * is kept, so turning it back on needs no retyping, and it still counts    *
 * when a new string is checked for clashes.                                *
 * ------------------------------------------------------------------------ */

static SHORT gsonof[GS_COUNT_OF];    /* command -> its ON/OFF setting, or -1 */
static GBOOL gsonmap = FALSE;

static VOID
gs_mapon(VOID)
{
     INT i;

     for (i = 0 ; i < GS_COUNT_OF ; i++) {
          gsonof[i] = -1;
     }
     for (i = 0 ; i < GS_COUNT_OF ; i++) {
          if (gsdefs[i].type == GST_ON) {
               gsonof[gsdefs[i].min] = (SHORT)i;
          }
     }
     gsonmap = TRUE;
}

GBOOL
gs_cmdon(INT idx)                    /* is command setting idx turned on?    */
{
     if (!gsonmap) {
          gs_mapon();
     }
     return idx < 0 || idx >= GS_COUNT_OF || gsonof[idx] < 0
         || gsnum[gsonof[idx]] != 0;
}

CHAR *
gs_cmd(INT idx)                      /* its string, or "" (never matches)    */
{
     static CHAR off[1];             /* writable, like the strings it stands */
                                     /*   in for                             */
     off[0] = '\0';
     return gs_cmdon(idx) ? gsval[idx] : off;
}

static const CHAR *
gs_commonpfx(VOID)                   /* punctuation most commands start with */
{
     static CHAR pfx[2];
     INT count[256];
     INT i, best = 0;

     setmem(count, sizeof(count), 0);
     for (i = 0 ; i < GS_COUNT_OF ; i++) {
          if (gsdefs[i].type == GST_CMD && ispunct((UCHAR)gsval[i][0])) {
               count[(UCHAR)gsval[i][0]]++;
          }
     }
     for (i = 1 ; i < 256 ; i++) {
          if (count[i] > count[best]) {
               best = i;
          }
     }
     if (count[best] == 0) {
          return gsdefs[GS_CMDPFX].def;
     }
     pfx[0] = (CHAR)best;
     pfx[1] = '\0';
     return pfx;
}

static GBOOL gs_pfxswap(INT i, const CHAR *val, const CHAR *oldp,
                        const CHAR *newp, CHAR *out);

static VOID
gs_defhere(INT i, CHAR *out)         /* setting i's default, with the board's*/
{                                    /*   own prefix (GS_VALSIZ * 2)         */
     /* Every default fits with a two-character prefix, and out holds    */
     /* GS_VALSIZ * 2.                                                   */
     if (!gs_pfxswap(i, gsdefs[i].def, gsdefs[GS_CMDPFX].def, gsval[GS_CMDPFX],
                     out)) {
          strcpy(out, gsdefs[i].def);
     }
}

static VOID
gs_followpfx(INT i)                  /* a default command string: the board's*/
{                                    /*   own prefix, not the default one    */
     CHAR here[GS_VALSIZ * 2];

     if (strcmp(gsval[i], gsdefs[i].def) == 0) {
          gs_defhere(i, here);
          gs_store(i, here);
     }
}

/* ------------------------------------------------------------------------ *
 * Importing a 3.43 board's values (ELWGLOB.V3, its old ELWGLOB.MSG).       *
 * ------------------------------------------------------------------------ */

static GBOOL gsimported[GS_COUNT_OF];     /* values the 3.43 file gave       */

static INT
gs_find(const CHAR *name)            /* setting by name, or -1               */
{
     INT i;

     for (i = 0 ; i < GS_COUNT_OF ; i++) {
          if (sameas(gsdefs[i].name, name)) {
               return i;
          }
     }
     return -1;
}

static VOID
gs_import1(                          /* one option from the old file         */
const CHAR *name,                    /*   option name                        */
CHAR *body,                          /*   text between its braces            */
CHAR type,                           /*   S, N, L, B or C (after the brace)  */
INT *nbad)                           /*   counts values refused              */
{
     INT idx = gs_find(name);
     CHAR *value, *p;
     CHAR norm[GS_VALSIZ];
     INT clash;

     if (idx < 0 || strchr("SNLBC", type) == NULL) {
          return;                    /* text, or an option no longer used    */
     }
     /* S: the braces hold the value.  N, L and B: a prompt, then the     */
     /* value as the last word.  C: a prompt, then the character last.    */
     if (type == 'S') {
          value = body;
     }
     else if (type == 'C') {
          value = body + strlen(body) - (*body != '\0');
     }
     else {
          for (p = body + strlen(body) ; p > body && isspace((UCHAR)p[-1]) ; ) {
               *--p = '\0';
          }
          value = strrchr(body, ' ');
          value = (value == NULL) ? body : value + 1;
     }
     if (gs_check(idx, value, norm, FALSE, &clash) != GSC_OK) {
          shocst("GLOBALS IMPORT: VALUE REFUSED",
                 "%s kept at its default", gsdefs[idx].name);
          (*nbad)++;
          return;
     }
     gs_store(idx, norm);
     gsimported[idx] = TRUE;
}

static VOID
gs_import(VOID)                      /* take a 3.43 board's option values    */
{
     FILE *fp;
     LONG len;
     CHAR *buf, *s, *b, *e, *nm;
     CHAR name[GS_NAMSIZ];
     CHAR here[GS_VALSIZ * 2];
     INT i, nset = 0, nbad = 0;

     if ((fp = fopen(GS_V3FILE, "rb")) == NULL) {
          return;
     }
     setmem(gsimported, sizeof(gsimported), 0);
     fseek(fp, 0L, SEEK_END);
     len = ftell(fp);
     fseek(fp, 0L, SEEK_SET);
     if (len < 0 || (buf = (CHAR *)malloc((size_t)len + 1)) == NULL) {
          fclose(fp);
          return;
     }
     len = (LONG)fread(buf, 1, (size_t)len, fp);
     fclose(fp);
     buf[len] = '\0';

     /* Walk the entries as the engine's message reader does: an entry's  */
     /* name is the last run of letters and digits before its '{', and its */
     /* text runs to the next '}' that is not escaped with '~'.            */
     for (s = buf ; (b = strchr(s, '{')) != NULL ; s = e + 1) {
          for (e = b + 1 ; *e != '\0' && *e != '}' ; e++) {
               if (*e == '~' && e[1] != '\0') {
                    memmove(e, e + 1, strlen(e));
               }
          }
          if (*e == '\0') {
               break;
          }
          *e = '\0';
          for (nm = b ; nm > s && !isalnum((UCHAR)nm[-1]) ; nm--) {
          }
          for (i = 0 ; nm > s && isalnum((UCHAR)nm[-1]) ; nm--, i++) {
          }
          if (i > 0 && i < GS_NAMSIZ) {
               stzcpy(name, nm, i + 1);
               for (nm = e + 1 ; *nm == ' ' || *nm == '\t' ; nm++) {
               }
               gs_import1(name, b + 1, *nm, &nbad);
          }
     }
     free(buf);

     /* 3.43 had no prefix setting: take the character most of the       */
     /* imported command strings start with, so changing it later changes */
     /* the strings this board actually uses.  Commands 3.43 did not have */
     /* (=gset, =config) take that prefix too.                             */
     gs_store(GS_CMDPFX, gs_commonpfx());

     for (i = 0 ; i < GS_COUNT_OF ; i++) {
          if (gsimported[i]) {       /* a change: not the default as this  */
               gs_defhere(i, here);  /*   board's prefix would have it     */
               if (strcmp(gsval[i], here) != 0) {
                    shocst("GLOBALS IMPORT: SETTING", "%s = %s",
                           gsdefs[i].name, gsval[i]);
                    nset++;
               }
          }
          else {
               gs_followpfx(i);
          }
          gs_save(i);
     }
     remove(GS_V3DONE);
     rename(GS_V3FILE, GS_V3DONE);
     shocst("GLOBALS SETTINGS IMPORTED",
            "From " GS_V3FILE ": %d changed, %d refused", nset, nbad);
}

/* ------------------------------------------------------------------------ *
 * Opening.                                                                 *
 * ------------------------------------------------------------------------ */

VOID
gs_open(VOID)                        /* open, load (or create/import), apply */
{
     struct gsrec rec;
     struct dfaSegSpec seg;
     struct dfaKeySpec key;
     CHAR norm[GS_VALSIZ];
     GBOOL created = FALSE;
     static GBOOL added[GS_COUNT_OF];  /* rows new to an existing file       */
     INT i, clash, nnew = 0;

     if (!isfile(GS_FILE)) {
          setmem(&seg, sizeof(seg), 0);
          seg.position = 0;
          seg.length = GS_NAMSIZ;
          seg.type = DFAST_ZSTRING;
          setmem(&key, sizeof(key), 0);
          key.nSegments = 1;
          key.segs = &seg;
          dfaCreateSpec(GS_FILE, FALSE, sizeof(struct gsrec), 1024,
                        0, 0, 1, &key, NULL);
          created = TRUE;
     }
     gsdat = dfaOpen(GS_FILE, sizeof(struct gsrec), NULL);

     for (i = 0 ; i < GS_COUNT_OF ; i++) {
          added[i] = FALSE;
          setmem(&rec, sizeof(rec), 0);
          stzcpy(rec.name, gsdefs[i].name, GS_NAMSIZ);   /* writable key   */
          dfaSetBlk(gsdat);
          if (dfaAcqEQ(&rec, rec.name, 0)) {
               dfaRstBlk();
               rec.value[GS_VALSIZ - 1] = '\0';
               if (gs_check(i, rec.value, norm, FALSE, &clash) == GSC_OK) {
                    gs_store(i, norm);
                    continue;
               }
               shocst("GLOBALS SETTING RESET",
                      "%s: stored value not allowed", gsdefs[i].name);
          }
          else {
               dfaRstBlk();
               nnew += !created;
               added[i] = !created;
          }
          gs_default(i);
     }

     /* A command added to an existing board takes that board's prefix    */
     /* (and a board from before the prefix setting gets the one its      */
     /* commands use), so an upgrade never mixes = and // commands.       */
     if (added[GS_CMDPFX]) {
          gs_store(GS_CMDPFX, gs_commonpfx());
          gs_save(GS_CMDPFX);
     }
     for (i = 0 ; i < GS_COUNT_OF ; i++) {
          if (added[i]) {
               gs_followpfx(i);
               gs_save(i);
          }
     }

     if (created) {
          if (isfile(GS_V3FILE)) {
               gs_import();
          }
          else {
               shocst("GLOBALS SETTINGS CREATED",
                      "New " GS_FILE ", default values");
          }
     }
     else if (nnew > 0) {
          shocst("GLOBALS SETTINGS ADDED",
                 "%d new settings at their defaults", nnew);
     }
     applysettings();
}

VOID
gs_close(VOID)
{
     dfaClose(gsdat);
}

/* ------------------------------------------------------------------------ *
 * The editor: a menu, opened with the =gset global command.                *
 *                                                                          *
 * Globals has no module of its own for the sysop to be in, so the menu     *
 * lives in the global command handler, as 3.43's telegram reply prompt    *
 * does: while a user is in it, gs_input() takes every line they type and  *
 * returns -1, so the engine passes it to no module and the module does    *
 * not prompt (WG33 MAJORBBS.C hdlcri()).  Leaving returns 1, and the      *
 * engine then has the user's module show its prompt again.                *
 *                                                                          *
 *   main menu      1-11 a group, R reset everything, X leave               *
 *   Enter alone, or a choice not on the screen, shows the screen again.    *
 *   group page     a number to change, N/P pages, D defaults, X back       *
 *   new value      Enter keeps it, "-" clears a key or text                *
 * ------------------------------------------------------------------------ */

#define GS_PAGESZ  15                /* settings per page                    */

enum {                               /* where a user is in the menu          */
     GSS_OFF = 0,                    /*   not in it                          */
     GSS_MAIN,                       /*   choosing a group                   */
     GSS_GROUP,                      /*   choosing a setting                 */
     GSS_VALUE,                      /*   typing a new value                 */
     GSS_RESET,                      /*   confirming a reset (YES)           */
     GSS_PREFIX                      /*   confirming a new prefix (Y/N)      */
};

static UCHAR gsst[MAXTERMS];         /* GSS_xxx, per channel                 */
static UCHAR gsgrp[MAXTERMS];        /* group shown (0 = all, for a reset)   */
static UCHAR gspage[MAXTERMS];       /* page of that group                   */
static SHORT gsidx[MAXTERMS];        /* setting being changed                */
static CHAR gsnew[MAXTERMS][3];      /* the new prefix awaiting Y/N          */

static INT
gs_count(INT group)                  /* settings in a group (0 = all)        */
{
     INT i, n = 0;

     for (i = 0 ; i < GS_COUNT_OF ; i++) {
          n += (group == 0 || gsdefs[i].group == group);
     }
     return n;
}

static INT
gs_nth(INT group, INT n)             /* the nth setting of a group, or -1    */
{
     INT i;

     for (i = 0 ; i < GS_COUNT_OF ; i++) {
          if (gsdefs[i].group == group && --n == 0) {
               return i;
          }
     }
     return -1;
}

static INT
gs_pages(INT group)
{
     return (gs_count(group) + GS_PAGESZ - 1) / GS_PAGESZ;
}

static const CHAR *
gs_limit(INT idx)                    /* the values a setting allows          */
{
     static CHAR buf[80];
     const struct gsdef *d = &gsdefs[idx];

     switch (d->type) {
     case GST_NUM:
          sprintf(buf, "a number from %ld to %ld", d->min, d->max);
          break;
     case GST_BOOL:
          strcpy(buf, "YES or NO");
          break;
     case GST_ON:
          strcpy(buf, "ON or OFF");
          break;
     case GST_CHR:
          strcpy(buf, "one character, not a space");
          break;
     case GST_PFX:
          strcpy(buf, "one or two punctuation characters, such as // or =");
          break;
     case GST_KEY:
          sprintf(buf, "a key, up to %ld characters, no spaces (- = blank = "
                  "everyone)", d->max);
          break;
     case GST_KEYNB:
          sprintf(buf, "a key, up to %ld characters, no spaces, not blank",
                  d->max);
          break;
     case GST_STR:
          sprintf(buf, "text, up to %ld characters (- clears it)", d->max);
          break;
     case GST_WORD:
          sprintf(buf, "one word, up to %ld characters", d->max);
          break;
     default:                        /* GST_CMD, GST_SUB                     */
          sprintf(buf, "one word, up to %ld characters, not used by another",
                  d->max);
          break;
     }
     return buf;
}

static VOID
gs_setvars(INT idx)                  /* text variables for one setting       */
{
     const CHAR *v = gsval[idx];
     CHAR shrt[31];

     if (*v == '\0') {
          v = getmsg(GSBLANK);
     }
     tv_set(TV_SETNAME, gsdefs[idx].name);
     tv_set(TV_SETVAL, v);
     if (strlen(v) > 30) {
          stzcpy(shrt, v, 29);
          strcat(shrt, "~");
          v = shrt;
     }
     else if (gsdefs[idx].type == GST_CMD && !gs_cmdon(idx)) {
          sprintf(shrt, "%s (off)", v);             /* 15 + 6: fits         */
          v = shrt;
     }
     else if (gsdefs[idx].type == GST_ON) {         /* "OFF  =cc"           */
          sprintf(shrt, "%-4s %.15s", v, gsval[gsdefs[idx].min]);
          v = shrt;
     }
     tv_set(TV_SETSHORT, v);
     tv_set(TV_SETDEF, *gsdefs[idx].def == '\0' ? getmsg(GSBLANK) : gsdefs[idx].def);
     /* An ON/OFF setting is labelled with its command's label.            */
     tv_set(TV_SETDESC, gsdefs[idx].type == GST_ON
                        ? gsdefs[gsdefs[idx].min].label : gsdefs[idx].label);
     tv_set(TV_SETLIM, gs_limit(idx));
     tv_set(TV_GROUP, grptitle[gsdefs[idx].group]);
}

static VOID
gs_audit(INT idx)                    /* a change in the Audit Trail          */
{
     shocst("GLOBALS SETTING CHANGED", "%s: %s = %s", usaptr->userid,
            gsdefs[idx].name, gsval[idx]);
}

static VOID
gs_main(VOID)                        /* show the main menu                   */
{
     INT g;

     gsst[usrnum] = GSS_MAIN;
     prfmsg(GSMENU);
     outprf(usrnum);
     for (g = 1 ; g < GSG_COUNT_OF ; g++) {
          tv_setn(TV_ITEM, g);
          tv_set(TV_GROUP, grptitle[g]);
          tv_setn(TV_COUNT, gs_count(g));
          prfmsg(GSGRP);
          outprf(usrnum);
     }
     tv_setn(TV_ITEM, GSG_COUNT_OF - 1);       /* "1-11" in the prompt      */
     prfmsg(GSMPRMT);
}

static VOID
gs_page(VOID)                        /* show the current page of a group     */
{
     INT g = gsgrp[usrnum];
     INT n, first = gspage[usrnum] * GS_PAGESZ + 1;
     INT last = first + GS_PAGESZ - 1;

     gsst[usrnum] = GSS_GROUP;
     if (last > gs_count(g)) {
          last = gs_count(g);
     }
     tv_setn(TV_ITEM, g);
     tv_set(TV_GROUP, grptitle[g]);
     tv_setn(TV_PAGE, gspage[usrnum] + 1);
     tv_setn(TV_PAGES, gs_pages(g));
     prfmsg(GSGHDR);
     outprf(usrnum);
     for (n = first ; n <= last ; n++) {
          gs_setvars(gs_nth(g, n));
          tv_setn(TV_ITEM, n);
          prfmsg(GSROW);
          outprf(usrnum);
     }
     prfmsg(GSGPRMT);
}

static VOID
gs_ask(INT idx)                      /* ask for a new value                  */
{
     gsst[usrnum] = GSS_VALUE;
     gsidx[usrnum] = (SHORT)idx;
     gs_setvars(idx);
     prfmsg(GSINFO);
     prfmsg(GSVPRMT);
}

static VOID
gs_askreset(INT group)               /* ask before resetting a group or all  */
{
     gsst[usrnum] = GSS_RESET;
     gsgrp[usrnum] = (UCHAR)group;
     tv_setn(TV_COUNT, gs_count(group));
     tv_set(TV_GROUP, grptitle[group]);
     prfmsg(GSRSTQ);
}

static GBOOL                         /*   TRUE if setting i follows          */
gs_pfxswap(                          /* setting i's value with a new prefix  */
INT i,                               /*   the setting                        */
const CHAR *val,                     /*   its value (gsval[i], or a default) */
const CHAR *oldp,                    /*   current prefix                     */
const CHAR *newp,                    /*   new prefix                         */
CHAR *out)                           /*   GS_VALSIZ * 2: the new value       */
{
     const CHAR *rest = val;
     size_t lo = strlen(oldp);

     strcpy(out, val);
     if ((gsdefs[i].type != GST_CMD && gsdefs[i].type != GST_WORD)
      || lo == 0 || strncmp(rest, oldp, lo) != 0) {
          return FALSE;
     }
     /* A string that is only the old prefix, repeated, stays as it is:   */
     /* "==" (read telegrams) goes with the telegram character "=".       */
     while (strncmp(rest, oldp, lo) == 0) {
          rest += lo;
     }
     if (*rest == '\0') {
          return FALSE;
     }
     sprintf(out, "%s%s", newp, val + lo);
     return TRUE;
}

static INT                           /*   commands that would change, -1 on  */
gs_pfxplan(                          /* what a new prefix would do  a clash, */
const CHAR *oldp,                    /*   current prefix          -2 if one  */
const CHAR *newp,                    /*   new prefix              is too long*/
INT *clash1,                         /*   on a clash: the two settings that  */
INT *clash2)                         /*   would end up the same (-2: *clash1)*/
{
     static CHAR after[GS_COUNT_OF][GS_VALSIZ * 2];
     INT i, j, n = 0;

     for (i = 0 ; i < GS_COUNT_OF ; i++) {
          if (gs_pfxswap(i, gsval[i], oldp, newp, after[i])) {
               if ((LONG)strlen(after[i]) > gsdefs[i].max) {
                    *clash1 = i;
                    return -2;
               }
               n += (gsdefs[i].type == GST_CMD);
          }
     }
     for (i = 0 ; i < GS_COUNT_OF ; i++) {
          for (j = i + 1 ; j < GS_COUNT_OF ; j++) {
               if (gsdefs[i].type == GST_CMD && gsdefs[j].type == GST_CMD
                && sameas(after[i], after[j])) {
                    *clash1 = i;
                    *clash2 = j;
                    return -1;
               }
          }
     }
     return n;
}

static VOID
gs_pfxapply(const CHAR *newp)        /* change the prefix, and the commands  */
{
     CHAR oldp[3], after[GS_VALSIZ * 2];
     INT i, n = 0;

     stzcpy(oldp, gsval[GS_CMDPFX], sizeof(oldp));
     for (i = 0 ; i < GS_COUNT_OF ; i++) {
          /* gs_pfxplan() checked every new value fits                     */
          if (gs_pfxswap(i, gsval[i], oldp, newp, after)) {
               stzcpy(gsval[i], after, GS_VALSIZ);
               gs_save(i);
               n += (gsdefs[i].type == GST_CMD);
          }
     }
     stzcpy(gsval[GS_CMDPFX], newp, GS_VALSIZ);
     gs_save(GS_CMDPFX);
     applysettings();
     shocst("GLOBALS PREFIX CHANGED", "%s: %s to %s, %d command strings",
            usaptr->userid, oldp, newp, n);
     tv_setn(TV_COUNT, n);
     gs_setvars(GS_CMDPFX);
     prfmsg(GSPFXD);
}

static VOID
gs_value(CHAR *text)                 /* a new value was typed                */
{
     INT idx = gsidx[usrnum];
     INT type = gsdefs[idx].type;
     CHAR norm[GS_VALSIZ];
     INT clash, c2, n;

     if (*text == '\0') {            /* Enter keeps it                       */
          gs_page();
          return;
     }
     /* "-" clears a value; the checks then refuse a blank where one is   */
     /* not allowed.  For a one-character setting it is just "-".          */
     if (strcmp(text, "-") == 0 && type != GST_NUM && type != GST_BOOL
      && type != GST_CHR && type != GST_PFX) {
          text = "";
     }
     switch (gs_check(idx, text, norm, TRUE, &clash)) {
     case GSC_OK:
          if (type == GST_PFX) {
               if (strcmp(norm, gsval[idx]) == 0) {
                    gs_page();
                    return;
               }
               n = gs_pfxplan(gsval[idx], norm, &clash, &c2);
               if (n == -2) {
                    gs_setvars(clash);
                    prfmsg(GSPFXLEN);
                    gs_page();
                    return;
               }
               if (n < 0) {
                    gs_setvars(c2);
                    tv_set(TV_OTHER, gsdefs[clash].name);
                    prfmsg(GSPFXDUP);
                    gs_page();
                    return;
               }
               gsst[usrnum] = GSS_PREFIX;
               stzcpy(gsnew[usrnum], norm, sizeof(gsnew[usrnum]));
               tv_setn(TV_COUNT, n);
               tv_set(TV_ARG, gsval[idx]);
               tv_set(TV_SETVAL, norm);
               prfmsg(GSPFXQ);
               return;
          }
          gs_store(idx, norm);
          gs_save(idx);
          applysettings();
          gs_audit(idx);
          gs_setvars(idx);
          prfmsg(GSSAVED);
          break;
     case GSC_DUP:
          gs_setvars(clash);
          tv_set(TV_OTHER, gsdefs[clash].name);
          tv_set(TV_ARG, text);
          prfmsg(GSDUP);
          break;
     default:
          gs_setvars(idx);
          prfmsg(GSBAD);
          break;
     }
     gs_page();
}

static VOID
gs_flip(INT idx)                     /* turn a command on or off             */
{
     gs_store(idx, gsnum[idx] != 0 ? "OFF" : "ON");
     gs_save(idx);
     applysettings();
     gs_audit(idx);
     gs_setvars(idx);
     prfmsg(GSSAVED);
     gs_page();
}

static VOID
gs_doreset(INT g)                    /* reset a group (0 = everything)       */
{
     INT i, j, n = 0;
     CHAR norm[GS_VALSIZ];

     for (i = 0 ; i < GS_COUNT_OF ; i++) {
          if (g == 0 || gsdefs[i].group == g) {
               gs_default(i);
               n++;
          }
     }
     /* Resetting one group can leave a command string equal to one in    */
     /* another group; the Audit Trail says so, the sysop fixes it.       */
     for (i = 0 ; i < GS_COUNT_OF ; i++) {
          if (gs_check(i, gsval[i], norm, TRUE, &j) == GSC_DUP && i < j) {
               shocst("GLOBALS SETTINGS CLASH", "%s and %s are both %s",
                      gsdefs[i].name, gsdefs[j].name, gsval[i]);
          }
     }
     applysettings();
     shocst("GLOBALS SETTINGS RESET", "%s: %d settings (%s)", usaptr->userid,
            n, grptitle[g]);
     tv_setn(TV_COUNT, n);
     tv_set(TV_GROUP, grptitle[g]);
     prfmsg(GSRSTD);
}

static VOID
gs_trimmed(CHAR **text)              /* the whole line, spaces kept inside   */
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
gs_command(VOID)                     /* =gset: open the menu                 */
{
     tv_set(TV_CMD, margv[0]);
     gs_main();
     doprintoutput();
}

INT                                  /*   0 not in the menu; -1 or 1 (exit)  */
gs_input(VOID)                       /* one line typed while in the menu     */
{
     CHAR *text;
     CHAR *end;
     INT n, idx, g, rc = -1;
     CHAR c = (CHAR)(margc > 0 ? toupper((UCHAR)margv[0][0]) : '\0');
     GBOOL word = (margc == 1 && margv[0][1] == '\0');  /* one letter alone  */

     if (gsst[usrnum] == GSS_OFF) {
          return 0;
     }
     setmbk(glomb);
     switch (gsst[usrnum]) {
     case GSS_MAIN:
          if (margc == 0) {
               gs_main();
          }
          else if (word && c == 'X') {
               gsst[usrnum] = GSS_OFF;
               prfmsg(GSBYE);
               rc = 1;
          }
          else if (word && c == 'R') {
               gs_askreset(0);
          }
          else {
               g = (INT)strtol(margv[0], &end, 10);
               if (margc == 1 && *end == '\0' && g >= 1 && g < GSG_COUNT_OF) {
                    gsgrp[usrnum] = (UCHAR)g;
                    gspage[usrnum] = 0;
                    gs_page();
               }
               else {
                    prfmsg(GSNOPICK);
                    gs_main();
               }
          }
          break;
     case GSS_GROUP:
          g = gsgrp[usrnum];
          if (margc == 0) {
               gs_page();
          }
          else if (word && c == 'X') {
               gs_main();
          }
          else if (word && c == 'N') {
               gspage[usrnum] = (UCHAR)((gspage[usrnum] + 1) % gs_pages(g));
               gs_page();
          }
          else if (word && c == 'P') {
               gspage[usrnum] = (UCHAR)((gspage[usrnum] + gs_pages(g) - 1) % gs_pages(g));
               gs_page();
          }
          else if (word && c == 'D') {
               gs_askreset(g);
          }
          else {
               n = (INT)strtol(margv[0], &end, 10);
               idx = (margc == 1 && *end == '\0') ? gs_nth(g, n) : -1;
               if (idx < 0) {
                    prfmsg(GSNOPICK);
                    gs_page();
               }
               else if (gsdefs[idx].type == GST_ON) {
                    gs_flip(idx);         /* no question: it just flips      */
               }
               else {
                    gs_ask(idx);
               }
          }
          break;
     case GSS_VALUE:
          gs_trimmed(&text);         /* text may have spaces in it           */
          gs_value(text);
          break;
     case GSS_RESET:
          g = gsgrp[usrnum];
          if (margc == 1 && sameas(margv[0], "YES")) {
               gs_doreset(g);
          }
          else {
               prfmsg(GSRSTN);
          }
          if (g == 0) {
               gs_main();
          }
          else {
               gs_page();
          }
          break;
     case GSS_PREFIX:
          if (word && c == 'Y') {
               gs_pfxapply(gsnew[usrnum]);
          }
          else {
               prfmsg(GSRSTN);
          }
          gs_page();
          break;
     }
     doprintoutput();
     return rc;
}

VOID
gs_userreset(INT unum)               /* log-on/hang-up: not in the menu      */
{
     if (unum >= 0 && unum < MAXTERMS) {
          gsst[unum] = GSS_OFF;
     }
}

VOID
gs_help(VOID)                        /* =? <the settings command>            */
{
     tv_set(TV_CMD, GSSTR(GSETSTG));
     prfmsg(GSHELP);
}
