# Globals - Technical Notes

## Module structure

| File | Purpose |
|------|---------|
| `ELWGLOB.C` | Module registration, initialization, and the global-command dispatcher (`elwglobals`). |
| `GLOBROUS.C` | The individual global commands, log-on/log-off handling, telegrams, chat channels, timers. |
| `DATEROUS.C` | Date helpers. |
| `MJRTIME.C` | Time helpers. |
| `GLOSET.C` | Settings: loading, checking and saving `ELWGLOBO.DAT`, the 3.43 import, and the `=gset` editor. |
| `GLOFILES.C` | Creates a missing data file from its `.VIR` template at start-up (the board does not). |
| `GLOUSER.C` | The user menu: Globals chosen from the BBS menu, or `=config`. |
| `GLOCONN.C` | Connection type (CONT) for Who's Online. |
| `GLOCLOCK.C` | Each user's 12- or 24-hour clock for the times Globals shows. |
| `GLOSETS.H` | The list of settings (name, group, type, range, default, label). |
| `TVARS.C` | Named text variables (`GLO_xxx`) used in `ELWGLOB.MSG`. |
| `ELWGLOB.H` | Message numbers for `ELWGLOB.MSG`, generated with it from `ELWGLOB.MST`. |

`init__elwglob` registers the module, opens `ELWGLOB.MCV` and the five data
files, installs the global command handler (`globalcmd`) and the connect and
disconnect hooks, reads the text options it keeps (help labels, Who's Online flags), loads
the settings (`gs_open`), loads chat channel topics into memory, and starts
the periodic timers (`rtkick`) for auto-unsuspend, busy checks, telegram
alerts, alarms and friends notices.

## Settings

There are no configuration options in `ELWGLOB.MSG`; it holds only text
(`LEVEL6`). The settings live in `ELWGLOBO.DAT`, one 49-byte row per setting
(`struct gsrec`: the name, which is the key, and the value as text), and are
changed online with `=gset`. `GLOSETS.H` lists every setting
once; it builds both the `GS_xxx` index and the table in `GLOSET.C`, and
each line carries the default that seeds a new install and backs "reset".

- **Start-up.** `gs_open` creates the file if it is missing, reads each row,
  checks it, and writes the default for a row that is missing or no longer
  allowed (Audit Trail: `GLOBALS SETTINGS ADDED` / `GLOBALS SETTING RESET`).
  On a new file it imports `ELWGLOB.V3` (a 3.43 `ELWGLOB.MSG`) if present.
- **Lookup keys** are copied into a stack buffer first. The engine copies a
  key only from writable memory (WG33 `DFAAPI.C` `dfaAcqLock` ->
  `ISGOODPT.C` `goodblk`); a string constant gives an empty key and the
  lookup silently fails. `TESTS/TEST_GLOSET.C` checks this.
- **Live values.** `gsval[]`/`gsnum[]` (`GSSTR`, `GSNUM`, `GSCHR`) hold the
  values. `applysettings()` in `ELWGLOB.C` copies them into the module's
  existing globals and rebuilds `help[]` (keys and command strings, through
  `helpmap[]`) at start-up and after every change, so the command code reads
  the live value wherever it reads the global.
- **Side effects.** Timers re-arm through `kick()`, which records whether a
  kick is pending; `armtimers()` starts a timer switched on from 0 and never
  arms one twice (`rtkick` with 0 seconds is dropped: WG33 `RTKICK.C`).
  `chatresize()` handles a new `NUMCHAT`: the Sysop channel moves to the new
  top, users in it follow, users above the top leave chat (the rule log-on
  already applied).
- **Keys.** A blank key lets everyone in (WG33 `LOCKNKEY.C` `low_haskey`),
  so `GSETKEY` cannot be blank, and `MASTER` can always use `=gset`.
- **Command strings** must differ from each other (ignoring case, as
  `sameas` matches them); the chat channel words must differ among
  themselves.
- **The menu.** `=gset` (`gs_command`) opens it; while `gsst[usrnum]` is
  set, `elwglobals` hands every line, blank ones too, to `gs_input()`
  before anything else, which returns -1 (the engine gives the line to no
  module and the module does not prompt; WG33 `MAJORBBS.C` `hdlcri()`).
  `X` on the groups menu returns 1, so the engine has the module prompt
  again. The state is cleared at connect/log-on (`connecthandler`) and
  hang-up (`endnote`).
- **The user menu** (`GLOUSER.C`). Choosing Globals from the BBS menu
  calls `globinput` with `substt` 0, which opens it; `=config` opens it
  from anywhere (the handler returns -1: the menu shows its own prompt). Like the settings
  menu it takes every line at the top of `elwglobals` (`gu_input`, -1;
  `X` returns 1). Each change builds the global command from the live
  command strings and runs it through `elwglobals` (with a guard so the
  menu does not take its own line), so the menu cannot drift from the
  commands; the user's repeat-last-command buffer is saved around it and
  the message block stack kept balanced. `globinput` also feeds the menus
  for a channel with NOGLOB set, which skips global handlers.
- **Connection type** (`GLOCONN.C`). `tcpipinf[n].server->name` is the
  name the accepting daemon registered with `regtcpsvr()`: "Telnet",
  "SSH" (SSH Server module), "Rlogin", "FTP", "World-Wide Web". NULL is a
  channel with no TCP/IP server (LOCAL). `_tcpipinf` and `_svrhead` are
  looked up in GALTCPIP.DLL on first use (Globals is not linked with it);
  if that fails everyone shows TELNET.
- **Command prefix** (`CMDPFX`, one or two punctuation characters, `//`
  by default). A change replaces the old prefix at the start of every
  command string and default macro command (`gs_pfxswap`); a string that
  is only the old prefix repeated (`==`, read telegrams) stays, since it
  goes with the telegram character. `gs_pfxplan` checks first that no two
  commands end up the same and none outgrows its maximum, `gs_pfxapply`
  does it. Defaults follow the board's prefix (`gs_defhere`,
  `gs_followpfx`): a 3.43 import, a row added to an existing file, and a
  group reset all give `=gset` on a `=` board, never a mix. The telegram
  character is separate on purpose.
- **Chat on/off** (`=cc on`/`=cc off`, words `CHONSTG`/`CHOFSTG`). Both go
  through `setchatchannel()` (the old `=cc set` body, rate limit included);
  leaving for channel 0 stores the channel in the user's generic-database
  record (`GLOCLOCK.C` `ck_setlastchan`, two bytes of the reserved space),
  so `on` finds it after a log-off.
- **User menu sections** (`GLOUSER.C`). `items[].sect` puts each item in
  one of `GU_SECTS` sections; the first screen lists the sections that have
  an item the user may change, and a section lists its items (at most 7, so
  every screen fits 24 lines). States `GUS_MENU` -> `GUS_SECT` ->
  `GUS_VALUE`; X steps back.
- **Who's Online 1 flags**: `flagletters()` (GLOBROUS.C) builds every flag
  the user has as letters (`FLAGL1`-`FLAGL5`, order S B I O L); displays 2
  and 3 keep the first flag's name.
- **Commands on/off** (`GST_ON`, group "Commands on/off"). Each command
  string except `GSETSTG` has an ON/OFF setting whose `min` field is the
  command's `GS_xxx` index (`TESTS/TEST_SIZES.PY` checks the pairing).
  `gs_cmd(idx)` / `GSCMD(name)` return the string, or "" while it is OFF;
  `applysettings()` takes every command string through it, so an OFF
  command matches no typed word and `elwglobals` returns 0 for the line,
  which goes on to the user's module. `listglobals` skips a help entry
  with no command string; `gu_allowed` hides a user-menu item whose
  command is OFF. The string itself is kept and still counts in the
  duplicate check. Picking an ON/OFF setting in the editor flips it.
- **Clock** (`GLOCLOCK.C`). `ck_time(unum, t)` formats a `now()` time for
  channel `unum`: the engine's `nctime()` (16:45:00), or `h:mm A/P`. A
  user's choice is a record in the engine's generic user database
  (`genbb`, key user-id + "ELWGLOB", as WG33 `BBSGEN.C` `getgen`/`setgen`),
  so `ELWGLOBU.DAT` keeps its layout; the engine deletes it with the
  account (WG33 `ACCOUNT.C` `lowdel` -> `delgen`). It is read at log-on
  (`connecthandler`) into a per-channel array and cleared at hang-up
  (`endnote`). A user who has not chosen gets `DEF12H`, read when the time
  is shown.

## Text

Every changing value in `ELWGLOB.MSG` is a named text variable; no message
has a `%s`/`%d` and no `prfmsg()` call passes arguments. `TOOLS/MKMSG.PY`
refuses all three: a printf specifier in a text message, a variable that
`TVARS.H` does not register, and a `prfmsg()` with arguments.

- **Set by the caller.** `GLOTVARS` in `TVARS.H` lists them (`GLO_USERID`,
  `GLO_DATE`, `GLO_CREDITS`, ...). The caller fills each one with
  `tv_set()`, `tv_setn()` or `tv_setc()` just before `prfmsg()`. Values are
  cut to 79 characters, the engine's limit (WG33 `MENUING.C` `grbtxv`).
- **Read live.** `GLOLIVECMD` and `GLOLIVE` list the settings shown in text.
  `GLO_<setting>` returns the live value when the text is printed
  (`GSCMD`, so a command turned off reads as ""; `GSSTR` for the rest), so
  the callers set nothing and a change made in `=gset` shows at once.
- **`%` is doubled** in every value, because `prfmsg()` expands the
  variables first and then runs the text through `vsprintf()` (WG33
  `PRF.C` `prfmsg`). Notice telegrams are built with `getmsg()`, which
  expands the variables without `vsprintf()`, so `notetext()` turns the
  doubling off with `tv_raw()` around it.
- **Words** that go into variables (ON, OFF, YES, UNLIMITED, ...) are `S`
  entries `WON`, `WOFF`, ... at the end of the template, fetched with
  `getmsg()` and copied by `tv_set()` straight away.
- **Text longer than 79 characters** (telegram text, chat lines,
  broadcasts, page messages, key lists) is printed with `prf("%s", ...)`
  between the message and a second entry with the same name ending in
  `END` (`TTXTMSG`/`TTXTMEND`, ...).

## Data files

| File | Record | Contents |
|------|--------|----------|
| `ELWGLOBU.DAT` | `struct userdata` (875 bytes) | Per-user settings, macros, page and telegram blocks, log-on/log-off messages |
| `ELWGLOBC.DAT` | `struct topicdata` (32 bytes) | Chat channel topics |
| `ELWGLOBG.DAT` | `struct telegram` | Telegrams |
| `ELWGLOBS.DAT` | `struct autounsus` | Automatic unsuspend schedule |
| `ELWGLOBN.DAT` | `struct notkey` (46 bytes) | Keys taken away from a user that their class would otherwise give ("negative" keys) |

Record layouts are in `GLOBROUS.H` and must not change: existing boards'
data files depend on them.

Each user's clock choice is not in these files: it is a record in the
engine's generic user database (`struct ckrec` in `GLOCLOCK.C`: user-id,
"ELWGLOB", the choice, 31 bytes spare).
