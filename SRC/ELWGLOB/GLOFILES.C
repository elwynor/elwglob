/* Build 26.10.29.3 07:15PM */
/*****************************************************************************
 *   GLOFILES.C                                 Globals - Data files        *
 *                                                                           *
 *   Copyright (C) 2026 Elwynor Technologies.                                *
 *   Created by Mark Laudenbach.                                             *
 *                                                                           *
 *   Creates a missing data file (ELWGLOBx.DAT) from its empty template      *
 *   (ELWGLOBx.VIR) before the module opens it.                              *
 *                                                                           *
 *   The engine does not do this when the board starts: Worldgroup copied   *
 *   GCVIRDAT\*.VIR to .DAT in its install utility (WG33 BBSINT.C            *
 *   copyvirs()), and without the .DAT files a new install stops with       *
 *   "BTRIEVE OPEN ERROR 12" (file not found).  3.43 shipped                *
 *   empty .DAT files instead, which would overwrite a board's live data if  *
 *   unzipped over it.  The templates are byte for byte the empty files.    *
 *                                                                           *
 *   An existing .DAT is never touched.                                      *
 *                                                                           *
 *   Licensed under the GNU Affero General Public License v3.0.              *
 *****************************************************************************/

#include <stdio.h>
#include "GLOBROUS.H"
#include "GLOFILES.H"

static GBOOL
virtodat(const CHAR *src, const CHAR *dst)     /* TRUE if copied whole      */
{
     FILE *in, *out;
     CHAR buf[4096];
     size_t n;
     GBOOL ok = TRUE;

     if ((in = fopen(src, "rb")) == NULL) {
          return FALSE;
     }
     if ((out = fopen(dst, "wb")) == NULL) {
          fclose(in);
          return FALSE;
     }
     while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
          if (fwrite(buf, 1, n, out) != n) {
               ok = FALSE;
               break;
          }
     }
     if (ferror(in)) {
          ok = FALSE;
     }
     fclose(in);
     if (fclose(out) != 0) {
          ok = FALSE;
     }
     if (!ok) {
          remove(dst);               /* never leave half a database behind  */
     }
     return ok;
}

VOID
makedat(const CHAR *base)            /* create <base>.DAT from <base>.VIR    */
{
     CHAR dat[64], vir[64];

     sprintf(dat, "%s.DAT", base);
     if (isfile(dat)) {
          return;
     }
     sprintf(vir, "%s.VIR", base);
     if (!isfile(vir)) {
          sprintf(vir, "GCVIRDAT\\%s.VIR", base);
     }
     if (virtodat(vir, dat)) {
          shocst("GLOBALS DATA FILE CREATED", "%s from %s", dat, vir);
     }
     else {
          shocst("GLOBALS DATA FILE MISSING", "%s: no template %s.VIR", dat, base);
     }
}
