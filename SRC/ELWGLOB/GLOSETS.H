/* Build 26.10.12.3 07:41AM */
/*****************************************************************************
 *   GLOSETS.H                                  Globals - Settings list     *
 *                                                                           *
 *   Copyright (C) 2026 Elwynor Technologies.                                *
 *   Created by Mark Laudenbach.                                             *
 *                                                                           *
 *   Every setting, one line each, in the order the online editor shows      *
 *   them (=gset).  GLOSET.H turns the list into the GS_xxx index enum and   *
 *   GLOSET.C into the settings table, so the two can never disagree.        *
 *                                                                           *
 *   X(name, group, type, min, max, default, label)                          *
 *     name     the 3.43 CNF option name; also the row key in ELWGLOBO.DAT   *
 *              and the name sysops type in the editor (8 characters max)    *
 *     min/max  numbers: the allowed range; text: max = maximum length       *
 *     default  as text, the 3.43 CNF default; seeds a new install and backs *
 *              "reset"                                                      *
 *     label    what the editor shows (34 characters max)                    *
 *                                                                           *
 *   Adding a setting: add a line (anywhere in its group).  An existing      *
 *   ELWGLOBO.DAT gets the new row, with its default, at the next start.     *
 *   Never rename a setting: its name is the key of its stored row.          *
 *                                                                           *
 *   3.43 options not carried over (3.43 never read them): CLOKKEY, UISTG.   *
 *                                                                           *
 *   Licensed under the GNU Affero General Public License v3.0.              *
 *****************************************************************************/

#define GLOSETS \
/* Access keys (3.43 LEVEL3).  A blank key lets everyone in. */ \
X(HLPKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "DEMO"   , "Global help (=?)") \
X(OFFKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "Instant log-off") \
X(PGKEY   , GSG_KEYS   , GST_KEY  ,    0,      15, "DEMO"   , "Page the console") \
X(CPGKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "SUPER"  , "Be told of console pages") \
X(BPSTPKEY, GSG_KEYS   , GST_KEY  ,    0,      15, "SUPER"  , "Stop console page beeps") \
X(SEECKEY , GSG_KEYS   , GST_KEY  ,    0,      15, "SYSOP"  , "See another user's credits") \
X(ANSTKEY , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "ANSI toggle") \
X(BSYKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "Busy flag toggle") \
X(BSYOKEY , GSG_KEYS   , GST_KEY  ,    0,      15, "SUPER"  , "Turn another user's Busy off") \
X(BSNKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "Busy notify") \
X(DUPKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "Duplex toggle") \
X(EDTKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "Editor toggle") \
X(RLGKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "Re-log") \
X(URLGKEY , GSG_KEYS   , GST_KEY  ,    0,      15, "SUPER"  , "Force another user to re-log") \
X(MBRKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "DEMO"   , "Membership information") \
X(IBMKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "IBM mode toggle") \
X(SCWKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "Set screen width") \
X(SCLKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "Set screen length") \
X(CTOKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "Continuous output toggle") \
X(UCIKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "DEMO"   , "Own class and credit info") \
X(WHOKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "DEMO"   , "Who's Online") \
X(EPSTKEY , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "Editor pester toggle") \
X(DNTKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "DEMO"   , "Date and time") \
X(RPTKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "Repeat previous command") \
X(GLBKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "Global chat channels") \
X(TGMKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "Telegrams") \
X(MACKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "Macros") \
X(TRCKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "Transfer credits") \
X(TRCRKEY , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "Receive a credit transfer") \
X(TRDKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "Transfer days") \
X(NMTDKEY , GSG_KEYS   , GST_KEY  ,    0,      15, "SUPER"  , "No minimum days for transfer") \
X(TEXTKEY , GSG_KEYS   , GST_KEY  ,    0,      15, "SUPER"  , "Extend on-line time") \
X(OTHKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "Another user's account info") \
X(ACSTKEY , GSG_KEYS   , GST_KEY  ,    0,      15, "DEMO"   , "Own account settings") \
X(PSTCKEY , GSG_KEYS   , GST_KEY  ,    0,      15, "SUPER"  , "Post credits") \
X(BTUKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "SUPER"  , "Boot a user off line") \
X(LOGKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "Log-on notices") \
X(LCLRKEY , GSG_KEYS   , GST_KEY  ,    0,      15, "SUPER"  , "Clear another user's log messages") \
X(SGNKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "SUPER"  , "List new sign-ups") \
X(SYSBKEY , GSG_KEYS   , GST_KEY  ,    0,      15, "SUPER"  , "System-wide broadcast") \
X(PDYKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "SUPER"  , "Post days in class") \
X(SWCKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "SYSOP"  , "Switch a user's class") \
X(GTUKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "SUPER"  , "Give or take a user's key") \
X(INVKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "SUPER"  , "Invisibility toggle") \
X(SUSKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "SUPER"  , "Suspend or unsuspend a user") \
X(DLUKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "SYSOP"  , "Delete or undelete a user") \
X(OUSKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "SUPER"  , "Another user's settings") \
X(PAGKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "Page commands") \
X(ALRMKEY , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "Alarm command") \
X(DISKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "SYSOP"  , "Display who is on chat channels") \
X(SYSCKEY , GSG_KEYS   , GST_KEY  ,    0,      15, "SYSOP"  , "Sysop chat channel") \
X(CLRKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "SUPER"  , "Clear chat channel topics") \
X(TDAKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "SYSOP"  , "Remove all telegrams by a user") \
X(COLKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "Colorize input") \
X(GCIKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "SUPER"  , "Chat while invisible") \
X(PGIKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "SUPER"  , "Page while invisible") \
X(TMIKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "SUPER"  , "Telegram while invisible") \
X(SYSLKEY , GSG_KEYS   , GST_KEY  ,    0,      15, "SUPER"  , "See invisible log-ons/log-offs") \
X(PTUKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "SUPER"  , "Page through page OFF or OK") \
X(TNKKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "SUPER"  , "Telegrams never blocked/cleared") \
X(NSUSKEY , GSG_KEYS   , GST_KEY  ,    0,      15, "MASTER" , "Cannot be suspended") \
X(NDELKEY , GSG_KEYS   , GST_KEY  ,    0,      15, "MASTER" , "Cannot be deleted") \
X(UNLTRKEY, GSG_KEYS   , GST_KEY  ,    0,      15, "SUPER"  , "Transfer credits without limits") \
X(UNLPTKEY, GSG_KEYS   , GST_KEY  ,    0,      15, "SYSOP"  , "Post credits without limits") \
X(FRNDKEY , GSG_KEYS   , GST_KEY  ,    0,      15, "NORMAL" , "Friends Notice") \
X(PGBSYKEY, GSG_KEYS   , GST_KEY  ,    0,      15, "SYSOP"  , "Page through Busy/Editor pester") \
X(GSETKEY , GSG_KEYS   , GST_KEYNB,    0,      15, "SYSOP"  , "Change these settings (=gset)") \
X(CFGKEY  , GSG_KEYS   , GST_KEY  ,    0,      15, "DEMO"   , "Own settings menu (=config)") \
/* Command strings.  Each must differ from every other one.  CMDPFX is */ \
/* what most of them start with (one or two characters): changing it   */ \
/* changes them all.  Read telegrams (==) goes with the telegram       */ \
/* character, not the prefix. */ \
X(CMDPFX  , GSG_CMDS   , GST_PFX  ,    0,       2, "="      , "Prefix of the command strings") \
X(HLPSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=?"     , "Help on global commands") \
X(OFFSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=x"     , "Instant log-off") \
X(CNTSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=ct"    , "Continuous output toggle") \
X(BPSTG   , GSG_CMDS   , GST_CMD  ,    0,      10, "=e"     , "Page the console") \
X(BPSTPSTG, GSG_CMDS   , GST_CMD  ,    0,      10, "=q"     , "End a console page") \
X(ANSSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=a"     , "ANSI toggle") \
X(BSYSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=b"     , "Busy flag toggle") \
X(BNTSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=bn"    , "Busy notify") \
X(DUPSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=du"    , "Duplex toggle") \
X(EDTSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=ed"    , "Editor type") \
X(RLGSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=re"    , "Re-log") \
X(MBRSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=$"     , "Membership information") \
X(IBMSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=g"     , "IBM mode toggle") \
X(SWSTG   , GSG_CMDS   , GST_CMD  ,    0,      10, "=sw"    , "Screen width") \
X(SLSTG   , GSG_CMDS   , GST_CMD  ,    0,      10, "=s"     , "Screen length") \
X(CRDSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=c"     , "Class and credit info") \
X(WHOSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "#"      , "Who's Online") \
X(EPSTSTG , GSG_CMDS   , GST_CMD  ,    0,      10, "=pest"  , "Editor pester toggle") \
X(DNTSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=dt"    , "Date and time") \
X(PSTSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=pc"    , "Post credits") \
X(ZAPSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=zap"   , "Boot a user off line") \
X(RPTSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=r"     , "Repeat previous command") \
X(NEWSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=ln"    , "List new sign-ups") \
X(LOGSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=log"   , "Log-on notices") \
X(LONSTR  , GSG_CMDS   , GST_CMD  ,    0,      15, "=lon"   , "Edit log-on message") \
X(LOFFSTR , GSG_CMDS   , GST_CMD  ,    0,      15, "=loff"  , "Edit log-off message") \
X(LCLRSTR , GSG_CMDS   , GST_CMD  ,    0,      15, "=lclear" , "Clear a user's log messages") \
X(SHOWSTR , GSG_CMDS   , GST_CMD  ,    0,      15, "=lshow"  , "Show a user's log messages") \
X(CHATSTG , GSG_CMDS   , GST_CMD  ,    0,      10, "=cc"    , "Chat channels") \
X(GTLSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=="     , "Read telegrams") \
X(TALTSTG , GSG_CMDS   , GST_CMD  ,    0,      10, "=ta"    , "Telegram notify toggle") \
X(MACSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=m"     , "Macro help") \
X(MCSET   , GSG_CMDS   , GST_CMD  ,    0,      10, "=mc"    , "Set a macro command") \
X(MMSET   , GSG_CMDS   , GST_CMD  ,    0,      10, "=mm"    , "Set a macro") \
X(MCLIST  , GSG_CMDS   , GST_CMD  ,    0,      10, "=mcl"   , "List macro commands") \
X(MMLIST  , GSG_CMDS   , GST_CMD  ,    0,      10, "=mml"   , "List macros") \
X(GLBSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=!"     , "System-wide broadcast") \
X(TRNSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=trc"   , "Transfer credits") \
X(TRDSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=trd"   , "Transfer days") \
X(TEXTSTG , GSG_CMDS   , GST_CMD  ,    0,      10, "=te"    , "Extend on-line time") \
X(PDYSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=pd"    , "Post days in class") \
X(CSTSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=cs"    , "Account settings") \
X(SWCSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=sc"    , "Switch a user's class") \
X(UKSTG   , GSG_CMDS   , GST_CMD  ,    0,      10, "=uk"    , "Give or take a user's key") \
X(INVSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=in"    , "Invisibility toggle") \
X(ASUSSTG , GSG_CMDS   , GST_CMD  ,    0,      10, "=sa"    , "Suspend with auto-unsuspend") \
X(SUSSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=su"    , "Suspend/unsuspend toggle") \
X(DLUSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=ku"    , "Delete/undelete toggle") \
X(PGUSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=p"     , "Page another user") \
X(FPUSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=pf"    , "Block a user's pages") \
X(RPGSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=pr"    , "Re-send the last page") \
X(TCLRSTG , GSG_CMDS   , GST_CMD  ,    0,      10, "=td"    , "Delete telegrams from a user") \
X(TDASTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=tx"    , "Remove a user's telegrams (all)") \
X(TBKSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=tb"    , "Block a user's telegrams") \
X(ALRMSTG , GSG_CMDS   , GST_CMD  ,    0,      10, "=*"     , "Alarm") \
X(FRNDSTG , GSG_CMDS   , GST_CMD  ,    0,      10, "=+"     , "Friends Notice") \
X(GSETSTG , GSG_CMDS   , GST_CMD  ,    0,      10, "=gset"  , "Change these settings") \
X(CFGSTG  , GSG_CMDS   , GST_CMD  ,    0,      10, "=config" , "Own settings menu") \
/* Commands on/off: one per command string above, except =gset (so the  */ \
/* sysop cannot shut themselves out).  "min" is the command's GS_xxx;   */ \
/* the editor shows that command's label and string.  OFF: Globals      */ \
/* ignores the command, and the line goes on to the user's module.      */ \
X(ONHLP   , GSG_ONOFF  , GST_ON   , GS_HLPSTG,   0, "ON"     , "") \
X(ONOFF   , GSG_ONOFF  , GST_ON   , GS_OFFSTG,   0, "ON"     , "") \
X(ONCNT   , GSG_ONOFF  , GST_ON   , GS_CNTSTG,   0, "ON"     , "") \
X(ONBP    , GSG_ONOFF  , GST_ON   , GS_BPSTG,    0, "ON"     , "") \
X(ONBPSTP , GSG_ONOFF  , GST_ON   , GS_BPSTPSTG, 0, "ON"     , "") \
X(ONANS   , GSG_ONOFF  , GST_ON   , GS_ANSSTG,   0, "ON"     , "") \
X(ONBSY   , GSG_ONOFF  , GST_ON   , GS_BSYSTG,   0, "ON"     , "") \
X(ONBNT   , GSG_ONOFF  , GST_ON   , GS_BNTSTG,   0, "ON"     , "") \
X(ONDUP   , GSG_ONOFF  , GST_ON   , GS_DUPSTG,   0, "ON"     , "") \
X(ONEDT   , GSG_ONOFF  , GST_ON   , GS_EDTSTG,   0, "ON"     , "") \
X(ONRLG   , GSG_ONOFF  , GST_ON   , GS_RLGSTG,   0, "ON"     , "") \
X(ONMBR   , GSG_ONOFF  , GST_ON   , GS_MBRSTG,   0, "ON"     , "") \
X(ONIBM   , GSG_ONOFF  , GST_ON   , GS_IBMSTG,   0, "ON"     , "") \
X(ONSW    , GSG_ONOFF  , GST_ON   , GS_SWSTG,    0, "ON"     , "") \
X(ONSL    , GSG_ONOFF  , GST_ON   , GS_SLSTG,    0, "ON"     , "") \
X(ONCRD   , GSG_ONOFF  , GST_ON   , GS_CRDSTG,   0, "ON"     , "") \
X(ONWHO   , GSG_ONOFF  , GST_ON   , GS_WHOSTG,   0, "ON"     , "") \
X(ONEPST  , GSG_ONOFF  , GST_ON   , GS_EPSTSTG,  0, "ON"     , "") \
X(ONDNT   , GSG_ONOFF  , GST_ON   , GS_DNTSTG,   0, "ON"     , "") \
X(ONPST   , GSG_ONOFF  , GST_ON   , GS_PSTSTG,   0, "ON"     , "") \
X(ONZAP   , GSG_ONOFF  , GST_ON   , GS_ZAPSTG,   0, "ON"     , "") \
X(ONRPT   , GSG_ONOFF  , GST_ON   , GS_RPTSTG,   0, "ON"     , "") \
X(ONNEW   , GSG_ONOFF  , GST_ON   , GS_NEWSTG,   0, "ON"     , "") \
X(ONLOG   , GSG_ONOFF  , GST_ON   , GS_LOGSTG,   0, "ON"     , "") \
X(ONLON   , GSG_ONOFF  , GST_ON   , GS_LONSTR,   0, "ON"     , "") \
X(ONLOFF  , GSG_ONOFF  , GST_ON   , GS_LOFFSTR,  0, "ON"     , "") \
X(ONLCLR  , GSG_ONOFF  , GST_ON   , GS_LCLRSTR,  0, "ON"     , "") \
X(ONSHOW  , GSG_ONOFF  , GST_ON   , GS_SHOWSTR,  0, "ON"     , "") \
X(ONCHAT  , GSG_ONOFF  , GST_ON   , GS_CHATSTG,  0, "ON"     , "") \
X(ONGTL   , GSG_ONOFF  , GST_ON   , GS_GTLSTG,   0, "ON"     , "") \
X(ONTALT  , GSG_ONOFF  , GST_ON   , GS_TALTSTG,  0, "ON"     , "") \
X(ONMAC   , GSG_ONOFF  , GST_ON   , GS_MACSTG,   0, "ON"     , "") \
X(ONMCSET , GSG_ONOFF  , GST_ON   , GS_MCSET,    0, "ON"     , "") \
X(ONMMSET , GSG_ONOFF  , GST_ON   , GS_MMSET,    0, "ON"     , "") \
X(ONMCLIST, GSG_ONOFF  , GST_ON   , GS_MCLIST,   0, "ON"     , "") \
X(ONMMLIST, GSG_ONOFF  , GST_ON   , GS_MMLIST,   0, "ON"     , "") \
X(ONGLB   , GSG_ONOFF  , GST_ON   , GS_GLBSTG,   0, "ON"     , "") \
X(ONTRN   , GSG_ONOFF  , GST_ON   , GS_TRNSTG,   0, "ON"     , "") \
X(ONTRD   , GSG_ONOFF  , GST_ON   , GS_TRDSTG,   0, "ON"     , "") \
X(ONTEXT  , GSG_ONOFF  , GST_ON   , GS_TEXTSTG,  0, "ON"     , "") \
X(ONPDY   , GSG_ONOFF  , GST_ON   , GS_PDYSTG,   0, "ON"     , "") \
X(ONCST   , GSG_ONOFF  , GST_ON   , GS_CSTSTG,   0, "ON"     , "") \
X(ONSWC   , GSG_ONOFF  , GST_ON   , GS_SWCSTG,   0, "ON"     , "") \
X(ONUK    , GSG_ONOFF  , GST_ON   , GS_UKSTG,    0, "ON"     , "") \
X(ONINV   , GSG_ONOFF  , GST_ON   , GS_INVSTG,   0, "ON"     , "") \
X(ONASUS  , GSG_ONOFF  , GST_ON   , GS_ASUSSTG,  0, "ON"     , "") \
X(ONSUS   , GSG_ONOFF  , GST_ON   , GS_SUSSTG,   0, "ON"     , "") \
X(ONDLU   , GSG_ONOFF  , GST_ON   , GS_DLUSTG,   0, "ON"     , "") \
X(ONPGU   , GSG_ONOFF  , GST_ON   , GS_PGUSTG,   0, "ON"     , "") \
X(ONFPU   , GSG_ONOFF  , GST_ON   , GS_FPUSTG,   0, "ON"     , "") \
X(ONRPG   , GSG_ONOFF  , GST_ON   , GS_RPGSTG,   0, "ON"     , "") \
X(ONTCLR  , GSG_ONOFF  , GST_ON   , GS_TCLRSTG,  0, "ON"     , "") \
X(ONTDA   , GSG_ONOFF  , GST_ON   , GS_TDASTG,   0, "ON"     , "") \
X(ONTBK   , GSG_ONOFF  , GST_ON   , GS_TBKSTG,   0, "ON"     , "") \
X(ONALRM  , GSG_ONOFF  , GST_ON   , GS_ALRMSTG,  0, "ON"     , "") \
X(ONFRND  , GSG_ONOFF  , GST_ON   , GS_FRNDSTG,  0, "ON"     , "") \
X(ONCFG   , GSG_ONOFF  , GST_ON   , GS_CFGSTG,   0, "ON"     , "") \
/* Chat channels.  The words typed after the chat command differ from */ \
/* each other, not from the command strings above. */ \
X(ECHSTG  , GSG_CHAT   , GST_SUB  ,    0,      10, "echo"   , "Word: echo toggle") \
X(SETSTG  , GSG_CHAT   , GST_SUB  ,    0,      10, "set"    , "Word: set channel") \
X(SHOWSTG , GSG_CHAT   , GST_SUB  ,    0,      10, "show"   , "Word: show topics") \
X(DISSTG  , GSG_CHAT   , GST_SUB  ,    0,      10, "display", "Word: display who is on") \
X(TPCSTG  , GSG_CHAT   , GST_SUB  ,    0,      10, "topic"  , "Word: change topic") \
X(NOTSTG  , GSG_CHAT   , GST_SUB  ,    0,      10, "notice" , "Word: join/leave notices") \
X(CLRSTG  , GSG_CHAT   , GST_SUB  ,    0,      10, "clear"  , "Word: clear topics") \
X(CHONSTG , GSG_CHAT   , GST_SUB  ,    0,      10, "on"     , "Word: chat back on") \
X(CHOFSTG , GSG_CHAT   , GST_SUB  ,    0,      10, "off"    , "Word: chat off") \
X(SYSTOPIC, GSG_CHAT   , GST_STR  ,    0,      29, "Sysop Chat Channel", "Sysop channel topic") \
X(LOGCHNOT, GSG_CHAT   , GST_BOOL ,    0,       1, "YES"    , "Channel notices at log-on/off") \
X(NUMCHAT , GSG_CHAT   , GST_NUM  , 1000,   10000, "10000"  , "Chat channels (top = Sysop's)") \
X(STPCHG  , GSG_CHAT   , GST_BOOL ,    0,       1, "YES"    , "Topic changes in Audit Trail") \
X(CHKCHG  , GSG_CHAT   , GST_NUM  ,    0,      10, "3"      , "Seconds between channel changes") \
/* Default macros for new users */ \
X(MACCM0  , GSG_MACROS , GST_WORD ,    0,       4, "=m0"    , "Macro 1 command") \
X(MACCM1  , GSG_MACROS , GST_WORD ,    0,       4, "=m1"    , "Macro 2 command") \
X(MACCM2  , GSG_MACROS , GST_WORD ,    0,       4, "=m2"    , "Macro 3 command") \
X(MACCM3  , GSG_MACROS , GST_WORD ,    0,       4, "=m3"    , "Macro 4 command") \
X(MACCM4  , GSG_MACROS , GST_WORD ,    0,       4, "=m4"    , "Macro 5 command") \
X(MACCM5  , GSG_MACROS , GST_WORD ,    0,       4, "=m5"    , "Macro 6 command") \
X(MACCM6  , GSG_MACROS , GST_WORD ,    0,       4, "=m6"    , "Macro 7 command") \
X(MACCM7  , GSG_MACROS , GST_WORD ,    0,       4, "=m7"    , "Macro 8 command") \
X(MACCM8  , GSG_MACROS , GST_WORD ,    0,       4, "=m8"    , "Macro 9 command") \
X(MACCM9  , GSG_MACROS , GST_WORD ,    0,       4, "=m9"    , "Macro 10 command") \
X(MACMC0  , GSG_MACROS , GST_STR  ,    0,      39, ""       , "Macro 1 text") \
X(MACMC1  , GSG_MACROS , GST_STR  ,    0,      39, ""       , "Macro 2 text") \
X(MACMC2  , GSG_MACROS , GST_STR  ,    0,      39, ""       , "Macro 3 text") \
X(MACMC3  , GSG_MACROS , GST_STR  ,    0,      39, ""       , "Macro 4 text") \
X(MACMC4  , GSG_MACROS , GST_STR  ,    0,      39, ""       , "Macro 5 text") \
X(MACMC5  , GSG_MACROS , GST_STR  ,    0,      39, ""       , "Macro 6 text") \
X(MACMC6  , GSG_MACROS , GST_STR  ,    0,      39, ""       , "Macro 7 text") \
X(MACMC7  , GSG_MACROS , GST_STR  ,    0,      39, ""       , "Macro 8 text") \
X(MACMC8  , GSG_MACROS , GST_STR  ,    0,      39, ""       , "Macro 9 text") \
X(MACMC9  , GSG_MACROS , GST_STR  ,    0,      39, ""       , "Macro 10 text") \
/* New-user defaults */ \
X(DEFCC   , GSG_NEWUSER, GST_NUM  ,    0,     100, "0"      , "Chat channel (0 = off)") \
X(DEFECH  , GSG_NEWUSER, GST_NUM  ,    0,       1, "1"      , "Chat echo (1 = on)") \
X(DEFDUP  , GSG_NEWUSER, GST_NUM  ,    0,       1, "1"      , "Full duplex (1 = on)") \
X(DEFPAG  , GSG_NEWUSER, GST_NUM  ,    0,       2, "2"      , "Pages (0 off, 1 OK, 2 on)") \
X(DEFTEL  , GSG_NEWUSER, GST_NUM  ,    0,       1, "1"      , "Telegram notify (1 = on)") \
X(DEFAGE  , GSG_NEWUSER, GST_BOOL ,    0,       1, "YES"    , "Show age in Who's Online") \
X(DEFWHO  , GSG_NEWUSER, GST_NUM  ,    1,       3, "1"      , "Who's Online display (1-3)") \
X(DEFPST  , GSG_NEWUSER, GST_NUM  ,    0,       1, "0"      , "Pester in the Editor (1 = yes)") \
X(DEFLOG  , GSG_NEWUSER, GST_NUM  ,    0,       1, "1"      , "Log-on notices (1 = on)") \
X(DEFCHN  , GSG_NEWUSER, GST_NUM  ,    0,       1, "1"      , "Join/leave notices (1 = on)") \
X(DEFLON  , GSG_NEWUSER, GST_STR  ,    0,      39, ""       , "Log-on message") \
X(DEFLOF  , GSG_NEWUSER, GST_STR  ,    0,      39, ""       , "Log-off message") \
X(DEF12H  , GSG_NEWUSER, GST_BOOL ,    0,       1, "NO"     , "12-hour clock (4:45 PM)") \
/* Paging and beeps */ \
X(BPDUR   , GSG_PAGING , GST_NUM  ,    0,     120, "60"     , "Beeps in a console page") \
X(BPPIT   , GSG_PAGING , GST_NUM  ,    0,    2000, "1000"   , "Console page pitch (Hz)") \
X(PGDLY   , GSG_PAGING , GST_NUM  ,    0,     300, "120"    , "Seconds between console pages") \
X(PGALL   , GSG_PAGING , GST_BOOL ,    0,       1, "YES"    , "Console page pages every Sysop") \
X(DOBEEPS , GSG_PAGING , GST_BOOL ,    0,       1, "YES"    , "Beep users at some prompts") \
X(ALRMBPS , GSG_PAGING , GST_NUM  ,    0,      10, "5"      , "Beeps in an alarm notice") \
/* Timers (seconds) */ \
X(CHKSECS , GSG_TIMERS , GST_NUM  ,    1,      10, "1"      , "Check for users no longer Busy") \
X(CHKUNS  , GSG_TIMERS , GST_NUM  ,    0,   28800, "60"     , "Auto-unsuspend check (0 = off)") \
X(CHKTELS , GSG_TIMERS , GST_NUM  ,    0,       5, "2"      , "Telegrams for Busy users (0=off)") \
X(CHKALRM , GSG_TIMERS , GST_NUM  ,    1,       5, "2"      , "Alarm check") \
X(CHKFRND , GSG_TIMERS , GST_NUM  ,    1,       5, "2"      , "Friends Notice check") \
/* Credits and days */ \
X(MINTRD  , GSG_CREDITS, GST_NUM  ,    1,     999, "7"      , "Minimum days in a transfer") \
X(MNTRFR  , GSG_CREDITS, GST_NUM  ,    1,  999999, "3600"   , "Minimum credits in a transfer") \
X(MXPST   , GSG_CREDITS, GST_NUM  ,    1, 9999999, "3600"   , "Maximum credits in one post") \
/* General */ \
X(TLCCHR  , GSG_GENERAL, GST_CHR  ,    0,       0, "="      , "Character that sends a telegram") \
X(ANSCHR  , GSG_GENERAL, GST_CHR  ,    0,       0, "\\"     , "Character that starts a color") \
X(BCKCHR  , GSG_GENERAL, GST_CHR  ,    0,       0, "b"      , "Color code: background") \
X(FRECHR  , GSG_GENERAL, GST_CHR  ,    0,       0, "f"      , "Color code: foreground") \
X(SPCCHR  , GSG_GENERAL, GST_CHR  ,    0,       0, "s"      , "Color code: special effect") \
X(ENBNUM  , GSG_GENERAL, GST_NUM  ,    0,       7, "0"      , "Background after colored input") \
X(LOGLIST , GSG_GENERAL, GST_BOOL ,    0,       1, "YES"    , "Who's Online shown at log-on") \
X(SNKUSER , GSG_GENERAL, GST_BOOL ,    0,       1, "YES"    , "Notices for forced invisible") \
X(SNEAK   , GSG_GENERAL, GST_BOOL ,    0,       1, "YES"    , "Notices for invisibility toggle") \
X(SHOWPG  , GSG_GENERAL, GST_BOOL ,    0,       1, "YES"    , "Show page title, not Menuing") \
X(SNDTHRU , GSG_GENERAL, GST_BOOL ,    0,       1, "YES"    , "Broadcasts reach Busy users") \
X(SHWMINV , GSG_GENERAL, GST_BOOL ,    0,       1, "YES"    , "Staff see invisible Sysops") \
/* Module names (as in the module's .MDF description) */ \
X(MNUNAM  , GSG_MODULES, GST_STR  ,    0,      24, "Menuing System", "Menuing System") \
X(EDTNAM  , GSG_MODULES, GST_STR  ,    0,      24, "Editor" , "Editor") \
X(FILNAM  , GSG_MODULES, GST_STR  ,    0,      24, "File Transfer Service", "File Transfer Service") \
X(BLOCK1  , GSG_MODULES, GST_STR  ,    0,      24, ""       , "No globals in module 1") \
X(BLOCK2  , GSG_MODULES, GST_STR  ,    0,      24, ""       , "No globals in module 2") \
X(BLOCK3  , GSG_MODULES, GST_STR  ,    0,      24, ""       , "No globals in module 3") \
X(BLOCK4  , GSG_MODULES, GST_STR  ,    0,      24, ""       , "No globals in module 4") \
X(BLOCK5  , GSG_MODULES, GST_STR  ,    0,      24, ""       , "No globals in module 5")
