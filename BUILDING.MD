# Building Globals

Globals is a Win32 DLL module for The Major BBS. It is built with
Microsoft Visual Studio against The Major BBS Module SDK.

## Prerequisites

- **Microsoft Visual Studio 2022** (or Build Tools 2022) with the C++
  desktop workload. Platform toolset **v143**.
- **Windows 10 SDK** (`WindowsTargetPlatformVersion` 10.0).
- **The Major BBS Module SDK**
  (https://github.com/TheMajorBBS/MBBS-v10-module-SDK).

> **Build for x86 (Win32) only. Never x64.** The Major BBS is a 32-bit
> program and cannot load a 64-bit module.

## Setup

Copy `MBBSSDK.props.example` to `MBBSSDK.props` and set `MbbsSdkDir` to your
SDK folder (with the trailing backslash). `MBBSSDK.props` is not committed.

## Build

Open `ELWGLOB.sln`, choose **Release** / **Win32**, and build. Or:

```
MSBuild.exe ELWGLOB.sln -p:Configuration=Release -p:Platform=Win32
```

The DLL is written to `BUILD/Release/ELWGLOB.dll`.

## Message file

The module's text is in `SRC/ELWGLOB/ELWGLOB.MSG`; it has no configuration
options (settings are changed online with `=gset`; their defaults are in
`SRC/ELWGLOB/GLOSETS.H`).
`SRC/ELWGLOB/ELWGLOB.H` holds the matching message numbers. Both are
generated from `SRC/ELWGLOB/ELWGLOB.MST`; edit the template, never the
`.MSG` or `.H`, then run (Python 3, from the project root):

```
python TOOLS/MKMSG.PY
```

The template spells control bytes as readable tokens (`<ESC>`, `<BEL>`,
`<TV:NAME>`, `<TV:NAME:J:W>`, `<HEX:CD*66>`). The script writes the `.MSG`
with the CRLF line endings the configuration editor needs and numbers the
messages in file order from 0, counting every named entry including each
`LEVELn` marker. It stops if a message name used in the sources is missing
from the template. Commit the `.MST`, `.MSG` and `.H` together and rebuild
the DLL. Deploy the DLL and `.MSG` together; the BBS compiles `ELWGLOB.MCV`
itself.

## Tests

```
TESTS\RUN_TESTS.CMD
```

Needs Python 3 and Visual Studio 2022 (or Build Tools). It runs
`TEST_SIZES.PY` (fixed buffers hold the longest values), `TEST_HELPMAP.PY`
(the help table matches 3.43) and `TEST_GLOSET.C`, which builds `GLOSET.C`
against a stand-in for the SDK (`TESTS/STUB/`) and tests loading, saving,
the `=gset` editor and the 3.43 import (against `TESTS/DATA/ELWGLOB.343`,
the 3.43 message file as released). Exit code 0 means every test passed.

## Data files

`SRC/ELWGLOB/VIR/` holds the empty database templates (`ELWGLOBC.VIR`,
`ELWGLOBG.VIR`, `ELWGLOBN.VIR`, `ELWGLOBS.VIR`, `ELWGLOBU.VIR`), byte for
byte the empty `.DAT` files. The board does not create `.DAT` files from
them when it starts; the module does (`makedat()` in `GLOFILES.C`), for any
that is missing, before it opens them. Empty `.DAT` files are never shipped:
unzipped over a running board they would replace its live data. The
settings file, `ELWGLOBO.DAT`, has no template: the module creates it
itself.

## Packaging a release

Set the version in two places: `GLO_VERSION` in `SRC/ELWGLOB/ELWGLOB.C` and
`SRC/ELWGLOB/ELWGLOB.rc` (FILEVERSION, PRODUCTVERSION and the two version
strings; the last FILEVERSION number is 0 for a release). Then, from the project root:

```
python TOOLS/MKDIST.PY
```

It regenerates the message file, runs the tests, rebuilds, checks the DLL's
product version matches `GLO_VERSION`, and copies the deployable files
(`ELWGLOB.DLL`, `ELWGLOB.MDF`, `ELWGLOB.MSG`, `ELWGLOB.RLN` and the five
`.VIR` files) into `DIST/<version>/`, zipped as `DIST/ELWGLOB-<version>.zip`.
It refuses to overwrite an existing `DIST/<version>/`: never rebuild under a
version number that has been shared. Bump the version instead.
