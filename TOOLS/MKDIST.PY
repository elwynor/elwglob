# Build 26.10.29.3 07:15PM
"""
MKDIST.PY - package a release of Globals into DIST/<version>/.

Globals for The Major BBS
Copyright (C) 2026 Elwynor Technologies.  Licensed under the AGPL-3.0.
Created by Mark Laudenbach.

Steps:
  1. Regenerate ELWGLOB.MSG / ELWGLOB.H from ELWGLOB.MST (TOOLS/MKMSG.PY).
  2. Run the unit tests (TESTS/RUN_TESTS.CMD).
  3. Rebuild the Release|Win32 configuration with MSBuild.
  4. Check the DLL's product version matches GLO_VERSION in ELWGLOB.C.
  5. Copy only the deployable files, with upper-case names, into
     DIST/<version>/ and zip them as DIST/ELWGLOB-<version>.zip.

A version number must name exactly one build, so this refuses to write into
a DIST/<version>/ folder that already exists - bump the version instead of
rebuilding under an old number.

Usage:  python TOOLS/MKDIST.PY [path-to-MSBuild.exe]   (from the project root)
"""

import os
import re
import shutil
import subprocess
import sys
import zipfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC = os.path.join(ROOT, "SRC", "ELWGLOB")
MSBUILD_DEFAULT = (r"C:\Program Files (x86)\Microsoft Visual Studio\2022"
                   r"\BuildTools\MSBuild\Current\Bin\MSBuild.exe")

# (source file, name in the release) - deployable files only.
DEPLOY = [
    (os.path.join(ROOT, "BUILD", "Release", "ELWGLOB.dll"), "ELWGLOB.DLL"),
    (os.path.join(SRC, "ELWGLOB.MDF"), "ELWGLOB.MDF"),
    (os.path.join(SRC, "ELWGLOB.MSG"), "ELWGLOB.MSG"),
    (os.path.join(SRC, "ELWGLOB.RLN"), "ELWGLOB.RLN"),
] + [(os.path.join(SRC, "VIR", "ELWGLOB%s.VIR" % c), "ELWGLOB%s.VIR" % c)
     for c in "CGNSU"]
CRLF_FILES = ("ELWGLOB.MDF", "ELWGLOB.MSG", "ELWGLOB.RLN")


def fail(text):
    sys.exit("MKDIST: " + text)


def version():
    with open(os.path.join(SRC, "ELWGLOB.C"), encoding="latin-1") as f:
        m = re.search(r'CHAR \*GLO_VERSION="([^"]+)"', f.read())
    if not m:
        fail("GLO_VERSION not found in ELWGLOB.C")
    return m.group(1)


def check_crlf(path):
    data = open(path, "rb").read()
    if data.count(b"\n") != data.count(b"\r\n"):
        fail("%s must have CRLF line endings" % os.path.basename(path))


def main():
    ver = version()
    out = os.path.join(ROOT, "DIST", ver)
    if os.path.exists(out):
        fail("DIST/%s already exists - bump the version, do not rebuild "
             "under a number that may already be shared" % ver)

    subprocess.check_call([sys.executable, os.path.join(ROOT, "TOOLS", "MKMSG.PY")])
    subprocess.check_call(["cmd", "/c", os.path.join(ROOT, "TESTS", "RUN_TESTS.CMD")])
    msbuild = sys.argv[1] if len(sys.argv) > 1 else MSBUILD_DEFAULT
    subprocess.check_call([msbuild, os.path.join(ROOT, "ELWGLOB.sln"), "-t:Rebuild",
                           "-p:Configuration=Release", "-p:Platform=Win32",
                           "-nologo", "-v:minimal"])

    dll = DEPLOY[0][0]
    pv = subprocess.check_output(
        ["powershell", "-NoProfile", "-Command",
         "(Get-Item '%s').VersionInfo.ProductVersion" % dll], text=True).strip()
    if pv != ver:
        fail("DLL product version %r does not match GLO_VERSION %r - update "
             "ELWGLOB.rc" % (pv, ver))

    for src, name in DEPLOY:
        if not os.path.exists(src):
            fail("missing " + src)
        if name in CRLF_FILES:
            check_crlf(src)

    os.makedirs(out)
    for src, name in DEPLOY:
        shutil.copyfile(src, os.path.join(out, name))
    zpath = os.path.join(ROOT, "DIST", "ELWGLOB-%s.zip" % ver)
    with zipfile.ZipFile(zpath, "w", zipfile.ZIP_DEFLATED) as z:
        for _, name in DEPLOY:
            z.write(os.path.join(out, name), name)
    print("MKDIST: packaged v%s -> DIST/%s/ and %s" % (ver, ver, os.path.basename(zpath)))
    for _, name in DEPLOY:
        print("   ", name)


if __name__ == "__main__":
    main()
