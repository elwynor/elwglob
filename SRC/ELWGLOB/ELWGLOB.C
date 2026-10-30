/* Build 26.10.29.2 05:10PM */
/*****************************************************************************
 *                                                                           *
 *       EZEGLOBS.C - Global commands for the Major BBS.                     *
 * Developed 07/25/94 by 'Easy Ed' Erdman.    Major BBS developer code 'EZE'.*
 *                                                                           *
 *       Updates : 12/18/94 - 2.0                                            *
 *                 Complete upgrade including:                               *
 *                       Continuous output toggle                            *
 *                       Bug fix in screen length command                    *
 *                       Increase maximum number of global chat channels     *
 *                       Store user's global chat channels in memory and     *
 *                               reduce disk accesses for chat channels.     *
 *                       Change lastcommand to array storage rather than     *
 *                               pointer.                                    *
 *                       Put all text blocks in .MSG file                    *
 *                       Add setmbk() and rstmbk() pairs to code in each     *
 *                               function.                                   *
 *                       Place Who's Online output in .MSG and cover         *
 *                               invisible Sysops at log-on and log-off.     *
 *                       Use array storage for =r repeat previous command    *
 *                               instead of dynamically allocated memory.    *
 *                       Add limit to amount of credits non-Sysops can post  *
 *                       Add overflow checks for amount of credits posted    *
 *                       Allow changing of global chat parameters, such as   *
 *                               display, show, etc.                         *
 *                       Allow read telegrams command string to be changed   *
 *                       Fix bug that only deletes first telegram to a       *
 *                               which is being deleted.                     *
 *                       Change telegrams so that users can't send telegrams *
 *                               to a user marked for deletion.              *
 *                       Change telegrams so that less disk accesses are     *
 *                               made by use of vdaptr and arrays.           *
 *                       Added macros with user-definable command strings;   *
 *                               ten macros and command strings are allowed, *
 *                               with up to 5 characters for each command    *
 *                               string, and up to 40 characters for each    *
 *                               macro.                                      *
 *                       Fix bug in ANSI setting that did not immediately    *
 *                               set user's ANSI mode.                       *
 *                       Allow character to send telegrams to be changed as  *
 *                               desired by Sysop.                           *
 *                       Add system-wide broadcasts command                  *
 *                       Add credit transfers                                *
 *                       Add post days in class                              *
 *                       Add information on other user command               *
 *                       Add current settings command                        *
 *                       Add switch user class command                       *
 *                       Add give/take user keys commmand                    *
 *                       Make each globally individually keyed               *
 *                       Set memory size of user-id input in post credits to *
 *                               INPSIZ to avoid possible GP.                *
 *                       Add invisibility toggle                             *
 *                       Updated boot command and added message to Audit     *
 *                               Trail.                                      *
 *                       Add suspend user toggle                             *
 *                       Add delete user toggle                              *
 *                       Add toggle for echo of global chat output to user   *
 *                       Fix bug that allowed Sysops to be suspended using   *
 *                               the =zap <user-id> /s command.              *
 *                       Clean up =? output, and put help string and usage   *
 *                               details into each global's help message     *
 *                       Add command to get another user's current settings  *
 *                       Add page another user with re-page and forget user  *
 *                       Add handle and topic of global chat channel topic   *
 *                               changes to the Audit Trail                  *
 *                       Add chat channel topic clearing commands            *
 *                       Add duplex (echo) toggle command                    *
 *                       Allow repeated console beeps for paging             *
 *                       Add notification of credit posting in telegram      *
 *                       Add toggle for notification of telegrams received   *
 *                       Add feature to delete all telegrams from a specified*
 *                               user.                                       *
 *                       Add feature to delete all telegrams in database     *
 *                               from a specified user (Sysop only).         *
 *                       Add feature to help to display only commands user   *
 *                               has access to on help menu.                 *
 *                       Add option to add more input to repeat last command *
 *                               function.                                   *
 *                       Add option to add more input to repeat last command *
 *                               function.                                   *
 *                       Add telegram blocks feature                         *
 *                       Add option to allow forcing another user to re-log  *
 *                       Add colorize input command                          *
 *                       Add option to toggle other users invisible          *
 *                       Add checks for invisibility before allowing telegram*
 *                               sends and replies, chat channel output, and *
 *                               paging.                                     *
 *		6/05/95 - 3.0                                                        *
 *		Minor upgrade including:                                             *
 *			Update to new access code by registration number                 *
 *			Display users on your chat channel when getting help             *
 *				and show when users enter your chat channel.                 *
 *			Fix bug showing bogus screen width setting when user types =sw.  *
 *			Fix bug showing bogus screen length setting when user types =s.  *
 *			Make command toggling another user invisible to be permanent.    *
 *			Add to output of user settings and other user                    *
 *				settings information on user's invisibility                  *
 *				and invisibility with respect to this module.                *
 *			Add option to display users on-line automatically at log-on.     *
 *			Place flag strings in .msg for Who's Online                      *
 *			Add option as to whether users with the key to                   *
 *				toggle invisibility can see MASTER key                       *
 *				holders who are invisible.                                   *
 *			Add "Busy" notifier routine                                      *
 *			Make "beep" at user's computer an option                         *
 *			Add Log-on notifier routine                                      *
 *			Add option to display Log-on and Log-off notices for             *
 *				Sysops who toggle invisibility.                              *
 *			Use more functions to take code out of ezeglobs()                *
 *			Display current page in Who's Online instead of "Menuing System" *
 *			Add toggle for users to not be bothered by module's              *
 *				routines while they are in the Editor.                       *
 *			Notify only users who can use telegrams that they've             *
 *				received a telegram.                                         *
 *			Allow console pages to be ended at BBS computer                  *
 *				without logging on by typing ALT_Q.                          *
 *			Allow multiple choices for Who's Online output                   *
 *			Optimize routines to decrease module size, and                   *
 *				allow immediate updating of user data to disk.               *
 *			Replace all bootem() calls to calls of bbsdsc() to               *
 *				avoid lockups caused by /zap.                                *
 *			Change post credits global to update totcreds and                *
 *				totpaid user account fields.                                 *
 *               8/21/95  - 3.1                                              *
 *               Minor upgrade including:                                    *
 *			Make all possible functions STATIC functions                     *
 *			Eliminate redundant global variables (command                    *
 *				strings and some keys).                                      *
 *			Allow re-log of user in The Editor or any character              *
 *				interceptor, and don't allow re-logs if in                   *
 *				Opchat or file transfer.                                     *
 *			Allow giving and taking of any key to user's                     *
 *				individual keys whether in class keyring or                  *
 *				not (note that this does not allow a Sysop                   *
 *				to remove keys in the class keyring).                        *
 *			Give users with key accurate users on-line counts                *
 *				with log notices, and send fake logs only                    *
 *				to users without this key.                                   *
 *			Allow key for paging users with Page set to OFF or               *
 *				OK, and to not have pages blocked.                           *
 *			Add key for user to not have telegrams blocked or                *
 *				deleted, and to have the user who receives                   *
 *				a telegram always be notified of this.                       *
 *			Update Colorize routine to make more efficient                   *
 *			Show new user's city and state in the new user                   *
 *				sign-up listing.                                             *
 *			Update Who's Online to show an invisible MASTER key              *
 *				holder logging on or off only to users who                   *
 *				are allowed to see this.                                     *
 *			Allow users who can page while invisible to also be              *
 *				able to page invisible users they can "see".                 *
 *			Add ability for Sysop to force a user "unbusy"                   *
 *			Allow users to be notified when "busy" users they                *
 *				can "see" are no longer "busy", page such                    *
 *				users, re-log them and boot them off-line.                   *
 *			Allow Sysop to set default macro command strings                 *
 *			Allow Sysop to name Sysop Chat Channel                           *
 *			Add time of last call to user info                               *
 *			Don't allow users in Opchat to be booted off-line                *
 *			Remove /s switch from Boot User global                           *
 *			Add keys for not being able to suspend or delete a               *
 *				user (default had been MASTER).                              *
 *			Add key for unlimited credit transfer (default was MASTER).      *
 *			Fix minor bug in post credits command in determining             *
 *				valid credit amount to post.                                 *
 *			Add transfer days in class command                               *
 *			Show Global user settings of off-line users                      *
 *			Fix minor bug in other user settings screen                      *
 *			Add toggle for notices of users joining/leaving chat channels.   *
 *			Add key to post credits without restriction (default was MASTER).*
 *			Fix bug in suspend with auto unsuspend where on-line             *
 *				users were booted but not suspended.                         *
 *			Add time extend command                                          *
 *			Add ability for Sysop to select all default Globals              *
 *				settings for new users.                                      *
 *			Allow option to switch class for desired # of days               *
 *			List macro commands and macros in current settings               *
 *			Add "negative" keys                                              *
 *			Add key user must have to receive credit transfers               *
 *			List users on global chat channel when joining a                 *
 *				channel (=cc set #).                                         *
 *			Fix bug that caused GP if date entered for list new              *
 *				users was very long or bogus.                                *
 *			Add option to send global message broadcasts to                  *
 *				users who are "Busy".                                        *
 *			Make "Busy" flag permanent                                       *
 *			Allow users to choose whether to show their ages in              *
 *				the Who's Online display.                                    *
 *			Allow users to choose which available Who's Online               *
 *				list to view and to select a default.                        *
 *		2/22/96  - 3.2                                                       *
 *		Minor upgrade for WG compatibility:                                  *
 *			Hook connect and disconnect handlers for C/S mode                *
 *				users for keys, invisible, log notices, etc.                 *
 *			Split source code into 2 files due to code size                  *
 *			Add up to 5 modules in which not to process global               *
 *				commands if users are in these, for                          *
 *				compatibility with games like TradeWars 2002, etc.           *
 *			Fix bugs in transfer credits and days in class                   *
 *				functions which allow users to cheat the system.             *
 *			Allow "Busy" users to be notified of new telegrams               *
 *				when they are no longer "Busy".                              *
 *			Allow pages to be converted to telegrams when a user is "Busy".  *
 *			Add command to alert the user when it is a certain               *
 *				time, with message.                                          *
 *			Add command to alert user when a specific person                 *
 *				logs onto or off of the system.                              *
 *			Add optional delay between chat channel topic changes.           *
 *			Add ability for keyed users to page thru "Busy"                  *
 *			Allow users with key to page thru "Busy" to also                 *
 *				page users in The Editor.                                    *
 *			Change Busy Notify command so that it continues to               *
 *				try to tell a user who is "Busy" that the                    *
 *				person they wish to be notified about is no                  *
 *				longer "Busy".                                               *
 *			Fix bug in time of day comparisons                               *
 *			Change DEMO period to 14 days rather 2 hours                     *
 *			Allow exiting reading of telegrams to not delete the             *
 *				telegram just read.                                          *
 *               5/25/96  - 3.3                                              *
 *               Extremely minor upgrade including:                          *
 *			Change method of generating access codes                         *
 *		4/6/97  - 3.3W                                                       *
 *		Add compatibility for Worldgroup 3.0 WIN NT/95.                      *
 *               Detect when menu page title is blank for Who's Online       *
 *                      location so a blank string isn't displayed.          *
 *               Allow user to determine whether to inform other person of   *
 *                      the use of certain commands such as relog other, give*
 *                      and take key, etc.                                   *
 *		Add key users must have to see other user's credits when             *
 *		       looking up info on others.                                    *
 *               Fix bug in suspend user command where online user was not   *
 *                      suspended, and message was printed twice.            *
 *		Don't display SLIP and other non-users in Who's Online               *
 *               Poll list new user, list users suspended with auto          *
 *                      unsuspend, and clear chat channel topic commands.    *
 *		Fix bug in set screen width command where usage wasn't               *
 *		       displayed properly.                                           *
 *		Allow only MASTER key holders to toggle the invisibility of          *
 *		       MASTER key holders.                                           *
 *		5/19/2021  - 3.4                                                     *
 *                  Worldgroup 3.2 Compatibility                             *
 *     8/1/2024   - 3.41                                                     *
 *                  Major BBS V10 Compatibility                              *
 *     8/4/2024   - 3.42                                                     *
 *                  Age display bug fix for V10 YYYYMMDD format              *
 *     3/5/2024   - 3.43                                                     *
 *                  Age display fix for > 2000 births, update WG32 too       *
 *                                                                           *
 *       Copyright (C) 1994 by EzSoft. All rights reserved.                  *
 *       Copyright (C) 2004-2024 Elwynor Technologies. All rights reserved.  *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU Affero General Public License as published  *
 * by the Free Software Foundation, either version 3 of the License, or      *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU Affero General Public License for more details.                       *
 *                                                                           *
 * You should have received a copy of the GNU Affero General Public License  *
 * along with this program. If not, see <https://www.gnu.org/licenses/>.     *
 *                                                                           *
 * Additional Terms for Contributors:                                        *
 * 1. By contributing to this project, you agree to assign all right, title, *
 *    and interest, including all copyrights, in and to your contributions   *
 *    to Rick Hadsall and Elwynor Technologies.                              *
 * 2. You grant Rick Hadsall and Elwynor Technologies a non-exclusive,       *
 *    royalty-free, worldwide license to use, reproduce, prepare derivative  *
 *    works of, publicly display, publicly perform, sublicense, and          *
 *    distribute your contributions                                          *
 * 3. You represent that you have the legal right to make your contributions *
 *    and that the contributions do not infringe any third-party rights.     *
 * 4. Rick Hadsall and Elwynor Technologies are not obligated to incorporate *
 *    any contributions into the project.                                    *
 * 5. This project is licensed under the AGPL v3, and any derivative works   *
 *    must also be licensed under the AGPL v3.                               *
 * 6. If you create an entirely new project (a fork) based on this work, it  *
 *    must also be licensed under the AGPL v3, you assign all right, title,  *
 *    and interest, including all copyrights, in and to your contributions   *
 *    to Rick Hadsall and Elwynor Technologies, and you must include these   *
 *    additional terms in your project's LICENSE file(s).                    *
 *                                                                           *
 * By contributing to this project, you agree to these terms.                *
 *                                                                           *
 *****************************************************************************/

#include "globrous.h"
#include "GLOSET.H"
#include "TVARS.H"
#include "GLOFILES.H"
#include "GLOUSER.H"
#include "GLOCONN.H"
#include "GLOCLOCK.H"

VOID EXPORT init__elwglob( VOID );                      /* module initialization routine */
extern INT elwglobals( VOID );                          /* the globals */

HMCVFILE glomb;                                         /* globals configuration variables */
DFAFILE *globchatdata;                                  /* global chat channel topics */
DFAFILE *userglobdata;                                  /* user's individual chat channel and macro data */
DFAFILE *telegramdata;                                  /* telegrams for users */
DFAFILE *suspenddata;                                   /* users to unsuspend on a given date */
DFAFILE *notkeydata;                                    /* negative keys users have */

CHAR *GLO_NAME="Globals";                               /* the module name */
CHAR *GLO_VERSION="4.0.0";                              /* version number: keep ELWGLOB.rc in step */

INT pitch;                                              /* pitch in Hz. of beep tone */
INT numbeeps;                                           /* number of beeps in console page */
INT pagedelay;                                          /* seconds between console pages */
INT pageall;                                            /* 1 = page every sysop on console page, otherwise 0 */
UINT nextpage;                                          /* time user can page console */
UINT pagedate;                                          /* date next page allowed, for accuracy near midnight */
CHAR lastpageid[UIDSIZ];                                /* user-id of person paging console */
INT i;                                                  /* number of beep loops */
USHORT checkdate;                                       /* date for autounsuspend to know when to check */
CHAR *pageme;                                           /* key to be alerted to console page */
CHAR ansistart='\x1B';                                  /* ANSI escape character */
CHAR bracket='[';                                       /* part of ANSI escape sequence */
CHAR endsequence='m';                                   /* end of ANSI output */
CHAR inbackground='4';                                  /* background color change character */
CHAR inforeground='3';                                  /* foreground color change character */
CHAR specialoutput[]={ "0157" };                        /* ANSI "special" attribute characters */
CHAR regularcolors[]={ "01234567" };                    /* ANSI normal foreground and background color characters */
INT loglist;                                            /* 1 if list users on-line automatically at log-on, otherwise 0 */
INT logchannotice;                                      /* 1 if notify users at log-on and log-off of users joining/leaving their chat channel */
INT showminvis;                                         /* 1 if staff can see invisible Sysops, otherwise 0 */
INT busyinterval;                                       /* number of seconds between checks for busy notices */
INT invisusernotify;                                    /* 1 if do log-on notice for users who are forced invisible, otherwise 0 */
INT invisnotify;                                        /* 1 if do log-on notice for Sysops toggling invisibility, otherwise 0 */
INT displaypages;                                       /* 1 if display current page when in "Menuing System" in Who's Online, otherwise 0 */
INT sendthrubusy;                                       /* 1 if send global message broadcasts to "Busy" users, 0 if not */
INT minimumtrandays;                                    /* minimum number of days to transfer in transfer days in class command */
CHAR *menuname;                                         /* the name of the Menuing System for Who's Online */
CHAR *editorname;                                       /* the name of the Editor module */
CHAR *fileservicename;                                  /* the name of the File Transfer Service module */
INT doglobals;                                          /* 1 if do globals, 0 if not */
flagstruct flagarray[NUMFLAGS];                         /* check for 5 possible flags */
lastcommand usercommand[MAXTERMS];                      /* array of previous input */
INT numchatchannels;                                    /* number of chat channels used on this BBS */
UCHAR telestate[MAXTERMS];                              /* each user's telegram state */
INT checkinterval;                                      /* > 0 if check at intervals for automatic unsuspend */
INT checktels;                                          /* > 0 if check at intervals for telegrams for "Busy" users */
INT checkalarm;                                         /* seconds between checks for user alarms */
INT alarmbeeps;                                         /* number of "beeps" in user's alarm message */
INT checkfriend;                                        /* seconds between checks for Friends Notices for "Busy" users */
INT changeseconds;                                      /* seconds between chat channel topic changes */

	
struct flddef userdataFDA[] =                            /* FDA for data conversion of user structure */
{
	   {CVTFLD_CHAR, UIDSIZ, fldoff(userdata, userid), NULL},
	   {CVTFLD_SHORT, 2, fldoff(userdata, chatchannel), NULL},
	   {CVTFLD_CHAR, 11, fldoff(userdata, echo), NULL},
	   {CVTFLD_CHAR, NUMMAC * CMSIZ, fldoff(userdata, command), NULL},
	   {CVTFLD_CHAR, NUMMAC * MACSIZ, fldoff(userdata, macro), NULL},
	   {CVTFLD_CHAR, NUMFORGETS * UIDSIZ, fldoff(userdata, pageforgets), NULL},
	   {CVTFLD_CHAR, NUMBLOCKS * UIDSIZ, fldoff(userdata, teleblocks), NULL},
	   {CVTFLD_CHAR, MESSSIZE, fldoff(userdata, logon), NULL},
	   {CVTFLD_CHAR, MESSSIZE, fldoff(userdata, logoff), NULL},
	   {CVTFLD_END, 0, 0, NULL}
};


CHAR *whotopics[NUMWHODIS];                             /* Who's On-line topic displays */
USHORT logtimes[MAXTERMS];                              /* time of log-on for saving in each user's data */
LONG minuteson[MAXTERMS];                               /* user's time on-line today in minutes, for display to others who can't "see" user */
UCHAR usrchannel[MAXTERMS];                             /* 1 if alert to log-ons, 0 otherwise */
busynotids far theids[MAXTERMS];                        /* array of users to "Busy" notify */
userhelp far help[HELPSIZE];                            /* help messages and keys to access */
pagedata userpage[MAXTERMS];                            /* our user's paging data */
teleblocks far blocked[MAXTERMS];                       /* users blocked from sending telegrams to online users */
UCHAR tgmalert[MAXTERMS];                               /* 1 = tell user when no longer "Busy" he has a telegram */
struct topicdata topics[MAXCHATCHAN];                   /* array of chat channel topics */
USHORT globalchatchannel[MAXTERMS];                     /* each user's channel is in globalchatchannel[usrnum] */
UCHAR chatecho[MAXTERMS];                               /* 1 = echo global chat output to user, 0 = don't */
UCHAR duplex[MAXTERMS];                                 /* 1 = full duplex mode on (echo), 0 = half duplex mode on */
UCHAR notifytelegrams[MAXTERMS];                        /* 1 = notify user of telegrams received, 0 = don't */
struct telegram teletouser[MAXTERMS];                   /* storage for telegram to user data */
UCHAR bugem[MAXTERMS];                                  /* 1 if pester users in the Editor, otherwise 0 */
UCHAR channotice[MAXTERMS];                             /* 1 if notify user of others joining/leaving chat channel, 0 if not */
UCHAR showages[MAXTERMS];                               /* 1 if show user's age in Who's Online display, 0 if not */
UCHAR whonumber[MAXTERMS];                              /* which Who's Online display to give user */
friends far frienddata[MAXTERMS];                       /* user special log notify data */
chanchange changedata[MAXTERMS];                        /* users chat channel change dates and times */
USHORT alarmtimes[MAXTERMS][2];                         /* dates and times at which to alert on-line users (date is first, followed by time) */
INT topchginaudit;                                      /* 1 if display chat channel topic changes in audit trail, otherwise 0 */
INT dobeeps;                                            /* 1 if "beep" users at certain prompts, otherwise 0 */
LONG minimumtransfer;                                   /* minimum amount of credits that user must transfer */
usermacros themacros[MAXTERMS];                         /* storage for each user's macros */
poll_data poll[MAXTERMS];                               /* each user's polling saved data */
LONG maxpostcreds;                                      /* maximum allowed credit post for non-Sysops */
CHAR *logonstr;                                         /* string to edit log-on message */
CHAR *logoffstr;                                        /* string to edit log-off message */
CHAR *clearstr;                                         /* string to clear a user's log-on and log-off messages */
CHAR *showmessstr;                                      /* string to display your current messages */
CHAR *echostring;                                       /* string to toggle global chat echo */
CHAR *setstring;                                        /* string to set user's global chat channel */
CHAR *showstring;                                       /* string to show global chat channel topics */
CHAR *channotestring;                                   /* string to toggle notices of users joining/leaving channel on or off */
CHAR *displaystring;                                    /* string to display users in global chat channels */
CHAR *clearstring;                                      /* string to clear global chat channel topics */
CHAR *topicstring;                                      /* string to change a chat channel topic */
CHAR *macrocommand;                                     /* string to set macro command */
CHAR *macromacstring;                                   /* string to set a macro itself */
CHAR *macrocommandlist;                                 /* string to list macro command strings */
CHAR *macrolist;                                        /* string to list the macros themselves */
CHAR *defaultcom[NUMMAC];                               /* default macro command strings */
CHAR *defaultmac[NUMMAC];                               /* default macros */
CHAR telesendchar;                                      /* character to send a telegram */
CHAR *logon;                                            /* output in who's online for user logging on */
CHAR *logoff;                                           /* output in who's online for user logging off */
CHAR *newsignup;                                        /* output in who's online for user signing up */
CHAR *repagestring;                                     /* string to re-send last page */
CHAR *pageforgetstring;                                 /* string to block a user's pages */
CHAR *endbeeps;                                         /* string to stop current console page */
CHAR *bpstpkey;                                         /* key required to end current console page */
CHAR *infolookkey;                                      /* key required to look up another user's class and credit info */
CHAR *unbusyothkey;                                     /* key required to toggle another user's "Busy" flag off */
CHAR *relogothkey;                                      /* key required to re-log another user */
CHAR *unlimpostkey;                                     /* key required to not have limit on post credits amount */
CHAR *logclearkey;                                      /* key required to clear another user's log messages */
CHAR *syschankey;                                       /* key required to be able to enter the Sysop's chat channel */
CHAR *chanclearkey;                                     /* key required to be able to clear global chat channel topics */
CHAR *chatinviskey;                                     /* key required to be able to use chat channels while invisible */
CHAR *displaykey;                                       /* key required to display all user's chat channels */
CHAR *unlimtrankey;                                     /* key required to be able to do an unlimited amount credit transfer */
CHAR *nomindaykey;                                      /* key to avoid a minimum amount of days in class to transfer */
CHAR *othinfokey;                                       /* key to display another user's global settings */
CHAR *pageinviskey;                                     /* key required to be able to page while invisible */
CHAR *pagebusykey;                                      /* key required to be able to page thru "Busy" */
CHAR *pagethrukey;                                      /* key required to be able to page thru page OFF or OK */
CHAR *deltelekey;                                       /* key required to be able to delete all telegrams from a user */
CHAR *teleinviskey;                                     /* key required to send telegrams while invisible */
CHAR *telenoblockkey;                                   /* key to not allow blocks of your telegrams */
CHAR *telegramalert;                                    /* string to toggle telegram received alert */
CHAR *telegramclear;                                    /* string to delete all telegrams from a certain user-id */
CHAR *deleteallteles;                                   /* string to delete all telegrams on system from a user-id */
CHAR *teleblockstring;                                  /* string to block/unblock telegrams from a user */
CHAR *seecredskey;                                      /* key required to see how many credits another user has */
CHAR *moduleblocks[NUMMODBLKS];                         /* modules in which users are blocked from using global commands */
CHAR ANSIchar;                                          /* character to begin coloring input */
CHAR backchar;                                          /* character to indicate to color the background of input */
CHAR forechar;                                          /* character to indicate to color the foreground of input */
CHAR specchar;                                          /* character to indicate to create special color effect */
INT endback;                                            /* color of background to end colorized input with */

INT globstat;                                           /* the module state no. */
static CHAR *helpappend;                                /* HELCAPP: added to the help command in its own help entry */
static CHAR *teleappend;                                /* TELCAPP: added to the telegram character in its help entry */
static GBOOL settingsready=FALSE;                       /* TRUE once init__elwglob has set up the chat topics */

static const struct                                     /* where each help[] entry's key, command and label come from */
	{
	INT key;                                        /* GS_ index of its key */
	INT cmd;                                        /* GS_ index of its command string, or -1 (built in applysettings) */
	INT msg;                                        /* its HELPn label in ELWGLOB.MSG */
	} helpmap[HELPSIZE]={
	{ GS_HLPKEY,  GS_HLPSTG,   HELP1 },  { GS_HLPKEY,  -1,          HELP2 },
	{ GS_ANSTKEY, GS_ANSSTG,   HELP3 },  { GS_BSYKEY,  GS_BSYSTG,   HELP4 },
	{ GS_BSNKEY,  GS_BNTSTG,   HELP5 },  { GS_UCIKEY,  GS_CRDSTG,   HELP6 },
	{ GS_GLBKEY,  GS_CHATSTG,  HELP7 },  { GS_ACSTKEY, GS_CSTSTG,   HELP8 },
	{ GS_CTOKEY,  GS_CNTSTG,   HELP9 },  { GS_DNTKEY,  GS_DNTSTG,   HELP10 },
	{ GS_DUPKEY,  GS_DUPSTG,   HELP11 }, { GS_PGKEY,   GS_BPSTG,    HELP12 },
	{ GS_EDTKEY,  GS_EDTSTG,   HELP13 }, { GS_IBMKEY,  GS_IBMSTG,   HELP14 },
	{ GS_INVKEY,  GS_INVSTG,   HELP15 }, { GS_DLUKEY,  GS_DLUSTG,   HELP16 },
	{ GS_SGNKEY,  GS_NEWSTG,   HELP17 }, { GS_LOGKEY,  GS_LOGSTG,   HELP18 },
	{ GS_MACKEY,  GS_MACSTG,   HELP19 }, { GS_PAGKEY,  GS_PGUSTG,   HELP20 },
	{ GS_PSTCKEY, GS_PSTSTG,   HELP22 }, { GS_PDYKEY,  GS_PDYSTG,   HELP23 },
	{ GS_EPSTKEY, GS_EPSTSTG,  HELP21 }, { GS_RPTKEY,  GS_RPTSTG,   HELP24 },
	{ GS_RLGKEY,  GS_RLGSTG,   HELP25 }, { GS_SCLKEY,  GS_SLSTG,    HELP26 },
	{ GS_SUSKEY,  GS_ASUSSTG,  HELP27 }, { GS_SWCKEY,  GS_SWCSTG,   HELP28 },
	{ GS_SUSKEY,  GS_SUSSTG,   HELP29 }, { GS_SCWKEY,  GS_SWSTG,    HELP30 },
	{ GS_TRCKEY,  GS_TRNSTG,   HELP31 }, { GS_TRDKEY,  GS_TRDSTG,   HELP32 },
	{ GS_TEXTKEY, GS_TEXTSTG,  HELP33 }, { GS_GTUKEY,  GS_UKSTG,    HELP34 },
	{ GS_OFFKEY,  GS_OFFSTG,   HELP35 }, { GS_BTUKEY,  GS_ZAPSTG,   HELP36 },
	{ GS_MBRKEY,  GS_MBRSTG,   HELP37 }, { GS_WHOKEY,  GS_WHOSTG,   HELP38 },
	{ GS_SYSBKEY, GS_GLBSTG,   HELP39 }, { GS_TGMKEY,  GS_GTLSTG,   HELP40 },
	{ GS_TGMKEY,  -1,          HELP41 }, { GS_ALRMKEY, GS_ALRMSTG,  HELP42 },
	{ GS_COLKEY,  -1,          HELP43 }, { GS_FRNDKEY, GS_FRNDSTG,  HELP44 },
	{ GS_GSETKEY, GS_GSETSTG,  HELP45 }, { GS_CFGKEY,  GS_CFGSTG,   HELP46 }
	};

struct module globalsmod={
	"",                                                  /* name used to refer to this module */
	glologon,                                            /* user logon supplemental routine */
	globinput,                                           /* input routine if selected */
	dfsthn,                                              /* status-input routine if selected */
	NULL,                                                /* "injoth" routine for this module */
	NULL,                                                /* user logoff supplemental routine */
	endnote,                                             /* hangup (lost carrier) routine */
	midnitunsuspend,                                     /* midnight cleanup routine */
	deleteuser,                                          /* delete-account routine */
	glofin,                                              /* finish-up (sys shutdown) routine */
	};

VOID EXPORT init__elwglob( VOID )          /* the module initialization routine */
	{
	INT count;                              /* loop counter for each CHAR in code */
	INT inittopiccnt;                       /* number of channel topics to initialize */
	INT found;                              /* 1 if more chat channel topics in database, otherwise 0 */
	struct topicdata initdata;              /* holder for initial chat channel topic data */
	INT chattopickey;                       /* chat channel number to check for saved topic */
	INT todaysdate;                         /* date as returned by today() */
	INT timenow;                            /* time as returned by now() */

	stzcpy(globalsmod.descrp, gmdnam("ELWGLOB.MDF"),MNMSIZ);
	globstat=register_module(&globalsmod);
	glomb=opnmsg("ELWGLOB.MCV");

	makedat("ELWGLOBU");       /* a new install: create the data files from their templates */
	makedat("ELWGLOBC");
	makedat("ELWGLOBG");
	makedat("ELWGLOBS");
	makedat("ELWGLOBN");
	userglobdata=dfaOpen("elwglobu.dat", sizeof(struct userdata), NULL); // ezeguser
	globchatdata=dfaOpen("elwglobc.dat", sizeof(struct topicdata), NULL); // ezegchat
	telegramdata=dfaOpen("elwglobg.dat", sizeof(struct telegram), NULL); // ezegram
	suspenddata=dfaOpen("elwglobs.dat", sizeof(struct autounsus), NULL); // ezegsus
	notkeydata=dfaOpen("elwglobn.dat", sizeof(struct notkey), NULL); // ezegnot
	globalcmd(elwglobals);
	hook_connect(wg_connect);
	hook_disconnect(wg_disconnect);
	flagarray[0].flagname=stgopt(FLAG1);
	flagarray[0].flagvalue=BYEBYE;
	flagarray[1].flagname=stgopt(FLAG2);
	flagarray[1].flagvalue=INVISB;
	flagarray[2].flagname=stgopt(FLAG3);
	flagarray[2].flagvalue=OPCHAT;
	flagarray[3].flagname=stgopt(FLAG4);
	flagarray[3].flagvalue=NOINJO;
	flagarray[4].flagname=stgopt(FLAG5);
	flagarray[4].flagvalue=MASTER;
	flagarray[0].flagletter=stgopt(FLAGL1);
	flagarray[1].flagletter=stgopt(FLAGL2);
	flagarray[2].flagletter=stgopt(FLAGL3);
	flagarray[3].flagletter=stgopt(FLAGL4);
	flagarray[4].flagletter=stgopt(FLAGL5);
	logon=stgopt(LGNSTG);
	logoff=stgopt(LGFSTG);
	newsignup=stgopt(SUPSTG);
	helpappend=stgopt(HELCAPP);
	teleappend=stgopt(TELCAPP);
	for ( count=0; count < HELPSIZE; count++ )
		{       /* help menu labels: text, so they stay in ELWGLOB.MSG */
		stzcpy( help[count].message, stgopt(helpmap[count].msg), HELPMSSSIZE );
		}
	whotopics[0]=stgopt(TOPIC1);    /* initialize Who's Online topic strings */
	whotopics[1]=stgopt(TOPIC2);
	whotopics[2]=stgopt(TOPIC3);
	tv_init();
	gs_open();               /* the settings (ELWGLOBO.DAT); calls applysettings() */
	dfaSetBlk(globchatdata);
	for ( inittopiccnt=1; inittopiccnt < MAXCHATCHAN; inittopiccnt++ )
		{       /* initialize chat channel topic data (all of them: NUMCHAT can be raised online) */
		setmem( topics[inittopiccnt].chantopic, CHATTOPSIZ, 0 );
		topics[inittopiccnt].channelnumber=(USHORT)inittopiccnt;
		}
	chattopickey=0;
	found=dfaAcqGT(&initdata, &chattopickey, 0);
	while ( found )
		{       /* get all chat channel topics and copy to proper location */
		chattopickey=initdata.channelnumber;
		strcpy( topics[initdata.channelnumber].chantopic, initdata.chantopic );
		found=dfaAcqGT(&initdata, &chattopickey, 0);
		}
	dfaRstBlk();
	stzcpy( topics[numchatchannels].chantopic, GSSTR(SYSTOPIC), CHATTOPSIZ );
	settingsready=TRUE;
	for ( count=0; count < MAXTERMS; count++ )
		{       /* initialize telegram "Busy" alerts */
		dotelalert(count, (INT)OFF);
		}
	for ( count=0; count < MAXTERMS; count++ )
		{	/* initialize alarm times */
		alarmtimes[count][0]=0;
		alarmtimes[count][1]=0;
		}
	for ( count=0; count < MAXTERMS; count++ )
		{	/* initialize alarm times */
		setmem( frienddata[count].logonid, UIDSIZ, 0 );
		setmem( frienddata[count].logoffid, UIDSIZ, 0 );
		}
	todaysdate=today();
	timenow=now();
	for ( count=0; count < MAXTERMS; count++ )
		{	/* initialize chat channel change data */
		changedata[count].switchdate=(USHORT)todaysdate;
		changedata[count].switchtime=(USHORT)timenow;
		}
	nextpage=now();
	pagedate=today();

	i=0;
	midnitunsuspend();
	armtimers();
	rtkick( 10, connreport );       /* once every module has started: is the SSH Server there? */

   shocst(spr("ELW Globals v%s",GLO_VERSION),"(C) Copyright 2004-2026 Elwynor Technologies - www.elwynor.com");

	}

VOID applysettings( VOID )                  /* copy the live settings into the module's globals */
	{                                   /* called at start-up and after every change made online */
	INT count;                              /* loop counter */
	INT oldchannels=numchatchannels;        /* chat channels before this change */

	/* Command strings come through GSCMD: a command the sysop has turned */
	/* OFF reads as "", which no typed word matches (GLOSET.C gs_cmd).    */
	logonstr=GSCMD(LONSTR);
	logoffstr=GSCMD(LOFFSTR);
	clearstr=GSCMD(LCLRSTR);
	showmessstr=GSCMD(SHOWSTR);
	echostring=GSSTR(ECHSTG);
	setstring=GSSTR(SETSTG);
	showstring=GSSTR(SHOWSTG);
	channotestring=GSSTR(NOTSTG);
	displaystring=GSSTR(DISSTG);
	clearstring=GSSTR(CLRSTG);
	topicstring=GSSTR(TPCSTG);
	macrocommand=GSCMD(MCSET);
	macromacstring=GSCMD(MMSET);
	macrocommandlist=GSCMD(MCLIST);
	macrolist=GSCMD(MMLIST);
	repagestring=GSCMD(RPGSTG);
	pageforgetstring=GSCMD(FPUSTG);
	endbeeps=GSCMD(BPSTPSTG);
	telegramalert=GSCMD(TALTSTG);
	telegramclear=GSCMD(TCLRSTG);
	deleteallteles=GSCMD(TDASTG);
	teleblockstring=GSCMD(TBKSTG);
	pageme=GSSTR(CPGKEY);
	unbusyothkey=GSSTR(BSYOKEY);
	relogothkey=GSSTR(URLGKEY);
	unlimpostkey=GSSTR(UNLPTKEY);
	logclearkey=GSSTR(LCLRKEY);
	syschankey=GSSTR(SYSCKEY);
	chanclearkey=GSSTR(CLRKEY);
	chatinviskey=GSSTR(GCIKEY);
	displaykey=GSSTR(DISKEY);
	unlimtrankey=GSSTR(UNLTRKEY);
	nomindaykey=GSSTR(NMTDKEY);
	othinfokey=GSSTR(OUSKEY);
	pageinviskey=GSSTR(PGIKEY);
	pagebusykey=GSSTR(PGBSYKEY);
	pagethrukey=GSSTR(PTUKEY);
	deltelekey=GSSTR(TDAKEY);
	teleinviskey=GSSTR(TMIKEY);
	telenoblockkey=GSSTR(TNKKEY);
	seecredskey=GSSTR(SEECKEY);
	bpstpkey=GSSTR(BPSTPKEY);
	infolookkey=GSSTR(OTHKEY);
	menuname=GSSTR(MNUNAM);
	editorname=GSSTR(EDTNAM);
	fileservicename=GSSTR(FILNAM);
	telesendchar=GSCHR(TLCCHR);
	ANSIchar=GSCHR(ANSCHR);
	backchar=GSCHR(BCKCHR);
	forechar=GSCHR(FRECHR);
	specchar=GSCHR(SPCCHR);
	endback=(INT)GSNUM(ENBNUM);
	pitch=(INT)GSNUM(BPPIT);
	numbeeps=(INT)GSNUM(BPDUR);
	pagedelay=(INT)GSNUM(PGDLY);
	busyinterval=(INT)GSNUM(CHKSECS);
	minimumtrandays=(INT)GSNUM(MINTRD);
	pageall=(INT)GSNUM(PGALL);
	loglist=(INT)GSNUM(LOGLIST);
	logchannotice=(INT)GSNUM(LOGCHNOT);
	showminvis=(INT)GSNUM(SHWMINV);
	topchginaudit=(INT)GSNUM(STPCHG);
	dobeeps=(INT)GSNUM(DOBEEPS);
	invisusernotify=(INT)GSNUM(SNKUSER);
	invisnotify=(INT)GSNUM(SNEAK);
	displaypages=(INT)GSNUM(SHOWPG);
	sendthrubusy=(INT)GSNUM(SNDTHRU);
	checkinterval=(INT)GSNUM(CHKUNS);
	checktels=(INT)GSNUM(CHKTELS);
	checkalarm=(INT)GSNUM(CHKALRM);
	alarmbeeps=(INT)GSNUM(ALRMBPS);
	checkfriend=(INT)GSNUM(CHKFRND);
	changeseconds=(INT)GSNUM(CHKCHG);
	maxpostcreds=GSNUM(MXPST);
	minimumtransfer=GSNUM(MNTRFR);
	numchatchannels=(INT)GSNUM(NUMCHAT);
	for ( count=0; count < NUMMAC; count++ )
		{       /* default macros and their command strings, for new users */
		defaultcom[count]=gsval[GS_MACCM0+count];
		defaultmac[count]=gsval[GS_MACMC0+count];
		}
	for ( count=0; count < NUMMODBLKS; count++ )
		{       /* modules in which globals are blocked */
		moduleblocks[count]=gsval[GS_BLOCK1+count];
		}
	for ( count=0; count < HELPSIZE; count++ )
		{       /* help entries: who may use each global and how it is typed */
		stzcpy( help[count].key, gsval[helpmap[count].key], KEYSIZ );
		if ( helpmap[count].cmd >= 0 )
			{
			stzcpy( help[count].command, gs_cmd(helpmap[count].cmd), HELPCOMSIZE );
			}
		}
	if ( gs_cmdon(GS_HLPSTG) )
		{       /* "=? <command>": goes with the help command */
		stzcpy( help[1].command, spr("%s%s", GSSTR(HLPSTG), helpappend), HELPCOMSIZE );
		}
	else
		{
		setmem( help[1].command, HELPCOMSIZE, 0 );
		}
	stzcpy( help[40].command, spr("%c%s", telesendchar, teleappend), HELPCOMSIZE );
	setmem( help[42].command, HELPCOMSIZE, 0 );
	help[42].command[0]=ANSIchar;
	if ( settingsready )
		{       /* changed online: settings that need more than a new value */
		if ( numchatchannels != oldchannels )
			{
			chatresize(oldchannels);
			}
		stzcpy( topics[numchatchannels].chantopic, GSSTR(SYSTOPIC), CHATTOPSIZ );
		armtimers();
		}
	}

GBOOL globinput( VOID )                     /* user input routine: Globals chosen from the BBS menu */
	{
	if ( usrptr->substt == 0 )
		{       /* just arrived: the user's own settings menu (GLOUSER.C) */
		usrptr->substt=1;
		gu_begin();
		return(TRUE);
		}
	/* While the menu is open the global handler takes every line, so a   */
	/* line normally reaches here only after the menu's X (the engine's   */
	/* injected blank line): back to the BBS menu.  A channel with NOGLOB */
	/* set skips global handlers (WG33 MAJORBBS.C hdlcri), so the menus   */
	/* are fed from here too; then the user is never stuck in them.       */
	if ( gs_input() != -1 )
		{       /* not a line for the system settings menu (or it was just */
		gu_input();     /* left): the user menu has it, or shows itself again */
		}
	return(gu_active(usrnum) ? TRUE : FALSE);
	}

INT elwglobals( VOID )                        /* the globals */
	{
	INT rc;                                 /* the settings menu's answer */

	if ( ( rc=gs_input() ) != 0 )
		{       /* in the settings menu: every line, blank ones too, goes to it */
		return(rc);
		}
	if ( ( rc=gu_input() ) != 0 )
		{       /* in the user menu (Globals chosen from the BBS menu): the same */
		return(rc);
		}
	if ( isblocked() )
		{	/* user is in a module where globals cannot be used */
		return(0);
		}
	if ( ( margc > 0 ) && ( haskey(help[42].key) ) )
		{       /* convert any commands to colorize input */
		colorize_input();
		}
	if ( margc > 0 && sameas( margv[0], help[23].command ) ) /* repeat previous command */
		{
		repeat_input();
		}
	else /* else is so we don't save repeat previous as the new command! */
		{
		if ( margc > 0 )
			{ /* save newest input */
			setmem( usercommand[usrnum].thecommand, INPSIZ, 0 );
			rstrin();
			strcpy( usercommand[usrnum].thecommand, input ); /* save input and count */
			parsin();
			}
                }
	if ( margc > 0 && sameas( margv[0], themacros[usrnum].command[0] ) )
		{       /* user is calling first macro */
		setmbk(glomb);
		if ( !haskey(help[18].key) )             /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		if ( macrohandler(0) )
			{	/* error - input added too long */
			return(1);
			}
		}
	if ( margc > 0 && sameas( margv[0], themacros[usrnum].command[1] ) )
		{       /* user is calling second macro */
		setmbk(glomb);
		if ( !haskey(help[18].key) )             /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		if ( macrohandler(1) )
			{	/* error - input added too long */
			return(1);
			}
		}
	if ( margc > 0 && sameas( margv[0], themacros[usrnum].command[2] ) )
		{       /* user is calling third macro */
		setmbk(glomb);
		if ( !haskey(help[18].key) )             /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		if ( macrohandler(2) )
			{	/* error - input added too long */
			return(1);
			}
		}
	if ( margc > 0 && sameas( margv[0], themacros[usrnum].command[3] ) )
		{       /* user is calling fourth macro */
		setmbk(glomb);
		if ( !haskey(help[18].key) )             /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		if ( macrohandler(3) )
			{	/* error - input added too long */
			return(1);
			}
		}
	if ( margc > 0 && sameas( margv[0], themacros[usrnum].command[4] ) )
		{       /* user is calling fifth macro */
		setmbk(glomb);
		if ( !haskey(help[18].key) )             /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		if ( macrohandler(4) )
			{	/* error - input added too long */
			return(1);
			}
		}
	if ( margc > 0 && sameas( margv[0], themacros[usrnum].command[5] ) )
		{       /* user is calling sixth macro */
		setmbk(glomb);
		if ( !haskey(help[18].key) )             /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		if ( macrohandler(5) )
			{	/* error - input added too long */
			return(1);
			}
		}
	if ( margc > 0 && sameas( margv[0], themacros[usrnum].command[6] ) )
		{       /* user is calling seventh macro */
		setmbk(glomb);
		if ( !haskey(help[18].key) )             /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		if ( macrohandler(6) )
			{	/* error - input added too long */
			return(1);
			}
		}
	if ( margc > 0 && sameas( margv[0], themacros[usrnum].command[7] ) )
		{       /* user is calling eigth macro */
		setmbk(glomb);
		if ( !haskey(help[18].key) )             /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		if ( macrohandler(7) )
			{	/* error - input added too long */
			return(1);
			}
		}
	if ( margc > 0 && sameas( margv[0], themacros[usrnum].command[8] ) )
		{       /* user is calling ninth macro */
		setmbk(glomb);
		if ( !haskey(help[18].key) )             /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		if ( macrohandler(8) )
			{	/* error - input added too long */
			return(1);
			}
		}
	if ( margc > 0 && sameas( margv[0], themacros[usrnum].command[9] ) )
		{       /* user is calling tenth macro */
		setmbk(glomb);
		if ( !haskey(help[18].key) )             /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		if ( macrohandler(9) )
			{	/* error - input added too long */
			return(1);
			}
		}
	if ( margc == 1 && sameas( margv[0], help[8].command ) ) /* toggle continuous output */
		{
		setmbk(glomb);
		if ( !haskey(help[8].key) )             /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		togglecontinuous();
		return(1);
		}
	if ( margc == 1 && sameas( margv[0], help[34].command ) ) /* quick log-off */
		{
		setmbk(glomb);
		if ( !haskey(help[34].key) )             /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		byenow(GOODBYE, usaptr->userid);
		rstmbk();
		return(-1);
		}
	if ( margc == 1 && sameas( margv[0], help[22].command ) ) /* toggle Editor pester setting */
		{
		struct userdata pestdat;		/* user's global data */

		setmbk(glomb);
		if ( !haskey(help[22].key) )             /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		bugem[usrnum]^=1;	/* toggle setting */
		tv_set(TV_PESTER, (bugem[usrnum]) ? getmsg(WON) : getmsg(WOFF));
		prfmsg(PSTTOG);
		doprintoutput();
		dfaSetBlk(userglobdata);
		if ( dfaAcqEQ(&pestdat, usaptr->userid, 0) )
			{	/* update data */
			cvtDataIP(&pestdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
			pestdat.buginedit=bugem[usrnum];
			cvtDataIP(&pestdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
			dfaUpdate(&pestdat);
			}
		dfaRstBlk();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], help[44].command ) ) /* settings editor */
		{
		setmbk(glomb);
		if ( !hasmaster() && !haskey(help[44].key) )   /* MASTER always, so a bad key cannot lock the sysop out */
			{
			cantuse();
			return(1);
			}
		gs_command();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], help[45].command ) ) /* the user's own settings menu */
		{
		setmbk(glomb);
		if ( !haskey(help[45].key) )
			{
			cantuse();
			return(1);
			}
		rstmbk();
		gu_begin();             /* it shows its own prompt: no module re-prompt */
		return(-1);
		}
	if ( margc > 0 && sameas( margv[0], help[0].command ) ) /* global help information */
		{
		helphandler();
		return(1);
		}
	if ( margc == 1 && sameas( margv[0], help[11].command ) ) /* page console */
		{
		USHORT curtime;    /* current time */
		USHORT curdate;    /* current date */

		setmbk(glomb);
		if ( !haskey(help[11].key) )           /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		curtime=now();
		curdate=today();
		if ( ( curtime >= nextpage ) && ( i == 0 ) && !( curdate < pagedate ) )
			{       /* user can page */
			prfmsg(PGMSG);
			if ( pageall == 1 )
				{
				prfmsg(PGALLMSG);
				}
			doprintoutput();
			i=1;    /* mark that we're in a page */
			strcpy( lastpageid, usaptr->userid );
			rtkick( 1, pagesysops );
			return(1);
			}
		if ( i == 0 )
			{       /* recent page */
			prfmsg(NOPG);
			}
		else
			{       /* page in progress right now */
			prfmsg(NOWPG);
			}
		doprintoutput();
		return(1);
		}
	if ( margc == 1 && sameas( margv[0], endbeeps ) ) /* end page beeps */
		{
		setmbk(glomb);
		if ( !haskey(bpstpkey) )           /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		if ( i == 0 )
			{       /* no page in progress */
			prfmsg(NOPGBUD);
			}
		else
			{       /* end it */
			i=numbeeps+1;     /* shoot to end of routine */
			prfmsg(PGQUIT);
			}
		doprintoutput();
		return(1);
		}
	if ( margc == 1 && sameas( margv[0], help[2].command ) ) /* toggle ANSI setting */
		{
		setmbk(glomb);
		if ( !haskey(help[2].key) )          /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		if ( usaptr->ansifl < ANSMAN )
			{	/* enable auto-sensing */
			usaptr->ansifl+=ANSMAN;
			}
		usaptr->ansifl^=ANSON;
		stansi();
		tv_set(TV_ANSI, (usaptr->ansifl&ANSON) ? getmsg(WON) : getmsg(WOFF));
		prfmsg(ANSIMSG);
		doprintoutput();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], help[3].command ) ) /* toggle "busy" flag */
		{
		handle_busy();
                return(1);
		}
	if ( margc > 0 && sameas( margv[0], help[4].command ) ) /* "Busy" notify */
		{
                busy_notify();
		return(1);
		}
	if ( margc == 1 && sameas( margv[0], help[10].command ) ) /* toggle duplex mode */
		{
		struct userdata dupdat;           /* structure to save new mode to disk in */

		setmbk(glomb);
		if ( !haskey(help[10].key) )           /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		if ( duplex[usrnum] == 0 )
			{       /* half duplex, toggle to full */
			duplex[usrnum]=1;
			echon();
			}
		else
			{       /* full duplex, toggle to half */
			duplex[usrnum]=0;
			btuech(usrnum, 0);
			}
		tv_set(TV_DUPLEX, duplex[usrnum] == 0 ? getmsg(WHALF) : getmsg(WFULL));
		prfmsg(DUPTOG);
		doprintoutput();
		dfaSetBlk(userglobdata);
		if ( dfaAcqEQ(&dupdat, usaptr->userid, 0) )
			{
			cvtDataIP(&dupdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
			dupdat.duplex=duplex[usrnum];
			cvtDataIP(&dupdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
			dfaUpdate(&dupdat);
			}
		dfaRstBlk();
		return(1);
		}
	if ( margc == 1 && sameas( margv[0], help[12].command ) ) /* toggle editor type */
		{
		setmbk(glomb);
		if ( !haskey(help[12].key) )           /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		usaptr->usrprf^=PRFLIN;         /* toggle editor flag */
		tv_set(TV_EDITOR, usaptr->usrprf == PRFLIN ? getmsg(WLINE) : getmsg(WFULLSCR));
		prfmsg(EDITTYP);
		doprintoutput();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], help[24].command ) ) /* re-log user on */
		{
		INT return_value;	/* return value from relog handler */

		return_value=relog();
                return(return_value);
		}
	if ( margc == 1 && sameas( margv[0], help[36].command ) ) /* display membership information */
		{
		setmbk(glomb);
		if ( !haskey(help[36].key) )           /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		prfmsg(MEMINFO);
		doprintoutput();
		return(1);
		}
	if ( margc == 1 && sameas( margv[0], help[13].command ) ) /* enable IBM graphics */
		{
		setmbk(glomb);
		if ( !haskey(help[13].key) )           /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		usaptr->systyp^=1;
		tv_set(TV_IBM, (usaptr->systyp) ? getmsg(WON) : getmsg(WOFF));
		prfmsg(IBMMSG);
		doprintoutput();
		return(1);
		}
	if ( ( margc > 0 ) && ( margc < 3 ) && sameas( margv[0], help[29].command ) ) /* set screen width */
		{
		INT screenwidth;        /* width of the user's screen */

		setmbk(glomb);
		if ( !haskey(help[29].key) )           /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		if ( margc == 1 )
			{       /* print usage information */
			tv_setn(TV_MIN, (INT)MIN_SCREEN_WIDTH);
			tv_setn(TV_MAX, (INT)MAX_SCREEN_WIDTH);
			tv_setn(TV_WIDTH, (INT)usaptr->scnwid);
			prfmsg(SCNHLP);
			doprintoutput();
			return(1);
			}
		screenwidth=atoi(margv[1]);
		if ( ( screenwidth >= MIN_SCREEN_WIDTH ) && ( screenwidth <= MAX_SCREEN_WIDTH ) )
			{
			usaptr->scnwid=(CHAR)screenwidth;
			btutsw(usrnum, usaptr->scnwid);/* needed for change to immediately take effect */
			tv_setn(TV_WIDTH, usaptr->scnwid);
			prfmsg(SCNSET);
			}
		else
			{
			tv_setn(TV_MIN, (INT)MIN_SCREEN_WIDTH);
			tv_setn(TV_MAX, (INT)MAX_SCREEN_WIDTH);
			prfmsg(NOSET);
			}
		doprintoutput();
		return(1);
		}
	if ( ( margc > 0 ) && ( margc < 3 ) && sameas( margv[0], help[25].command ) ) /* set screen length */
		{
		INT screenlength;       /* length of the user's screen */

		setmbk(glomb);
		if ( !haskey(help[25].key) )           /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		if ( margc == 1 )
			{       /* print usage information */
			tv_setn(TV_MIN, (INT)MIN_SCREEN_LENGTH);
			tv_setn(TV_MAX, (INT)MAX_SCREEN_LENGTH);
			tv_set(TV_LENGTH, (usaptr->scnbrk == CTNUOS) ? getmsg(WCONT) : spr("%d", usaptr->scnbrk));
			prfmsg(LNGHLP);
			doprintoutput();
			return(1);
			}
		screenlength=atoi(margv[1]);
		if ( ( screenlength >= MIN_SCREEN_LENGTH ) && ( screenlength <= MAX_SCREEN_LENGTH ) )
			{
			usaptr->scnbrk=(CHAR)screenlength;
			usaptr->scnfse=(CHAR)screenlength; /* set full screen editor length */
			rstrxf();       /* needed for change to immediately take effect */
			tv_setn(TV_LENGTH, screenlength);
			prfmsg(LNGSET);
			}
		else
			{
			tv_setn(TV_MIN, (INT)MIN_SCREEN_LENGTH);
			tv_setn(TV_MAX, (INT)MAX_SCREEN_LENGTH);
			prfmsg(NOLNG);
			}
		doprintoutput();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], help[5].command ) ) /* display user credit info */
		{
                show_credits();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], help[37].command ) ) /* show online users */
		{
		struct userdata udata;		/* user's global data for updating settings */
		INT displaychoice;		/* user's new desired Who's Online default choice */

		setmbk(glomb);
		if ( !haskey(help[37].key) )           /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		if ( margc == 2 && sameas( margv[1], "ON" ) )
			{	/* user wishes to toggle show age setting on */
			dfaSetBlk(userglobdata);
			if ( dfaAcqEQ(&udata, usaptr->userid, 0) )
				{	/* update setting */
				cvtDataIP(&udata, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
				udata.showage=1;
				cvtDataIP(&udata, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
				dfaUpdate(&udata);
				}
			dfaRstBlk();
			showages[usrnum]=1;
			tv_set(TV_SHOWAGE, getmsg(WON));
			prfmsg(SHOWAGE);
			doprintoutput();
			return(1);
			}
		if ( margc == 2 && sameas( margv[1], "OFF" ) )
			{	/* user wishes to toggle show age setting off */
			dfaSetBlk(userglobdata);
			if ( dfaAcqEQ(&udata, usaptr->userid, 0) )
				{	/* update setting */
				cvtDataIP(&udata, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
				udata.showage=0;
				cvtDataIP(&udata, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
				dfaUpdate(&udata);
				}
			dfaRstBlk();
			showages[usrnum]=0;
			tv_set(TV_SHOWAGE, getmsg(WOFF));
			prfmsg(SHOWAGE);
			doprintoutput();
			return(1);
			}
		if ( margc == 3 && sameas( margv[1], "SET" ) && ( strlen(margv[2]) == 1 ) )
			{	/* user wishes to change default Who's Online display option */
			displaychoice=atoi(margv[2]);
			if ( ( displaychoice < 1 ) || ( displaychoice > (INT)NUMWHODIS ) )
				{	/* choice is bogus */
				tv_setn(TV_MIN, 1);
				tv_setn(TV_MAX, (INT)NUMWHODIS);
				prfmsg(BADWHO);
				doprintoutput();
				return(1);
				}
			dfaSetBlk(userglobdata);
			if ( dfaAcqEQ(&udata, usaptr->userid, 0) )
				{	/* update setting */
				cvtDataIP(&udata, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
				udata.whochoice=(UCHAR)displaychoice;
				cvtDataIP(&udata, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
				dfaUpdate(&udata);
				}
			dfaRstBlk();
			whonumber[usrnum]=(UCHAR)displaychoice;
			tv_setn(TV_WHODISP, displaychoice);
			prfmsg(DEFLTWHO);
			doprintoutput();
			return(1);
			}
		if ( margc == 2 && ( strlen(margv[1]) == 1 ) )
			{	/* user wishes to view different Who's Online from default */
			displaychoice=atoi(margv[1]);
			if ( ( displaychoice < 1 ) || ( displaychoice > (INT)NUMWHODIS ) )
				{	/* choice is bogus */
				tv_setn(TV_MIN, 1);
				tv_setn(TV_MAX, (INT)NUMWHODIS);
				prfmsg(BADWHO);
				doprintoutput();
				return(1);
				}
			listusers((UCHAR)displaychoice);
			return(1);
			}
		rstmbk();
		listusers(whonumber[usrnum]);
		return(1);
		}
	if ( margc == 1 && sameas( margv[0], help[9].command ) ) /* print date and time */
		{
		setmbk(glomb);
		if ( !haskey(help[9].key) )           /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		tv_set(TV_DATE, ncdate(today()));
		tv_set(TV_TIME, ck_time(usrnum, now()));
		prfmsg(DATTIM);
		doprintoutput();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], help[20].command ) )    /* post credits to user */
		{
		post_credits();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], help[35].command ) )    /* boot user off system and possibly suspend */
		{
                boot_user();
		return(1);
		}
	if ( ( margc > 0 ) && ( margc < 3 ) && sameas( margv[0], help[16].command ) )
		{       /* list new sign-ups */
		INT gotrecord;            /* 1 if user record obtained, 0 if none */
		struct usracc newacc;     /* structure for user's account data */
		CHAR callingfrom[NADSIZ]; /* user's city and state with ZIP code removed */

		setmbk(glomb);
		if ( !haskey(help[16].key) )           /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		poll[usrnum].newusers=0;
		if ( ( margc == 1 ) || ( strlen(margv[1]) > (DATSIZ-1) ) )
			{
			poll[usrnum].date=today();
			}
		else
			{
			poll[usrnum].date=dcdate(margv[1]);
			}
		dfaSetBlk(accbb);
		gotrecord=dfaAcqLO(&newacc, 0);   /* get newest record */
		if ( gotrecord )
			{
			if ( (INT)newacc.credat == poll[usrnum].date )
				{	/* new sign-up */
				stripnonalppun( newacc.usrad3, callingfrom );
				tv_set(TV_DATE, ncdate(poll[usrnum].date));
				prfmsg(NEWHDR);
				tv_set(TV_USERID, newacc.userid);
				tv_set(TV_FROM, callingfrom);
				prfmsg(NWSMSG);
				poll[usrnum].newusers=1;
				}
			strcpy(poll[usrnum].saveid, newacc.userid);
			poll[usrnum].action=LIST_NEW;
			doprintoutput();
			dfaRstBlk();
			begin_polling(usrnum, poll_routine);
			return(-1);
			}
		else
			{
			prfmsg(SGNMSG);
			doprintoutput();
			dfaRstBlk();
			}
		return(1);
		}
	if ( margc == 1 && sameas( margv[0], help[17].command ) )
		{       /* log-on notices */
		struct userdata logdat;			/* user's global data */

		setmbk(glomb);
		if ( !haskey(help[17].key) )           /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		usrchannel[usrnum]^=1;
		dfaSetBlk(userglobdata);
		if ( dfaAcqEQ(&logdat, usaptr->userid, 0) )
			{
			logdat.ourswitch=usrchannel[usrnum];
			dfaUpdate(&logdat);
			}
		dfaRstBlk();
		tv_set(TV_NOTICES, (usrchannel[usrnum] == 1) ? getmsg(WON) : getmsg(WOFF));
		prfmsg(NTONOF);
		doprintoutput();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], logonstr ) )
		{       /* user wishes to edit log-on message */
		CHAR logmess[INPSIZ];   /* new log-on message */
		struct userdata logdat;	/* user's global data */

		setmbk(glomb);
		if ( !haskey(help[17].key) )           /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		if ( margc == 1 )
			{       /* clear message */
			dfaSetBlk(userglobdata);
			if ( dfaAcqEQ(&logdat, usaptr->userid, 0) )
				{       /* update record */
				cvtDataIP(&logdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
				setmem( logdat.logon, MESSSIZE, 0 );
				cvtDataIP(&logdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
				dfaUpdate(&logdat);
				prfmsg(LONCLR);
				doprintoutput();
				}
			dfaRstBlk();
			return(1);
			}
		getstring( logmess, 1, (margc-1) );
		if ( strlen(logmess) > MESSSIZE-1 )
			{       /* too long! */
			tv_setn(TV_MAX, (INT)MESSSIZE-1);
			prfmsg(MES2BIG);
			doprintoutput();
			return(1);
			}
		dfaSetBlk(userglobdata);
		if ( dfaAcqEQ(&logdat, usaptr->userid, 0) )
			{       /* update record */
			cvtDataIP(&logdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
			strcpy( logdat.logon, logmess );
			cvtDataIP(&logdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
			dfaUpdate(&logdat);
			tv_set(TV_LOGONMSG, logmess);
			prfmsg(LONSET);
			doprintoutput();
			}
		dfaRstBlk();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], logoffstr ) )
		{	/* user wishes to edit log-off message */
		CHAR logmess[INPSIZ];   /* new log-on message */
		struct userdata logdat;	/* user's global data */

		setmbk(glomb);
		if ( !haskey(help[17].key) )           /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		if ( margc == 1 )
			{       /* clear message */
			dfaSetBlk(userglobdata);
			if ( dfaAcqEQ(&logdat, usaptr->userid, 0) )
				{       /* update record */
				cvtDataIP(&logdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
				setmem( logdat.logoff, MESSSIZE, 0 );
				cvtDataIP(&logdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
				dfaUpdate(&logdat);
				prfmsg(LOFFCLR);
				doprintoutput();
				}
			dfaRstBlk();
			return(1);
			}
		getstring( logmess, 1, (margc-1) );
		if ( strlen(logmess) > MESSSIZE-1 )
			{       /* too long! */
			tv_setn(TV_MAX, (INT)MESSSIZE-1);
			prfmsg(MES2BIG);
			doprintoutput();
			return(1);
			}
		dfaSetBlk(userglobdata);
		if ( dfaAcqEQ(&logdat, usaptr->userid, 0) )
			{       /* update record */
			cvtDataIP(&logdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
			strcpy( logdat.logoff, logmess );
			cvtDataIP(&logdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
			dfaUpdate(&logdat);
			tv_set(TV_LOGOFFMSG, logmess);
			prfmsg(LOFFSET);
			doprintoutput();
			}
		dfaRstBlk();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], clearstr ) )
		{       /* delete a user's messages */
		struct usracc delacc;   /* user to clear user account data */
		struct userdata dellog; /* user's messages and other log-on data */
		CHAR delid[INPSIZ];     /* user-id of user to clear */

		setmbk(glomb);
		if ( !haskey(logclearkey) )          /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		if ( margc == 1 )
			{       /* show proper usage of command */
			prfmsg(CLRUSG);
			doprintoutput();
			return(1);
			}
		getuserid( delid, 1, (margc-1) );
		dfaSetBlk(accbb);
		if ( !dfaAcqEQ(&delacc, delid, 0) )
			{       /* no such user on here */
			tv_set(TV_USERID, delid);
			prfmsg(NOSCHGUY);
			doprintoutput();
			dfaRstBlk();
			return(1);
			}
		dfaRstBlk();
		dfaSetBlk(userglobdata);
		if ( !dfaAcqEQ(&dellog, delacc.userid, 0) )
			{       /* user has no messages */
			tv_set(TV_USERID, delacc.userid);
			prfmsg(NODATA);
			}
		else
			{       /* check for actual messages */
			cvtDataIP(&dellog, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
			if ( sameas( dellog.logon, "" ) && sameas( dellog.logoff, "" ) )
				{       /* already blank dude */
				tv_set(TV_USERID, delacc.userid);
				prfmsg(CLRNOW);
				}
			else
				{       /* delete 'em */
				setmem( dellog.logon, MESSSIZE, 0 );
				setmem( dellog.logoff, MESSSIZE, 0 );
				cvtDataIP(&dellog, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
				dfaUpdate(&dellog);
				tv_set(TV_USERID, delacc.userid);
				prfmsg(MESCLR);
				shocst("USER'S LOG MESSAGES CLEARED",
				       "%s by %s", delacc.userid, usaptr->userid);
				}
			}
		doprintoutput();
		dfaRstBlk();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], showmessstr ) )
		{       /* display user's log-on and log-off messages */
		CHAR showid[INPSIZ];    /* user-id to show messages of */
		struct usracc showacc;  /* user account data for showid */
		struct userdata showlog;/* showid's message data */

		setmbk(glomb);
		if ( !haskey(help[17].key) )           /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		if ( margc == 1 )
			{       /* show user's messages */
			dfaSetBlk(userglobdata);
			if ( dfaAcqEQ(&showlog, usaptr->userid, 0) )
				{       /* show user's messages */
				cvtDataIP(&showlog, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
				tv_set(TV_LOGONMSG, sameas( showlog.logon, "" ) ? getmsg(WNOMSG) : showlog.logon);
				tv_set(TV_LOGOFFMSG, sameas( showlog.logoff, "" ) ? getmsg(WNOMSG) : showlog.logoff);
				prfmsg(SHOW);
				outprf(usrnum);
				}
			rstmbk();
			dfaRstBlk();
			return(1);
			}
		if ( !haskey(logclearkey) )          /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		getuserid( showid, 1, (margc-1) );
		dfaSetBlk(accbb);
		if ( !dfaAcqEQ(&showacc, showid, 0 ) )
			{       /* no such user */
			tv_set(TV_USERID, showid);
			prfmsg(NOSCHGUY);
			doprintoutput();
			dfaRstBlk();
			return(1);
			}
		dfaRstBlk();
		dfaSetBlk(userglobdata);
		if ( !dfaAcqEQ(&showlog, showacc.userid, 0) )
			{       /* no data available on user */
			tv_set(TV_USERID, showacc.userid);
			prfmsg(NODATA);
			}
		else
			{       /* show the messages */
			cvtDataIP(&showlog, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
			tv_set(TV_USERID, showacc.userid);
			tv_set(TV_LOGONMSG, sameas( showlog.logon, "" ) ? getmsg(WNOMSG) : showlog.logon);
			tv_set(TV_LOGOFFMSG, sameas( showlog.logoff, "" ) ? getmsg(WNOMSG) : showlog.logoff);
			prfmsg(USRMSG);
			}
		doprintoutput();
		dfaRstBlk();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], help[6].command ) )
		{       /* global chat channel command */
		INT return_value;	/* return value from global_chat() */

		return_value=global_chat();
		return(return_value);
		}
	if ( margc > 0 && sameas( margv[0], help[38].command ) )
		{
		setmbk(glomb);
		if ( !haskey(help[38].key) )          /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		if ( margc == 1 )
			{       /* print usage of command */
			prfmsg(BRCSTHLP);
			doprintoutput();
			return(1);
			}
		globalmessage();
		rstmbk();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], help[30].command ) ) /* tranfer credits */
		{
		transfer_credits();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], help[31].command ) ) /* transfer days in class */
		{
                transfer_days();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], help[32].command ) )
		{	/* user wants to extend time on-line of someone */
                extend_time();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], help[21].command ) )
		{       /* post days in class to user */
                post_days();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], help[7].command ) )
		{       /* display user settings */
		CHAR settingid[INPSIZ]; /* the user-id of the person to get settings on */
		struct usracc ouruser;  /* our user's account data */
		struct usracc *ourptr;	/* pointer to user's account data */
		struct userdata udat;	/* our user's global data */
		struct userdata *udptr; /* pointer to user's global data */

		INIT_STRUCT_TO_ZERO(ouruser);
		INIT_STRUCT_TO_ZERO(udat);

		setmbk(glomb);
		if ( margc == 1 )
			{	/* display current user settings */
			if ( !haskey(help[7].key) )          /* check that user may use this global */
				{
				cantuse();
				return(1);
				}
			udptr=&udat;
			dfaSetBlk(userglobdata);
			dfaAcqEQ(udptr, usaptr->userid, 0);
			cvtDataIP(udptr, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
			dfaRstBlk();
			ourusersettings(udptr);
			}
		else
			{
			if ( !haskey(othinfokey) )
				{
				cantuse();
				return(1);
				}
			getuserid( settingid, 1, (margc-1) );
			ourptr=&ouruser;
			dfaSetBlk(accbb);
			if ( !dfaAcqEQ(ourptr, settingid, 0) )
				{       /* no such person exists */
				tv_set(TV_USERID, settingid);
				prfmsg(NOSCHGUY);
				doprintoutput();
				dfaRstBlk();
				return(1);
				}
			dfaRstBlk();
			udptr=&udat;
			dfaSetBlk(userglobdata);
			if ( !dfaAcqEQ(udptr, ourptr->userid, 0) )
				{       /* no such user on-line */
				tv_set(TV_USERID, ourptr->userid);
				prfmsg(NOUSRDAT);
				doprintoutput();
				dfaRstBlk();
				return(1);
				}
			dfaRstBlk();
			cvtDataIP(udptr, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
			otherusersettings(udptr, ourptr);
			}
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], help[27].command ) )
		{	/* switch a user's class */
		switch_class();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], help[33].command ) )
		{	/* give/take user key */
		key_handler();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], help[14].command ) )
		{       /* toggle invisibility */
                toggle_invis();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], help[28].command ) )
		{       /* suspend/unsuspend user */
                suspend_user();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], help[26].command ) )
		{       /* suspend user with automatic unsuspend */
		INT return_value;	/* return value from autosuspend_user() */

		return_value=autosuspend_user();
		return(return_value);
		}
	if ( margc > 0 && sameas( margv[0], help[15].command ) )
		{       /* delete/undelete user */
                delete_user();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], pageforgetstring ) )
		{       /* block/unblock a user's pages */
                block_page();
		return(1);
		}
	if ( margc == 1 && sameas( margv[0], repagestring ) )
		{       /* re-send last page */
		setmbk(glomb);
		if ( !haskey(help[19].key) )
			{
			cantuse();
			return(1);
			}
		if ( userinvis() && !haskey(pageinviskey) )
			{       /* can't do this while invisible */
			prfmsg(URINVIS);
			doprintoutput();
			return(1);
			}
		if ( sameas( userpage[usrnum].lastpage, "" ) )
			{       /* no previous page */
			prfmsg(NPPAG);
			doprintoutput();
			return(1);
			}
		strcpy( input, userpage[usrnum].lastpage );
		parsin();
		rstmbk();
		}
	if ( margc > 0 && sameas( margv[0], help[19].command ) )
		{       /* check for page service */
                handle_page();
		return(1);
		}
	if ( margc == 1 && sameas( margv[0], help[18].command ) )
		{       /* display macro help */
		setmbk(glomb);
		if ( !haskey(help[18].key) )            /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		tv_setn(TV_MAX, (INT)MACSIZ-1);
		tv_setn(TV_MAXCMD, (INT)CMSIZ-1);
		prfmsg(MACHLP);
		doprintoutput();
		return(1);
		}
	if ( margc == 3 && sameas( margv[0], macrocommand ) )
		{       /* user wishes to set a macro command string */
		struct userdata usermacs;       /* user macro data to save to disk */
		INT macronumber;                /* number of macro to set */
		CHAR newcommand[INPSIZ];        /* new macro command string */

		setmbk(glomb);
		if ( !haskey(help[18].key) )           /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		macronumber=atoi(margv[1]);
		if ( ( macronumber < 0 ) || ( macronumber > 9 ) )
			{       /* user entered a bogus number */
			prfmsg(BDNMSG);
			doprintoutput();
			return(1);
			}
		strcpy( newcommand, margv[2] );
		if ( strlen(newcommand) > CMSIZ-1 )
			{       /* command string too long */
			tv_setn(TV_MAX, CMSIZ-1);
			prfmsg(CTLMSG);
			doprintoutput();
			return(1);
			}
		strcpy( themacros[usrnum].command[macronumber], newcommand );
		tv_setn(TV_NUMBER, macronumber);
		tv_set(TV_MACROCMD, newcommand);
		prfmsg(CMDCHG);
		doprintoutput();
		dfaSetBlk(userglobdata);
		if ( dfaAcqEQ(&usermacs, usaptr->userid, 0) )
			{	/* save new command */
			cvtDataIP(&usermacs, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
			strcpy( usermacs.command[macronumber], newcommand );
			cvtDataIP(&usermacs, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
			dfaUpdate(&usermacs);
			}
		dfaRstBlk();
		return(1);
		}
	if ( margc > 1 && sameas( margv[0], macromacstring ) )
		{       /* user wishes to set a macro */
		struct userdata usermacs;       /* save new macro to disk */
		INT macronumber;                /* number of macro to set */
		CHAR newmacro[INPSIZ];          /* new macro string */

		setmbk(glomb);
		if ( !haskey(help[18].key) )           /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		macronumber=atoi(margv[1]);
		if ( ( macronumber < 0 ) || ( macronumber > 9 ) )
			{       /* user entered a bogus number */
			prfmsg(BDNMSG);
			doprintoutput();
			return(1);
			}
		if ( margc == 2 )
			{       /* clear the desired macro */
			setmem( themacros[usrnum].macro[macronumber], MACSIZ, 0 );
			tv_setn(TV_NUMBER, macronumber);
			prfmsg(MACCLR);
			doprintoutput();
			return(1);
			}
		getstring( newmacro, 2, (margc-1) );
		if ( strlen(newmacro) > MACSIZ-1 )
			{       /* macro too long */
			tv_setn(TV_MAX, MACSIZ-1);
			prfmsg(MTLMSG);
			doprintoutput();
			return(1);
			}
		strcpy( themacros[usrnum].macro[macronumber], newmacro );
		tv_setn(TV_NUMBER, macronumber);
		tv_set(TV_MACRO, newmacro);
		prfmsg(MACCHG);
		doprintoutput();
		dfaSetBlk(userglobdata);
		if ( dfaAcqEQ(&usermacs, usaptr->userid, 0) )
			{	/* save new macro */
			cvtDataIP(&usermacs, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
			strcpy( usermacs.macro[macronumber], newmacro );
			cvtDataIP(&usermacs, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
			dfaUpdate(&usermacs);
			}
		dfaRstBlk();
		return(1);
		}
	if ( margc == 1 && sameas( margv[0], macrocommandlist ) )
		{       /* user wishes to list macro command strings */
		CHAR listcount;         /* loop counter for printing macro commands */

		setmbk(glomb);
		if ( !haskey(help[18].key) )           /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		prfmsg(CMDHDR);
		for ( listcount=0; listcount < NUMMAC; listcount++ )
			{
			tv_setn(TV_NUMBER, listcount);
			tv_set(TV_MACROCMD, themacros[usrnum].command[listcount]);
			prfmsg(CMD);
			}
		doprintoutput();
		return(1);
		}
	if ( margc == 1 && sameas( margv[0], macrolist ) )
		{       /* user wishes to list macros */
		CHAR listcount;         /* loop counter for printing macros */

		setmbk(glomb);
		if ( !haskey(help[18].key) )           /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		prfmsg(MACHDR);
		for ( listcount=0; listcount < NUMMAC; listcount++ )
			{
			tv_setn(TV_NUMBER, listcount);
			tv_set(TV_MACRO, themacros[usrnum].macro[listcount]);
			prfmsg(MACOUTFM);
			}
		doprintoutput();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], help[41].command ) )
		{	/* user wishes to set or remove an "alarm" */
		do_alarm();
		return(1);
		}
	if ( ( margc > 0 ) && ( margc < 3 ) && sameas( margv[0], telegramalert ) )
		{       /* toggle telegram alert setting */
		struct userdata tmalt;            /* copy new alert setting to disk */

		setmbk(glomb);
		if ( !haskey(help[39].key) )           /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		if ( margc == 1 )
			{       /* toggle current setting */
			notifytelegrams[usrnum]^=1;
			dfaSetBlk(userglobdata);
			if ( dfaAcqEQ(&tmalt, usaptr->userid, 0) )
				{
				cvtDataIP(&tmalt, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
				tmalt.telegramnotify=notifytelegrams[usrnum];
				cvtDataIP(&tmalt, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
				dfaUpdate(&tmalt);
				}
			dfaRstBlk();
			tv_set(TV_TELENOTE, (notifytelegrams[usrnum] == 1) ? getmsg(WON) : getmsg(WOFF));
			prfmsg(NTTMSET);
			doprintoutput();
			return(1);
			}
		if ( sameas( margv[1], "?" ) )
			{       /* display current setting */
			tv_set(TV_TELENOTE, (notifytelegrams[usrnum] == 1) ? getmsg(WON) : getmsg(WOFF));
			prfmsg(TMNTSET);
			doprintoutput();
			return(1);
			}
		rstmbk();
		}
	if ( margc > 0 && sameas( margv[0], teleblockstring ) )
		{       /* block/unblock a user's telegrams */
		tele_blocks();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], help[43].command ) )
		{	/* friends log notification */
                handle_friends();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], telegramclear ) )
		{       /* erase all telegrams from someone */
		erase_telegrams();
		return(1);
		}
	if ( margc > 0 && sameas( margv[0], deleteallteles ) )
		{       /* delete all telegrams on system from a user */
                delete_telegrams();
		return(1);
		}
	if ( margc == 0 && ( telestate[usrnum] == GETTELE ) )
		{       /* user is trying to reply with a blank telegram */
		setmbk(glomb);
		prfmsg(TMRMSG);
		doprintoutput();
		return(-1);
		}
	if ( margc == 0 && ( telestate[usrnum] == GETRESPONSE ) )
		{       /* user pressed return or old input */
		setmbk(glomb);
		prfmsg(TMMSG);
		doprintoutput();
		return(-1);
		}
	if ( margc > 0 && ( telestate[usrnum] == GETTELE ) )
		{       /* user is responding to telegram */
		struct telegram reply;          /* reply data for saving to disk */

		if ( margc == 1 && sameas( margv[0], "x" ) )
			{       /* don't send this reply */
			setmbk(glomb);
			tv_set(TV_USERID, teletouser[usrnum].from);
			prfmsg(NORPLY);
			tv_set(TV_USERID, teletouser[usrnum].from);
			tv_set(TV_DATE, ncdate(teletouser[usrnum].datesent));
			tv_set(TV_TIME, ck_time(usrnum, teletouser[usrnum].timesent));
			prfmsg(TELEHDR);
			prfmsg(TTXTMSG);
			prf("%s", teletouser[usrnum].telegramtext);
			prfmsg(TTXTMEND);
			prfmsg(TMMSG);
			doprintoutput();
			telestate[usrnum]=GETRESPONSE;
			return(-1);
			}
		else
			{       /* send the reply */
			dfaSetBlk(telegramdata);
			preparetelegram(&reply, teletouser[usrnum].from);
			getstring( reply.telegramtext, 0, (margc-1) );
			dfaInsert(&reply);
			setmbk(glomb);
			tv_set(TV_USERID, reply.to);
			prfmsg(TSNTMSG);
			doprintoutput();
			if ( notifyoftelegram(reply.to) )
				{
				telltelegram();
				}
			if ( dfaAcqEQ(NULL, usaptr->userid, 0) )
				{	/* delete this telegram */
				dfaDelete();
				}
			}
		if ( !dfaAcqEQ(&teletouser[usrnum], usaptr->userid, 0) )
			{
			setmbk(glomb);
			prfmsg(NOTMMSG);
			doprintoutput();
			telestate[usrnum]=NORMAL;
			dfaRstBlk();
			return(1);
			}
		dfaRstBlk();
		setmbk(glomb);
		prfmsg(NXTTMSG);
		tv_set(TV_USERID, teletouser[usrnum].from);
		tv_set(TV_DATE, ncdate(teletouser[usrnum].datesent));
		tv_set(TV_TIME, ck_time(usrnum, teletouser[usrnum].timesent));
		prfmsg(TELEHDR);
		prfmsg(TTXTMSG);
		prf("%s", teletouser[usrnum].telegramtext);
		prfmsg(TTXTMEND);
		prfmsg(TMMSG);
		doprintoutput();
		telestate[usrnum]=GETRESPONSE;
		return(-1);
		}
	if ( margc == 1 && ( telestate[usrnum] == GETRESPONSE ) )
		{       /* check for possible telegram response */
		struct usracc validuser;        /* account information of user to reply to */
		INT userfound;                  /* 1 if user is valid, otherwise 0 */

		setmbk(glomb);
		dfaSetBlk(telegramdata);
		if ( sameas( margv[0], "r" ) )
			{       /* reply to telegram */
			if ( userinvis() && !haskey(teleinviskey) )
				{       /* can't do this while invisible */
				prfmsg(URINVIS);
				doprintoutput();
				return(1);
				}
			dfaSetBlk(accbb);
			userfound=dfaAcqEQ(&validuser, teletouser[usrnum].from, 0);
			dfaRstBlk();
			if ( !userfound )
				{       /* user has been deleted */
				tv_set(TV_USERID, teletouser[usrnum].from);
				prfmsg(TDLMSG);
				prfmsg(TMMSG);
				doprintoutput();
				dfaRstBlk();
				return(1);
				}
			if ( taggedforkill(&validuser) )
				{       /* user is tagged for deletion */
				tv_set(TV_USERID, teletouser[usrnum].from);
				prfmsg(TDLMSG);
				prfmsg(TMMSG);
				doprintoutput();
				dfaRstBlk();
				return(1);
				}
			prfmsg(TMRMSG);
			doprintoutput();
			telestate[usrnum]=GETTELE;
			dfaRstBlk();
			return(-1);
			}
		if ( sameas( margv[0], "n" ) )
			{       /* get next telegram */
			if ( dfaAcqEQ(NULL, usaptr->userid, 0) )
				{	/* delete this telegram */
				dfaDelete();
				}
			if ( !dfaAcqEQ(&teletouser[usrnum], usaptr->userid, 0) )
				{	/* no other telegrams */
				prfmsg(NOTMMSG);
				doprintoutput();
				telestate[usrnum]=NORMAL;
				dfaRstBlk();
				return(1);
				}
			dfaDelete();
			prfmsg(NXTTMSG);
			tv_set(TV_USERID, teletouser[usrnum].from);
			tv_set(TV_DATE, ncdate(teletouser[usrnum].datesent));
			tv_set(TV_TIME, ck_time(usrnum, teletouser[usrnum].timesent));
			prfmsg(TELEHDR);
			prfmsg(TTXTMSG);
			prf("%s", teletouser[usrnum].telegramtext);
			prfmsg(TTXTMEND);
			prfmsg(TMMSG);
			doprintoutput();
			telestate[usrnum]=GETRESPONSE;
			dfaRstBlk();
			return(-1);
			}
		if ( sameas( margv[0], "e" ) || sameas( margv[0], "x" ) )
			{       /* quit telegrams */
			telestate[usrnum]=NORMAL;
			dfaRstBlk();
			rstmbk();
			return(1);
			}
		if ( sameas( margv[0], "s" ) )
			{       /* re-display telegram */
			tv_set(TV_USERID, teletouser[usrnum].from);
			tv_set(TV_DATE, ncdate(teletouser[usrnum].datesent));
			tv_set(TV_TIME, ck_time(usrnum, teletouser[usrnum].timesent));
			prfmsg(TELEHDR);
			prfmsg(TTXTMSG);
			prf("%s", teletouser[usrnum].telegramtext);
			prfmsg(TTXTMEND);
			prfmsg(TMMSG);
			doprintoutput();
			dfaRstBlk();
			return(-1);
			}
		if ( sameas( margv[0], "?" ) )
			{       /* print help */
			prfmsg(THLPSTG);
			}
		prfmsg(TMMSG);
		doprintoutput();
		dfaRstBlk();
		return(-1);
		}
	if ( margc == 1 && sameas( margv[0], help[39].command ) )
		{       /* check for telegram */
		setmbk(glomb);
		if ( !haskey(help[39].key) )           /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		dfaSetBlk(telegramdata);
		if ( dfaAcqEQ(&teletouser[usrnum], usaptr->userid, 0) )
			{       /* user has telegram */
			tv_set(TV_USERID, teletouser[usrnum].from);
			tv_set(TV_DATE, ncdate(teletouser[usrnum].datesent));
			tv_set(TV_TIME, ck_time(usrnum, teletouser[usrnum].timesent));
			prfmsg(TELEHDR);
			prfmsg(TTXTMSG);
			prf("%s", teletouser[usrnum].telegramtext);
			prfmsg(TTXTMEND);
			prfmsg(TMMSG);
			doprintoutput();
			telestate[usrnum]=GETRESPONSE;
			dfaRstBlk();
			return(-1);
			}
		else
			{
			prfmsg(NOTMMSG);
			doprintoutput();
			dfaRstBlk();
			return(1);
			}
		}
	if ( margc == 1 && ( margv[0][0] == telesendchar ) && ( strlen(margv[0]) == 1 ) )
		{	/* print help on telegrams */
		setmbk(glomb);
		if ( !haskey(help[39].key) )            /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		telegramhelp();
		return(1);
		}
	if ( margc > 0 && ( margv[0][0] == telesendchar ) && ( strlen(margv[0]) > 1 ) )
		{       /* check for telegram service */
		handle_telegrams();
		return(1);
		}
	return(0);
	}
