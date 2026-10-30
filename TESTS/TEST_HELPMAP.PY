# Build 26.10.29.3 07:15PM
"""
TEST_HELPMAP.PY - check the help table wiring against 3.43.

Globals for The Major BBS
Copyright (C) 2026 Elwynor Technologies.  Licensed under the AGPL-3.0.
Created by Mark Laudenbach.

3.43 filled help[] in init__elwglob with a strcpy per field.  ELWGLOB.C now
fills it from helpmap[] (key, command, label of each entry) every time a
setting changes.  The global commands find each other through help[n]
(help[6] is chat, help[39] telegrams, ...), so every entry must keep the
key, command string and label 3.43 gave it.  EXPECTED below was taken from
the 3.43 init__elwglob; entries 44 (=gset) and 45 (=config) are new.  Entries 1,
40 and 42 have command strings built from more than a setting (-1 in
helpmap); applysettings() builds them.

Usage:  python TESTS/TEST_HELPMAP.PY     (run from anywhere)
"""

import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SOURCE = os.path.join(ROOT, "SRC", "ELWGLOB", "ELWGLOB.C")

# index: (key, command or None if built, label)
EXPECTED = """
0 HLPKEY HLPSTG HELP1        1 HLPKEY - HELP2           2 ANSTKEY ANSSTG HELP3
3 BSYKEY BSYSTG HELP4        4 BSNKEY BNTSTG HELP5      5 UCIKEY CRDSTG HELP6
6 GLBKEY CHATSTG HELP7       7 ACSTKEY CSTSTG HELP8     8 CTOKEY CNTSTG HELP9
9 DNTKEY DNTSTG HELP10       10 DUPKEY DUPSTG HELP11    11 PGKEY BPSTG HELP12
12 EDTKEY EDTSTG HELP13      13 IBMKEY IBMSTG HELP14    14 INVKEY INVSTG HELP15
15 DLUKEY DLUSTG HELP16      16 SGNKEY NEWSTG HELP17    17 LOGKEY LOGSTG HELP18
18 MACKEY MACSTG HELP19      19 PAGKEY PGUSTG HELP20    20 PSTCKEY PSTSTG HELP22
21 PDYKEY PDYSTG HELP23      22 EPSTKEY EPSTSTG HELP21  23 RPTKEY RPTSTG HELP24
24 RLGKEY RLGSTG HELP25      25 SCLKEY SLSTG HELP26     26 SUSKEY ASUSSTG HELP27
27 SWCKEY SWCSTG HELP28      28 SUSKEY SUSSTG HELP29    29 SCWKEY SWSTG HELP30
30 TRCKEY TRNSTG HELP31      31 TRDKEY TRDSTG HELP32    32 TEXTKEY TEXTSTG HELP33
33 GTUKEY UKSTG HELP34       34 OFFKEY OFFSTG HELP35    35 BTUKEY ZAPSTG HELP36
36 MBRKEY MBRSTG HELP37      37 WHOKEY WHOSTG HELP38    38 SYSBKEY GLBSTG HELP39
39 TGMKEY GTLSTG HELP40      40 TGMKEY - HELP41         41 ALRMKEY ALRMSTG HELP42
42 COLKEY - HELP43           43 FRNDKEY FRNDSTG HELP44  44 GSETKEY GSETSTG HELP45
45 CFGKEY CFGSTG HELP46
"""


def main():
    want = {}
    toks = EXPECTED.split()
    for i in range(0, len(toks), 4):
        want[int(toks[i])] = (toks[i + 1], toks[i + 2], toks[i + 3])

    with open(SOURCE, "r", encoding="latin-1") as f:
        src = f.read()
    body = src[src.index("helpmap[HELPSIZE]={"):]
    body = body[:body.index("};")]
    rows = re.findall(r"\{\s*GS_(\w+),\s*(?:GS_(\w+)|-1),\s*(\w+)\s*\}", body)
    got = {n: (k, c or "-", m) for n, (k, c, m) in enumerate(rows)}

    failures = []
    if len(got) != len(want):
        failures.append("helpmap has %d entries, expected %d" % (len(got), len(want)))
    for n in sorted(want):
        if got.get(n) != want[n]:
            failures.append("help[%d]: %s, expected %s" % (n, got.get(n), want[n]))
    hsize = re.search(r"#define HELPSIZE (\d+)",
                      open(os.path.join(ROOT, "SRC", "ELWGLOB", "GLOBROUS.H"),
                           encoding="latin-1").read())
    if int(hsize.group(1)) != len(want):
        failures.append("HELPSIZE is %s, expected %d" % (hsize.group(1), len(want)))

    if failures:
        print("TEST_HELPMAP: FAIL")
        for f in failures:
            print("  " + f)
        sys.exit(1)
    print("TEST_HELPMAP: OK")


if __name__ == "__main__":
    main()
