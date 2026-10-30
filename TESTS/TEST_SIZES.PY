# Build 26.10.29.3 07:15PM
"""
TEST_SIZES.PY - check that every value Globals copies into a fixed buffer fits.

Globals for The Major BBS
Copyright (C) 2026 Elwynor Technologies.  Licensed under the AGPL-3.0.
Created by Mark Laudenbach.

The help[] table (GLOBROUS.H, struct userhelp) holds each global's key,
command string and menu label in fixed buffers.  applysettings() (ELWGLOB.C)
fills them from the settings named in helpmap[], whose maximum lengths are
in GLOSETS.H; the labels and the HELCAPP/TELCAPP endings are text options in
ELWGLOB.MST.  Each buffer must hold the longest value plus the NUL.  3.43
had two too small: the telegram entry (one character plus TELCAPP, 10
characters) wrote its NUL past help[].command, and a 25-character HELPn label
did the same to help[].message.

The settings store (GLOSET.H) must also hold the longest setting.

Usage:  python TESTS/TEST_SIZES.PY       (run from anywhere)
"""

import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRCDIR = os.path.join(ROOT, "SRC", "ELWGLOB")


def read(name):
    with open(os.path.join(SRCDIR, name), "r", encoding="latin-1") as f:
        return f.read()


def defines(text):
    return {m.group(1): int(m.group(2))
            for m in re.finditer(r"^#define\s+(\w+)\s+(\d+)\b", text, re.M)}


def main():
    h = defines(read("GLOBROUS.H"))
    g = defines(read("GLOSET.H"))
    texts = {m.group(1): int(m.group(2))
             for m in re.finditer(r"^(\w+) \{[^}]*\} S (\d+)", read("ELWGLOB.MST"), re.M)}
    xlines = re.findall(r"^X\((\w+)\s*,\s*\w+\s*,\s*(\w+)\s*,\s*(\w+),\s*(\d+),",
                        read("GLOSETS.H"), re.M)
    sets = {n: (t, int(hi)) for n, t, lo, hi in xlines}
    nx = len(re.findall(r"^X\(", read("GLOSETS.H"), re.M))
    main_c = read("ELWGLOB.C")
    body = main_c[main_c.index("helpmap[HELPSIZE]={"):]
    rows = re.findall(r"\{\s*GS_(\w+),\s*(?:GS_(\w+)|-1),\s*\w+\s*\}", body[:body.index("};")])
    failures = []

    def check(what, need, have):
        if need + 1 > have:
            failures.append("%s needs %d bytes, buffer has %d" % (what, need + 1, have))

    keymax = max(sets[k][1] for k, c in rows)
    cmdmax = max(sets[c][1] for k, c in rows if c)
    check("help[].key (a key setting)", keymax, h["KEYSIZ"] if "KEYSIZ" in h else 16)
    check("help[].command (a command string)", cmdmax, h["HELPCOMSIZE"])
    check("help[1].command (help command + HELCAPP)",
          sets["HLPSTG"][1] + texts["HELCAPP"], h["HELPCOMSIZE"])
    check("help[40].command (telegram character + TELCAPP)",
          1 + texts["TELCAPP"], h["HELPCOMSIZE"])
    check("help[].message (HELPn)",
          max(n for k, n in texts.items() if re.match(r"HELP\d+$", k)), h["HELPMSSSIZE"])
    check("a stored setting value", max(n for t, n in sets.values()
                                        if t not in ("GST_NUM", "GST_BOOL", "GST_CHR")),
          g["GS_VALSIZ"])
    check("a setting name", max(len(n) for n in sets), g["GS_NAMSIZ"])
    if nx != len(xlines):
        failures.append("GLOSETS.H: %d X() lines, %d understood" % (nx, len(xlines)))

    # Commands on/off: every command string but the settings command has
    # exactly one ON/OFF setting, and each ON/OFF setting names a command.
    cmds = [n for n, t, lo, hi in xlines if t == "GST_CMD"]
    ons = [lo[3:] if lo.startswith("GS_") else lo
           for n, t, lo, hi in xlines if t == "GST_ON"]
    for c in cmds:
        want = 0 if c == "GSETSTG" else 1
        if ons.count(c) != want:
            failures.append("command %s has %d ON/OFF settings, expected %d"
                            % (c, ons.count(c), want))
    for o in ons:
        if o not in cmds:
            failures.append("ON/OFF setting names %s, not a command string" % o)

    if failures:
        print("TEST_SIZES: FAIL")
        for f in failures:
            print("  " + f)
        sys.exit(1)
    print("TEST_SIZES: OK (%d settings, %d help entries)" % (len(sets), len(rows)))


if __name__ == "__main__":
    main()
