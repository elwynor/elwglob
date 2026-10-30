/* Build 26.10.11.5 09:24PM */
/*****************************************************************************
 *   STUB\GLOBROUS.H                     Globals - Test stand-in for the SDK *
 *                                                                           *
 *   Copyright (C) 2026 Elwynor Technologies.                                *
 *   Created by Mark Laudenbach.                                             *
 *                                                                           *
 *   TEST_GLOSET compiles GLOSET.C against this header instead of the real   *
 *   GLOBROUS.H (RUN_TESTS.CMD copies GLOSET.C next to it, so this one is    *
 *   found first).  It declares only what GLOSET.C, GLOFILES.C and          *
 *   GLOCLOCK.C use; TEST_GLOSET.C                                           *
 *   implements it, including an in-memory Btrieve file that copies a key    *
 *   only from writable memory, as the engine does.                          *
 *                                                                           *
 *   Licensed under the GNU Affero General Public License v3.0.              *
 *****************************************************************************/

#ifndef GLOBROUS_H
#define GLOBROUS_H

#include <windows.h>
#include <stdio.h>
#include <string.h>

typedef int GBOOL;
typedef void *HMCVFILE;
typedef struct dfablk { int unused; } DFAFILE;

struct dfaSegSpec {
     size_t position;
     size_t length;
     INT type;
     INT flags;
     CHAR nullChar;
};
struct dfaKeySpec {
     INT flags;
     INT nSegments;
     struct dfaSegSpec *segs;
};
#define DFAST_ZSTRING 11

struct usracc { CHAR userid[30]; };

#define UIDSIZ 30
#define MNMSIZ 30
extern DFAFILE *genbb;               /* the generic user database            */

#define MAXTERMS 256                 /* as GLOBROUS.H                        */

#include "ELWGLOB.H"                 /* the generated message numbers        */

extern INT margc;
extern CHAR input[];
extern CHAR *margv[];
extern INT usrnum;
extern struct usracc *usaptr;

INT sameas(const CHAR *a, const CHAR *b);
CHAR *stzcpy(CHAR *dst, const CHAR *src, INT len);
VOID setmem(VOID *p, UINT n, CHAR c);
VOID shocst(const CHAR *summary, const CHAR *fmt, ...);
GBOOL isfile(const CHAR *name);
VOID dfaCreateSpec(const CHAR *name, GBOOL overwrite, size_t reclen,
                   size_t pagesize, INT flags, INT nprealloc, INT nkeys,
                   struct dfaKeySpec *keys, const CHAR *alt);
DFAFILE *dfaOpen(const CHAR *name, UINT reclen, const CHAR *owner);
VOID dfaClose(DFAFILE *f);
VOID dfaSetBlk(DFAFILE *f);
VOID dfaRstBlk(VOID);
GBOOL dfaAcqEQ(VOID *rec, const VOID *key, INT keynum);
VOID dfaUpdate(VOID *rec);
VOID dfaInsert(VOID *rec);
VOID dfaUpdateV(VOID *rec, UINT len);
VOID dfaInsertV(VOID *rec, UINT len);
const CHAR *nctime(USHORT t);
VOID prfmsg(INT msgnum, ...);
VOID outprf(INT unum);
CHAR *getmsg(INT msgnum);
CHAR *spr(const CHAR *fmt, ...);
VOID rstrin(VOID);
VOID setmbk(HMCVFILE mb);

#endif /* GLOBROUS_H */
