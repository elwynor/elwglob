# Changelog

All notable changes to Globals are recorded here.

## [4.0.0] - 2026-10-29

### Added
- Settings are changed online with the new `=gset` global command (key
  `GSETKEY`, `SYSOP` to start with; `MASTER` always). It opens a menu of
  eleven groups, 15 settings to a page: pick a number, type the value, or
  reset a group or everything. Every change is saved in the new
  `ELWGLOBO.DAT`, applies at once (no restart), and goes in the Audit
  Trail. Settings keep their 3.43 option names.
- Upgrading from 3.43: a copy of the old `ELWGLOB.MSG` saved as
  `ELWGLOB.V3` is read once on the first start, so a board keeps its keys,
  command strings and other settings. Every value that differs from the
  default is listed in the Audit Trail, and the file is renamed
  `ELWGLOB.V3I`.
- Command prefix setting (`CMDPFX`, first in Command strings): one or two
  punctuation characters, `=` by default. Changing it changes every command
  string (and default macro command) that starts with the old prefix; it
  asks first, and refuses a change that would make two commands the same
  or one longer than its maximum. Read telegrams (`==`) goes with the
  telegram character, which does not follow the prefix.
- Commands on/off: a settings group with an ON/OFF setting for every
  command string except `=gset`. An OFF command is ignored completely, so
  the line goes on to the user's module; it is left out of `=?` and of the
  user settings menu, and keeps its string (marked "(off)" in the editor).
- User settings menu: Globals can be a page of the BBS menu (Menu Tree
  editor), and `=config` (`CFGSTG`, key `CFGKEY`) opens the same menu from
  anywhere on the board. It is in five sections (Chat; Pages and
  messages; Log-on, Who's Online and clock; Macros; Terminal and screen)
  so every screen fits a 24-line terminal. Each item runs the global
  command behind it and shows only with that command's key; `S` opens
  the system settings for users who may use `=gset`.
- `=cc off` leaves global chat and remembers the channel, even after a
  log-off; `=cc on` goes back to it, or to the new-user channel (`DEFCC`),
  or 1. The words are settings (`CHONSTG`, `CHOFSTG`).
- Clock: each user picks 12-hour (`4:45 PM`) or 24-hour (`16:45:00`)
  times for `=dt`, the Who's Online heading, telegrams, alarms and last
  log-on times. `DEF12H` (New-user defaults) is the clock for users who
  have not chosen. The choice is kept in the engine's generic user
  database, so `ELWGLOBU.DAT` is unchanged.
- Who's Online shows the connection type (CONT) instead of the baud rate:
  TELNET, SSH, RLOGIN, FTP, WEB, LOCAL or OTHER, from the TCP/IP server
  that accepted the call. Works with or without the SSH Server module; an
  Audit Trail line after start-up says whether it is there.
- Who's Online display 1 shows every flag a user has, one letter each
  (S Sysop, B Busy, I invisible, O Op Chat, L logging off; text entries
  `FLAGL1`-`FLAGL5`), instead of only the first flag's name.
- Globals creates each missing data file from its `.VIR` template at
  start-up and never touches one that exists.
- `ELWGLOB.MSG` and `ELWGLOB.H` are generated from `ELWGLOB.MST` by
  `TOOLS/MKMSG.PY`, which refuses a printf specifier in a text message, a
  text variable `TVARS.H` does not register, and a `prfmsg()` with
  arguments.
- Unit tests: `TESTS/RUN_TESTS.CMD`.

### Changed
- `ELWGLOB.MSG` no longer has configuration options (`LEVEL3`, `LEVEL4`);
  it keeps only its text.
- Every message uses named text variables; no text has a `%s` or `%d`
  any more and no `prfmsg()` call passes arguments. Sysops can reword,
  reorder or drop values in the configuration editor. About 90 variables
  are set by the code (`GLO_USERID`, `GLO_DATE`, `GLO_CREDITS`, ...), and
  every setting shown in text is `GLO_` plus its name (`GLO_CHATSTG`,
  `GLO_ECHSTG`, `GLO_TLCCHR`, ...), read live, so a command string
  changed in `=gset` shows in every help screen at once.
- The words put into the text (ON, OFF, YES, UNLIMITED, <None>, ...) are
  text entries (`WON`, `WOFF`, ...) instead of English in the code.
- Telegram text, chat lines, broadcasts, page messages and key lists can
  be longer than a text variable, so each is printed between its message
  and a new `...END` entry (`TTXTMEND`, `CTMSGEND`, `BCSTMEND`,
  `PGOUTEND`, `CLKMSEND`, `SKYMSEND`).
- Changing the number of chat channels online moves the Sysop channel to
  the new top; users in it follow, users above the new top leave chat.
- The release ships the `.VIR` templates instead of empty `.DAT` files,
  which would overwrite a board's live data if unzipped over it.
- Settings that 3.43 read from the message file every time they were used
  (new-user defaults, the "cannot be suspended/deleted" keys, the invisible
  log-on key) no longer allocate memory on each use.
- Two options that 3.43 never read are gone: `CLOKKEY` and `UISTG`.
- Project layout: sources in `SRC/ELWGLOB/`, solution `ELWGLOB.sln`,
  build output in `BUILD/`. The project locates the SDK through
  `MBBSSDK.props` and builds for Win32 only.

### Fixed
- Buffer overrun in the help table: the telegram entry (send character
  plus `<user-id>`, 10 characters) and a 25-character help label each wrote
  their terminating NUL one byte past their buffer.
- Who's Online headings did not line up with their columns (the Baud
  heading was 6 wide over a 9-wide value, and Class one short), so SEX
  through FLAGS drifted. Every heading is now exactly its column's width,
  in capitals.
- `DEFWHO` (new users' Who's Online display) allowed 4 and 5; only 1 to 3
  exist. The README said there were 5 displays.
- The Who's Online help said the default display was ON or OFF; it shows
  the display number.
- The telegram sent when a Sysop changes a user's time on-line had five
  values for three places, so the day's minutes showed a wrong number.
- The help for looking up another user's account (`=c` with a user-id)
  showed the transfer-days command string.
- Notice telegrams (credits or days posted, keys, class, invisibility,
  time) allocated a copy of their text each time one was sent and never
  freed it, and used it as a `spr()` format.
- The Who's Online heading text was passed to `prf()` as a format.

## [3.43] - 2024

Last upstream release (https://github.com/elwynor/elwglob).
