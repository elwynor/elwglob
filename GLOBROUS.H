/*****************************************************************************
 *                                                                           *
 *       GLOBROUS.H - Function and variable definitions for Globals          *
 * Developed 07/25/94 by 'Easy Ed' Erdman.   Major BBS developer code 'EZE'. *
 *                                                                           *
 * Updates: 12/18/94  - 2.0                                                  *
 *		       6/05/95  - 3.0                                                *
 *           8/21/95  - 3.1                                                  *
 *		       2/22/96  - 3.2                                                *
 *           5/25/96  - 3.3                                                  *
 *		        4/6/97  - 3.3W                                               *
 *		     5/19/2021  - 3.4                                                *
 *          8/1/2024  - 3.41                                                 *
 *									                                         *
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

#include "elwglob.h"
#include "gcomm.h"
#include "majorbbs.h"
#include <limits.h>
#include <float.h>
#include "mjrtime.h"
#include "daterous.h"
//include "cvtapi.h"

#define MIN_SCREEN_WIDTH 40             /* smallest possible screen width */
#define MAX_SCREEN_WIDTH 136            /* largest possible screen width */
#define MIN_SCREEN_LENGTH 3             /* smallest possible screen length */
#define MAX_SCREEN_LENGTH 254           /* largest possible screen length */
#define MAXTERMS 256                    /* maximum number of user channels */
#define CHATTOPSIZ 30                   /* maximum size of chat channel topic */
#define MAXCHATCHAN 10001               /* maximum number of chat channels, minus 1 */
#define MAXCHANDIG 5			             /* maximum number of digits in chat channel number */
#define NORMAL 0                        /* regular state of user in telestate[usrnum] */
#define GETRESPONSE 1                   /* need to get user's choice of next action */
#define GETTELE 2                       /* getting telegram to send in reply */
#define USEROFF 1                       /* user exists but is not online */
#define USERON 2                        /* user exists and is online */
#define NUMFLAGS 5                      /* number of flags to show in who's online */
#define NUMMAC 10                       /* number of macros each user has */
#define CMSIZ 5                         /* maximum length of macro command string */
#define MACSIZ 40                       /* maximum size of a macro */
#define MINDAYSPOST -999                /* minimum number of days in class to post */
#define MAXDAYSPOST 999                 /* maximum number of days in class to post */
#define MAXPOSTDIGITS 4                 /* maximum number of digits in days in class post */
#define MAXTRANDAYDIG 3			          /* maximum number of digits in days in class transfer */
#define MAXPOST 9999999L		          /* maximum absolute value of a credit posting */
#define MAXPOSTDIG 8			             /* maximum number of digits in credit posting amount */
#define TRANCRDDIG 7			             /* maximum number of digits in credit transfer amount */
#define MAXSUSDIG 3			             /* maximum number of digits in autosuspend days */
#define NUMFORGETS 5                    /* number of users whose pages can be blocked */
#define PAGEOK 120                      /* two minutes (120 seconds) between pages with page = ok */
#define PON 2                           /* userpage[].pagesetting for ok to page */
#define POK 1                           /* userpage[].pagesetting for page every two min */
#define POFF 0                          /* userpage[].pagesetting for pages off */
#define MAXSUS 365                      /* maximum number of days to automatically unsuspend after */
#define HELPSIZE 44                     /* number of topics to have help about */
#define NUMBLOCKS 5                     /* number of users whose telegrams can be blocked */
#define NUMANSICOLORS 8                     /* number of ANSI colors (0-7) to choose from */
#define NUMSPECIAL 4                    /* number of special ANSI color options (0-5) */
#define HELPCOMSIZE 10                  /* number of characters in user help command */
#define HELPMSSSIZE 25                  /* number of characters in user help message */
#define MESSSIZE 40                     /* maximum number of characters in log-on and log-off messages */
#define NUMWHODIS 3			             /* number of possible Who's Online display outputs to choose from */
#define ON 1                            /* pass to log notice handler for a log-on */
#define OFF 0                           /* pass to log notice handler for a log-off */
#define REAL 1                          /* pass to useralrt() if real (not faked) log */
#define FAKE 0                          /* pass to useralrt() if faked log */
#define NORMNBOLD 9                     /* characters from end of input string we need to place normal, then bold ANSI */
#define JUSTBOLD 5                      /* characters from end of input string we need to place bold ANSI */
#define DOBACK 5                        /* characters from end of input string we need for restoring background */
#define NORMALCOLOR 0                   /* ANSI special effect to remove blink or reverse effects */
#define BOLDCOLOR 1                     /* ANSI special effect to restore bold after blink or reverse or normal color */
#define JOINCHAN 1			             /* a user has joined chat channel */
#define LEFTCHAN 0			             /* a user has left chat channel */
#define MINTIMEEXT -999                 /* minimum number of minutes to extend on-line time */
#define MAXTIMEEXT 999                  /* maximum number of minutes to extend on-line time */
#define MAXTIMEDIGITS 4                 /* maximum number of digits in minutes in time extend */
#define NUMMODBLKS 5			             /* maximum number of modules in which users are blocked from using globals */
#define DEMODAYS 14			             /* number of days in DEMO period */
#define LIST_NEW 0			             /* poll[usrnum].action when listing new users */
#define CLEAR_CHAT 1			             /* poll[usrnum].action when clearing chat channel topics */
#define LIST_SUSPEND 2			          /* poll[usrnum].action when listing suspended users with auto unsuspend */

#define INIT_STRUCT_TO_ZERO(structure) memset(&structure, 0, sizeof(structure))

typedef struct                          /* user flags in usrptr */
	{
	CHAR *flagname;                 /* name of user flag */
	ULONG flagvalue;        	/* value of flag to check for */
	} flagstruct;
typedef struct                          /* user's last input string and count */
	{
	CHAR thecommand[INPSIZ];        /* input string */
	} lastcommand;
typedef struct                          /* user's paging data */
	{
	CHAR lastpage[INPSIZ];          /* user's last page sent */
	USHORT nextpage;          	/* next time user may be paged, for page = ok */
	USHORT pagedate;          	/* next date user may be paged, for page = ok */
	CHAR pagesetting;               /* 0 = off, 1 = ok, 2 = on */
	CHAR forget[NUMFORGETS][UIDSIZ];/* users whose pages are blocked */
	} pagedata;
typedef struct                          /* user's macro commands and macros */
	{
	CHAR command[NUMMAC][CMSIZ];    /* macro command string to call macro */
	CHAR macro[NUMMAC][MACSIZ];     /* user's macro, where command[x] calls macro[x] */
	} usermacros;
typedef struct                          /* help access and message structure */
	{
	CHAR command[HELPCOMSIZE];      /* the command string for global */
	CHAR message[HELPMSSSIZE];      /* the help information */
	CHAR key[KEYSIZ];               /* key to access help information */
	} userhelp;
typedef struct                          /* users blocked from sending telegrams */
	{
	CHAR block[NUMBLOCKS][UIDSIZ];  /* users blocked */
	} teleblocks;
typedef struct                          /* user-ids of person checking and person to check */
	{
	CHAR busyid[UIDSIZ];            /* user-id of person with busy flag */
	CHAR notid[UIDSIZ];             /* user-id to notify when no longer busy */
	} busynotids;
typedef struct				/* users to be notified of when they log on or off the BBS */
	{
	CHAR logonid[UIDSIZ];		/* user to be notified of if they log on */
	CHAR logoffid[UIDSIZ];		/* user to be notified of if they log off */
	} friends;
typedef struct				/* time and date of next allowed chat channel number change */
	{
	USHORT switchdate;		/* date of allowed change */
	USHORT switchtime;	        /* time of allowed change */
	} chanchange;
typedef struct 				/* save data when polling disk reads */
	{
	CHAR saveid[UIDSIZ];		/* key of last record */
	USHORT date;			/* date if any */
	INT action;			/* what we are doing */
	INT newusers;			/* 1 if new users found, 0 if not */
	INT start_chan;			/* starting chat channel number to clear */
	INT end_chan;			/* ending chat channel number to clear */
	INT cur_chan;			/* chat channel we are currently clearing */
	} poll_data;
struct userdata                                         /* user's globals data */
{
	CHAR userid[UIDSIZ];                   		        /* user's handle */
	USHORT chatchannel;                   	              /* user's chat channel */
	USHORT logtime;					                       /* time of last log-on */
	UCHAR echo;                             	           /* 1 = echo global chat output to user, 0 = don't */
	UCHAR duplex;                           	           /* 1 = full duplex (echo), 0 = half duplex */
	UCHAR pagemode;                         	           /* 0 = off, 1 = ok, 2 = on */
	UCHAR telegramnotify;                   	           /* 1 = notify of telegrams, 0 = don't notify */
	UCHAR invis;					                          /* 1 = set user invisible, 0 = don't */
	UCHAR buginedit;				                          /* 1 = injoth() to user while in Editor, 0 = don't */
	UCHAR ourswitch;                                     /* 1 = receive log-on notices, 0 = don't */
	UCHAR channelnotice;			                          /* 1 = give join/leave chat channel notices, 0 = don't */
	UCHAR busy;					                             /* 1 = user's "Busy" flag is on, 0 = "Busy" off */
	UCHAR showage;					                          /* 1 = show user's age in Who's Online display, 0 = don't show age */
	UCHAR whochoice;				                          /* choice of which Who's Online display to show by default */ //45
	CHAR command[NUMMAC][CMSIZ];                         /* macro command string to call macro */ //95
	CHAR macro[NUMMAC][MACSIZ];                          /* user's macro, where command[x] calls macro[x] */ //495
	CHAR pageforgets[NUMFORGETS][UIDSIZ];                /* users blocked from paging */ //645
	CHAR teleblocks[NUMBLOCKS][UIDSIZ];                  /* users blocked from sending telegrams */ //795
	CHAR logon[MESSSIZE];                                /* user's log-on message */ //835
	CHAR logoff[MESSSIZE];                               /* user's log-off message */ //875
}; // size 875
struct topicdata                                        /* chat channel topic data ezegchat.dat / elwglobc.dat */
{
	USHORT channelnumber;                                /* the channel in question */
	CHAR chantopic[CHATTOPSIZ];                          /* the topic for the channel */
}; //size 32

struct telegram                                         /* telegram data structure - ezegram.dat / elwglobg.dat */
{
	CHAR to[UIDSIZ];                                     /* telegram is to this user */
	CHAR from[UIDSIZ];                                   /* telegram is from this user */
	USHORT datesent;                                     /* telegram sent on this date */
	USHORT timesent;                                     /* telegram sent at this time */
	CHAR telegramtext[INPSIZ];                           /* pointer to message */
}; // size 320

struct autounsus                                        /* user to automatically unsuspend ezegsus.dat / elwglobs.dat */
{
	CHAR susid[UIDSIZ];                                  /* user-id */
	USHORT date;                                         /* date to unsuspend on */
}; // size 32

struct notkey                                           /* "negative" key data ezegnot.dat / elwglobn.dat */
{
	CHAR keyid[UIDSIZ];                                  /* user with key removed from class keys */
	CHAR keyname[KEYSIZ];                                /* the key to remove */
}; // size: 46

extern GBOOL globinput( VOID );                         /* module user input routine */
extern VOID glofin( VOID );                           	/* BBS shutdown routine */
extern GBOOL glologon( VOID );                          /* user log-on routine */
extern VOID deleteuser( CHAR *usertodelete );           /* delete user from our database */
extern INT blockpage( INT oth, CHAR *pageid );         	/* return 1 if ok to page, 0 if not */
extern VOID pagesysops( VOID );                        	/* pages console and all Sysops online */
extern VOID autounsuspend( VOID );                      /* automatically unsuspend users */
extern VOID midnitunsuspend( VOID );                    /* midnight cleanup automatic unsuspend routine */
extern INT getusersinchannel( USHORT channel );        	/* gets the number of users using a given chat channel */
extern VOID listusers( UCHAR number );			/* list users on the BBS, with Who's Online choice of number */
extern VOID endnote( VOID );				/* end user's "Busy" notification, if any in effect */
extern VOID isbusy( VOID );		                /* check if user is busy */
extern INT bsysearch( CHAR *uid );                     	/* returns 0 if no such user, 1 if found, 2 if more than 1 possibility */
extern VOID lofalrt( struct userdata *uptr );          	/* user log-off handler */
extern VOID lonalrt( struct userdata *uptr );          	/* user log-on handler */
extern INT getchanused( INT unum, INT onoroff, INT forreal );/* returns # of users on system user unum can see */
extern INT uidsearch( CHAR *uid );                     	/* return 0 if no match, else # of possible matches */
extern VOID getuserid( CHAR *id, INT startarg, INT endarg );/* returns a user-id from margv[startarg] to margv[endarg] */
extern VOID getstring( CHAR *string, INT startarg, INT endarg );/* returns a string from margv[startarg] to margv[endarg] */
extern VOID beep( INT num_beeps );                      	/* "beeps" the user's computer numbeeps times */
extern VOID togglecontinuous( VOID );			/* toggle continuous output mode */
extern VOID chatchannelhelp( VOID );			/* display chat channel help messages */
extern VOID listchannelusers( VOID );			/* show user's chat channel and users on it */
extern VOID telegramhelp( VOID );			/* print help message on telegrams */
extern VOID ourusersettings( struct userdata *dptr );	/* display current user settings */
extern VOID otherusersettings( struct userdata *uptr, struct usracc *aptr );/* display another user's current settings */
extern VOID colorizehelp( VOID );			/* display help on colorize input command */
extern INT macrohandler( INT macronumber );		/* the generic macro handler */
extern INT userineditor( CHAR *username );		/* returns 1 if usrnum usernum is in the Editor, 0 if not, -1 if error */
extern INT userintransfer( CHAR *username );		/* returns 1 if username is in a file transfer, 0 if not, -1 if error */
extern VOID dochannotice( INT join );			/* notify users someone has joined or left their chat channel */
extern VOID globalmessage( VOID );			/* send a message to all users on-line */
extern VOID showcreditinfo( SHORT logtime );		/* display a user's credits and class info */
extern VOID otheruserinfo( struct usracc *otherptr, SHORT logtime, INT online );/* show credit and class info on other user */
extern INT oktotell( VOID );				/* return 1 if ok to injoth() user, 0 if not */
extern VOID stripnonalppun( CHAR *oldstr, CHAR *newstr );/* remove non-alpha characters from string str */
extern VOID chanselecterror( VOID );			/* tell user he has picked an invalid channel range to clear */
extern VOID sysalrt( struct userdata *uptr, INT onoroff );/* announce invisible logs to Sysops only */
extern VOID useralrt( struct userdata *uptr, INT onoroff, INT forreal );/* announce invisible logs to Sysops only */
extern VOID showlogmessage( struct userdata *uptr, INT onoroff );/* display log-on or log-off message */
extern VOID showteleblocks( VOID );			/* shows which user's telegrams are blocked */
extern INT cansee( VOID );				/* returns 1 if usaptr->userid can see othuap->userid, 0 if not */
extern INT canbeseen( VOID );				/* returns 1 if othuap->userid can see usaptr->userid, 0 if not */
extern INT notifyoftelegram( CHAR *usertoinform );     	/* returns 1 if tell usertoinform of telegram received, 0 if not */
extern VOID telltelegram( VOID );			/* tell user he has received a telegram */
extern INT inopchat( VOID );				/* returns 1 if othusn is in opchat, 0 if not */
extern VOID sayinchat( CHAR *userinchat );		/* tell that userinchat is in Opchat */
extern INT userinvis( VOID );				/* returns 1 if usrnum is invisible, 0 if not */
extern INT otherinvis( VOID );				/* returns 1 if othusn is invisible, 0 if not */
extern INT restricted( VOID );				/* returns 1 if usrptr doesn't have MASTER key and othusp does */
extern INT hasmaster( VOID );				/* returns 1 if usrptr has MASTER key, 0 if not */
extern INT othmaster( VOID );				/* returns 1 if othusp has MASTER key, 0 if not */
extern INT usrprotected( struct usracc *usrprot );	/* returns 1 if usrprot if PROTECTED, 0 if not */
extern INT numok( CHAR *check, INT negativeok );	/* returns 1 if check is valid number, 0 if not */
extern INT usesdays( struct usracc *chk );		/* returns 1 if chk->curcls uses days in class, 0 if not or error */
extern VOID suspendem( struct usracc *sus );		/* suspends sus->userid */
extern VOID unsuspendem( struct usracc *sus );		/* unsuspends sus->userid */
extern INT taggedforkill( struct usracc *uaptr );	/* returns 1 if uaptr->userid is DELETED, 0 if not */
extern VOID suspendtoggle( struct usracc *sus );	/* toggles suspension for uaptr->userid */
extern INT calllimit( struct clstab *tabptr, struct user *uptr );/* returns -1 if no limit per call, else minutes per call for class tabptr */
extern INT daylimit( struct clstab *tabptr, struct usracc *uaptr );/* returns -1 if no limit per day, else minutes on today for uaptr->userid */
extern INT adjustcalltime( struct user *uptr, struct clstab *cptr, INT thiscall, INT mintoadd );/* adjust time for current call */
extern INT adjustdaytime( struct usracc *uptr, struct clstab *cptr, INT thisday, INT mintoadd, INT online );/* adjust time for current call */
extern VOID helphandler( VOID );                       	/* help routine; returns 1 if help requested, 0 if not */
extern VOID cantuse( VOID );				/* informs a user he has no access to do something */
extern VOID listglobals( VOID );			/* lists all globals user has access to */
extern VOID togglebusy( struct user *uptr );		/* toggles uptr's "Busy" flag */
extern INT isauser( VOID );				/* returns 1 if othusn is an actual user, 0 if not */
extern INT usrbusy( struct user *uptr );		/* returns 1 if uptr is "Busy", 0 if not */
extern VOID setuserpointers( VOID );			/* sets up othuap and othexp to othusn */
extern VOID listnotkeys( CHAR *listid );		/* show any negative keys for user listid */
extern INT hasnotkey( CHAR *notkeyid, CHAR *key );     	/* returns 1 if notkeyid has negative key key, 0 if not */
extern VOID updateonline( CHAR *userid, CHAR *keyname, INT give );/* updates user's keys if on-line, if give == 1 gives key, else removes */
extern VOID preparetelegram( struct telegram *ptgm, CHAR *toid );/* sets fields telegram ptgm such as date and time, for any telegram */
extern VOID doprintoutput( VOID );			/* causes print of buffer contents to current user */
extern VOID toggleinvis( struct user *uptr, struct usracc *uaptr, INT onoroff );/* toggles uaptr invisible if onoroff == 1, visible if onoroff == 0 */
extern VOID connecthandler( VOID );			/* handle user's connection, both at entering password and possibly WG C/S connect */
extern VOID wg_connect( VOID );				/* C/S mode user connect handler */
extern VOID wg_disconnect( VOID );			/* C/S mode user disconnect handler */
extern INT isblocked( VOID );				/* returns 1 if user cannot use globals in current module, 0 if ok to use globals */
extern VOID telalert( VOID );				/* tells "Busy" users who are no longer "Busy" they have telegrams */
extern VOID dotelalert( INT usernum, INT onoroff );	/* mark user for alerting to telegram when he's no longer "Busy" */
extern INT alertnewtel( INT usernum );			/* returns 1 if user usernum needs to be alerted of new telegrams when not "Busy", else 0 */
extern VOID doalarm( VOID );				/* alert users at desired times */
extern VOID friendhelp( VOID );                         /* display help on Friend notice global */
extern VOID dofriend( VOID );				/* do Friends Notices for "Busy" users */
extern INT friendlonsee( INT othusernum );     		/* returns 1 if user othusernum can "see" user othusn, 0 if not */
extern INT friendloffsee( INT othusernum );    		/* returns 1 if user othusernum can "see" user othusn, 0 if not */
extern INT sendtobusy( struct usracc *uaptr );		/* returns 1 if uptr can be paged thru "Busy", 0 if not */
extern VOID handle_telegrams( VOID );			/* handle user's reading of telegrams */
extern VOID setkey( CHAR *lock );                       /* turn cur user's bit on for this key  */
extern VOID tele_blocks( VOID );			/* set and remove telegram blocks */
extern VOID do_alarm( VOID );				/* user wishes to set or remove an alarm */
extern VOID delete_telegrams( VOID );			/* delete all telegrams on the system from a user */
extern VOID erase_telegrams( VOID );			/* erase all telegrams to yourself from a user */
extern VOID handle_friends( VOID );			/* handle friends notice command */
extern VOID colorize_input( VOID );			/* color any input desired */
extern VOID repeat_input( VOID );			/* repeat previous input */
extern VOID handle_busy( VOID );			/* toggle "Busy" flag for ourselves or another user */
extern VOID busy_notify( VOID );			/* notify user when someone is no longer "Busy" */
extern INT relog( VOID );				/* relog handler */
extern VOID show_credits( VOID );			/* display a user's credit information */
extern VOID post_credits( VOID );			/* post credits to a user */
extern VOID boot_user( VOID );				/* disconnect a user */
extern INT global_chat( VOID );				/* global chat channel handler */
extern VOID transfer_credits( VOID );			/* transfer credits handler */
extern VOID transfer_days( VOID );			/* transfer days in class handler */
extern VOID extend_time( VOID );			/* extend time online of user */
extern VOID post_days( VOID );				/* post days in class to a user */
extern VOID switch_class( VOID );			/* switch a user's class */
extern VOID key_handler( VOID );			/* give/take user key */
extern VOID toggle_invis( VOID );			/* toggle invisibility for a user */
extern VOID suspend_user( VOID );			/* suspend/unsuspend a user */
extern INT autosuspend_user( VOID );			/* suspend user with automatic unsuspend */
extern VOID delete_user( VOID );			/* delete/undelete a user */
extern VOID block_page( VOID );				/* block/unblock a user's pages */
extern VOID handle_page( VOID );			/* handle paging service */
extern VOID poll_routine( VOID );			/* our polled routine for disk accesses */
