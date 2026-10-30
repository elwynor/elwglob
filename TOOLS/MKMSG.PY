# Build 26.10.29.3 07:15PM
"""
MKMSG.PY - build ELWGLOB.MSG and ELWGLOB.H from the message template ELWGLOB.MST.

Globals for The Major BBS
Copyright (C) 2026 Elwynor Technologies.  Licensed under the AGPL-3.0.
Created by Mark Laudenbach.

Why a template?  A .MSG file holds raw control bytes - ESC for ANSI color,
BEL, and 0x01 for named text variables - that are awkward to type and easy to
damage in an editor.  The template spells them as readable tokens:

    <ESC>                 the ESC byte (0x1B) that starts an ANSI sequence
    <BEL>                 the BEL byte (0x07), a beep
    <TV:NAME>             a named text variable, natural width
    <TV:NAME:J:W>         a text variable justified J (N, L, R or C) to width W
    <HEX:xx>              any other single byte, e.g. <HEX:CD> for a box line
    <HEX:xx*n>            that byte n times

This script expands the tokens, writes ELWGLOB.MSG with the CRLF line endings
the Major BBS configuration editor requires, and writes the message-number
header ELWGLOB.H exactly as the Galacticomm WGSMSX utility numbers it: every
named entry, including each LEVELn marker, takes the next slot counting from
zero, and LEVELn markers get no #define.

It also checks that every message name the C sources pass to a message
routine exists in the template, so a missing message is caught here instead
of on the board.  Every changing value in the text is a named text variable
so it refuses a text message with a printf-style specifier, a
text variable that TVARS.H does not register, and a prfmsg() call that
passes arguments.

Usage:  python TOOLS/MKMSG.PY            (run from the project root)
"""

import glob
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRCDIR = os.path.join(ROOT, "SRC", "ELWGLOB")
TEMPLATE = os.path.join(SRCDIR, "ELWGLOB.MST")
MSGOUT = os.path.join(SRCDIR, "ELWGLOB.MSG")
HDROUT = os.path.join(SRCDIR, "ELWGLOB.H")

MOPTLEN = 8          # longest allowed message name (WGSMSX limit)
TVRSIZ = 16          # text variable names must be shorter than this
TVMAXW = 79          # the engine rejects wider text variables (MENUING.C)

# Routines whose first argument is a message number, and those whose second
# argument is one.
MSGFUNCS = ("prfmsg", "getmsg", "stgopt", "numopt", "lngopt", "ynopt",
            "chropt", "byenow", "rawmsg")
MSGFUNCS2 = ("uhsmkey",)

TVARSH = os.path.join(SRCDIR, "TVARS.H")
ENGINE_TVARS = {"SYSTEM_NAME"}   # text variables the engine itself registers

TV_RE = re.compile(r"<TV:([A-Z0-9_]+)(?::([NLRC]):(\d+))?>")
HEX_RE = re.compile(r"<HEX:([0-9A-Fa-f]{2})(?:\*(\d+))?>")
SPEC_RE = re.compile(r"%[-+ #0]*\d*(?:\.\d+)?[lh]?[diouxXcsfeEgG]")


def fail(text):
    sys.exit("MKMSG: " + text)


def expand(text):
    """Replace the readable tokens with the real control bytes."""
    def tv(m):
        name, just, width = m.group(1), m.group(2) or "N", int(m.group(3) or 1)
        if len(name) >= TVRSIZ:
            fail("text variable name too long: " + name)
        if width > TVMAXW:
            fail("text variable %s wider than %d" % (name, TVMAXW))
        return "\x01" + just + chr(32 + width) + name + "\x01"
    text = TV_RE.sub(tv, text)
    if "<TV:" in text:
        fail("malformed <TV:...> token near: " + text[text.index("<TV:"):][:40])
    text = HEX_RE.sub(lambda m: chr(int(m.group(1), 16)) * int(m.group(2) or 1), text)
    if "<HEX:" in text:
        fail("malformed <HEX:..> token near: " + text[text.index("<HEX:"):][:40])
    return text.replace("<ESC>", "\x1b").replace("<BEL>", "\x07")


def scan(text):
    """Return (name, body, spec) for each entry in file order, mirroring
    MSGRDR.C/rdmsg().

    The name of an entry is the last run of letters/digits before its '{'.
    The body runs to the next '}' that is not escaped with '~'.
    """
    entries, i, n = [], 0, len(text)
    while True:
        j = text.find("{", i)
        if j < 0:
            break
        m = re.search(r"([A-Za-z0-9]+)[^A-Za-z0-9]*$", text[i:j])
        if not m:
            fail("an entry with no name before '{' at offset %d" % j)
        name = m.group(1)
        k, esc = j + 1, False
        while k < n:
            c = text[k]
            if esc:
                esc = False
            elif c == "~":
                esc = True
            elif c == "}":
                break
            k += 1
        if k >= n:
            fail("no closing '}' for " + name)
        eol = text.find("\n", k)
        spec = text[k + 1:eol if eol >= 0 else n].strip()
        entries.append((name, text[j + 1:k], spec))
        i = k + 1
    return entries


def used_names():
    """Message names the C sources pass to a message routine."""
    used = set()
    pats = [re.compile(r"\b(?:%s)\s*\(\s*([A-Z][A-Z0-9]*)\b" % "|".join(MSGFUNCS)),
            re.compile(r"\b(?:%s)\s*\([^,()]*,\s*([A-Z][A-Z0-9]*)\b"
                       % "|".join(MSGFUNCS2))]
    for fn in glob.glob(os.path.join(SRCDIR, "*.C")):
        with open(fn, "r", encoding="latin-1") as f:
            src = f.read()
        src = re.sub(r"/\*.*?\*/", "", src, flags=re.S)   # drop comments
        src = re.sub(r"//[^\n]*", "", src)
        for pat in pats:
            used.update(pat.findall(src))
    return used


def registered():
    """The text variable names TVARS.H registers (GLO_ plus each X() name)."""
    with open(TVARSH, "r", encoding="latin-1") as f:
        hdr = f.read()
    names = set()
    for block in re.findall(r"#define GLO\w+ \\\n((?:.*\\\n)*.*)", hdr):
        names.update("GLO_" + n for n in re.findall(r"X\((\w+)\)", block))
    return names | ENGINE_TVARS


def prfmsg_args():
    """file:line of each prfmsg() call that passes more than the message."""
    bad = []
    for fn in glob.glob(os.path.join(SRCDIR, "*.C")):
        with open(fn, "r", encoding="latin-1") as f:
            src = f.read()
        for m in re.finditer(r"\bprfmsg\s*\(", src):
            i, depth, commas = m.end(), 1, 0
            while depth:
                c = src[i]
                if c in "([":
                    depth += 1
                elif c in ")]":
                    depth -= 1
                elif c == "," and depth == 1:
                    commas += 1
                i += 1
            if commas:
                bad.append("%s:%d" % (os.path.basename(fn), src.count("\n", 0, m.start()) + 1))
    return bad


def main():
    with open(TEMPLATE, "r", encoding="ascii") as f:
        src = f.read().replace("\r\n", "\n")
    entries = scan(src)
    names = [e[0] for e in entries]

    seen, levels, printf = set(), set(), []
    for name, body, spec in entries:
        if len(name) > MOPTLEN:
            fail("message name longer than %d characters: %s" % (MOPTLEN, name))
        if name in seen:
            fail("duplicate message name: " + name)
        seen.add(name)
        if name.startswith("LEVEL"):
            if name in levels:
                fail(name + " appears twice; the CNF editor shows only the first")
            levels.add(name)
        if spec[:1] == "T":
            rest = SPEC_RE.sub("", body.replace("%%", ""))
            if "%" in rest:
                fail("message %s has a bare %% - write %%%% for a literal percent" % name)
            if SPEC_RE.search(body.replace("%%", "")):
                printf.append(name)

    if printf:
        fail("text messages with printf specifiers (use text variables): " + " ".join(printf))
    unknown = sorted({m[0] for m in TV_RE.findall(src)} - registered())
    if unknown:
        fail("text variables TVARS.H does not register: " + " ".join(unknown))
    withargs = prfmsg_args()
    if withargs:
        fail("prfmsg() with arguments (use text variables): " + " ".join(withargs))

    missing = sorted(used_names() - seen)
    if missing:
        fail("used in the sources but not in ELWGLOB.MST: " + " ".join(missing))

    out = expand(src).replace("\n", "\r\n")
    with open(MSGOUT, "wb") as f:
        f.write(out.encode("latin-1"))

    lines = ['/*  Message-number definitions derived from "ELWGLOB.MSG"  */', ""]
    for slot, name in enumerate(names):
        if not (name.startswith("LEVEL") and name[5:].isdigit()):
            lines.append("#define %-9s %3d" % (name, slot))
    with open(HDROUT, "w", newline="\r\n") as f:
        f.write("\n".join(lines) + "\n")

    print("MKMSG: %d entries -> %s, %s" % (len(names), os.path.relpath(MSGOUT, ROOT),
                                          os.path.relpath(HDROUT, ROOT)))


if __name__ == "__main__":
    main()
