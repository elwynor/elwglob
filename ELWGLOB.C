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

VOID EXPORT init__elwglob( VOID );                      /* module initialization routine */
extern INT elwglobals( VOID );                          /* the globals */

HMCVFILE glomb;                                         /* globals configuration variables */
DFAFILE *globchatdata;                                  /* global chat channel topics */
DFAFILE *userglobdata;                                  /* user's individual chat channel and macro data */
DFAFILE *telegramdata;                                  /* telegrams for users */
DFAFILE *suspenddata;                                   /* users to unsuspend on a given date */
DFAFILE *notkeydata;                                    /* negative keys users have */

CHAR *GLO_NAME="Globals";                               /* the module name */
CHAR *GLO_VERSION="3.43";                               /* version number */

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
	logonstr=stgopt(LONSTR);
	logoffstr=stgopt(LOFFSTR);
	clearstr=stgopt(LCLRSTR);
	showmessstr=stgopt(SHOWSTR);
	echostring=stgopt(ECHSTG);
	setstring=stgopt(SETSTG);
	showstring=stgopt(SHOWSTG);
	channotestring=stgopt(NOTSTG);
	displaystring=stgopt(DISSTG);
	clearstring=stgopt(CLRSTG);
	topicstring=stgopt(TPCSTG);
	macrocommand=stgopt(MCSET);
	macromacstring=stgopt(MMSET);
	macrocommandlist=stgopt(MCLIST);
	macrolist=stgopt(MMLIST);
	telesendchar=(CHAR)chropt(TLCCHR);
	logon=stgopt(LGNSTG);
	logoff=stgopt(LGFSTG);
	newsignup=stgopt(SUPSTG);
	repagestring=stgopt(RPGSTG);
	pageforgetstring=stgopt(FPUSTG);
	pageme=stgopt(CPGKEY);
	endbeeps=stgopt(BPSTPSTG);
	unbusyothkey=stgopt(BSYOKEY);
	relogothkey=stgopt(URLGKEY);
	unlimpostkey=stgopt(UNLPTKEY);
	logclearkey=stgopt(LCLRKEY);
	syschankey=stgopt(SYSCKEY);
	chanclearkey=stgopt(CLRKEY);
	chatinviskey=stgopt(GCIKEY);
	displaykey=stgopt(DISKEY);
	unlimtrankey=stgopt(UNLTRKEY);
	nomindaykey=stgopt(NMTDKEY);
	othinfokey=stgopt(OUSKEY);
	pageinviskey=stgopt(PGIKEY);
	pagebusykey=stgopt(PGBSYKEY);
	pagethrukey=stgopt(PTUKEY);
	deltelekey=stgopt(TDAKEY);
	teleinviskey=stgopt(TMIKEY);
	telenoblockkey=stgopt(TNKKEY);
	telegramalert=stgopt(TALTSTG);
	telegramclear=stgopt(TCLRSTG);
	deleteallteles=stgopt(TDASTG);
	teleblockstring=stgopt(TBKSTG);
   seecredskey=stgopt(SEECKEY);
	bpstpkey=stgopt(BPSTPKEY);
	infolookkey=stgopt(OTHKEY);
	ANSIchar=(CHAR)chropt(ANSCHR);
	backchar=(CHAR)chropt(BCKCHR);
	forechar=(CHAR)chropt(FRECHR);
	specchar=(CHAR)chropt(SPCCHR);
	menuname=stgopt(MNUNAM);
	editorname=stgopt(EDTNAM);
	fileservicename=stgopt(FILNAM);
	endback=numopt(ENBNUM, 0, 7);
	pitch=numopt(BPPIT, 0, 2000);
	numbeeps=numopt(BPDUR, 0, 120);
	pagedelay=numopt(PGDLY, 0, 300);
	busyinterval=numopt(CHKSECS, 1, 10);
	minimumtrandays=numopt(MINTRD, 1, MAXDAYSPOST);
	pageall=ynopt(PGALL);
	loglist=ynopt(LOGLIST);
	logchannotice=ynopt(LOGCHNOT);
	showminvis=ynopt(SHWMINV);
	topchginaudit=ynopt(STPCHG);
	dobeeps=ynopt(DOBEEPS);
	invisusernotify=ynopt(SNKUSER);
	invisnotify=ynopt(SNEAK);
	displaypages=ynopt(SHOWPG);
	sendthrubusy=ynopt(SNDTHRU);
	checkinterval=numopt(CHKUNS, 0, 28800);
	checktels=numopt(CHKTELS, 0, 5);
	checkalarm=numopt(CHKALRM, 1, 5);
	alarmbeeps=numopt(ALRMBPS, 0, 10);
	checkfriend=numopt(CHKFRND, 1, 5);
	changeseconds=numopt(CHKCHG, 0, 10);
	maxpostcreds=lngopt(MXPST, 1, 9999999L);
	minimumtransfer=lngopt(MNTRFR, 1, 999999L);
	numchatchannels=numopt(NUMCHAT, 1000, 10000);
	strcpy( help[0].message, stgopt(HELP1) );
	strcpy( help[0].key, stgopt(HLPKEY) );
	strcpy( help[0].command, stgopt(HLPSTG) );
	strcpy( help[1].key, stgopt(HLPKEY) );
	strcpy( help[1].command, stgopt(HLPSTG) );
	strcat( help[1].command, stgopt(HELCAPP) );
	strcpy( help[1].message, stgopt(HELP2) );
	strcpy( help[2].message, stgopt(HELP3) );
	strcpy( help[2].key, stgopt(ANSTKEY) );
	strcpy( help[2].command, stgopt(ANSSTG) );
	strcpy( help[3].message, stgopt(HELP4) );
	strcpy( help[3].key, stgopt(BSYKEY) );
	strcpy( help[3].command, stgopt(BSYSTG) );
	strcpy( help[4].message, stgopt(HELP5) );
	strcpy( help[4].key, stgopt(BSNKEY) );
	strcpy( help[4].command, stgopt(BNTSTG) );
	strcpy( help[5].message, stgopt(HELP6) );
	strcpy( help[5].key, stgopt(UCIKEY) );
	strcpy( help[5].command, stgopt(CRDSTG) );
	strcpy( help[6].message, stgopt(HELP7) );
	strcpy( help[6].key, stgopt(GLBKEY) );
	strcpy( help[6].command, stgopt(CHATSTG) );
	strcpy( help[7].message, stgopt(HELP8) );
	strcpy( help[7].key, stgopt(ACSTKEY) );
	strcpy( help[7].command, stgopt(CSTSTG) );
	strcpy( help[8].message, stgopt(HELP9) );
	strcpy( help[8].key, stgopt(CTOKEY) );
	strcpy( help[8].command, stgopt(CNTSTG) );
	strcpy( help[9].message, stgopt(HELP10) );
	strcpy( help[9].key, stgopt(DNTKEY) );
	strcpy( help[9].command, stgopt(DNTSTG) );
	strcpy( help[10].message, stgopt(HELP11) );
	strcpy( help[10].key, stgopt(DUPKEY) );
	strcpy( help[10].command, stgopt(DUPSTG) );
	strcpy( help[11].message, stgopt(HELP12) );
	strcpy( help[11].key, stgopt(PGKEY) );
	strcpy( help[11].command, stgopt(BPSTG) );
	strcpy( help[12].message, stgopt(HELP13) );
	strcpy( help[12].key, stgopt(EDTKEY) );
	strcpy( help[12].command, stgopt(EDTSTG) );
	strcpy( help[13].message, stgopt(HELP14) );
	strcpy( help[13].key, stgopt(IBMKEY) );
	strcpy( help[13].command, stgopt(IBMSTG) );
	strcpy( help[14].message, stgopt(HELP15) );
	strcpy( help[14].key, stgopt(INVKEY) );
	strcpy( help[14].command, stgopt(INVSTG) );
	strcpy( help[15].message, stgopt(HELP16) );
	strcpy( help[15].key, stgopt(DLUKEY) );
	strcpy( help[15].command, stgopt(DLUSTG) );
	strcpy( help[16].message, stgopt(HELP17) );
	strcpy( help[16].key, stgopt(SGNKEY) );
	strcpy( help[16].command, stgopt(NEWSTG) );
	strcpy( help[17].message, stgopt(HELP18) );
	strcpy( help[17].key, stgopt(LOGKEY) );
	strcpy( help[17].command, stgopt(LOGSTG) );
	strcpy( help[18].message, stgopt(HELP19) );
	strcpy( help[18].key, stgopt(MACKEY) );
	strcpy( help[18].command, stgopt(MACSTG) );
	strcpy( help[19].message, stgopt(HELP20) );
	strcpy( help[19].key, stgopt(PAGKEY) );
	strcpy( help[19].command, stgopt(PGUSTG) );
	strcpy( help[20].message, stgopt(HELP22) );
	strcpy( help[20].key, stgopt(PSTCKEY) );
	strcpy( help[20].command, stgopt(PSTSTG) );
	strcpy( help[21].message, stgopt(HELP23) );
	strcpy( help[21].key, stgopt(PDYKEY) );
	strcpy( help[21].command, stgopt(PDYSTG) );
	strcpy( help[22].message, stgopt(HELP21) );
	strcpy( help[22].key, stgopt(EPSTKEY) );
	strcpy( help[22].command, stgopt(EPSTSTG) );
	strcpy( help[23].message, stgopt(HELP24) );
	strcpy( help[23].key, stgopt(RPTKEY) );
	strcpy( help[23].command, stgopt(RPTSTG) );
	strcpy( help[24].message, stgopt(HELP25) );
	strcpy( help[24].key, stgopt(RLGKEY) );
	strcpy( help[24].command, stgopt(RLGSTG) );
	strcpy( help[25].message, stgopt(HELP26) );
	strcpy( help[25].key, stgopt(SCLKEY) );
	strcpy( help[25].command, stgopt(SLSTG) );
	strcpy( help[26].message, stgopt(HELP27) );
	strcpy( help[26].key, stgopt(SUSKEY) );
	strcpy( help[26].command, stgopt(ASUSSTG) );
	strcpy( help[27].message, stgopt(HELP28) );
	strcpy( help[27].key, stgopt(SWCKEY) );
	strcpy( help[27].command, stgopt(SWCSTG) );
	strcpy( help[28].message, stgopt(HELP29) );
	strcpy( help[28].key, stgopt(SUSKEY) );
	strcpy( help[28].command, stgopt(SUSSTG) );
	strcpy( help[29].message, stgopt(HELP30) );
	strcpy( help[29].key, stgopt(SCWKEY) );
	strcpy( help[29].command, stgopt(SWSTG) );
	strcpy( help[30].message, stgopt(HELP31) );
	strcpy( help[30].key, stgopt(TRCKEY) );
	strcpy( help[30].command, stgopt(TRNSTG) );
	strcpy( help[31].message, stgopt(HELP32) );
	strcpy( help[31].key, stgopt(TRDKEY) );
	strcpy( help[31].command, stgopt(TRDSTG) );
	strcpy( help[32].message, stgopt(HELP33) );
	strcpy( help[32].key, stgopt(TEXTKEY) );
	strcpy( help[32].command, stgopt(TEXTSTG) );
	strcpy( help[33].message, stgopt(HELP34) );
	strcpy( help[33].key, stgopt(GTUKEY) );
	strcpy( help[33].command, stgopt(UKSTG) );
	strcpy( help[34].message, stgopt(HELP35) );
	strcpy( help[34].key, stgopt(OFFKEY) );
	strcpy( help[34].command, stgopt(OFFSTG) );
	strcpy( help[35].message, stgopt(HELP36) );
	strcpy( help[35].command, stgopt(ZAPSTG) );
	strcpy( help[35].key, stgopt(BTUKEY) );
	strcpy( help[36].message, stgopt(HELP37) );
	strcpy( help[36].key, stgopt(MBRKEY) );
	strcpy( help[36].command, stgopt(MBRSTG) );
	strcpy( help[37].message, stgopt(HELP38) );
	strcpy( help[37].key, stgopt(WHOKEY) );
	strcpy( help[37].command, stgopt(WHOSTG) );
	strcpy( help[38].message, stgopt(HELP39) );
	strcpy( help[38].key, stgopt(SYSBKEY) );
	strcpy( help[38].command, stgopt(GLBSTG) );
	strcpy( help[39].message, stgopt(HELP40) );
	strcpy( help[39].key, stgopt(TGMKEY) );
	strcpy( help[39].command, stgopt(GTLSTG) );
	strcpy( help[40].key, stgopt(TGMKEY) );
	setmem( help[40].command, HELPCOMSIZE, 0 );
	help[40].command[0]=telesendchar;
	strcat( help[40].command, stgopt(TELCAPP) );
	strcpy( help[40].message, stgopt(HELP41) );
	strcpy( help[41].key, stgopt(ALRMKEY) );
	strcpy( help[41].command, stgopt(ALRMSTG) );
	strcpy( help[41].message, stgopt(HELP42) );
	strcpy( help[42].message, stgopt(HELP43) );
	strcpy( help[42].key, stgopt(COLKEY) );
	setmem( help[42].command, HELPCOMSIZE, 0 );
	help[42].command[0]=ANSIchar;
	strcpy( help[43].command, stgopt(FRNDSTG) );
	strcpy( help[43].message, stgopt(HELP44) );
	strcpy( help[43].key, stgopt(FRNDKEY) );
	defaultcom[0]=stgopt(MACCM0);	/* initialize default macro command strings */
	defaultcom[1]=stgopt(MACCM1);
	defaultcom[2]=stgopt(MACCM2);
	defaultcom[3]=stgopt(MACCM3);
	defaultcom[4]=stgopt(MACCM4);
	defaultcom[5]=stgopt(MACCM5);
	defaultcom[6]=stgopt(MACCM6);
	defaultcom[7]=stgopt(MACCM7);
	defaultcom[8]=stgopt(MACCM8);
	defaultcom[9]=stgopt(MACCM9);
	defaultmac[0]=stgopt(MACMC0);	/* initialize default macro command strings */
	defaultmac[1]=stgopt(MACMC1);
	defaultmac[2]=stgopt(MACMC2);
	defaultmac[3]=stgopt(MACMC3);
	defaultmac[4]=stgopt(MACMC4);
	defaultmac[5]=stgopt(MACMC5);
	defaultmac[6]=stgopt(MACMC6);
	defaultmac[7]=stgopt(MACMC7);
	defaultmac[8]=stgopt(MACMC8);
	defaultmac[9]=stgopt(MACMC9);
	whotopics[0]=stgopt(TOPIC1);    /* initialize Who's Online topic strings */
	whotopics[1]=stgopt(TOPIC2);
	whotopics[2]=stgopt(TOPIC3);
	dfaSetBlk(globchatdata);
	for ( inittopiccnt=1; inittopiccnt < numchatchannels; inittopiccnt++ )
		{       /* initialize chat channel topic data */
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
	strcpy( topics[numchatchannels].chantopic, stgopt(SYSTOPIC) );
	topics[numchatchannels].channelnumber=(USHORT)numchatchannels;
	moduleblocks[0]=stgopt(BLOCK1);
	moduleblocks[1]=stgopt(BLOCK2);
	moduleblocks[2]=stgopt(BLOCK3);
	moduleblocks[3]=stgopt(BLOCK4);
	moduleblocks[4]=stgopt(BLOCK5);
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
	if ( checkinterval > 0 )
		{       /* check for automatic unsuspend at specified intervals */
		rtkick( checkinterval, autounsuspend );
		}
	rtkick( busyinterval, isbusy );
	if ( checktels > 0 )
		{	/* check for "Busy" users having telegrams */
		rtkick( checktels, telalert );
		}
	rtkick( checkalarm, doalarm );
	rtkick( checkfriend, dofriend );
   
   shocst(spr("ELW Globals v%s",GLO_VERSION),"(C) Copyright 2021-2024 Elwynor Technologies - www.elwynor.com");

	}

GBOOL globinput( VOID )                     /* user input routine */
	{
	return(FALSE);
	}

INT elwglobals( VOID )                        /* the globals */
	{
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
		prfmsg(PSTTOG, (bugem[usrnum]) ? "ON" : "OFF");
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
			prfmsg(NOPG, pagedelay);
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
		prfmsg(ANSIMSG, (usaptr->ansifl&ANSON) ? "ON" : "OFF");
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
		prfmsg(DUPTOG, duplex[usrnum] == 0 ? "HALF" : "FULL");
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
		prfmsg(EDITTYP, usaptr->usrprf == PRFLIN ? "LINE" : "FULL SCREEN");
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
		prfmsg(IBMMSG, (usaptr->systyp) ? "ON" : "OFF");
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
			prfmsg(SCNHLP, (INT)MIN_SCREEN_WIDTH, (INT)MAX_SCREEN_WIDTH, help[29].command, (INT)usaptr->scnwid);
			doprintoutput();
			return(1);
			}
		screenwidth=atoi(margv[1]);
		if ( ( screenwidth >= MIN_SCREEN_WIDTH ) && ( screenwidth <= MAX_SCREEN_WIDTH ) )
			{
			usaptr->scnwid=(CHAR)screenwidth;
			btutsw(usrnum, usaptr->scnwid);/* needed for change to immediately take effect */
			prfmsg(SCNSET, usaptr->scnwid);
			}
		else
			{
			prfmsg(NOSET, (INT)MIN_SCREEN_WIDTH, (INT)MAX_SCREEN_WIDTH);
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
			prfmsg(LNGHLP, (INT)MIN_SCREEN_LENGTH, (INT)MAX_SCREEN_LENGTH, help[25].command, (usaptr->scnbrk == CTNUOS) ? "CONTINUOUS" : spr("%d", usaptr->scnbrk));
			doprintoutput();
			return(1);
			}
		screenlength=atoi(margv[1]);
		if ( ( screenlength >= MIN_SCREEN_LENGTH ) && ( screenlength <= MAX_SCREEN_LENGTH ) )
			{
			usaptr->scnbrk=(CHAR)screenlength;
			usaptr->scnfse=(CHAR)screenlength; /* set full screen editor length */
			rstrxf();       /* needed for change to immediately take effect */
			prfmsg(LNGSET, screenlength);
			}
		else
			{
			prfmsg(NOLNG, (INT)MIN_SCREEN_LENGTH, (INT)MAX_SCREEN_LENGTH);
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
			prfmsg(SHOWAGE, "ON");
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
			prfmsg(SHOWAGE, "OFF");
			doprintoutput();
			return(1);
			}
		if ( margc == 3 && sameas( margv[1], "SET" ) && ( strlen(margv[2]) == 1 ) )
			{	/* user wishes to change default Who's Online display option */
			displaychoice=atoi(margv[2]);
			if ( ( displaychoice < 1 ) || ( displaychoice > (INT)NUMWHODIS ) )
				{	/* choice is bogus */
				prfmsg(BADWHO, 1, (INT)NUMWHODIS);
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
			prfmsg(DEFLTWHO, displaychoice);
			doprintoutput();
			return(1);
			}
		if ( margc == 2 && ( strlen(margv[1]) == 1 ) )
			{	/* user wishes to view different Who's Online from default */
			displaychoice=atoi(margv[1]);
			if ( ( displaychoice < 1 ) || ( displaychoice > (INT)NUMWHODIS ) )
				{	/* choice is bogus */
				prfmsg(BADWHO, 1, (INT)NUMWHODIS);
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
		prfmsg(DATTIM, ncdate(today()), nctime(now()));
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
				prfmsg(NEWHDR, ncdate(poll[usrnum].date));
				prfmsg(NWSMSG, newacc.userid, callingfrom);
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
		prfmsg(NTONOF, (usrchannel[usrnum] == 1) ? "ON" : "OFF");
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
			prfmsg(MES2BIG, (INT)MESSSIZE-1);
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
			prfmsg(LONSET, logmess);
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
			prfmsg(MES2BIG, (INT)MESSSIZE-1);
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
			prfmsg(LOFFSET, logmess);
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
			prfmsg(CLRUSG, clearstr);
			doprintoutput();
			return(1);
			}
		getuserid( delid, 1, (margc-1) );
		dfaSetBlk(accbb);
		if ( !dfaAcqEQ(&delacc, delid, 0) )
			{       /* no such user on here */
			prfmsg(NOSCHGUY, delid);
			doprintoutput();
			dfaRstBlk();
			return(1);
			}
		dfaRstBlk();
		dfaSetBlk(userglobdata);
		if ( !dfaAcqEQ(&dellog, delacc.userid, 0) )
			{       /* user has no messages */
			prfmsg(NODATA, delacc.userid);
			}
		else
			{       /* check for actual messages */
			cvtDataIP(&dellog, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
			if ( sameas( dellog.logon, "" ) && sameas( dellog.logoff, "" ) )
				{       /* already blank dude */
				prfmsg(CLRNOW, delacc.userid);
				}
			else
				{       /* delete 'em */
				setmem( dellog.logon, MESSSIZE, 0 );
				setmem( dellog.logoff, MESSSIZE, 0 );
				cvtDataIP(&dellog, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
				dfaUpdate(&dellog);
				prfmsg(MESCLR, delacc.userid);
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
				prfmsg(SHOW, sameas( showlog.logon, "" ) ? "<No message>" : showlog.logon,
				       sameas( showlog.logoff, "" ) ? "<No message>" : showlog.logoff);
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
			prfmsg(NOSCHGUY, showid);
			doprintoutput();
			dfaRstBlk();
			return(1);
			}
		dfaRstBlk();
		dfaSetBlk(userglobdata);
		if ( !dfaAcqEQ(&showlog, showacc.userid, 0) )
			{       /* no data available on user */
			prfmsg(NODATA, showacc.userid);
			}
		else
			{       /* show the messages */
			cvtDataIP(&showlog, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
			prfmsg(USRMSG, showacc.userid, sameas( showlog.logon, "" ) ? "<No message>" : showlog.logon,
			       sameas( showlog.logoff, "" ) ? "<No message>" : showlog.logoff);
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
			prfmsg(BRCSTHLP, help[38].command);
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
				prfmsg(NOSCHGUY, settingid);
				doprintoutput();
				dfaRstBlk();
				return(1);
				}
			dfaRstBlk();
			udptr=&udat;
			dfaSetBlk(userglobdata);
			if ( !dfaAcqEQ(udptr, ourptr->userid, 0) )
				{       /* no such user on-line */
				prfmsg(NOUSRDAT, ourptr->userid);
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
		prfmsg(MACHLP, (INT)MACSIZ-1, (INT)CMSIZ-1, help[0].command, help[18].command, help[18].command, macrocommand, macromacstring, macrocommandlist, macrolist);
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
			prfmsg(CTLMSG, CMSIZ-1);
			doprintoutput();
			return(1);
			}
		strcpy( themacros[usrnum].command[macronumber], newcommand );
		prfmsg(CMDCHG, macronumber, newcommand);
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
			prfmsg(MACCLR, macronumber);
			doprintoutput();
			return(1);
			}
		getstring( newmacro, 2, (margc-1) );
		if ( strlen(newmacro) > MACSIZ-1 )
			{       /* macro too long */
			prfmsg(MTLMSG, MACSIZ-1);
			doprintoutput();
			return(1);
			}
		strcpy( themacros[usrnum].macro[macronumber], newmacro );
		prfmsg(MACCHG, macronumber, newmacro);
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
			prfmsg(CMD, listcount, themacros[usrnum].command[listcount]);
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
			prfmsg(MACOUTFM, listcount, themacros[usrnum].macro[listcount]);
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
			prfmsg(NTTMSET, (notifytelegrams[usrnum] == 1) ? "ON" : "OFF");
			doprintoutput();
			return(1);
			}
		if ( sameas( margv[1], "?" ) )
			{       /* display current setting */
			prfmsg(TMNTSET, (notifytelegrams[usrnum] == 1) ? "ON" : "OFF");
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
			prfmsg(NORPLY, teletouser[usrnum].from);
			prfmsg(TELEHDR, teletouser[usrnum].from, ncdate(teletouser[usrnum].datesent), nctime(teletouser[usrnum].timesent));
			prfmsg(TTXTMSG, teletouser[usrnum].telegramtext);
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
			prfmsg(TSNTMSG, reply.to);
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
		prfmsg(TELEHDR, teletouser[usrnum].from, ncdate(teletouser[usrnum].datesent), nctime(teletouser[usrnum].timesent));
		prfmsg(TTXTMSG, teletouser[usrnum].telegramtext);
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
				prfmsg(TDLMSG, teletouser[usrnum].from);
				prfmsg(TMMSG);
				doprintoutput();
				dfaRstBlk();
				return(1);
				}
			if ( taggedforkill(&validuser) )
				{       /* user is tagged for deletion */
				prfmsg(TDLMSG, teletouser[usrnum].from);
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
			prfmsg(TELEHDR, teletouser[usrnum].from, ncdate(teletouser[usrnum].datesent), nctime(teletouser[usrnum].timesent));
			prfmsg(TTXTMSG, teletouser[usrnum].telegramtext);
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
			prfmsg(TELEHDR, teletouser[usrnum].from, ncdate(teletouser[usrnum].datesent), nctime(teletouser[usrnum].timesent));
			prfmsg(TTXTMSG, teletouser[usrnum].telegramtext);
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
			prfmsg(TELEHDR, teletouser[usrnum].from, ncdate(teletouser[usrnum].datesent), nctime(teletouser[usrnum].timesent));
			prfmsg(TTXTMSG, teletouser[usrnum].telegramtext);
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
