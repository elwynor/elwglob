/*****************************************************************************
 *                                                                           *
 *       GLOBROUS.C - Function definitions for Globals                       *
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

#include "globrous.h"

extern INT globstat;                                    /* the module state no. */
extern HMCVFILE glomb;                                  /* globals configuration variables */
extern DFAFILE *globchatdata;                           /* global chat channel topics */
extern DFAFILE *userglobdata;                           /* user's individual chat channel and macro data */
extern DFAFILE *telegramdata;                           /* telegrams for users */
extern DFAFILE *suspenddata;                            /* users to unsuspend on a given date */
extern DFAFILE *notkeydata;				/* negative keys users have */
extern INT base;					/* initial value to use for creating access code */
extern INT pitch;                                       /* pitch in Hz. of beep tone */
extern INT numbeeps;                                    /* number of beeps in console page */
extern INT pagedelay;                                   /* seconds between console pages */
extern INT pageall;                                     /* 1 = page every sysop on console page, otherwise 0 */
extern USHORT nextpage;                         	/* time user can page console */
extern USHORT pagedate;                           	/* date next page allowed, for accuracy near midnight */
extern CHAR lastpageid[UIDSIZ];                        	/* user-id of person paging console */
extern INT i;                                           /* number of beep loops */
extern USHORT checkdate;                                /* date for autounsuspend to know when to check */
extern CHAR *pageme;                                    /* key to be alerted to console page */
extern CHAR *GLO_NAME;					/* the module name */
extern CHAR *GLO_VERSION;                         	/* version number */
extern CHAR ansistart;                                  /* ANSI escape character */
extern CHAR bracket;                                    /* part of ANSI escape sequence */
extern CHAR endsequence;                                /* end of ANSI output */
extern CHAR inbackground;                               /* background color change character */
extern CHAR inforeground;                               /* foreground color change character */
extern CHAR specialoutput[];	                        /* ANSI "special" attribute characters */
extern CHAR regularcolors[];		                /* ANSI normal foreground and background color characters */
extern INT loglist;					/* 1 if list users on-line automatically at log-on, otherwise 0 */
extern INT logchannotice;				/* 1 if notify users at log-on and log-off of users joining/leaving their chat channel */
extern INT showminvis;					/* 1 if staff can see invisible Sysops, otherwise 0 */
extern INT busyinterval;                                /* number of seconds between checks for busy notices */
extern INT invisusernotify;				/* 1 if do log-on notice for users who are forced invisible, otherwise 0 */
extern INT invisnotify;				     	/* 1 if do log-on notice for Sysops toggling invisibility, otherwise 0 */
extern INT displaypages;				/* 1 if display current page when in "Menuing System" in Who's Online, otherwise 0 */
extern INT sendthrubusy;				/* 1 if send global message broadcasts to "Busy" users, 0 if not */
extern INT minimumtrandays;				/* minimum number of days to transfer in transfer days in class command */
extern CHAR *menuname;					/* the name of the Menuing System for Who's Online */
extern CHAR *editorname;				/* the name of the Editor module */
extern CHAR *fileservicename;				/* the name of the File Transfer Service module */
extern flagstruct flagarray[NUMFLAGS];                  /* check for 5 possible flags */
extern lastcommand usercommand[MAXTERMS];               /* array of previous input */
extern INT numchatchannels;                             /* number of chat channels used on this BBS */
extern UCHAR telestate[MAXTERMS];               	/* each user's telegram state */
extern INT checkinterval;                               /* > 0 if check at intervals for automatic unsuspend */
extern INT checktels;					/* > 0 if check at intervals for telegrams for "Busy" users */
extern INT checkalarm;					/* seconds between checks for user alarms */
extern INT alarmbeeps;					/* number of "beeps" in user's alarm message */
extern INT checkfriend;					/* seconds between checks for Friends Notices for "Busy" users */
extern INT changeseconds;				/* seconds between chat channel topic changes */

extern struct flddef userdataFDA[];

extern CHAR *whotopics[NUMWHODIS];			/* Who's On-line topic displays */
extern USHORT logtimes[MAXTERMS]; 				/* time of log-on for saving in each user's data */
extern LONG minuteson[MAXTERMS];			/* user's time on-line today in minutes, for display to others who can't "see" user */
extern UCHAR usrchannel[MAXTERMS];             		/* 1 if alert to log-ons, 0 otherwise */
extern busynotids far theids[MAXTERMS];                	/* array of users to notify */
extern userhelp far help[HELPSIZE];                    	/* help messages and keys to access */
extern pagedata userpage[MAXTERMS];                     /* our user's paging data */
extern teleblocks far blocked[MAXTERMS];               	/* users blocked from sending telegrams to online users */
extern UCHAR tgmalert[MAXTERMS];			/* 1 = tell user when no longer "Busy" he has a telegram */
extern struct topicdata topics[MAXCHATCHAN];      	/* array of chat channel topics */
extern INT globalchatchannel[MAXTERMS];                	/* each user's channel is in globalchatchannel[usrnum] */
extern UCHAR chatecho[MAXTERMS];               		/* 1 = echo global chat output to user, 0 = don't */
extern UCHAR duplex[MAXTERMS];	               		/* 1 = full duplex mode on (echo), 0 = half duplex mode on */
extern UCHAR notifytelegrams[MAXTERMS];        		/* 1 = notify user of telegrams received, 0 = don't */
extern struct telegram teletouser[MAXTERMS];            /* storage for telegram to user data */
extern UCHAR bugem[MAXTERMS];				/* 1 if pester users in the Editor, otherwise 0 */
extern UCHAR channotice[MAXTERMS];			/* 1 if notify user of others joining/leaving chat channel, 0 if not */
extern UCHAR showages[MAXTERMS];			/* 1 if show user's age in Who's Online display, 0 if not */
extern UCHAR whonumber[MAXTERMS];			/* which Who's Online display to give user */
extern friends far frienddata[MAXTERMS];		/* user special log notify data */
extern chanchange changedata[MAXTERMS];			/* users chat channel change dates and times */
extern USHORT alarmtimes[MAXTERMS][2];			/* dates and times at which to alert on-line users (date is first, followed by time) */
extern INT topchginaudit;                               /* 1 if display chat channel topic changes in audit trail, otherwise 0 */
extern INT dobeeps;					/* 1 if "beep" users at certain prompts, otherwise 0 */
extern LONG minimumtransfer;                            /* minimum amount of credits that user must transfer */
extern usermacros themacros[MAXTERMS];                  /* storage for each user's macros */
extern poll_data poll[MAXTERMS];			/* each user's polling saved data */
extern LONG maxpostcreds;                               /* maximum allowed credit post for non-Sysops */
extern CHAR *logonstr;                                  /* string to edit log-on message */
extern CHAR *logoffstr;                                 /* string to edit log-off message */
extern CHAR *clearstr;                                  /* string to clear a user's log-on and log-off messages */
extern CHAR *showmessstr;                               /* string to display your current messages */
extern CHAR *echostring;                                /* string to toggle global chat echo */
extern CHAR *setstring;                                 /* string to set user's global chat channel */
extern CHAR *showstring;                                /* string to show global chat channel topics */
extern CHAR *channotestring;			 	/* string to toggle notices of users joining/leaving channel on or off */
extern CHAR *displaystring;                             /* string to display users in global chat channels */
extern CHAR *clearstring;                               /* string to clear global chat channel topics */
extern CHAR *topicstring;                               /* string to change a chat channel topic */
extern CHAR *macrocommand;                              /* string to set macro command */
extern CHAR *macromacstring;                            /* string to set a macro itself */
extern CHAR *macrocommandlist;                          /* string to list macro command strings */
extern CHAR *macrolist;                                 /* string to list the macros themselves */
extern CHAR *defaultcom[NUMMAC];			/* default macro command strings */
extern CHAR *defaultmac[NUMMAC];			/* default macros */
extern CHAR telesendchar;                               /* character to send a telegram */
extern CHAR *logon;                                     /* output in who's online for user logging on */
extern CHAR *logoff;                                    /* output in who's online for user logging off */
extern CHAR *newsignup;                                 /* output in who's online for user signing up */
extern CHAR *repagestring;                              /* string to re-send last page */
extern CHAR *pageforgetstring;                          /* string to block a user's pages */
extern CHAR *endbeeps;                                  /* string to stop current console page */
extern CHAR *bpstpkey;					/* key required to end current console page */
extern CHAR *infolookkey;				/* key required to look up another user's class and credit info */
extern CHAR *unbusyothkey;				/* key required to toggle another user's "Busy" flag off */
extern CHAR *relogothkey;				/* key required to re-log another user */
extern CHAR *unlimpostkey;				/* key required to not have limit on post credits amount */
extern CHAR *logclearkey;				/* key required to clear another user's log messages */
extern CHAR *syschankey;				/* key required to be able to enter the Sysop's chat channel */
extern CHAR *chanclearkey;				/* key required to be able to clear global chat channel topics */
extern CHAR *chatinviskey;				/* key required to be able to use chat channels while invisible */
extern CHAR *displaykey;				/* key required to display all user's chat channels */
extern CHAR *unlimtrankey;				/* key required to be able to do an unlimited amount credit transfer */
extern CHAR *nomindaykey;				/* key to avoid a minimum amount of days in class to transfer */
extern CHAR *othinfokey;				/* key to display another user's global settings */
extern CHAR *pageinviskey;				/* key required to be able to page while invisible */
extern CHAR *pagebusykey;				/* key required to be able to page thru "Busy" */
extern CHAR *pagethrukey;				/* key required to be able to page thru page OFF or OK */
extern CHAR *deltelekey;				/* key required to be able to delete all telegrams from a user */
extern CHAR *teleinviskey;				/* key required to send telegrams while invisible */
extern CHAR *telenoblockkey;				/* key to not allow blocks of your telegrams */
extern CHAR *telegramalert;                             /* string to toggle telegram received alert */
extern CHAR *telegramclear;                             /* string to delete all telegrams from a certain user-id */
extern CHAR *deleteallteles;                            /* string to delete all telegrams on system from a user-id */
extern CHAR *teleblockstring;                           /* string to block/unblock telegrams from a user */
extern CHAR *seecredskey;                               /* key required to see how many credits another user has */
extern CHAR *moduleblocks[NUMMODBLKS];			/* modules in which users are blocked from using global commands */
extern CHAR ANSIchar;                                   /* character to begin coloring input */
extern CHAR backchar;                                   /* character to indicate to color the background of input */
extern CHAR forechar;                                   /* character to indicate to color the foreground of input */
extern CHAR specchar;                                   /* character to indicate to create special color effect */
extern INT endback;                                     /* color of background to end colorized input with */

void glofin( void )                  		/* globals shutdown */
	{
	clsmsg(glomb);
	dfaClose(globchatdata);
	dfaClose(userglobdata);
	dfaClose(telegramdata);
	dfaClose(suspenddata);
	dfaClose(notkeydata);
	}

GBOOL glologon( void )                          /* user log-on routine */
	{
	connecthandler();
	setmbk(glomb);
	dfaSetBlk(telegramdata);
	if ( dfaQueryEQ(usaptr->userid, 0) && ( haskey(help[39].key) ) && ( notifytelegrams[usrnum] == 1 ) )
		{       /* user has a telegram */
		if ( dobeeps )
			{
			beep(1);
			}
		prfmsg(TWMSG, help[39].command);
		outprf(usrnum);
		}
	dfaRstBlk();
	prfmsg(EZSHLP, GLO_VERSION, help[0].command);
	doprintoutput();
	if ( loglist )
		{	/* show Who's Online display */
		listusers(whonumber[usrnum]);
		}
	return(FALSE);
	}

void deleteuser( CHAR *usertodelete )           /* delete user from our database */
	{
	dfaSetBlk(userglobdata);
	if ( dfaAcqEQ(NULL, usertodelete, 0) )    /* if record in database */
		{
		dfaDelete();                       /* delete it */
		}
	dfaRstBlk();
	dfaSetBlk(telegramdata);
	while ( dfaAcqEQ(NULL, usertodelete, 0) ) /* check telegrams */
		{	/* get them all, as there may be more than one */
		dfaDelete();
		}
	dfaRstBlk();
	dfaSetBlk(suspenddata);                    /* remove autounsuspend */
	if ( dfaAcqEQ(NULL, usertodelete, 0) )
		{
		dfaDelete();
		}
	dfaRstBlk();
	dfaSetBlk(notkeydata);
	while ( dfaAcqEQ(NULL, usertodelete, 0) )
		{	/* remove negative keys */
		dfaDelete();
		}
	dfaRstBlk();
	}

INT blockpage( INT oth, CHAR *pageid )           	/* return 1 if ok to page, 0 if not */
	{
	CHAR checkcount=0;              /* looping counter for checking user's "forgets" */
	INT returncode=1;               /* 1 if ok to send page, 0 if not */

	while ( ( checkcount < NUMFORGETS ) && returncode )
		{
		if ( sameas( pageid, userpage[oth].forget[checkcount] ) )
			{       /* this user's pages are blocked */
			returncode=0;
			}
		checkcount++;
		}
	return(returncode);
	}

void pagesysops( void )                   	/* pages console and all Sysops online */
	{
	USHORT curtime;    /* current time */
	USHORT curdate;    /* current date */
	USHORT keyshit;	   /* whatever Sysop has typed at console */

	if ( kbhit() )
		{	/* see if Sysop has ended page at console */
		keyshit=getchc();
		if ( keyshit == ALT_Q )
			{	/* end current console page */
			i=numbeeps+1;
			}
		}
	if ( i == 1 )
		{       /* print to audit trail only on first entry to routine */
		shocst("*** CONSOLE PAGE ***", "%s just paged the console", lastpageid); /* put page message in audit trail */
		if ( pageall )
			{       /* page every Sysop on line */
			setmbk(glomb);
			for ( othusn=0, othusp=usroff(othusn); othusn < nterms; othusn++, othusp=usroff(othusn) )
				{
				setuserpointers();
				if ( ( othkey(pageme) ) && oktotell() )
					{
					if ( dobeeps )
						{
						beep(2);
						}
					prfmsg(SYSPG, lastpageid);
					if ( dobeeps )
						{
						beep(2);
						}
					injoth();
					}
				}
			rstmbk();
			}
		}
	if ( i <= numbeeps )
		{       /* beep console */
		i++;
		belper(pitch);
		printf("\7");
		rtkick( 1, pagesysops );
		return;
		}
	curtime=now();
	curdate=today();
	nextpage=(USHORT)mjrtime(pagedelay, curtime); /* find next time page is allowed */
	if ( nextpage < curtime )
		{       /* clock is set back to 0 for midnight rollover, so adjust pagedate */
		pagedate=getfdate(1, curdate);
		}
	else
		{       /* update the paging date */
		pagedate=curdate;
		}
	i=0;                                  /* reset beep counter */
	}

void midnitunsuspend( void )                  /* midnight cleanup automatic unsuspend routine */
	{
	USHORT todaysdate;              /* the date as returned by today() */
	struct autounsus sususer;       /* user to possibly unsuspend */
	INT found;                      /* 1 if records found in suspend file */
	struct usracc unsusacct;        /* user account to unsuspend */

	todaysdate=today();
	checkdate=todaysdate;   /* update date to check for */
	dfaSetBlk(suspenddata);
	found=dfaAcqLO(&sususer, 0);
	while ( found )
		{
		if ( todaysdate >= sususer.date )
			{       /* time to unsuspend */
			dfaDelete();
			dfaSetBlk(accbb);
			if ( dfaAcqEQ(&unsusacct, sususer.susid, 0) )
				{
				if ( unsusacct.flags&SUSPEN )
					{       /* still suspended, so do it */
					unsuspendem(&unsusacct);
					dfaUpdate(&unsusacct);
					shocst("USER AUTO-UNSUSPENDED",
					       "User-ID: %s", unsusacct.userid);
					}
				}
			dfaRstBlk();
			}
		found=dfaAcqGT(&sususer, sususer.susid, 0);
		}
	dfaRstBlk();
	}

void autounsuspend( void )                    /* automatically unsuspend users */
	{
	USHORT todaysdate;              /* the date as returned by today() */
	struct autounsus sususer;       /* user to possibly unsuspend */
	INT found;                      /* 1 if records found in suspend file */
	struct usracc unsusacct;        /* user account to unsuspend */

	todaysdate=today();
	if ( checkdate == todaysdate )
		{       /* no need to check since dates are the same */
		rtkick( checkinterval, autounsuspend );
		return;
		}
	checkdate=todaysdate;   /* update date to check for */
	dfaSetBlk(suspenddata);
	found=dfaAcqLO(&sususer, 0);
	while ( found )
		{
		if ( todaysdate >= sususer.date )
			{       /* time to unsuspend */
			dfaDelete();
			dfaSetBlk(accbb);
			if ( dfaAcqEQ(&unsusacct, sususer.susid, 0) )
				{
				if ( unsusacct.flags&SUSPEN )
					{       /* still suspended, so do it */
					unsuspendem(&unsusacct);
					dfaUpdate(&unsusacct);
					shocst("USER AUTO-UNSUSPENDED",
					       "User-ID: %s", unsusacct.userid);
					}
				}
			dfaRstBlk();
			}
		found=dfaAcqGT(&sususer, sususer.susid, 0);
		}
	dfaRstBlk();
	rtkick( checkinterval, autounsuspend );
	}

INT getusersinchannel( USHORT whichchannel )             	/* gets the number of users using a given chat channel */
	{
	INT usersinchan=0;              /* users currently using this channel */

	for ( othusn=0, othusp=usroff(othusn); othusn < nterms; othusn++, othusp=usroff(othusn) )
		{
		setuserpointers();
		if ( isauser() )
			{       /* legitimate user */
			if ( ( globalchatchannel[othusn] == whichchannel) && cansee() )
				{       /* found a user on this channel */
				usersinchan++;
				}
			}
		}
	return(usersinchan);
	}

void listusers( UCHAR number )				/* list users on the BBS with Who's Online choice of number */
	{
	INT flagindex;                   /* flag to check for */
	ULONG flagmatch;         	 /* & with flags to check value */
	struct usrmnu *pagptr;		 /* pointer to user's menu structure for getting current page */
	CHAR callingfrom[NADSIZ];	 /* user's city and state with ZIP code removed */
	USHORT ourdate;			 /* today's date as returned by today() */
	UCHAR offset;		 	 /* our offset for reading topic and display Who's Online info */
	CHAR agestore[4];		 /* storage space for itoa to convert age to a string */
	CHAR *padding="    ";		 /* pad second line of 3rd Who's Online display */

	ourdate=today();
	setmbk(glomb);
	prfmsg(WHOMSG, ncdate(ourdate), nctime(now()));
	offset=number;
	if ( offset )
		{	 /* normal condition where globals data is initialized */
		offset--;
		}
	prf(whotopics[offset]);
	outprf(usrnum);
	for (othusn=0, othusp=usroff(othusn); othusn < nterms; othusn++, othusp=usroff(othusn) )
		{
		setuserpointers();
		switch (othusp->usrcls)
			{
			case VACANT:
				{
				break;
				}
			case ONLINE:
				{
				prfmsg(ONLMSG, channel[othusn], logon);
				outprf(usrnum);
				break;
				}
			case SUPIPG:
				{
				prfmsg(SUPMSG, channel[othusn], newsignup);
				outprf(usrnum);
				break;
				}
			case SUPLON:
				{
				if ( cansee() )
					{       /* don't show invisible Sysops */
					prfmsg(SLONMSG, channel[othusn], othuap->userid, logon);
					outprf(usrnum);
					}
				break;
				}
			case SUPLOF:
				{
				if ( cansee() )
					{       /* don't show invisible Sysops */
					prfmsg(SLOFMSG, channel[othusn], othuap->userid, logoff);
					outprf(usrnum);
					}
				break;
				}
			default:
				{
				if ( isuidc(othuap->userid[0]) && othuap->userid[0] != '(' )
					{
					flagindex=0;
					flagmatch=0L;
					while ( flagindex < NUMFLAGS && !flagmatch ) /* check for flags */
						{
						flagmatch=flagarray[flagindex].flagvalue & othusp->flags;
						flagindex++;
						}
					if ( flagmatch )
						{
						flagindex--;
						}
					if ( cansee() )
						{       /* don't show invisible Sysop to non-Sysops */
						pagptr=mnuoff(othusn);
						switch ( offset )
							{
							case 0 :
								{
								prfmsg(REGON1, channel[othusn], othuap->userid, (displaypages && !sameas(pagptr->mnuttl, "") && sameas(module[othusp->state]->descrp, menuname)) ? pagptr->mnuttl : module[othusp->state]->descrp, othuap->curcls, othuap->sex, (showages[othusn] == 1) ? itoa(getage(ourdate, othuap->birthd), agestore, 10) : " ?", l2as(othusp->baud), othusp->minut4 >> 2, flagindex < NUMFLAGS ? flagarray[flagindex].flagname : "");
								break;
								}
							case 1 :
								{
								stripnonalppun( othuap->usrad3, callingfrom );
								prfmsg(REGON2, channel[othusn], othuap->userid, (displaypages && !sameas(pagptr->mnuttl, "") && sameas(module[othusp->state]->descrp, menuname)) ? pagptr->mnuttl : module[othusp->state]->descrp, othuap->sex, (USHORT) (showages[othusn] == 1) ? itoa(getage(ourdate, othuap->birthd), agestore, 10) : " ?", callingfrom, othusp->minut4 >> 2, flagindex < NUMFLAGS ? flagarray[flagindex].flagname : "");
								break;
								}
							case 2 :
								{
								prfmsg(REGON3, channel[othusn], othuap->userid, (displaypages && !sameas(pagptr->mnuttl, "") && sameas(module[othusp->state]->descrp, menuname)) ? pagptr->mnuttl : module[othusp->state]->descrp, othuap->curcls, padding, othuap->sex, (showages[othusn] == 1) ? itoa(getage(ourdate, othuap->birthd), agestore, 10) : " ?", l2as(othusp->baud), othusp->minut4 >> 2, flagindex < NUMFLAGS ? flagarray[flagindex].flagname : "");
								break;
								}
							}
						outprf(usrnum);
						}
					}
				}
			}
		}
	prf("\n");
	rstmbk();
	}

void endnote( void )					/* end user's "Busy" notification, if any in effect */
{	/* this ensures that user can't "beat" our code by logging on different line */
	struct userdata savetimedat;		/* save user's log-on time before leaving */

	dotelalert(usrnum, (INT)OFF);
	alarmtimes[usrnum][0]=0;
	alarmtimes[usrnum][1]=0;
	setmem( frienddata[usrnum].logonid, UIDSIZ, 0 );
	setmem( frienddata[usrnum].logoffid, UIDSIZ, 0 );
	setmem( theids[usrnum].busyid, UIDSIZ, 0 );
	setmem( theids[usrnum].notid, UIDSIZ, 0 );
	dfaSetBlk(userglobdata);
	if ( dfaAcqEQ(&savetimedat, usaptr->userid, 0) ) {	/* this about has to be TRUE */
		savetimedat.logtime=logtimes[usrnum];
		dfaUpdate(&savetimedat);
	}
	dfaRstBlk();
   /* tell others we are now leaving */
	setmbk(glomb);
	for ( othusn=0, othusp=usroff(othusn); othusn < nterms; othusn++, othusp=usroff(othusn) ) {	/* check all on-line users */
		setuserpointers();
		if ( !usrbusy(othusp) && sameas( frienddata[othusn].logoffid, usaptr->userid ) && canbeseen() ) {	/* tell user his Friend has logged off */
			if ( dobeeps )	{
				beep(1);
			}
			prfmsg(FRNDNOT, usaptr->userid, "off");
			if ( injoth() ) {	/* only clear notice if successful */
				setmem( frienddata[othusn].logoffid, UIDSIZ, 0 );
			}
		}
	}
	rstmbk();
	if ( logchannotice )	{	/* tell others we have left their chat channel */
		dochannotice(LEFTCHAN);
	}
	lofalrt(&savetimedat);	
}

void isbusy( void )                     /* check if user is busy */
	{
	INT checkbusy;                  /* loop counter to cycle through theids[] looking for checks to make */

	setmbk(glomb);
	for ( checkbusy=0; checkbusy < nterms; checkbusy++ )
		{
		if ( !sameas( theids[checkbusy].busyid, "" ) )
			{       /* check for busy user */
			if ( !onsys(theids[checkbusy].busyid) )
				{       /* user to check split */
				if ( onsysn(theids[checkbusy].notid, 1) && oktotell() )
					{       /* user to notify is still around */
					if ( dobeeps )
						{
						beep(1);
						}
					prfmsg(LEFT, theids[checkbusy].busyid);
					if ( injoth() )
						{	/* clear notice only if it goes thru */
						setmem( theids[checkbusy].busyid, UIDSIZ, 0 );
						setmem( theids[checkbusy].notid, UIDSIZ, 0 );
						}
					}
				}
			if ( !usrbusy(othusp) )
				{       /* user to check is no longer busy */
				if ( onsysn(theids[checkbusy].notid, 1) && oktotell() )
					{       /* user to notify is still with us */
					if ( dobeeps )
						{
						beep(1);
						}
					prfmsg(NTBSY, theids[checkbusy].busyid);
					if ( injoth() )
						{	/* clear notice only if it goes thru */
						setmem( theids[checkbusy].busyid, UIDSIZ, 0 );
						setmem( theids[checkbusy].notid, UIDSIZ, 0 );
						}
					}
				}
			}
		}
       rtkick( busyinterval, isbusy );
       rstmbk();
       }

INT bsysearch( CHAR *uid )       			/* returns 0 if no such user, 1 if found, 2 if more than 1 possibility */
	{
	INT match=0;                    /* result of search for matching user-id */
	CHAR possibleid[UIDSIZ];        /* user-id of match, for return in uid */

	for ( othusn=0, othusp=usroff(othusn); othusn < nterms; othusn++, othusp=usroff(othusn) )
		{
		setuserpointers();
		if ( isauser() )
			{       /* possible user here */
			if ( sameas( uid, othuap->userid ) )
				{       /* exact match, this is the dude */
				strcpy( uid, othuap->userid );
				return(1);
				}
			if ( sameto( uid, othuap->userid ) )
				{       /* partial match found */
				if ( !match )
					{       /* copy potential user-id for return */
					strcpy( possibleid, othuap->userid );
					}
				else
					{       /* more than one possible match, ERROR */
					return(2);
					}
				match++;
				}
			}
		}
	if ( match )
		{       /* 1 match found, so get exact user-id */
		strcpy( uid, possibleid );
		}
	return(match);
	}

void lonalrt( struct userdata *uptr )			/* user log-on handler */
	{
	if ( userinvis() )
		{	/* announce only to Sysops */
		sysalrt(uptr, (INT)ON);
		return;
		}
	useralrt(uptr, (INT)ON, (INT)REAL);
	}

void lofalrt( struct userdata *uptr )			/* user log-off handler */
	{
	if ( userinvis() )
		{	/* invisible user, so announce only to Sysops */
		sysalrt(uptr, (INT)OFF);
		return;
		}
	useralrt(uptr, (INT)OFF, (INT)REAL);
	}

INT getchanused( INT unum, INT onoroff, INT forreal )	/* returns # of users on system user unum can see */
	{	/* onoroff == 1 if log-on, 0 if log-off, forreal == 1 if real log, 0 if faked */
	INT numchans=0;                 /* number of lines used */
	INT saveothusn;			/* save variables because we are already in a loop */
	INT saveusrnum;
	struct user *saveothusp;
	struct usracc *saveothuap;

	saveothusn=othusn;	/* save pointers */
	saveothusp=othusp;
	saveothuap=othuap;
	saveusrnum=usrnum;
	curusr(unum);
	for ( othusn=0, othusp=usroff(othusn); othusn < nterms; othusn++, othusp=usroff(othusn) )
		{
		setuserpointers();
		if ( isauser() && cansee() )
			{      /* make sure someone is on channel */
			numchans++;
			}
		}
	if ( !onoroff && forreal && numchans )
		{	/* account for user logging off */
		numchans--;
		}
	othusn=saveothusn;	/* restore when done */
	othusp=saveothusp;
	othuap=saveothuap;
	curusr(saveusrnum);
	return(numchans);
	}

INT uidsearch( CHAR *uid )                       	/* return 0 if no match, else # of possible matches */
	{
	INT match=0;                    /* result of search for matching user-id */
	CHAR possibleid[UIDSIZ];        /* user-id of first possible match, for return in uid */

	for ( othusn=0, othusp=usroff(othusn); othusn < nterms; othusn++, othusp=usroff(othusn) )
		{
		setuserpointers();
		if ( othusp->usrcls >= SUPLON )
			{       /* possible user here */
			if ( sameto( uid, othuap->userid ) )
				{       /* partial match found */
				if ( !match )
					{       /* copy first potential match for later copy if needed */
					strcpy( possibleid, othuap->userid );
					}
				match++;
				}
			}
		}
	if ( match == 1 )
		{       /* get exact user-id for only match */
		strcpy( uid, possibleid );
		}
	return(match);
	}

void getuserid( CHAR *id, INT startarg, INT endarg )    /* returns a user-id from margv[startarg] to margv[endarg] */
	{
	INT idloop;                     /* number of strings to concat together */

	strcpy( id, margv[startarg] );
	for ( idloop=startarg+1; idloop <= endarg; idloop++ )
		{
		strcat( id, " " );
		strcat( id, margv[idloop] );
		}
	}

void getstring( CHAR *string, INT startarg, INT endarg )/* returns a string from margv[startarg] to margv[endarg] */
	{
	INT strloop;                     /* number of strings to concat together */

	strcpy( string, margv[startarg] );
	for ( strloop=startarg+1; strloop <= endarg; strloop++ )
		{
		strcat( string, " " );
		strcat( string, margv[strloop] );
		}
	}

void beep( INT num_beeps )                         	/* "beeps" the user's computer numbeeps times */
	{
	INT beepcount;                  /* looping counter for doing beeps */

	for ( beepcount=0; beepcount < num_beeps; beepcount++ )
		{
		prf("\a");
		}
	}

void togglecontinuous( void )				/* toggle continuous output mode */
	{
	if ( usaptr->scnbrk == CTNUOS )
		{       /* turn continuous output off */
		prfmsg(CNTOUT, "OFF");
		usaptr->scnbrk=usaptr->scnfse;
		}
	else
		{       /* turn continuous output on */
		prfmsg(CNTOUT, "ON");
		usaptr->scnbrk=CTNUOS;
		}
	rstrxf();
	doprintoutput();
	}

void chatchannelhelp( void )			 	/* display chat channel help messages */
	{
	prfmsg(CHATHDR);
	if ( haskey(displaykey) )
		{
		prfmsg(CHATHLP1, help[6].command, displaystring); /* print chat channel help */
		}
	prfmsg(CHATHLP2, help[6].command, echostring, help[6].command, topicstring, help[6].command, setstring, help[6].command, showstring, help[6].command, help[6].command, channotestring, help[6].command);
	if ( haskey(chanclearkey) )
		{
		prfmsg(CHATHLP3, help[6].command, clearstring, help[6].command, clearstring, help[6].command, clearstring);
		}
	outprf(usrnum);
	}

void listchannelusers( void )				/* show user's chat channel and users on it */
	{
	INT usersonchan=0;		/* 1 if other users on our chat channel, else 0 */

	for ( othusn=0, othusp=usroff(othusn); othusn < nterms; othusn++, othusp=usroff(othusn) )
		{       /* show each user on chat channel */
		setuserpointers();
		if ( isauser() && ( othusn != usrnum ) && cansee() )
			{
			if ( globalchatchannel[othusn] == globalchatchannel[usrnum] )
				{	/* same chat channel */
				if ( !usersonchan )
					{	/* first user found on channel */
					prfmsg(USRONCH);
					}
				usersonchan=1;
				prfmsg(CHUSR, othuap->userid);
				}
			}
		}
	if ( !usersonchan )
		{	/* no users currently on this channel */
		prfmsg(NOCHUSR);
		}
	}

void telegramhelp( void )				/* print help message on telegrams */
	{
	prfmsg(TELHLP1, telesendchar, help[39].command, telegramalert, telegramalert, telegramclear, teleblockstring, teleblockstring);
	if ( haskey(deltelekey) )
		{
		prfmsg(TELHLP2, deleteallteles);
		}
	doprintoutput();
	}

void ourusersettings( struct userdata *dptr )		/* display current user settings */
	{
	CHAR counter;           /* looping counter for listing page and telegram blocks */

	prfmsg(CSTMSG1, usaptr->userid, usaptr->scnwid, usaptr->scnbrk == CTNUOS ? "CONTINUOUS" : spr("%d", usaptr->scnbrk),
	       userinvis() ? "ON " : "OFF", (dptr->invis == 1) ? "YES" : "NO",
	       ( usaptr->ansifl & ANSON ) == ANSON ? "ON " : "OFF",
	       (usaptr->usrprf == PRFLIN) ? "LINE" : "FULL SCREEN",
	       (usaptr->scnbrk == CTNUOS) ? "CONTINUOUS" : "PAUSED    ",
	       ( globalchatchannel[usrnum] == 0 ) ? "OFF" : spr("%d", globalchatchannel[usrnum]), ( chatecho[usrnum] == 0 ) ? "OFF" : "ON ", ( duplex[usrnum] == 0 ) ? "HALF" : "FULL",
	       usaptr->systyp == 1 ? "IBM    " : "NON-IBM",
	       (usrchannel[usrnum] == 1) ? "ON" : "OFF",
	       usrbusy(usrptr) ? "ON      " : "OFF     ",
	       (bugem[usrnum] == 1) ? "ON" : "OFF",
	       (userpage[usrnum].pagesetting > POFF) ? ((userpage[usrnum].pagesetting == POK) ? "OK" : "ON " ) : "OFF",
	       (notifytelegrams[usrnum] == 1) ? "ON" : "OFF",
	       (showages[usrnum] == 1) ? "ON " : "OFF",
	       (INT)whonumber[usrnum],
	       (channotice[usrnum] == 1) ? "ON" : "OFF",
	       sameas( dptr->logon, "" ) ? "<None>" : dptr->logon, sameas( dptr->logoff, "" ) ? "<None>" : dptr->logoff);
	prfmsg(CSTHDR1);
	for ( counter=0; counter < NUMFORGETS; counter++ )
		{       /* list page and telegram blocks */
		prfmsg(CSTMSG2, counter+1, userpage[usrnum].forget[counter], counter+1, blocked[usrnum].block[counter]);
		}
	prfmsg(CSTHDR2);
	for ( counter=0; counter < NUMMAC; counter++ )
		{	/* list macros and command strings */
		prfmsg(CSTMSG3, counter, dptr->command[counter], dptr->macro[counter]);
		}
	prfmsg(CSTMSG4, sameas( theids[usrnum].busyid, "" ) ? "<None>" : theids[usrnum].busyid, sameas( frienddata[usrnum].logonid, "" ) ? "<None>" : frienddata[usrnum].logonid, sameas( frienddata[usrnum].logoffid, "" ) ? "<None>" : frienddata[usrnum].logoffid);
	if ( alarmtimes[usrnum][0] == 0 )
		{	/* user has no alarm set */
		prfmsg(CSTNALRM);
		}
	else
		{	/* show user's alarm */
		prfmsg(CSTMSG5, ncdate(alarmtimes[usrnum][0]), nctime(alarmtimes[usrnum][1]));
		}
	doprintoutput();
	}

void otherusersettings( struct userdata *uptr, struct usracc *aptr )/* display another user's current settings */
	{
	CHAR counter;           /* looping counter for listing page and telegram blocks */
	INT online;		/* if if aptr->userid is online and can be seen by usaptr->userid, 0 if not */

	if ( onsysn(aptr->userid, 1) && cansee() )
		{
		online=1;
		}
	else
		{
		online=0;
		}
	prfmsg(CSTMSG1, aptr->userid, aptr->scnwid, aptr->scnbrk == CTNUOS ? "CONTINUOUS" : spr("%d", aptr->scnbrk),
	       (online) ? (((othusp->flags&INVISB) != 0L) ? "ON " : "OFF") : ((aptr->flags&INVISB) || (uptr->invis == 1)) ? "ON " : "?  ", (uptr->invis == 1) ? "YES" : "NO",
	       ((aptr->ansifl&ANSON) == ANSON) ? "ON " : "OFF",
	       (aptr->usrprf == PRFLIN) ? "LINE" : "FULL SCREEN",
	       (aptr->scnbrk == CTNUOS) ? "CONTINUOUS" : "PAUSED    ",
	       (uptr->chatchannel == 0) ? "OFF" : spr("%d", uptr->chatchannel), (uptr->echo == 0) ? "OFF" : "ON ", (uptr->duplex == 0) ? "HALF" : "FULL",
	       (aptr->systyp == 1) ? "IBM    " : "NON-IBM",
	       (uptr->ourswitch == 1) ? "ON" : "OFF",
	       (uptr->busy == 1) ? "ON      " : "OFF     ",
	       (uptr->buginedit == 1) ? "ON" : "OFF",
	       (uptr->pagemode > POFF) ? ((uptr->pagemode == POK) ? "OK" : "ON ") : "OFF",
	       (uptr->telegramnotify == 1) ? "ON" : "OFF",
	       (uptr->showage == 1) ? "ON " : "OFF",
	       (INT)uptr->whochoice,
	       (uptr->channelnotice == 1) ? "ON" : "OFF",
	       sameas( uptr->logon, "" ) ? "<None>" : uptr->logon, sameas( uptr->logoff, "" ) ? "<None>" : uptr->logoff);
	prfmsg(CSTHDR1);
	for ( counter=0; counter < NUMFORGETS; counter++ )
		{       /* list page and telegram blocks */
		prfmsg(CSTMSG2, counter+1, uptr->pageforgets[counter], counter+1, uptr->teleblocks[counter]);
		}
	prfmsg(CSTHDR2);
	for ( counter=0; counter < NUMMAC; counter++ )
		{	/* list macros and command strings */
		prfmsg(CSTMSG3, counter, uptr->command[counter], uptr->macro[counter]);
		}
	if ( online )
		{	/* Friends Notice settings valid only for on-line users */
		prfmsg(CSTMSG4, sameas( theids[othusn].busyid, "" ) ? "<None>" : theids[othusn].busyid, sameas( frienddata[othusn].logonid, "" ) ? "<None>" : frienddata[othusn].logonid, sameas( frienddata[othusn].logoffid, "" ) ? "<None>" : frienddata[othusn].logoffid);
		if ( alarmtimes[othusn][0] == 0 )
			{	/* user has no alarm set */
			prfmsg(CSTNALRM);
			}
		else
			{	/* show user's alarm */
			prfmsg(CSTMSG5, ncdate(alarmtimes[othusn][0]), nctime(alarmtimes[othusn][1]));
			}
		}
	doprintoutput();
	}

void colorizehelp( void )			  	/* display help on colorize input command */
	{
	prfmsg(CLRHLP, ANSIchar, forechar, ANSIchar, backchar, ANSIchar, specchar);
	doprintoutput();
	}

INT macrohandler( INT macronumber )		   	/* the generic macro handler */
	{			/* returns 1 if error (too much input), 0 if ok */
	INT copycount;          /* loop counter for copying additions to end of macro */
	CHAR newinput[INPSIZ];  /* the new input from macro */

	if ( margc > 1 )
		{       /* append input to end of macro */
		strcpy( newinput, themacros[usrnum].macro[macronumber] );
		for ( copycount=1; copycount < margc; copycount++ )
			{
			if ( ( strlen( newinput ) + strlen( margv[copycount] ) ) >= INPSIZ )
				{       /* user cannot overflow input[] */
				prfmsg(MTOLNG, (INT)INPSIZ);
				doprintoutput();
				return(1);
				}
			strcat( newinput, " " );
			strcat( newinput, margv[copycount] );
			}
		strcpy( input, newinput );
		}
	else
		{       /* just place macro in input */
		strcpy( input, themacros[usrnum].macro[macronumber] );
		}
	parsin();
	rstmbk();
	return(0);
	}

INT userineditor( CHAR *username )		    	/* returns 1 if username is in the Editor, 0 if not, -1 if error */
	{
	INT fsestate;					/* module number for Editor; usrptr->state when user is in the Editor */

	if ( !onsysn( username, 1 ) )
		{	/* this user-id is not on-line */
		return(-1);
		}
	fsestate=findmod(editorname);
	if ( fsestate == -1 )
		{	/* no such module loaded */
		return(-1);
		}
	if ( fsestate == othusp->state )
		{	/* user is in the Editor */
		return(1);
		}
	return(0);
	}

INT userintransfer( CHAR *username )		    	/* returns 1 if username is in a file transfer, 0 if not, -1 if error */
	{
	INT filestate;					/* module number for File Transfer Service; usrptr->state when user is in the Editor */

	if ( !onsysn( username, 1 ) )
		{	/* this user-id is not on-line */
		return(0);
		}
	filestate=findmod(fileservicename);
	if ( filestate == -1 )
		{	/* no such module loaded */
		return(0);
		}
	if ( filestate == othusp->state )
		{	/* user is in File Transfer Service */
		return(1);
		}
	return(0);
	}

void dochannotice( INT join )				/* notify users someone has joined their chat channel */
	{	/* join == 1 if joining, 0 if leaving */
	setmbk(glomb);
	if ( globalchatchannel[usrnum] )
		{	/* let others know someone has joined channel */
		for ( othusn=0, othusp=usroff(othusn); othusn < nterms; othusn++, othusp=usroff(othusn) )
			{       /* tell each user on chat channel that someone has tuned in */
			setuserpointers();
			if ( isauser() && ( othusn != usrnum ) && oktotell() && canbeseen() )
				{
				if ( ( globalchatchannel[othusn] == globalchatchannel[usrnum] ) && (channotice[othusn] == 1) )
					{	/* same chat channel */
					if ( join )
						{
						prfmsg(JOINCH, usaptr->userid);
						}
					else
						{
						prfmsg(LEFTCH, usaptr->userid);
						}
					injoth();
					}
				}
			}
		}
	rstmbk();
	}

void globalmessage( void )				/* send a message to all users on-line */
	{
	CHAR broadcast[INPSIZ];         /* message to be broadcast */

	getstring( broadcast, 1, (margc-1) );
	for ( othusn=0, othusp=usroff(othusn); othusn < nterms; othusn++, othusp=usroff(othusn) )
		{       /* send out broadcast to users */
		setuserpointers();
		if ( isauser() && ( othusn != usrnum ) && oktotell() )
			{
			prfmsg(BCSTMSG, usaptr->userid, broadcast);
			if ( sendthrubusy && sendtobusy(othuap) )
				{	/* send message thru "Busy" flag */
				outprf(othusn);
				}
			else
				{	/* don't send message thru "Busy" setting */
				injoth();
				}
			}
		}
	prfmsg(BCST);
	outprf(usrnum);
	}

void showcreditinfo( SHORT logtime )			/* display a user's credits and class info */
	{	/* logtime == -1 means don't know, otherwise logtime is ok */
	long minutes;                    /* user's time on today in minutes */
	struct clstab *tabptr;           /* user's class data */
	INT minthiscall;        	 /* minutes left in this call */
	INT mintoday;           	 /* minutes left today */

	if ( (tabptr=fndcls(usaptr->curcls)) == NULL )
		{       /* class is not in table */
		tabptr=usrptr->cltptr;
		}
	minthiscall=calllimit( tabptr, usrptr );
	mintoday=daylimit( tabptr, usaptr );
	if ( ( mintoday != -1 ) && ( mintoday < minthiscall ) )
		{       /* adjust for previous time online */
		minthiscall=mintoday;
		}
	if ( ( minthiscall == -1 ) && ( minthiscall < mintoday ) )
		{       /* adjust minutes for this call to time allowed for day */
		minthiscall=mintoday;
		}
	minutes=usaptr->timtdy;
	minutes=minutes / 60;   /* convert seconds to minutes */
	prfmsg(CRDMSG1, usaptr->userid, l2as(usaptr->creds), ncdate(usaptr->credat));
	prfmsg(CRDMSG2, ncdate(usaptr->usedat), (logtime != -1) ? nctime((USHORT)logtime) : "", usaptr->curcls, usesdays(usaptr) ? spr("%d", usaptr->daystt) : "UNLIMITED");
	prfmsg(CRDMSG3, l2as(minutes));
	prfmsg(CRDMSG4, tabptr->limcal == -1 ? "UNLIMITED" : spr("%d", tabptr->limcal),
	       tabptr->limday == -1 ? "UNLIMITED" : spr("%d", tabptr->limday), usrptr->minut4 >> 2, minthiscall == -1 ? "UNLIMITED" : spr("%d", minthiscall),
	       mintoday == -1 ? "UNLIMITED" : spr("%d", mintoday));
	outprf(usrnum);
	}

void otheruserinfo( struct usracc *otherptr, SHORT logtime, INT online )/* show credit and class info on other user */
	{	/* logtime == -1 means we don't know, otherwise logtime is ok, online == 1 means user online, otherwise not */
	LONG minutes;                   /* user's time on today in minutes */
	struct clstab *tabptr;          /* user's class data */
	CHAR hometown[NADSIZ];		/* where the user is calling from */
	INT minthiscall;        	/* minutes left in this call */
	INT mintoday;           	/* minutes left today */

	if ( (tabptr=fndcls(otherptr->curcls)) == NULL )
		{       /* class not in table */
		if ( online )
			{	/* can use this info if we have to */
			tabptr=othusp->cltptr;
			}
		else
			{	/* can't really do anything, so return */
			return;
			}
		}
	stripnonalppun( otherptr->usrad3, hometown );
	if ( online )
		{
		if ( !cansee() )
			{	/* don't allow user to see time for current call */
			minutes=minuteson[othusn];
			}
		else
			{	/* show true time on-line */
			minutes=otherptr->timtdy/60;
			}
		if ( tabptr->limcal != -1 )
			{       /* class has a limit of time per call */
			minthiscall=tabptr->limcal - ( othusp->minut4 >> 2 );
			}
		else
			{       /* unlimited time for this call */
			minthiscall=-1;
			}
		}
	else
		{	/* show time on-line */
		minutes=otherptr->timtdy/60;
		minthiscall = -1; // initialize it
		}
	if ( tabptr->limday != -1 )
		{       /* class has a limit of time per day */
		mintoday=tabptr->limday - (INT)( otherptr->timtdy / 60 );
		}
	else
		{       /* unlimited time for the day */
		mintoday=-1;
		}
	if ( online )
		{
		if ( ( mintoday != -1 ) && ( mintoday < minthiscall ) )
			{       /* adjust for previous time online */
			minthiscall=mintoday;
			}
		if ( ( minthiscall == -1 ) && ( minthiscall < mintoday ) )
			{       /* adjust minutes for this call to time allowed for day */
			minthiscall=mintoday;
			}
		}
	prfmsg(ACTMSG1, otherptr->userid, haskey(seecredskey) ? l2as(otherptr->creds) : "?", ncdate(otherptr->credat));
	prfmsg(ACTMSG2, ncdate(otherptr->usedat), (logtime != -1) ? nctime((USHORT)logtime) : "", otherptr->curcls, usesdays(otherptr) ? spr("%d", otherptr->daystt) : "UNLIMITED");
	prfmsg(ACTMSG3, hometown, l2as(minutes), (tabptr->limcal == -1) ? "UNLIMITED" : spr("%d", tabptr->limcal), (tabptr->limday == -1) ? "UNLIMITED" : spr("%d", tabptr->limday));
	if ( online && cansee() )
		{	/* show current time limits */
		prfmsg(ACTMSG4, othusp->minut4 >> 2, (minthiscall == -1) ? "UNLIMITED" : spr("%d", minthiscall), (mintoday == -1) ? "UNLIMITED" : spr("%d", mintoday));
		}
	outprf(usrnum);
	}

INT oktotell( void )				 	/* return 1 if ok to injoth() user, 0 if not */
	{
	return(!(userineditor(othuap->userid) && !bugem[othusn]));
	}

void stripnonalppun( CHAR *oldstr, CHAR *newstr ) 	/* copy alpha and punct chars in oldstr to newstr */
	{
	CHAR index;			/* index into strings */
	INT todo;			/* how many characters in oldstr to check */

	todo=strlen(oldstr);
	setmem( newstr, NADSIZ, 0 );	/* notice: assumes NADSIZ string length */
	for ( index=0; index < todo; index++ )
		{
		if ( isalpha(oldstr[index]) || ispunct(oldstr[index]) || isspace(oldstr[index]) )
			{	/* copy to newstr */
			newstr[index]=oldstr[index];
			}
		}
	}

void chanselecterror( void )		       		/* tell user he has picked an invalid channel range to clear */
	{
	prfmsg(CLRNUM1, help[6].command, clearstring, numchatchannels-1);
	prfmsg(CLRNUM2, help[6].command, clearstring, numchatchannels-1);
	doprintoutput();
	}

void sysalrt( struct userdata *uptr, INT onoroff )	/* announce invisible logs to Sysops only */
	{       /* uptr is user's global data, onoroff is 1 if log-on, 0 if log-off */
	INT userson;            /* # of users on system */

	setmbk(glomb);
	for ( othusn=0, othusp=usroff(othusn); othusn < nterms; othusn++, othusp=usroff(othusn) )
		{       /* announce to every Sysop */
		setuserpointers();
		if ( ( isauser() && ( othuap->userid[0] != '(' ) ) && othkey(stgopt(SYSLKEY)) &&
		   ( usrchannel[othusn] == 1 ) && ( othusn != usrnum ) && oktotell() &&
		   canbeseen() )
			{       /* make sure someone is on channel */
			if ( dobeeps )
				{
				beep(1);
				}
			userson=getchanused( othusn, onoroff, 1);
			prfmsg((onoroff) ? ILONMSG : ILOFMSG, usaptr->userid, userson, ( userson != 1 ) ? "users" : "user");
			showlogmessage(uptr, onoroff);
			injoth();
			}
		}
	rstmbk();
	}

void useralrt( struct userdata *uptr, INT onoroff, INT forreal )/* announce invisible logs to Sysops only */
	{       /* uptr is user's global data, onoroff is 1 if log-on, 0 if log-off, forreal 1 if genuine, 0 if faked */
	INT showusers;          /* # of users on system to tell user */

	setmbk(glomb);
	for ( othusn=0, othusp=usroff(othusn); othusn < nterms; othusn++, othusp=usroff(othusn) )
		{       /* announce to every channel */
		setuserpointers();
		if ( ( isauser() && ( othuap->userid[0] != '(' ) ) && othkey(help[17].key) &&
		   ( usrchannel[othusn] == 1 ) && ( othusn != usrnum ) && oktotell() )
			{       /* make sure someone is on channel */
			if ( forreal || (!othkey(stgopt(SYSLKEY))) )
				{       /* don't send fake message to users who can fake logs */
				showusers=getchanused( othusn, onoroff, forreal);
				if ( dobeeps )
					{
					beep(1);
					}
				prfmsg((onoroff) ? LONMSG : LOFMSG, usaptr->userid, showusers, ( showusers != 1 ) ? "users" : "user");
				showlogmessage(uptr, onoroff);
				injoth();
				}
			}
		}
	rstmbk();
	}

void showlogmessage( struct userdata *uptr, INT onoroff )/* display log-on or log-off message */
	{       /* if onoroff == 1 then log-on, else if onoroff == 0 then log-off */
	switch ( onoroff )
		{
		case 0 :
			{
			if ( !sameas( uptr->logoff, "" ) )
				{       /* print personal log-off message */
				setmbk(glomb);
				prfmsg(USRLOFF, uptr->logoff);
				rstmbk();
				}
			break;
			}
		case 1 :
			{   /* user log-on */
			if ( !sameas( uptr->logon, "" ) )
				{       /* print personal log-off message */
				setmbk(glomb);
				prfmsg(USRLON, uptr->logon);
				rstmbk();
				}
			}
		}
	}

void showteleblocks( void )				/* shows which user's telegrams are blocked */
	{
	INT counter=0;		/* check from 0 to NUMFORGETS block positions */
	INT found=0;		/* 1 if a user is blocked at blocked[usrnum].block[counter], 0 if not */

	do
		{       /* check if any blocks */
		if ( !sameas(blocked[usrnum].block[counter], "" ) )
			{
			found=1;
			}
		else
			{
			counter++;
			}
		}
	while ( ( counter < NUMBLOCKS ) && !found );
	if ( counter < NUMBLOCKS )
		{       /* found at least one block */
		prfmsg(TBKHDR);
		for ( counter=0; counter < NUMBLOCKS; counter++ )
			{       /* show users blocked */
			if ( !sameas(blocked[usrnum].block[counter], "" ) )
				{
				prfmsg(USRBLK, blocked[usrnum].block[counter]);
				}
			}
		}
	else
		{       /* no blocks */
		prfmsg(NOBLCKS);
		}
	doprintoutput();
	}

INT cansee( void )					/* returns 1 if usaptr->userid can see othuap->userid, 0 if not */
	{
	if ( ( (othusp->flags&INVISB) != 0L ) && ( ( restricted() && !showminvis ) || !haskey(help[14].key) ) )
		{
		return(0);
		}
	return(1);
	}

INT canbeseen( void )					/* returns 1 if othuap->userid can see usaptr->userid, 0 if not */
	{
	if ( ( (usrptr->flags&INVISB) != 0L ) && ( ( !othmaster() && hasmaster() && !showminvis ) || !othkey(help[14].key) ) )
		{
		return(0);
		}
	return(1);
	}

INT notifyoftelegram( CHAR *usertoinform )       	/* returns 1 if tell usertoinform of telegram received, 0 if not */
	{
	INT oktoinform;			/* 1 if notify, 0 if don't */

	setmbk(glomb);
	if ( onsysn(usertoinform, 1) && ( othkey(help[39].key) ) && ( ( notifytelegrams[othusn] == 1 ) || haskey(telenoblockkey) ) &&
	     oktotell() && canbeseen() )
		{	/* go ahead and notify */
		oktoinform=1;
		}
	else
		{
		oktoinform=0;
		}
	rstmbk();
	return(oktoinform);
	}

void telltelegram( void )				/* tell user he has received a telegram */
	{
	if ( dobeeps )
		{
		beep(1);
		}
	setmbk(glomb);
	prfmsg(TRCMSG, usaptr->userid, help[39].command);
	rstmbk();
	if ( !injoth() )
		{	/* user receiving telegram is "Busy", notify when possible */
		dotelalert(othusn, (INT)ON);
		}
	}

INT inopchat( void )					/* returns 1 if othusn is in opchat, 0 if not */
	{
	if ( (othusp->flags&OPCHAT) != 0L )
		{	/* opchatting */
		return(1);
		}
	return(0);
	}

void sayinchat( CHAR *userinchat )			/* tell that userinchat is in Opchat */
	{
	prfmsg(INCHAT, userinchat);
	doprintoutput();
	}

INT userinvis( void )					/* returns 1 if usrnum is invisible, 0 if not */
	{
	if ( (usrptr->flags&INVISB) != 0L )
		{	/* usrnum is invisible */
		return(1);
		}
	return(0);
	}

INT otherinvis( void )					/* returns 1 if othusn is invisible, 0 if not */
	{
	if ( (othusp->flags&INVISB) != 0L )
		{	/* othusn is invisible */
		return(1);
		}
	return(0);
	}

INT restricted( void )					/* returns 1 if usrptr doesn't have MASTER key and othusp does */
	{
	if ( ( (usrptr->flags&MASTER) == 0L ) && ( (othusp->flags&MASTER) != 0L ) )
		{	/* can't do this */
		return(1);
		}
	return(0);
	}

INT hasmaster( void )					/* returns 1 if usrptr has MASTER key, 0 if not */
	{
	if ( (usrptr->flags&MASTER) != 0L )
		{	/* has the key */
		return(1);
		}
	return(0);
	}

INT othmaster( void )					/* returns 1 if othusp has MASTER key, 0 if not */
	{
	if ( (othusp->flags&MASTER) != 0L )
		{	/* has the key */
		return(1);
		}
	return(0);
	}

INT usrprotected( struct usracc *usrprot )		/* returns 1 if usrprot if PROTECTED, 0 if not */
	{
	if ( usrprot->flags&UNDAXS )
		{	/* protected */
		return(1);
		}
	return(0);
	}

INT numok( CHAR *check, INT negativeok )		/* returns 1 if check is valid number, 0 if not */
	{	/* negativeok == 1 means negative numbers ok */
	INT digitfound;		/* 1 if character in input is digit, otherwise 0 */
	INT numdigits;		/* number of digits in input for checking */
	INT length;		/* length of check string */

	length=strlen(check);
	if ( !length )
		{	/* a problem here, empty string */
		return(0);
		}
	numdigits=0;
	do
		{
		digitfound=isdigit(check[numdigits++]);
		if ( (digitfound == 0) && (numdigits == 1) && negativeok && (check[numdigits-1] == '-') && (length > 1) )
			{	/* negative sign is first CHAR, so ok */
			digitfound=1;
			}
		}
	while ( (numdigits < length) && digitfound );
	return(digitfound);
	}

INT usesdays( struct usracc *chk )			/* returns 1 if chk->curcls uses days in class, 0 if not or error */
	{
	if ( ((clsptr=fndcls(chk->curcls)) == NULL ) || ( !(clsptr->flags&DAYEXP) ) )
		{	/* can't get data, or doesn't use days */
		return(0);
		}
	return(1);
	}

void suspendem( struct usracc *sus )			/* suspends sus->userid */
	{
	if ( usrprotected(sus) )
		{       /* turn off protection temporarily */
		sus->flags^=UNDAXS;
		sus->flags|=SUSPEN;
		sus->flags^=UNDAXS;
		}
	else
		{
		sus->flags|=SUSPEN;
		}
	}

void unsuspendem( struct usracc *sus )			/* unsuspends sus->userid */
	{
	if ( usrprotected(sus) )
		{       /* turn off protection temporarily */
		sus->flags^=UNDAXS;
		sus->flags&=~SUSPEN;
		sus->flags^=UNDAXS;
		}
	else
		{
		sus->flags&=~SUSPEN;
		}
	}

INT taggedforkill( struct usracc *uaptr )		/* returns 1 if uaptr->userid is DELETED, 0 if not */
	{
	if ( uaptr->flags&DELTAG )
		{
		return(1);
		}
	return(0);
	}

void suspendtoggle( struct usracc *sus )		/* toggles suspension for uaptr->userid */
	{
	if ( usrprotected(sus) )
		{       /* turn off protection temporarily */
		sus->flags^=UNDAXS;
		sus->flags^=SUSPEN;
		sus->flags^=UNDAXS;
		}
	else
		{
		sus->flags^=SUSPEN;
		}
	}

INT calllimit( struct clstab *tabptr, struct user *uptr )/* returns -1 if no limit per call, else minutes today for uptr */
	{
	INT minthiscall;		/* -1 for unlimited, else minutes for this call */

	if ( tabptr->limcal != -1 )
		{       /* class has a limit of time per call */
		minthiscall=tabptr->limcal - (uptr->minut4>>2);
		}
	else
		{       /* unlimited time for this call */
		minthiscall=-1;
		}
	return(minthiscall);
	}

INT daylimit( struct clstab *tabptr, struct usracc *uaptr )/* returns -1 if no limit per day, else minutes on today for uaptr->userid */
	{
	INT mintoday;		/* -1 if no limit, else minutes on for day */

	if ( tabptr->limday != -1 )
		{       /* class has a limit of time per day */
		mintoday=tabptr->limday - (INT)(uaptr->timtdy/60);
		}
	else
		{       /* unlimited time for the day */
		mintoday=-1;
		}
	return(mintoday);
	}

INT adjustcalltime( struct user *uptr, struct clstab *cptr, INT thiscall, INT mintoadd )/* adjust time for current call */
	{	/* returns actual minutes call time adjusted by; negative means time removed from being on-line */
		/* thiscall is minutes online for this call, minutes is number of minutes to extend */
	if ( mintoadd < 0 )
		{	/* remove time from user */
		if ( abs(mintoadd) <= (cptr->limcal - thiscall) )
			{	/* can add mintoadd minutes to uptr */
			uptr->minut4-=mintoadd<<2;
			return(mintoadd);
			}
		else
			{	/* put user to limit of call */
			uptr->minut4=cptr->limcal<<2;
			return(cptr->limcal-thiscall);
			}
		}
	else
		{	/* extend user's time */
		if ( mintoadd <= thiscall )
			{	/* can subtract full amount of minutes */
			uptr->minut4-=(mintoadd<<2);
			return(mintoadd);
			}
		else
			{	/* can only re-set time of current call to 0 */
			uptr->minut4=0;
			return(thiscall);
			}
		}
	}

INT adjustdaytime( struct usracc *uptr, struct clstab *cptr, INT thisday, INT mintoadd, INT online )/* adjust time for current call */
	{	/* returns actual minutes call time adjusted by; negative means time removed from being on-line */
		/* thisday is number of minutes online today, mintoadd is minutes to extend, online == 1 if user on-line, 0 if not */
	if ( mintoadd < 0 )
		{	/* remove time from user */
		if ( abs(mintoadd) <= (cptr->limday - thisday) )
			{	/* can add mintoadd minutes to uptr */
			uptr->timtdy-=mintoadd*60;	/* convert to seconds */
			if ( online )
				{	/* update othusp data */
				othuap->timtdy-=mintoadd*60;
				}
			return(mintoadd);
			}
		else
			{	/* put user to limit of day */
			uptr->timtdy=cptr->limday*60;
			if ( online )
				{	/* update othusp data */
				othuap->timtdy=cptr->limday*60;
				}
			return((INT)(uptr->timtdy/60)-thisday);
			}
		}
	else
		{	/* extend user's time */
		if ( mintoadd <= thisday )
			{	/* can subtract full amount of minutes */
			uptr->timtdy-=(long)(mintoadd*60);	/* convert to seconds */
			if ( online )
				{	/* update othusp data */
				othuap->timtdy-=mintoadd*60;
				}
			return(mintoadd);
			}
		else
			{	/* can only re-set time of current day to 0 */
			uptr->timtdy=0L;
			if ( online )
				{	/* update othusp data */
				othuap->timtdy=0L;
				}
			return(thisday);
			}
		}
	}

void helphandler( void )                         	/* help routine */
	{
	if ( margc == 1 )
		{	/* list all globals and command strings user has access to */
		listglobals();
		return;
		}
	else
		{
		setmbk(glomb);
		if ( sameas( margv[1], help[0].command ) )
			{
			if ( !haskey(help[0].key) )             /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(HLPHLP); /* print help help message */
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[8].command ) )
			{
			if ( !haskey(help[8].key) )             /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(CNTHLP, help[8].command, (usaptr->scnbrk == CTNUOS) ? "ON" : "OFF"); /* print continuous output help message */
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[34].command ) )
			{
			if ( !haskey(help[34].key) )             /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(LOFFHLP, help[34].command); /* print quick log-off help */
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[22].command ) )
			{
			if ( !haskey(help[22].key) )             /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(EPSTHLP, editorname, help[22].command, (bugem[usrnum]) ? "ON" : "OFF");
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[11].command ) || ( sameas( margv[1], endbeeps ) && haskey(bpstpkey) ) )
			{
			if ( !haskey(help[11].key) )             /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(CPGHLP1, help[11].command); /* print page console help */
			if ( haskey(bpstpkey) )
				{
				prfmsg(CPGHLP2, endbeeps);
				}
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[2].command ) )
			{
			if ( !haskey(help[2].key) )            /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(ANSHLP, help[2].command, (usaptr->ansifl&ANSON) ? "ON" : "OFF"); /* print ansi toggle help */
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[3].command ) )
			{
			if ( !haskey(help[3].key) )            /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(BUSYHLP1, help[3].command, usrbusy(usrptr) ? "ON" : "OFF"); /* print busy toggle help */
			if ( haskey(unbusyothkey) )
				{	/* can toggle another user's "busy" flag off */
				prfmsg(BUSYHLP2, help[3].command);
				}
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[4].command ) )
			{
			if ( !haskey(help[4].key) )
				{
				cantuse();
				return;
				}
			prfmsg(BSNHLP, help[4].command, help[4].command, help[4].command, (sameas(theids[usrnum].busyid, "")) ? "<Nobody>" : theids[usrnum].busyid);
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[10].command ) )
			{
			if ( !haskey(help[10].key) )         /* check that user has access */
				{
				cantuse();
				return;
				}
			prfmsg(DUPHLP, help[10].command, ( duplex[usrnum] == 0 ) ? "HALF" : "FULL");
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[12].command ) )
			{
			if ( !haskey(help[12].key) )            /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(EDITHLP, help[12].command, (usaptr->usrprf == PRFLIN) ? "LINE" : "FULL SCREEN"); /* print editor type help */
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[24].command ) )
			{
			if ( !haskey(help[24].key) )            /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(RLGHLP1, help[24].command); /* print re-log help */
			if ( haskey(relogothkey) )
				{       /* print force other re-log help */
				prfmsg(RLGHLP2, help[24].command);
				}
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[36].command ) )
			{
			if ( !haskey(help[36].key) )            /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(MEMHLP, help[36].command); /* print membership help */
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[13].command ) )
			{
			if ( !haskey(help[13].key) )            /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(IBMHLP, help[13].command, (usaptr->systyp == 1) ? "IBM" : "NON-IBM" ); /* print IBM mode help */
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[29].command ) )
			{
			if ( !haskey(help[29].key) )            /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(SCNHLP, (INT)MIN_SCREEN_WIDTH, (INT)MAX_SCREEN_WIDTH, help[29].command, usaptr->scnwid); /* print screen width help */
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[25].command ) )
			{
			if ( !haskey(help[25].key) )            /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(LNGHLP, (INT)MIN_SCREEN_LENGTH, (INT)MAX_SCREEN_LENGTH, help[25].command, (usaptr->scnbrk == CTNUOS) ? "CONTINUOUS" : spr("%d", usaptr->scnbrk)); /* print screen length help */
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[5].command ) )
			{
			if ( !haskey(help[5].key) )            /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(CRDHLP, help[5].command); /* print user credit help */
			if ( haskey(infolookkey) )
				{
				prfmsg(UIHLP, help[5].command);
				}
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[37].command ) )
			{
			if ( !haskey(help[37].key) )            /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(WHOHLP, help[37].command, (showages[usrnum] == 1) ? "will" : "won't", help[37].command, help[37].command, help[37].command,
			       help[37].command, 1, (INT)NUMWHODIS, (showages[usrnum] ==1) ? "ON" : "OFF", help[37].command, 1, (INT)NUMWHODIS); /* print who's online help */
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[9].command ) )
			{
			if ( !haskey(help[9].key) )            /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(DTHLP, help[9].command); /* print date and time help */
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[20].command ) )
			{
			if ( !haskey(help[20].key) )            /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(PSTHLP, help[20].command, (haskey(unlimpostkey)) ? l2as(MAXPOST) : l2as(maxpostcreds)); /* print post credits help */
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[35].command ) )
			{
			if ( !haskey(help[35].key) )            /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(BTHLP, help[35].command, help[35].command); /* print boot user help */
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[23].command ) )
			{
			if ( !haskey(help[23].key) )            /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(RPTHLP, help[23].command); /* print repeat command help */
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[16].command ) )
			{
			if ( !haskey(help[16].key) )            /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(NEWHLP, help[16].command, help[16].command); /* print list new sign-ups help */
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[17].command ) || sameas( margv[1], logonstr ) || sameas( margv[1], logoffstr ) ||
		     sameas( margv[1], clearstr ) || sameas( margv[1], showmessstr ) )
			{
			if ( ( sameas( margv[1], clearstr ) || sameas( margv[1], showmessstr ) ) && !haskey(logclearkey) )
				{
				cantuse();
				return;
				}
			if ( !haskey(help[17].key) )
				{
				cantuse();
				return;
				}
			prfmsg(LNTHLP1, help[17].command, ( usrchannel[usrnum] == 0 ) ? "OFF" : "ON",
			       logonstr, logonstr, logoffstr, logoffstr, MESSSIZE-1, showmessstr);
			if ( haskey(logclearkey) )
				{
				prfmsg(LNTHLP2, showmessstr, clearstr);
				}
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[6].command ) || sameas( margv[1], displaystring ) || sameas( margv[1], topicstring ) ||
		     sameas( margv[1], showstring ) || sameas( margv[1], clearstring ) )
			{
			if ( sameas( margv[1], clearstring ) && !haskey(chanclearkey) )
				{
				cantuse();
				return;
				}
			if ( !haskey(help[6].key) )            /* check that user may use this global */
				{
				cantuse();
				return;
				}
			chatchannelhelp();
			rstmbk();
			return;
			}
		if ( sameas( margv[1], help[43].command ) )
			{
			if ( !haskey(help[43].key) )            /* check that user may use this global */
				{
				cantuse();
				return;
				}
			friendhelp();
			return;
			}
		if ( sameas( margv[1], help[39].command ) || sameas( margv[1], telegramalert ) || sameas( margv[1], telegramclear ) || ( ( margv[1][0] == telesendchar ) && ( strlen( margv[1] ) == 1 ) ) || ( sameas( margv[1], deleteallteles ) ) ||
		     sameas( margv[1], teleblockstring ) )
			{
			if ( sameas( margv[1], deleteallteles ) && !haskey(deltelekey) )
				{
				cantuse();
				return;
				}
			if ( !haskey(help[39].key) )            /* check that user may use this global */
				{
				cantuse();
				return;
				}
			telegramhelp();
			return;
			}
		if ( sameas( margv[1], help[18].command ) || sameas( margv[1], macrocommand ) || sameas( margv[1], macromacstring ) ||
		     sameas( margv[1], macrocommandlist ) || sameas( margv[1], macrolist ) )
			{
			if ( !haskey(help[18].key) )            /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(MACHLP, (INT)MACSIZ-1, (INT)CMSIZ-1, help[0].command, help[18].command, help[18].command, macrocommand, macromacstring, macrocommandlist, macrolist); /* print macro help */
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[41].command ) )
			{
			if ( !haskey(help[41].key) )            /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(ALRMHLP, help[41].command, help[41].command, help[41].command);
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[38].command ) )
			{
			if ( !haskey(help[38].key) )           /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(BRCSTHLP, help[38].command); /* print system-wide broadcast help */
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[30].command ) )
			{
			if ( !haskey(help[30].key) )          /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(TRNHLP, help[30].command, (haskey(unlimtrankey)) ? 1 : minimumtransfer, l2as(usaptr->creds));
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[21].command ) )
			{
			if ( !haskey(help[21].key) )          /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(PDYHLP, (INT)MINDAYSPOST, (INT)MAXDAYSPOST, help[21].command); /* print post days in class help */
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[31].command ) )
			{
			if ( !haskey(help[31].key) )          /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(TDYHLP, help[31].command, (haskey(nomindaykey)) ? 1 : minimumtrandays, (INT)MAXDAYSPOST); /* print transfer days help */
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[32].command ) )
			{
			if ( !haskey(help[32].key) )	 /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(TEXTHLP, (INT)MINTIMEEXT, (INT)MAXTIMEEXT, help[32].command);
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[7].command ) )
			{
			if ( !haskey(help[7].key) )         /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(CSTHLP, help[7].command); /* print current settings help */
			if ( haskey(othinfokey) )
				{
				prfmsg(OUSHLP, help[7].command);
				}
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[27].command ) )
			{
			if ( !haskey(help[27].key) )          /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(SWCHLP, help[27].command, (INT)MAXDAYSPOST); /* print switch user class help */
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[33].command ) )
			{
			if ( !haskey(help[33].key) )          /* check that user may use this global */
				{
				cantuse();
				return;
				}
			prfmsg(UKHLP, help[33].command, help[33].command); /* print give/take user key help */
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[14].command ) )
			{
			if ( !haskey(help[14].key) )
				{
				cantuse();
				return;
				}
			prfmsg(INVHLP, help[14].command, userinvis() ? "ON" : "OFF", help[14].command); /* print invisible help */
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[28].command ) )
			{
			if ( !haskey(help[26].key) )
				{
				cantuse();
				return;
				}
			prfmsg(SUSHLP, help[28].command); /* print suspend/unsuspend user help */
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[26].command ) )
			{
			if ( !haskey(help[26].key) )
				{
				cantuse();
				return;
				}
			prfmsg(ASUSHLP, (INT)MAXSUS, help[26].command, help[26].command, help[28].command);
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[15].command ) )
			{
			if ( !haskey(help[15].key) )
				{
				cantuse();
				return;
				}
			prfmsg(DLUHLP, help[15].command); /* print delete/undelete user help */
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], help[19].command ) || sameas( margv[1], repagestring ) || sameas( margv[1], pageforgetstring ) )
			{
			if ( !haskey(help[19].key) )
				{
				cantuse();
				return;
				}
			prfmsg(PGHLP, help[19].command, help[19].command, help[19].command, repagestring, pageforgetstring, pageforgetstring); /* print page help */
			doprintoutput();
			return;
			}
		if ( ( margv[1][0] == ANSIchar ) && ( strlen(margv[1]) == 1 ) )
			{
			if ( !haskey(help[41].key) )
				{
				cantuse();
				return;
				}
			colorizehelp();
			return;
			}
		prfmsg(NOHLP, margv[1]);
		doprintoutput();
		return;
		}
	}

void cantuse( void )					/* informs a user he has no access to do something */
	{
	prfmsg(CNTUSE);
	doprintoutput();
	}

void listglobals( void )				/* lists all globals user has access to */
	{
	INT linefeed=0;                 /* 1 if add linefeed to output, 0 if not */
	INT printcount;                 /* looping counter for displaying commands */

	setmbk(glomb);
	if ( !haskey(help[0].key) )             /* check that user may use this global */
		{
		cantuse();
		return;
		}
	prfmsg(GLOBHDR); /* list all global commands */
	for ( printcount=0; printcount < HELPSIZE; printcount++ )
		{       /* print out globals user has access to */
		if ( haskey(help[printcount].key) )
			{       /* print this one */
			prfmsg(GLOBHLP, help[printcount].command, help[printcount].message);
			if ( linefeed )
				{       /* go to start of next line and change linefeed */
				prf("\n");
				}
			linefeed^=1;    /* code toggles linefeed between 0 and 1 */
			}
		}
	if ( linefeed )
		{
		prf("\n");
		}
	doprintoutput();
	}

void togglebusy( struct user *uptr )			/* toggles uptr's "Busy" flag */
	{
	uptr->flags^=NOINJO;
	}

INT isauser( void )					/* returns 1 if othusn is an actual user, 0 if not */
	{
	if ( ( ( othusp->usrcls > ONLINE ) && ( othusp->usrcls != SUPIPG ) ) && ( isuidc(othuap->userid[0]) ) )
		{	/* this is a user */
		return(1);
		}
	return(0);
	}

INT usrbusy( struct user *uptr )			/* returns 1 if uptr is "Busy", 0 if not */
	{
	if ( (uptr->flags&NOINJO) != 0L )
		{	/* this user is "Busy" */
		return(1);
		}
	return(0);
	}

void setuserpointers( void )				/* sets up othuap and othexp to othusn */
	{
	othuap=uacoff(othusn);
	}

void listnotkeys( CHAR *listid )			/* show any negative keys for user listid */
	{
	struct notkey keydat;		/* listid's negative keys */
	INT found;			/* 1 if found negative key in database, 0 if not */
	CHAR count=0;			/* number of keys shown so we know when to go to next line of display */

	dfaSetBlk(notkeydata);
	found=dfaAcqEQ(&keydat, listid, 0);
	if ( found )
		{	/* user has negative keys, show header */
		prfmsg(NOTKHDR);
		}
	while ( found )
		{
		prfmsg(NOTKEY, keydat.keyname);
		if ( count == 4 )
			{	/* outprf CR to go to next line */
			prf("\n");
			count=0;
			}
		else
			{	/* add a space for next key if any */
			prf(" ");
			count++;
			}
		found=dfaAcqNX(&keydat);
		if ( !found && ( count > 0 ) )
			{	/* send CR only if we haven't already just sent one */
			prf("\n");
			}
		}
	dfaRstBlk();
	}

INT hasnotkey( CHAR *notkeyid, CHAR *key )             	/* returns 1 if notkeyid has negative key key, 0 if not */
	{
	struct notkey keydat;		/* listid's negative keys */
	INT found;			/* 1 if found negative key in database, 0 if not */
	INT hasit=0;			/* 1 if user has key, 0 if not */

	dfaSetBlk(notkeydata);
	found=dfaAcqEQ(&keydat, notkeyid, 0);
	while ( found && !hasit)
		{
		if ( sameas( keydat.keyname, key ) )
			{
			hasit=1;
			}
		found=dfaAcqNX(&keydat);
		}
	dfaRstBlk();
	return(hasit);
	}

void updateonline( CHAR *userid, CHAR *keyname, INT give )/* updates user's keys if on-line, if give == 1 gives key, else removes */
	{
	INT saveusernum;		/* save for changing current usrnum */

	if ( onsysn(userid, 1) )
		{
		if ( give )
			{	/* give user the key */
			saveusernum=usrnum;
			curusr(othusn);
			setkey(keyname);
			curusr(saveusernum);
			}
		else
			{	/* remove key from user */
			rmvkey(userid, keyname);
			}
		}
	}

void preparetelegram( struct telegram *ptgm, CHAR *toid )	/* sets fields telegram ptgm such as date and time, for any telegram */
	{	/* toid is user telegram is to (from usaptr->userid) */
	strcpy( ptgm->to, toid );
	strcpy( ptgm->from, usaptr->userid );
	ptgm->datesent=today();
	ptgm->timesent=now();
	}

void doprintoutput( void )					/* causes print of buffer contents to current user */
	{
	outprf(usrnum);
	rstmbk();
	}

void toggleinvis( struct user *uptr, struct usracc *uaptr, INT onoroff )/* toggles uaptr invisible if onoroff == 1, visible if onoroff == 0 */
	{
	if ( onoroff )
		{	/* toggle invisible */
		uptr->flags|=INVISB;
		uaptr->flags|=GOINVB;
		}
	else
		{	/* toggle visible */
		uptr->flags&=~INVISB;
		uaptr->flags&=~GOINVB;
		}
	}

void connecthandler( void )					/* handle user's connection, both at entering password and possibly WG C/S connect */
	{
	struct userdata newuserdata;            /* new user to add to database */
	INT copycount;                          /* counter for copying data into usermacros[usrnum] */
	struct notkey notdat;			/* user's negative keys */
	INT found;				/* 1 if negative key found, 0 if not */

	if ( (usrptr->flags&WSGCSU) != 0L )
		{	/* C/S mode user going to terminal mode */
		return;
		}
	dfaSetBlk(notkeydata);			/* remove negative keys from user */
	found=dfaAcqEQ(&notdat, usaptr->userid, 0);
	while ( found )
		{
		updateonline(usaptr->userid, notdat.keyname, 0);
		found=dfaAcqNX(&notdat);
		}
	dfaRstBlk();
	minuteson[usrnum]=(INT)usaptr->timtdy/60;	/* save in case user is or goes invisible */
	setmbk(glomb);
	setmem( usercommand[usrnum].thecommand, INPSIZ, 0 );      /* user has no previous command */
	dfaSetBlk(userglobdata);
	if ( !dfaAcqEQ(&newuserdata, usaptr->userid, 0) )
		{       /* user not in database */
		strcpy( newuserdata.userid, usaptr->userid );
		strcpy( newuserdata.logon, stgopt(DEFLON) );
		strcpy( newuserdata.logoff, stgopt(DEFLOF) );
		newuserdata.chatchannel=(USHORT)numopt(DEFCC, 0, 100);
		newuserdata.echo=(UCHAR)numopt(DEFECH, 0, 1);
		newuserdata.duplex=(UCHAR)numopt(DEFDUP, 0, 1);
		newuserdata.pagemode=(UCHAR)numopt(DEFPAG, 0, 2);
		newuserdata.telegramnotify=(UCHAR)numopt(DEFTEL, 0, 1);
		newuserdata.showage=(UCHAR)ynopt(DEFAGE);
		newuserdata.whochoice=(UCHAR)numopt(DEFWHO, 1, 5);
		newuserdata.invis=0;
		newuserdata.busy=0;
		newuserdata.buginedit=(UCHAR)numopt(DEFPST, 0, 1);
		newuserdata.ourswitch=(UCHAR)numopt(DEFLOG, 0, 1);
		newuserdata.channelnotice=(UCHAR)numopt(DEFCHN, 0, 1);
		for ( copycount=0; copycount < NUMMAC; copycount++ )
			{       /* initialize macros */
			strcpy( themacros[usrnum].command[copycount], defaultcom[copycount] );
			strcpy( newuserdata.command[copycount], defaultcom[copycount] );
			strcpy( themacros[usrnum].macro[copycount], defaultmac[copycount] );
			strcpy( newuserdata.macro[copycount], defaultmac[copycount] );
			}
		for ( copycount=0; copycount < NUMFORGETS; copycount++ )
			{       /* initialize page forgets and telegram blocks */
			setmem( newuserdata.pageforgets[copycount], UIDSIZ, 0 );
			setmem( userpage[usrnum].forget[copycount], UIDSIZ, 0 );
			setmem( newuserdata.teleblocks[copycount], UIDSIZ, 0 );
			setmem( blocked[usrnum].block[copycount], UIDSIZ, 0 );
			}
		globalchatchannel[usrnum]=newuserdata.chatchannel;    /* set global variables */
		cvtDataIP(&newuserdata, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
		dfaInsert(&newuserdata);           /* save new user data */
		}
	else
		{
		cvtDataIP(&newuserdata, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
		if ( ( newuserdata.chatchannel > numchatchannels ) || ( ( newuserdata.chatchannel == numchatchannels ) && !haskey(syschankey) ) )
			{       /* number of channels reduced, turn off to avoid problem */
			globalchatchannel[usrnum]=0;
			}
		else
			{       /* no problem */
			globalchatchannel[usrnum]=newuserdata.chatchannel;
			}
		for ( copycount=0; copycount < NUMMAC; copycount++ )
			{       /* initialize macro data */
			strcpy( themacros[usrnum].command[copycount], newuserdata.command[copycount] );
			strcpy( themacros[usrnum].macro[copycount], newuserdata.macro[copycount] );
			}
		for ( copycount=0; copycount < NUMFORGETS; copycount++ )
			{       /* initialize page forgets and telegram blocks */
			strcpy( blocked[usrnum].block[copycount], newuserdata.teleblocks[copycount] );
			strcpy( userpage[usrnum].forget[copycount], newuserdata.pageforgets[copycount] );
			}
		if ( newuserdata.invis == 1 )
			{	/* set user invisible */
			usrptr->flags|=INVISB;
			}
		if ( newuserdata.busy == 1 )
			{	/* set user's "Busy" flag */
			usrptr->flags|=NOINJO;
			}
		}
	dotelalert(usrnum, (INT)OFF);
	alarmtimes[usrnum][0]=0;
	alarmtimes[usrnum][1]=0;
	changedata[usrnum].switchdate=today();
	changedata[usrnum].switchtime=now();
	setmem( frienddata[usrnum].logonid, UIDSIZ, 0 );
	setmem( frienddata[usrnum].logoffid, UIDSIZ, 0 );
	bugem[usrnum]=newuserdata.buginedit;
	usrchannel[usrnum]=newuserdata.ourswitch;
	chatecho[usrnum]=newuserdata.echo;
	duplex[usrnum]=newuserdata.duplex;
	userpage[usrnum].pagesetting=newuserdata.pagemode;
	notifytelegrams[usrnum]=newuserdata.telegramnotify;
	showages[usrnum]=newuserdata.showage;
	whonumber[usrnum]=newuserdata.whochoice;
	dfaRstBlk();
	if ( duplex[usrnum] == 0 )
		{       /* set half duplex mode */
		btuech(usrnum, 0);
		}
	else
		{       /* set full duplex mode */
		echon();
		}
	setmem( theids[usrnum].busyid, UIDSIZ, 0 );
	setmem( theids[usrnum].notid, UIDSIZ, 0 );
	userpage[usrnum].nextpage=now();
	userpage[usrnum].pagedate=today();
	setmem( userpage[usrnum].lastpage, INPSIZ, 0 );
	telestate[usrnum]=NORMAL;            /* initialize telegram state */
	logtimes[usrnum]=now();
	rstmbk();
	lonalrt(&newuserdata);
	if ( logchannotice )
		{
		dochannotice(JOINCHAN);
		}
	setmbk(glomb);
	for ( othusn=0, othusp=usroff(othusn); othusn < nterms; othusn++, othusp=usroff(othusn) )
		{	/* check all on-line users */
		setuserpointers();
		if ( !usrbusy(othusp) && sameas( frienddata[othusn].logonid, usaptr->userid ) && canbeseen() )
			{	/* tell user his Friend has logged on */
			if ( dobeeps )
				{
				beep(1);
				}
			prfmsg(FRNDNOT, usaptr->userid, "on");
			if ( injoth() )
				{	/* only clear notice if successful */
				setmem( frienddata[othusn].logonid, UIDSIZ, 0 );
				}
			}
		}
	rstmbk();
		
	}

void wg_connect( void )					/* C/S mode user connect handler */
	{
	connecthandler();
	}

void wg_disconnect( void )				/* C/S mode user disconnect handler */
	{
	if ( (usrptr->flags&WSGCSU) != 0L )
		{	/* C/S mode user going to terminal mode */
		return;
		}
	endnote();
	}

INT isblocked( void )					/* returns 1 if user cannot use globals in current module, 0 if ok to use globals */
	{
	INT modstate;					/* module number for blocked modules */
	INT checkcount;					/* looping count for checking each possible blocked module */

	for ( checkcount=0; checkcount < NUMMODBLKS; checkcount++ )
		{
		if ( !sameas( moduleblocks[checkcount], "" ) )
			{	/* check for this module */
			modstate=findmod(moduleblocks[checkcount]);
			if ( ( modstate != -1 ) && ( modstate == usrptr->state ) )
				{	/* no such module loaded */
				return(1);
				}
			}
		}
	return(0);
	}

void telalert( void )					/* tells "Busy" users who are no longer "Busy" they have telegrams */
	{
	dfaSetBlk(telegramdata);
	setmbk(glomb);
	for ( othusn=0, othusp=usroff(othusn); othusn < nterms; othusn++, othusp=usroff(othusn) )
		{	/* check all on-line users */
		setuserpointers();
		if ( !usrbusy(othusp) && alertnewtel(othusn) )
			{	/* tell user he has a telegram */
			if ( dfaQueryEQ(othuap->userid, 0) )
				{	/* only if one exists on disk */
				prfmsg(TWMSG, help[39].command);
				injoth();
				}
			dotelalert(othusn, (INT)OFF);
			}
		}
	rstmbk();
	dfaRstBlk();
	rtkick( checktels, telalert );
	}

void dotelalert( INT usernum, INT onoroff )		/* mark user for alerting to telegram when he's no longer "Busy" */
	{	/* usernum is usrnum of user to set flag for, onoroff == 1 if turn on, 0 if turn alert off */
	if ( onoroff )
		{	/* turn alert on */
		tgmalert[usernum]=1;
		}
	else
		{	/* turn alert off */
		tgmalert[usernum]=0;
		}
	}

INT alertnewtel( INT usernum )				/* returns 1 if user usernum needs to be alerted of new telegrams when not "Busy", else 0 */
	{
	return(tgmalert[usernum]);
	}

void doalarm( void )					/* alert users at desired times */
	{
	USHORT todaysdate;		/* the date as returned by today() */
	USHORT timenow;			/* the time as returned by now() */

	todaysdate=today();
	timenow=now();
	setmbk(glomb);
	for ( othusn=0, othusp=usroff(othusn); othusn < nterms; othusn++, othusp=usroff(othusn) )
		{	/* check all on-line users */
		setuserpointers();
		if ( alarmtimes[othusn][0] == todaysdate )
			{	/* date matches, now check time */
			if ( timenow >= alarmtimes[othusn][1] )
				{	/* we can alert user and cancel alarm */
				beep(alarmbeeps);
				prfmsg(ALRM, nctime(alarmtimes[othusn][1]));
				if ( injoth() )
					{	/* alarm got thru, so we can cancel it */
					alarmtimes[othusn][0]=0;
					alarmtimes[othusn][1]=0;
					}
				}
			}
		}
	rstmbk();
	rtkick( checkalarm, doalarm );
	}

void friendhelp( void )                                 /* display help on Friend notice global */
	{
	prfmsg(FRNDHLP, help[43].command, help[43].command, help[43].command, help[43].command, help[43].command);
	doprintoutput();
	}

void dofriend( void )					/* do Friends Notices for "Busy" users */
	{
	INT saveothusn;                 /* save variables because we are already in a loop */
	struct user *saveothusp;
	struct usracc *saveothuap;
	CHAR friendid[UIDSIZ];          /* User-ID of Friend Notice */

	setmbk(glomb);
	for ( othusn=0, othusp=usroff(othusn); othusn < nterms; othusn++, othusp=usroff(othusn) )
		{	/* check all on-line users */
		setuserpointers();
		saveothusn=othusn;      /* save pointers */
		saveothusp=othusp;
		saveothuap=othuap;
		if ( !sameas( frienddata[othusn].logonid, "" ) && onsysn( frienddata[othusn].logonid, 1 ) && friendlonsee(saveothusn) )
			{	/* Friend Notice has logged on */
			strcpy( friendid, othuap->userid );
			if ( dobeeps )
				{
				beep(1);
				}
			prfmsg(FRNDNOT, friendid, "on");
			othusn=saveothusn;      /* restore when done */
			othusp=saveothusp;
			othuap=saveothuap;
			if ( injoth() )
				{	/* Notice got thru, so we can cancel it */
				setmem( frienddata[othusn].logonid, UIDSIZ, 0 );
				}
			}
		else
			{	/* restore pointers */
			othusn=saveothusn;      /* restore when done */
			othusp=saveothusp;
			othuap=saveothuap;
			}
		}
	for ( othusn=0, othusp=usroff(othusn); othusn < nterms; othusn++, othusp=usroff(othusn) )
		{	/* check all on-line users */
		setuserpointers();
		saveothusn=othusn;      /* save pointers */
		saveothusp=othusp;
		saveothuap=othuap;
		if ( !sameas( frienddata[othusn].logoffid, "" ) && !onsys( frienddata[othusn].logoffid ) && !friendloffsee(saveothusn) )
			{	/* Friend Notice has logged off */
			strcpy( friendid, othuap->userid );
			if ( dobeeps )
				{
				beep(1);
				}
			prfmsg(FRNDNOT, friendid, "off");
			othusn=saveothusn;      /* restore when done */
			othusp=saveothusp;
			othuap=saveothuap;
			if ( injoth() )
				{	/* Notice got thru, so we can cancel it */
				setmem( frienddata[othusn].logoffid, UIDSIZ, 0 );
				}
			}
		else
			{	/* restore pointers */
			othusn=saveothusn;      /* restore when done */
			othusp=saveothusp;
			othuap=saveothuap;
			}
		}
	rstmbk();
	rtkick( checkfriend, dofriend );
	}

INT friendlonsee( INT othusernum )             		/* returns 1 if user othusernum can "see" user othusn, 0 if not */
	{
	INT saveusernum;		/* save current usernum here for restoring */
	INT seefriend;			/* result to return */

	saveusernum=usrnum;
	curusr(othusernum);
	seefriend=cansee();
	curusr(saveusernum);
	return(seefriend);
	}

INT friendloffsee( INT othusernum )            		/* returns 1 if user othusernum can "see" user othusn, 0 if not */
	{
	if ( !onsysn(frienddata[othusernum].logoffid, 1) )
		{	/* user has really logged off */
		return(1);
		}
	return(friendlonsee(othusernum)); /* user in question went invis, check if can be seen */
	}

INT sendtobusy( struct usracc *uaptr )			/* returns 1 if uptr can be paged thru "Busy", 0 if not */
	{
	if ( !inopchat() && !userintransfer(uaptr->userid) )
		{	/* can send it thru */
		return(1);
		}
	return(0);
	}

VOID handle_telegrams( VOID )				/* handle user's reading of telegrams */
	{
	CHAR checkuserid[INPSIZ];       /* store possible userid here */
	INT count;                      /* loop control counter */
	INT attempts=1;                 /* number of attempts to find valid userid */
	CHAR found=0;                   /* 2 if userid is online, 1 if offline, 0 if no such user */
	GBOOL acq;                      /* true boolean for dfaAcquire routines - RH 8/2/2024 */
	struct usracc teleacc;          /* account data of userid */
	struct telegram sendgram;       /* telegram user wishes to send */
	struct userdata sendiddat;      /* see if we're blocked in user's data */
	INT userblocked;                /* -1 if ok to send telegram, otherwise not ok */
	INT checkcount;                 /* number of users blocked from sending telegrams */

	INIT_STRUCT_TO_ZERO(teleacc);
	INIT_STRUCT_TO_ZERO(sendgram);
	INIT_STRUCT_TO_ZERO(sendiddat);

	setmbk(glomb);
	if ( !haskey(help[39].key) )           /* check that user may use this global */
		{
		cantuse();
		return;
		}
	if ( userinvis() && !haskey(teleinviskey) )
		{       /* can't do this while invisible */
		prfmsg(URINVIS);
		doprintoutput();
		return;
		}
	dfaSetBlk(accbb);
	setmem( checkuserid, INPSIZ, 0 );
	for ( count = 1; count < (INT)strlen( margv[0] ); count++ )
		{       /* save first word */
		checkuserid[count-1]=margv[0][count];
		}
	acq=dfaAcqEQ(&teleacc, checkuserid, 0);
	while ( ( attempts < margc ) && ( acq == 0 ) )
		{       /* add to possible userid */
		strcat( checkuserid, " " );
		strcat( checkuserid, margv[attempts] );
		acq=dfaAcqEQ(&teleacc, checkuserid, 0);
		attempts++;
		}
	if ( acq )
		{	/* check for handles with same beginning and make sure we match correctly */
		while ( ( attempts < margc ) && found )
			{	/* get full handle if there is more */
			strcat( checkuserid, " " );
			strcat( checkuserid, margv[attempts] );
			acq=dfaAcqEQ(&teleacc, checkuserid, 0);
			if ( acq )
				{	/* check for next word as part of handle */
				attempts++;
				}
			}
		found=1;	/* we did find a valid user-id */
		}
	dfaRstBlk();
	if ( found )
		{       /* found valid userid */
		if ( taggedforkill(&teleacc) )
			{       /* trying to send telegram to deleted user */
			found=0;
			}
		else if ( attempts >= margc )
			{       /* trying to send blank telegram */
			prfmsg(BTMMSG);
			doprintoutput();
			return;
			}
		else
			{       /* everything ok */
			strcpy( checkuserid, teleacc.userid );
			if ( onsysn( checkuserid, 1 ) )
				{       /* user is on the system */
				found=USERON;
				}
			}
		}
	userblocked=-1;
	checkcount=0;
	switch ( found )
		{
		case USEROFF : /* user is off-line */
			{
			dfaSetBlk(userglobdata);
			if ( dfaAcqEQ(&sendiddat, checkuserid, 0) )
				{       /* check account data for block */
				cvtDataIP(&sendiddat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
				while ( ( checkcount < NUMBLOCKS ) && ( userblocked == -1 ) )
					{
					if ( sameas( usaptr->userid, sendiddat.teleblocks[checkcount] ) )
						{       /* this user's pages are blocked */
						userblocked=checkcount;
						}
					checkcount++;
					}
				}
			dfaRstBlk();
			if ( ( userblocked != -1 ) && !haskey(telenoblockkey) )
				{       /* cannot send telegram to this user */
				prfmsg(TMBLOCK, checkuserid);
				doprintoutput();
				return;
				}
			getstring( sendgram.telegramtext, attempts, (margc-1) );
			preparetelegram( &sendgram, teleacc.userid );
			dfaSetBlk(telegramdata);
			dfaInsert(&sendgram);
			dfaRstBlk();
			prfmsg(TSNTMSG, teleacc.userid);
			doprintoutput();
			break;
			}
		case USERON :	/* user is on-line */
			{
			while ( ( checkcount < NUMBLOCKS ) && ( userblocked == -1 ) )
				{
				if ( sameas( usaptr->userid, blocked[othusn].block[checkcount] ) )
					{       /* this user's pages are blocked */
					userblocked=checkcount;
					}
				checkcount++;
				}
			if ( ( userblocked != -1 ) && !haskey(telenoblockkey) )
				{       /* cannot send telegram to this user */
				prfmsg(TMBLOCK, checkuserid);
				doprintoutput();
				return;
				}
			getstring( sendgram.telegramtext, attempts, (margc-1) );
			preparetelegram( &sendgram, teleacc.userid );
			dfaSetBlk(telegramdata);
			dfaInsert(&sendgram);
			dfaRstBlk();
			prfmsg(TSNTMSG, teleacc.userid);
			doprintoutput();
			if ( notifyoftelegram(teleacc.userid) )
				{
				telltelegram();
				}
			break;
			}
		default : /* no such user */
			{
			prfmsg(TSEMSG, checkuserid);
			doprintoutput();
			}
		}
	}

VOID setkey( CHAR *lock )                           	/* turn cur user's bit on for this key  */
	{	/* lock is the key name */
	int bit;	/* which bit in usrptr->keys[] to set */

	bit=lockbit(lock,1);
	usrptr->keys[bit>>4]|=(1<<(bit&15));
	}

extern VOID tele_blocks( VOID )				/* set and remove telegram blocks */
	{
	CHAR blockid[INPSIZ];           /* user-id to block/unblock */
	struct usracc blockacc;         /* user account data of blockid */
	struct userdata blockdat;       /* user doing blocking global data */
	CHAR counter;                   /* general purpose looping counter */
	INT blockposition;              /* -1 if user not blocked, else position in blocked[] */
	INT found;                      /* 1 if space found for telegram block, otherwise 0 */
	INT lower;			/* offset to copy string from */
	INT upper;			/* offset to copy string to */

	setmbk(glomb);
	if ( !haskey(help[39].key) )           /* check that user may use this global */
		{
		cantuse();
		return;
		}
	if ( margc == 1 )
		{       /* show proper usage of command */
		prfmsg(TBKUSG, teleblockstring, teleblockstring);
		doprintoutput();
		return;
		}
	if ( margc == 2 && sameas( margv[1], "?" ) )
		{       /* show who is currently blocked */
		showteleblocks();
		return;
		}
	getuserid( blockid, 1, margc-1 );
	dfaSetBlk(accbb);
	if ( !dfaAcqEQ(&blockacc, blockid, 0) )
		{       /* no such user */
		prfmsg(NOSCHGUY, blockid);
		doprintoutput();
		dfaRstBlk();
		return;
		}
	dfaRstBlk();
	blockposition=0;
	found=0;
	do
		{       /* see if the user is blocked */
		if ( sameas( blocked[usrnum].block[blockposition], blockid ) )
			{       /* found, blocked */
			found=1;
			}
		else
			{
			blockposition++;
			}
		}
	while ( ( blockposition < NUMBLOCKS ) && !found );
	if ( found )
		{       /* user is blocked, so unblock */
		setmem( blocked[usrnum].block[blockposition], UIDSIZ, 0 );
		lower=blockposition;
		upper=blockposition+1;
		while ( ( upper < NUMBLOCKS ) && !sameas( blocked[usrnum].block[upper], "" ) )
			{	/* copy any strings at higher offsets down 1 to eliminate "holes" */
			strcpy( blocked[usrnum].block[lower++], blocked[usrnum].block[upper++] );
			}
		setmem( blocked[usrnum].block[lower], UIDSIZ, 0 );	/* clear so we don't have it twice */
		dfaSetBlk(userglobdata);
		if ( dfaAcqEQ(&blockdat, usaptr->userid, 0) )
			{
			cvtDataIP(&blockdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
			for ( counter=0; counter < NUMBLOCKS; counter++ )
				{	/* update data to disk */
				strcpy( blockdat.teleblocks[counter], blocked[usrnum].block[counter] );
				}
			cvtDataIP(&blockdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
			dfaUpdate(&blockdat);
			}
		dfaRstBlk();
		prfmsg(UNBLCK, blockacc.userid);
		}
	else
		{       /* block this dude */
		if ( uhskey(blockacc.userid, telenoblockkey ) )
			{       /* of course you can't block a Sysop */
			prfmsg(NOBLKSYS, blockacc.userid);
			doprintoutput();
			return;
			}
		found=0;
		blockposition=0;
		do
			{    /* get first free position (if any) */
			if ( sameas( blocked[usrnum].block[blockposition], "" ) )
				{       /* this spot is free */
				strcpy( blocked[usrnum].block[blockposition], blockacc.userid );
				found=1;
				}
			else
				{       /* check next spot */
				blockposition++;
				}
			}
		while ( ( blockposition < NUMBLOCKS ) && !found );
		if ( blockposition < NUMBLOCKS )
			{       /* block this user */
			dfaSetBlk(userglobdata);
			if ( dfaAcqEQ(&blockdat, usaptr->userid, 0) )
				{
				cvtDataIP(&blockdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
				strcpy( blockdat.teleblocks[blockposition], blockacc.userid );
				cvtDataIP(&blockdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
				dfaUpdate(&blockdat);
				}
			dfaRstBlk();
			prfmsg(BLCK, blockacc.userid);
			}
		else
			{       /* could not find a spot */
			prfmsg(CNBLCK, blockacc.userid);
			prfmsg(TBKHDR);
			for ( counter=0; counter < NUMBLOCKS; counter++ )
				{       /* show users blocked */
				if ( !sameas(blocked[usrnum].block[counter], "" ) )
					{
					prfmsg(USRBLK, blocked[usrnum].block[counter]);
					}
				}
			}
		}
	doprintoutput();
	}

VOID do_alarm( VOID )					/* user wishes to set or remove an alarm */
	{
	USHORT todaysdate;	/* date as returned by today() */
	USHORT timenow;		/* time as returned by now() */

	setmbk(glomb);
	if ( !haskey(help[41].key) )           /* check that user may use this global */
		{
		cantuse();
		return;
		}
	if ( margc == 1 )
		{	/* user wishes to clear current alarm */
		if ( alarmtimes[usrnum][0] == 0 )
			{	/* user has no current alarm to clear */
			prfmsg(NOALRM);
			doprintoutput();
			return;
			}
		else
			{	/* clear the alarm */
			prfmsg(ALRMCLR, nctime(alarmtimes[usrnum][1]));
			doprintoutput();
			alarmtimes[usrnum][0]=0;
			alarmtimes[usrnum][1]=0;
			return;
			}
		}
	if ( margc == 2 )
		{
		if ( sameas( margv[1], "?" ) )
			{	/* user wants help and to view his current setting */
			if ( alarmtimes[usrnum][0] == 0 )
				{	/* no current setting */
				prfmsg(ALRMNTIM);
				}
			else
				{
				prfmsg(ALRMTIM, nctime(alarmtimes[usrnum][1]));
				}
			doprintoutput();
			return;
			}
		if ( strlen(margv[1]) > (DATSIZ-1) )
			{	/* bogus time value entered */
			prfmsg(ALRMUSG, help[41].command, help[41].command);
			doprintoutput();
			return;
			}
		alarmtimes[usrnum][1]=dctime(margv[1]);
		timenow=now();
		todaysdate=today();
		if ( alarmtimes[usrnum][1] < timenow )
			{	/* set alarm for tomorrow */
			alarmtimes[usrnum][0]=getfdate(1, todaysdate);
			}
		else
			{	/* alarm is for today */
			alarmtimes[usrnum][0]=todaysdate;
			}
		prfmsg(ALRMSET, nctime(alarmtimes[usrnum][1]));
		doprintoutput();
		return;
		}
	prfmsg(ALRMUSG, help[41].command, help[41].command);
	doprintoutput();
	}

VOID delete_telegrams( VOID )				/* delete all telegrams on the system from a user */
	{
	CHAR deleteid[INPSIZ];            /* user-id to delete all telegrams from */
	struct usracc deldat;             /* user account data of deleteid */
	INT found;                        /* 1 if telegram from deleteid found, otherwise 0 */
	LONG telegramcount=0L;            /* number of telegrams to user from deleteid */

	setmbk(glomb);
	if ( !haskey(deltelekey) )           /* check that user may use this global */
		{
		cantuse();
		return;
		}
	if ( margc == 1 )
		{       /* print details on usage */
		prfmsg(TDAUSG, deleteallteles);
		doprintoutput();
		return;
		}
	getuserid(deleteid, 1, margc-1);
	dfaSetBlk(accbb);
	if ( !dfaAcqEQ(&deldat, deleteid, 0) )
		{       /* no such user */
		prfmsg(NOSCHGUY, deleteid);
		doprintoutput();
		dfaRstBlk();
		return;
		}
	dfaRstBlk();
	strcpy( deleteid, deldat.userid );      /* get actual handle */
	dfaSetBlk(telegramdata);
	found=dfaAcqEQ(NULL, deleteid, 1);
	while ( found )
		{       /* delete all telegrams from deleteid */
		dfaDelete();
		telegramcount++;
		found=dfaAcqEQ(NULL, deleteid, 1);
		}
	dfaRstBlk();
	if ( telegramcount != 0L )
		{
		prfmsg(TDADELT, l2as(telegramcount), deleteid);
		}
	else
		{
		prfmsg(NOTDA, deleteid);
		}
	doprintoutput();
	}

VOID erase_telegrams( VOID )				/* erase all telegrams to yourself from a user */
	{
	CHAR deleteid[INPSIZ];            /* user-id to blast telegrams from */
	struct telegram cleargm;          /* potential telegram to delete */
	struct usracc deldat;             /* user account data of deleteid */
	INT found;                        /* 1 if telegram from deleteid found, otherwise 0 */
	LONG telegramcount=0L;            /* number of telegrams to user from deleteid */

	setmbk(glomb);
	if ( !haskey(help[39].key) )           /* check that user may use this global */
		{
		cantuse();
		return;
		}
	if ( margc == 1 )
		{       /* print details on usage */
		prfmsg(TCLUSG, telegramclear);
		doprintoutput();
		return;
		}
	getuserid( deleteid, 1, (margc-1) );
	dfaSetBlk(accbb);
	if ( !dfaAcqEQ(&deldat, deleteid, 0) )
		{       /* no such user */
		prfmsg(NOSCHGUY, deleteid);
		doprintoutput();
		dfaRstBlk();
		return;
		}
	dfaRstBlk();
	strcpy( deleteid, deldat.userid );      /* get actual handle */
	if ( uhskey( deldat.userid, telenoblockkey ) )
		{       /* can't delete a Sysop's telegrams */
		prfmsg(NODELSYS, deleteid);
		doprintoutput();
		return;
		}
	dfaSetBlk(telegramdata);
	found=dfaAcqEQ(&cleargm, usaptr->userid, 0);
	while ( found )
		{       /* delete all telegrams from deleteid */
		if ( sameas( deleteid, cleargm.from ) )
			{       /* we have a match; delete */
			dfaDelete();
			telegramcount++;
			}
		found=dfaAcqNX(&cleargm);
		}
	dfaRstBlk();
	if ( telegramcount != 0L )
		{
		prfmsg(TELDELT, l2as(telegramcount), deleteid);
		}
	else
		{
		prfmsg(NOTEL, deleteid);
		}
	doprintoutput();
	}

VOID handle_friends( VOID )				/* handle friends notice command */
	{
	struct usracc frienddat;	/* user account data of Friend */
	CHAR friendid[UIDSIZ];		/* User-ID of possible Friend */

	setmbk(glomb);
	if ( !haskey(help[43].key) )           /* check that user may use this global */
		{
		cantuse();
		return;
		}
	if ( margc == 1 )
		{	/* display help on command */
		friendhelp();
		return;
		}
	if ( margc == 2 )
		{
		if ( sameas( margv[1], "?" ) )
			{	/* show current friends */
			prfmsg(CURFRNDS, sameas( frienddata[usrnum].logonid, "" ) ? "<None>" : frienddata[usrnum].logonid, sameas( frienddata[usrnum].logoffid, "" ) ? "<None>" : frienddata[usrnum].logoffid);
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], "ON" ) )
			{	/* clear friend log on */
			if ( sameas( frienddata[usrnum].logonid, "" ) )
				{	/* nothing to clear */
				prfmsg(NOFLOG, "on");
				}
			else
				{	/* clear current friend log on notice */
				setmem( frienddata[usrnum].logonid, UIDSIZ, 0 );
				prfmsg(FLOGCLR, "on");
				}
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], "OFF" ) )
			{	/* clear friend log off */
			if ( sameas( frienddata[usrnum].logoffid, "" ) )
				{	/* nothing to clear */
				prfmsg(NOFLOG, "off");
				}
			else
				{	/* clear current friend log on notice */
				setmem( frienddata[usrnum].logoffid, UIDSIZ, 0 );
				prfmsg(FLOGCLR, "off");
				}
			doprintoutput();
			return;
			}
		friendhelp();
		return;
		}
	if ( sameas( margv[1], "ON" ) )
		{	/* set friend log on */
		getuserid(friendid, 2, margc-1);
		dfaSetBlk(accbb);
		if ( !dfaAcqEQ(&frienddat, friendid, 0) )
			{	/* no such user */
			prfmsg(NOSCHGUY, friendid);
			doprintoutput();
			dfaRstBlk();
			return;
			}
		dfaRstBlk();
		if ( onsysn( frienddat.userid, 1 ) && cansee() )
			{	/* user is already on-line */
			prfmsg(FRNDNSET, frienddat.userid, "on");
			doprintoutput();
			return;
			}
		strcpy( frienddata[usrnum].logonid, frienddat.userid );
		prfmsg(FRNDSET, frienddat.userid, "on");
		doprintoutput();
		return;
		}
	if ( sameas( margv[1], "OFF" ) )
		{	/* set friend log off */
		getuserid(friendid, 2, margc-1);
		dfaSetBlk(accbb);
		if ( !dfaAcqEQ(&frienddat, friendid, 0) )
			{	/* no such user */
			prfmsg(NOSCHGUY, friendid);
			doprintoutput();
			dfaRstBlk();
			return;
			}
		dfaRstBlk();
		if ( !onsysn( frienddat.userid, 1 ) || !cansee() )
			{	/* user isnt on-line, or can't be "seen" */
			prfmsg(FRNDNSET, frienddat.userid, "off");
			doprintoutput();
			return;
			}
		strcpy( frienddata[usrnum].logoffid, frienddat.userid );
		prfmsg(FRNDSET, frienddat.userid, "off");
		doprintoutput();
		return;
		}
	friendhelp();
	}

VOID colorize_input( VOID )				/* color any input desired */
	{
	CHAR initialinput[INPSIZ];          /* input as given to globals */
	CHAR convertedinput[INPSIZ];        /* location to store colored input */
	INT inputoffset;                    /* offset of input we're converting */
	INT inputstringlen;                 /* length of our initial input string */
	INT constroffset;                   /* position in converted string we're at */
	INT arrayoffset;                    /* number in input converted to integer for accessing array offset */
	CHAR number;                        /* character in inputstring to convert with atoi() */
	INT alteredinput=0;                 /* 1 if input colored, else 0 */
	INT convertpoint;                   /* offset in convertedinput[] to place end ANSI sequence */
	INT currentfore=-1;		    /* foreground color as set by user */
	INT currentback=-1;		    /* background color as set by user */
	INT currentspec=-1;		    /* special effect as set by user */
	INT restorespecial=0;		    /* 1 if need to remove special effect at end of input, 0 if not */
	INT restoreback=0;		    /* 1 if need to restore desired background color, 0 if not */

	setmbk(glomb);
	rstrin();
	setmem( convertedinput, INPSIZ, 0 );
	strcpy( initialinput, input );
	inputstringlen=strlen(initialinput);
	constroffset=0;
	inputoffset=0;
	while ( ( inputoffset < inputstringlen ) && ( constroffset < INPSIZ ) )
		{       /* do the work */
		if ( ( ( inputoffset + 2 ) <= inputstringlen ) && ( initialinput[inputoffset] == ANSIchar ) && ( initialinput[inputoffset+1] == backchar ) && ( isdigit(initialinput[inputoffset+2]) ) &&
		   ( ( constroffset + 5 ) < INPSIZ ) )
			{       /* check for background change */
			number=initialinput[inputoffset+2];
			arrayoffset=atoi(&number);
			if ( ( arrayoffset < NUMANSICOLORS ) && ( arrayoffset != currentback ) )
				{       /* valid offset, and different color */
				convertedinput[constroffset++]=ansistart;
				convertedinput[constroffset++]=bracket;
				convertedinput[constroffset++]=inbackground;
				inputoffset+=2; /* advance beyond backchar and digit */
				convertedinput[constroffset++]=initialinput[inputoffset++];
				convertedinput[constroffset++]=endsequence;
				currentback=arrayoffset;
				if ( currentback != endback )
					{	/* restore background color when done */
					restoreback=1;
					}
				else
					{	/* no need to bother */
					restoreback=0;
					}
				alteredinput=1;
				}
			else
				{       /* just copy the next character */
				convertedinput[constroffset++]=initialinput[inputoffset++];
				}
			}
		else if ( ( ( inputoffset + 2 ) <= inputstringlen ) && ( initialinput[inputoffset] == ANSIchar ) && ( initialinput[inputoffset+1] == forechar ) && ( isdigit(initialinput[inputoffset+2]) ) &&
			( ( constroffset + 5 ) < INPSIZ ) )
			{       /* check for foreground change */
			number=initialinput[inputoffset+2];
			arrayoffset=atoi(&number);
			if ( ( arrayoffset < NUMANSICOLORS ) && ( arrayoffset != currentfore ) )
				{       /* valid offset */
				convertedinput[constroffset++]=ansistart;
				convertedinput[constroffset++]=bracket;
				convertedinput[constroffset++]=inforeground;
				inputoffset+=2; /* advance beyond forechar and digit */
				convertedinput[constroffset++]=initialinput[inputoffset++];
				convertedinput[constroffset++]=endsequence;
				currentfore=arrayoffset;
				alteredinput=1;
				}
			else
				{       /* just copy the next character */
				convertedinput[constroffset++]=initialinput[inputoffset++];
				}
			}
		else if ( ( ( inputoffset + 2 ) <= inputstringlen ) && ( initialinput[inputoffset] == ANSIchar ) && ( initialinput[inputoffset+1] == specchar ) && ( isdigit(initialinput[inputoffset+2]) ) &&
			( ( constroffset + 4 ) < INPSIZ ) )
			{       /* check for special change */
			number=initialinput[inputoffset+2];
			arrayoffset=atoi(&number);
			if ( ( arrayoffset < NUMSPECIAL ) && ( arrayoffset != currentspec ) )
				{       /* valid offset */
				convertedinput[constroffset++]=ansistart;
				convertedinput[constroffset++]=bracket;
				convertedinput[constroffset++]=specialoutput[arrayoffset];
				convertedinput[constroffset++]=endsequence;
				inputoffset+=3; /* advance beyond ANSI sequence */
				currentspec=arrayoffset;
				if ( currentspec != (INT)BOLDCOLOR )
					{	/* need to restore special effect */
					restorespecial=1;
					}
				else
					{	/* no need to bother */
					restorespecial=0;
					}
				alteredinput=1;
				}
			else
				{       /* just copy the next character */
				convertedinput[constroffset++]=initialinput[inputoffset++];
				}
			}
		   else
			{       /* just another character to copy */
			convertedinput[constroffset++]=initialinput[inputoffset++];
			}
		}
	if ( alteredinput )
		{       /* adjust final output */
		convertpoint=1;	/* account for string terminator at end */
		if ( restorespecial )
			{	/* have to first set normal ANSI, then bold */
			convertpoint+=(INT)NORMNBOLD;
			}
		else if ( currentspec == 1 )
			{	/* just bring back bold since our current ANSI is normal */
			convertpoint+=(INT)JUSTBOLD;
			}
		if ( restoreback )
			{	/* restore desired background color */
			convertpoint+=(INT)DOBACK;
			}
		if ( strlen( convertedinput ) > (size_t)( INPSIZ - convertpoint ) )
			{	/* have to overwrite some of the input */
			constroffset=INPSIZ-convertpoint;
			}
		else
			{	/* just tack it on to end of input */
			constroffset=strlen( convertedinput );
			}
		if ( restorespecial )
			{	/* set ANSI to normal, then bold */
			convertedinput[constroffset++]=ansistart; /* restore normal ANSI */
			convertedinput[constroffset++]=bracket;
			convertedinput[constroffset++]=specialoutput[NORMALCOLOR];
			convertedinput[constroffset++]=endsequence;
			}
		if ( restoreback )
			{       /* set background to desired */
			convertedinput[constroffset++]=ansistart;
			convertedinput[constroffset++]=bracket;
			convertedinput[constroffset++]=inbackground;
			convertedinput[constroffset++]=regularcolors[endback];
			convertedinput[constroffset++]=endsequence;
			}
		if (constroffset < 251) 
		    {
			convertedinput[constroffset++] = ansistart; /* set bold ANSI */
			convertedinput[constroffset++] = bracket;
			convertedinput[constroffset++] = specialoutput[BOLDCOLOR];
			convertedinput[constroffset++] = endsequence;
		    }
		convertedinput[constroffset]='\0';
		}
	convertedinput[constroffset] = '\0';
	strcpy( input, convertedinput );
	parsin();
	rstmbk();
	}

extern VOID repeat_input( VOID )			/* repeat previous input */
	{
	CHAR addinput[INPSIZ];          /* input to tack onto last command */
	INT inlen;                      /* length of extra input */
	INT comlen;                     /* length of last command */

	setmbk(glomb);
	if ( !haskey(help[23].key) )             /* check that user may use this global */
		{
		cantuse();
		return;
		}
	if ( margc > 1 )
		{       /* get extra input */
		setmem( addinput, INPSIZ, 0 );
		getstring( addinput, 1, (margc-1) );
		inlen=strlen(addinput);
		comlen=strlen(usercommand[usrnum].thecommand);
		if ( ( comlen + inlen ) >= INPSIZ )
			{       /* too much, can't do this */
			prfmsg(TOOLONG);
			doprintoutput();
			return;
			}
		strcat( usercommand[usrnum].thecommand, " " );
		strcat( usercommand[usrnum].thecommand, addinput );
		}
	strcpy( input, usercommand[usrnum].thecommand );
	parsin();
	rstmbk();
	}

VOID handle_busy( VOID )				/* toggle "Busy" flag for ourselves or another user */
	{
	struct userdata bsydat;		/* user's global data for updating */

	setmbk(glomb);
	if ( margc == 1 )
		{	/* toggle user "busy" */
		if ( !haskey(help[3].key) )           /* check that user may use this global */
			{
			cantuse();
			return;
			}
		togglebusy(usrptr);
		dfaSetBlk(userglobdata);
		if ( dfaAcqEQ(&bsydat, usaptr->userid, 0) )
			{	/* update globals data */
			cvtDataIP(&bsydat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
			if ( usrbusy(usrptr) )
				{	/* toggle on */
				bsydat.busy=1;
				}
			else
				{	/* toggle off */
				bsydat.busy=0;
				}
			cvtDataIP(&bsydat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
			dfaUpdate(&bsydat);
			}
		dfaRstBlk();
		prfmsg(BUSYMSG, usrbusy(usrptr) ? "ON" : "OFF");
		doprintoutput();
		return;
		}
	else
		{	/* toggle another user "unbusy" */
		CHAR unbusyid[INPSIZ];		/* user-id to toggle "unbusy" */
		struct usracc busyacc;		/* user account data of user to toggle */

		if ( !haskey(unbusyothkey) )           /* check that user may use this global */
			{
			cantuse();
			return;
			}
		getuserid( unbusyid, 1, (margc-1) );
		dfaSetBlk(accbb);
		if ( !dfaAcqEQ(&busyacc, unbusyid, 0) )
			{	/* no such user */
			prfmsg(NOSCHGUY, unbusyid);
			doprintoutput();
			dfaRstBlk();
			return;
			}
		dfaRstBlk();
		if ( !onsysn(unbusyid, 1) || !cansee() )
			{	/* user cannot "see" other user, so cannot toggle busy flag off */
			prfmsg(NOTON, busyacc.userid);
			doprintoutput();
			return;
			}
		if ( !usrbusy(othusp) )
			{	/* user is not "busy" at the moment */
			prfmsg(NOTBUSY, busyacc.userid);
			doprintoutput();
			return;
			}
		if ( inopchat() )
			{	/* user is in Opchat, can't turn "busy" off */
			sayinchat(busyacc.userid);
			return;
			}
		if ( userintransfer(busyacc.userid) )
			{	/* user is transferring a file, so can't toggle "busy" OFF */
			prfmsg(INFILE, busyacc.userid);
			doprintoutput();
			return;
			}
		togglebusy(othusp);
		dfaSetBlk(userglobdata);
		if ( dfaAcqEQ(&bsydat, othuap->userid, 0) )
			{	/* update globals data */
			cvtDataIP(&bsydat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, othusn);
			bsydat.busy=0;
			cvtDataIP(&bsydat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, othusn);
			dfaUpdate(&bsydat);
			}
		dfaRstBlk();
		if ( dobeeps )
			{
			beep(1);
			}
		prfmsg(URNTBSY, usaptr->userid);
		injoth();
		prfmsg(USRNBSY, busyacc.userid);
		doprintoutput();
		shocst("USER FORCED UNBUSY",
		       "%s by %s", busyacc.userid, usaptr->userid);
		return;
		}
	}

VOID busy_notify( VOID )				/* notify user when someone is no longer "Busy" */
	{
	CHAR busyid[INPSIZ];            /* user-id to check for validity and busy */
	INT found;                      /* result of bsysearch; 0 if not found, 1 if ok, 2 if more than 1 possible user-id */

	setmbk(glomb);
	if ( !haskey(help[4].key) )
		{
		cantuse();
		return;
		}
	if ( margc == 1 )
		{       /* user wishes to clear current notify */
		if ( sameas( theids[usrnum].busyid, "" ) )
			{       /* not set to be notified */
			prfmsg(NONOT);
			doprintoutput();
			return;
			}
		prfmsg(NOTCAN, theids[usrnum].busyid);
		doprintoutput();
		setmem( theids[usrnum].busyid, UIDSIZ, 0 );
		setmem( theids[usrnum].notid, UIDSIZ, 0 );
		return;
		}
	if ( margc == 2 && sameas( margv[1], "?" ) )
		{	/* show user to currently notify about */
		prfmsg(CURNOT, (sameas( theids[usrnum].notid, "")) ? "<<Nobody>>" : theids[usrnum].notid);
		doprintoutput();
		return;
		}
	getuserid( busyid, 1, (margc-1) );
	found=bsysearch(busyid);
	switch ( found )
		{
		case 0 :        /* no such user on */
			{
			prfmsg(GONE, busyid);
			break;
			}
		case 1 :        /* found exact person */
			{
			onsysn(busyid, 1);          /* set othusp and othuap; we know they are on-line */
			if ( !cansee() )
				{	/* user cannot "see" other user, so cannot toggle busy flag off */
				prfmsg(NOTON, busyid);
				doprintoutput();
				return;
				}
			if ( !usrbusy(othusp) )
				{
				prfmsg(NTBSY, othuap->userid);
				doprintoutput();
				return;
				}
			strcpy( theids[usrnum].busyid, othuap->userid );
			strcpy( theids[usrnum].notid, usaptr->userid );
			prfmsg(NOTIFY, othuap->userid);
			break;
			}
		case 2 :        /* need to better specify who you mean */
			{
			prfmsg(WHO, busyid, help[4].command);
			}
		}
	doprintoutput();
	}

INT relog( VOID )					/* relog handler */
	{
	CHAR relogid[INPSIZ];           /* user-id to re-log */
	struct usracc relogacc;         /* account data of user to re-log */
	INT saveusrnum;			/* usrnum saved for rstwin() to othusn */
	INT inform;			/* 1 if tell other user of relog, 0 if don't tell */

	setmbk(glomb);
	if ( margc == 1 )
		{       /* user wishes to re-log */
		if ( !haskey(help[24].key) )           /* check that user may use this global */
			{
			cantuse();
			return(1);
			}
		prfmsg(RLGMSG);
		doprintoutput();
		btuinj(usrnum, RELOG);
		return(-1);
		}
	if ( !haskey(relogothkey) )           /* check that user may use this global */
		{
		cantuse();
		return(1);
		}
	if ( (margc > 2) && sameas(margv[margc-1], "+") )
		{	/* tell other user we are re-logging him */
		inform=1;
		getuserid(relogid, 1, (margc-2));
		}
	else
		{	/* don't tell other user we are re-logging him */
		inform=0;
		getuserid(relogid, 1, (margc-1));
		}
	dfaSetBlk(accbb);
	if ( !dfaAcqEQ(&relogacc, relogid, 0) )
		{       /* no such user to re-log */
		prfmsg(NOSCHGUY, relogid);
		doprintoutput();
		dfaRstBlk();
		return(1);
		}
	dfaRstBlk();
	if ( !onsysn(relogid, 1) || !cansee() )
		{       /* not presently logged on */
		prfmsg(NOTON, relogacc.userid);
		doprintoutput();
		return(1);
		}
	if ( inopchat() )
		{	/* user is in Opchat, can't re-log */
		sayinchat(relogacc.userid);
		return(1);
		}
	if ( userintransfer(relogacc.userid) )
		{	/* user is in a file transfer, can't re-log */
		prfmsg(REINFIL, relogacc.userid);
		doprintoutput();
		return(1);
		}
	prfmsg(RLGING, relogacc.userid);
	outprf(usrnum);
	saveusrnum=usrnum;
	curusr(othusn);
	rstwin();
	curusr(saveusrnum);
	othusp->state=0;
	othusp->substt=0;
	btuchi(othusn, NULL);
	echonu(othusn);
	btuche(othusn, 0);
	btulfd(othusn, '\n');
	btuscr(othusn, '\n');
	btumil(othusn, 0);
	rstrxf();
	btutsw(othusn, othuap->scnwid);
	if ( inform )
		{	/* tell user who has done this to him */
		prfmsg(RLGED, usaptr->userid);
		injoth();
                }
	btuinj(othusn, RELOG);
	shocst("USER FORCED TO RELOG",
	       "%s by %s", relogacc.userid, usaptr->userid);
	rstmbk();
	return(1);
	}

VOID show_credits( VOID )				/* display a user's credit information */
	{
	struct userdata logdat;	 	/* for getting date of last log-on */
	SHORT logtime;			/* time of last log-on - was USHORT but assigning -1 to it is no bueno - RH 8/2/2024 */
	CHAR infoid[INPSIZ];            /* user id to get info on */
	INT online;                     /* 1 if user on-line, 0 if not */
	struct usracc info;             /* user account to post credits to (on disk) */
	struct usracc *infoptr;         /* pointer to user account to post credits to */

	INIT_STRUCT_TO_ZERO(info);

	setmbk(glomb);
	if ( margc == 1 )
		{	/* display info on current user */
		if ( !haskey(help[5].key) )           /* check that user may use this global */
			{
			cantuse();
			return;
			}
		dfaSetBlk(userglobdata);
		if ( dfaAcqEQ(&logdat, usaptr->userid, 0) )
			{	/* get time of last log-on */
			cvtDataIP(&logdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
			logtime=logdat.logtime;
			}
		else
			{	/* don't know last log-on time */
			logtime=-1;
			}
		dfaRstBlk();
		showcreditinfo(logtime);
		}
	else
		{
		if ( !haskey(infolookkey) )           /* check that user may use this global */
			{
			cantuse();
			return;
			}
		if ( margc < 2 )
			{       /* need command, userid and number */
			prfmsg(UIHLP, help[31].command);
			doprintoutput();
			return;
			}
		getuserid( infoid, 1, (margc-1) );
		online=onsysn( infoid, 1 );
		if ( online )       /* online user, so modify account in memory */
			{
			infoptr=othuap;
			}
		else
			{
			infoptr=&info;
			dfaSetBlk(accbb);
			if ( !dfaAcqEQ(infoptr, infoid, 0) )
				{
				prfmsg(NOSCHGUY, infoid);
				doprintoutput();
				dfaRstBlk();
				return;
				}
			dfaRstBlk();
			}
		dfaSetBlk(userglobdata);
		if ( dfaAcqEQ(&logdat, infoptr->userid, 0) )
			{	/* get time of last log-on */
			cvtDataIP(&logdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
			logtime=logdat.logtime;
			}
		else
			{	/* don't know last log-on time */
			logtime=-1;
			}
		dfaRstBlk();
		otheruserinfo( infoptr, logtime, online );
		}
	rstmbk();
	}

VOID post_credits( VOID )				/* post credits to a user */
	{
	CHAR postid[INPSIZ];    /* user id to post credits to */
	LONG postcreds;         /* credits to post to account */
	LONG testpost;          /* convert postcreds to positive for check */
	INT plusminus;          /* 1 if credits paid, 0 if credits free */
	struct usracc crd;      /* user account to post credits to (on disk) */
	struct usracc *crdptr;  /* pointer to user account to post credits to */
	struct telegram notgm;  /* send telegram to user to notify if necessary */
	INT sendok;             /* 0 if send telegram to notify, otherwise 1 */
	INT online;		/* 1 if user posted to is online, 0 if not */
	GBOOL inform;           /* TRUE if tell user posted to, FALSE if don't */
	INT paid_position;	/* margv[paid_position] is where P/F should be */
	INT cred_position;	/* margv[cred_position] is where credit amount should be */
	INT uid_position;	/* margv[1] to margv[uid_position] should be user to post to */

	INIT_STRUCT_TO_ZERO(crd);

	setmbk(glomb);
	if ( !haskey(help[20].key) )          /* check that user may use this global */
		{
		cantuse();
		return;
		}
	if ( sameas(margv[margc-1], "+") )
		{	/* tell user posted to */
		inform=TRUE;
		paid_position=margc-2;
		cred_position=margc-3;
		uid_position=margc-4;
		}
	else
		{	/* don't tell user posted to */
		inform=FALSE;
		paid_position=margc-1;
		cred_position=margc-2;
		uid_position=margc-3;
		}
	if ( (inform && (margc < 5)) || (margc < 4) )
		{       /* need command, userid, amount and paid or free and optionally tell user */
		prfmsg(PSTHLP, help[20].command, (haskey(unlimpostkey)) ? l2as(MAXPOST) : l2as(maxpostcreds)); /* print post credits help */
		doprintoutput();
		return;
		}
	plusminus=sameas(margv[paid_position], "P");
	if ( plusminus != 1 )
		{	/* not PAID credits, check for FREE */
		plusminus=sameas(margv[paid_position], "F");
		if ( plusminus != 1 )
			{
			prfmsg(PSTHLP, help[20].command, (haskey(unlimpostkey)) ? l2as(MAXPOST) : l2as(maxpostcreds)); /* print post credits help */
			doprintoutput();
			return;
			}
		else
			{	/* set to FREE credits */
			plusminus--;
			}
		}
	if ( !numok(margv[cred_position], 1) || (strlen(margv[cred_position]) > MAXPOSTDIG ) )
		{       /* bogus input */
		prfmsg(PSTLIMS, l2as(-MAXPOST), l2as(MAXPOST));
		doprintoutput();
		return;
		}
	postcreds=atol(margv[cred_position]);
	if ( postcreds == 0L )
		{
		prfmsg(NOZEROP);
		doprintoutput();
		return;
		}
	if ( postcreds < 0L )
		{       /* convert tested amount to positive number */
		testpost=postcreds * -1;
		}
	else
		{
		testpost=postcreds;
		}
	if ( ( testpost > maxpostcreds ) && !haskey(unlimpostkey) )
		{
		prfmsg(HICRDMSG, l2as(-maxpostcreds), l2as(maxpostcreds));
		doprintoutput();
		return;
		}
	getuserid(postid, 1, (uid_position));
	dfaSetBlk(accbb);
	crdptr=&crd;
	if ( !dfaAcqEQ(crdptr, postid, 0) )
		{	/* cannot find userid entered */
		prfmsg(NOSCHGUY, postid);
		doprintoutput();
		return;
		}
	if ( taggedforkill(crdptr) )
		{       /* if tagged for delete don't post credits */
		dfaRstBlk();
		prfmsg(PSTDLT, crdptr->userid);
		doprintoutput();
		return;
		}
	if ( crdptr->creds < 0L )
		{	/* check for negative overflow */
		if ( (double)(postcreds + crdptr->creds) < LONG_MIN )
			{
			prfmsg(LOOFLW, crdptr->userid, l2as(LONG_MAX - crdptr->creds));
			doprintoutput();
			return;
			}
		}
	else
		{	/* check for positive overflow */
		if ( postcreds > (LONG_MAX - crdptr->creds) )
			{
			prfmsg(HIOFLW, crdptr->userid, l2as(-LONG_MIN - crdptr->creds));
			doprintoutput();
			return;
			}
		}
	crdptr->creds+=postcreds;
	crdptr->totcreds+=postcreds;
	if ( plusminus )
		{	/* credits are paid */
		crdptr->totpaid+=postcreds;
		}
	dfaUpdate(crdptr);
	dfaRstBlk();
	prfmsg(PSYSMSG, l2as(postcreds), plusminus == 0 ? "FREE" : "PAID", crdptr->userid);
	outprf(usrnum);
	if ( crdptr->creds > 0L && (clsptr=fndcls(crdptr->curcls)) != NULL )
		{       /* if needed switch user's class */
		if ( clsptr->flags&HASCRD )
			{
			swtcls(crdptr, sameas(crdptr->curcls, crdptr->prmcls),
			       clsptr->nxtcls[DCREDIT],1,0);
			}
		}
	online=onsysn(crdptr->userid, 1);
	if ( online )
		{	/* update online data */
		othuap->creds+=postcreds;
		}
	sendok=( inform && online && oktotell() && canbeseen() );
	if ( sendok )
		{       /* attempt to inform online user */
		if ( dobeeps )
			{
			beep(1);
			}
		prfmsg(PSTINJ, usaptr->userid, l2as( postcreds ));
		sendok=injoth();
		}
	if ( !sendok && inform )
		{       /* inform by telegram */
		dfaSetBlk(telegramdata);   /* send telegram to notify */
		preparetelegram(&notgm, crdptr->userid);
		strcpy( notgm.telegramtext, spr(stgopt(PSTNOTE), l2as( postcreds )) );
		dfaInsert(&notgm);
		dfaRstBlk();
		if ( notifyoftelegram(crdptr->userid) )
			{       /* go ahead and let him know */
			telltelegram();
			}
		}
	if ( plusminus )
		{       /* update system variables */
		sv2.paidpst+=postcreds;
		}
	else
		{
		sv2.freepst+=postcreds;
		}
	shocst(spr("%s %s CREDITS POSTED", l2as(postcreds), (plusminus == 0 ? "FREE" : "PAID")),
		   "%s to %s", usaptr->userid, crdptr->userid);
	rstmbk();
	}

VOID boot_user( VOID )					/* disconnect a user */
	{
	CHAR bootid[INPSIZ];            /* user-id to boot */
	GBOOL inform;			/* TRUE if tell user who booted him, FALSE if don't */

	setmbk(glomb);
	if ( !haskey(help[35].key) )           /* check that user may use this global */
		{
		cantuse();
		return;
		}
	if ( margc == 1 )
		{
		prfmsg(BTHLP, help[35].command);
		doprintoutput();
		return;
		}
	if ( sameas(margv[margc-1], "+") && (margc > 2) )
		{	/* tell user who booted */
		inform=TRUE;
		getuserid(bootid, 1, margc-2);
		}
	else
		{	/* don't tell the user */
		inform=FALSE;
		getuserid(bootid, 1, margc-1);
		}
	if ( !onsysn(bootid, 1) || !cansee() )
		{	/* user is not on, or can't see user */
		prfmsg(NOTON, bootid);
		doprintoutput();
		return;
		}
	if ( inopchat() )
		{	/* user is in Opchat, can't "zap" */
		sayinchat(othuap->userid);
		return;
		}
	if ( dobeeps )
		{
		beep(1);
		}
	if ( inform )
		{	/* tell user who did it */
		prfmsg(BTMSG, usaptr->userid);
		injoth();
		}
	prfmsg(BTDUN, othuap->userid);
	doprintoutput();
	shocst("USER BOOTED",
	       "%s by %s", othuap->userid, usaptr->userid);
	bbsdsc( othusn );
	}

INT global_chat( VOID )					/* global chat channel handler */
	{
	setmbk(glomb);
	if ( !haskey(help[6].key) )           /* check that user may use this global */
		{
		cantuse();
		return(1);
		}
	if ( margc == 1 )
		{       /* print help on chat channels */
		chatchannelhelp();
		rstmbk();
		return(1);
		}
	if ( margc == 2 && sameas( margv[1], "?" ) )
		{       /* list current chat channel and users on it */
		if ( globalchatchannel[usrnum] == 0 )
			{
			prfmsg(NOCHMSG);
			prfmsg(HICHMSG, numchatchannels);
			}
		else
			{
			prfmsg(CHNMSG, globalchatchannel[usrnum], topics[globalchatchannel[usrnum]].chantopic );
			prfmsg(HICHMSG, numchatchannels);
			listchannelusers();
			}
		doprintoutput();
		return(1);
		}
	if ( margc == 2 && sameas( margv[1], echostring ) )
		{       /* toggle user global chat echo */
		struct userdata curuserdata;    /* user global chat channel and echo state */

		chatecho[usrnum]^=1;
		dfaSetBlk(userglobdata);
		if ( dfaAcqEQ(&curuserdata, usaptr->userid, 0) )
			{	/* save new setting */
			cvtDataIP(&curuserdata, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
			curuserdata.echo=chatecho[usrnum];
			cvtDataIP(&curuserdata, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
			dfaUpdate(&curuserdata);
			}
		dfaRstBlk();
		prfmsg(ECHTOG, ( chatecho[usrnum] == 1 ) ? "ON" : "OFF");
		doprintoutput();
		return(1);
		}
	if ( margc == 2 && sameas( margv[1], showstring ) )
		{       /* list all chat channels and topics on */
		INT chancount;          /* which channel we're checking */
		INT usercount;          /* number of users in channel we're checking */

		prfmsg(TPCHDR);
		for ( chancount = 1; chancount <= numchatchannels; chancount++ )
			{
			if ( !sameas( topics[chancount].chantopic, "" ) )
				{
				usercount=getusersinchannel((USHORT)chancount);
				prfmsg(CHTOPMSG, chancount, usercount, topics[chancount].chantopic);
				}
			}
		doprintoutput();
		return(1);
		}
	if ( margc == 3 && sameas( margv[1], setstring ) )
		{       /* set chat channel to margv[2] 0 to numchatchannels */
		INT setchannel;                 /* set chat channel to this number */
		struct userdata curuserdata;    /* user global chat channel and echo state */
		USHORT todaysdate;		/* date as returned by today() */
		USHORT timenow;			/* time as returned by now() */

		if ( !numok(margv[2], 0) || ( strlen(margv[2]) > MAXCHANDIG ) )
			{       /* bogus input */
			prfmsg(BDSMSG, numchatchannels);
			doprintoutput();
			return(1);
			}
		setchannel=atoi(margv[2]);
		if ( ( setchannel < 0 ) || ( setchannel > numchatchannels ) )
			{
			prfmsg(BDSMSG, numchatchannels);
			doprintoutput();
			return(1);
			}
		if ( ( setchannel == numchatchannels ) && !haskey(syschankey) )
			{       /* non-Sysops can't use Sysop chat channel */
			prfmsg(SYSCMSG);
			doprintoutput();
			return(1);
			}
		todaysdate=today();
		timenow=now();
		if ( !hasmaster() && todaysdate < changedata[usrnum].switchdate )
			{	/* can't switch channels yet */
			prfmsg(NOCHGYET);
			doprintoutput();
			return(1);
			}
		if ( !hasmaster() && ( todaysdate == changedata[usrnum].switchdate ) && ( timenow < changedata[usrnum].switchtime ) )
			{	/* can't switch channels yet */
			prfmsg(NOCHGYET);
			doprintoutput();
			return(1);
			}
		changedata[usrnum].switchtime=(USHORT)mjrtime(changeseconds, timenow);
		if ( changedata[usrnum].switchtime < timenow )
			{	/* next date allowed is tomorrow */
			changedata[usrnum].switchdate=getfdate(1, todaysdate);
			}
		else
			{	/* can switch today */
			changedata[usrnum].switchdate=todaysdate;
			}
		dochannotice(LEFTCHAN);
		globalchatchannel[usrnum]=setchannel;
		dfaSetBlk(userglobdata);
		if ( dfaAcqEQ(&curuserdata, usaptr->userid, 0) )
			{
			cvtDataIP(&curuserdata, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
			curuserdata.chatchannel=(USHORT)setchannel;
			cvtDataIP(&curuserdata, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
			dfaUpdate(&curuserdata);
			}
		dfaRstBlk();
		prfmsg(NCHMSG, (setchannel == 0) ? "OFF" : spr("%d", setchannel));
		if ( !sameas( topics[setchannel].chantopic, "" ) )
			{
			prfmsg(CHTPMSG, topics[setchannel].chantopic);
			}
		listchannelusers();
		doprintoutput();
		dochannotice(JOINCHAN);
		return(1);
		}
	if ( margc == 2 && sameas( margv[1], channotestring ) )
		{	/* toggle notices of users joining or leaving channel on or off */
		struct userdata notdat;		/* save new setting */

		channotice[usrnum]^=1;
		dfaSetBlk(userglobdata);
		if ( dfaAcqEQ(&notdat, usaptr->userid, 0) )
			{
			cvtDataIP(&notdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
			notdat.channelnotice=channotice[usrnum];
			cvtDataIP(&notdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
			dfaUpdate(&notdat);
			}
		dfaRstBlk();
		prfmsg(NOTSET, (channotice[usrnum] == 1) ? "ON" : "OFF");
		doprintoutput();
		return(1);
		}
	if ( margc > 1 && sameas( margv[1], clearstring ) )
		{       /* clear global chat channel topic(s) */
		INT lowclear;           /* low channel number of range to clear */
		INT highclear;          /* high channel number of range to clear */

		if ( !haskey(chanclearkey) )
			{       /* don't have access to this command */
			cantuse();
			return(1);
			}
		if ( margc == 2 )
			{       /* clear all chat channel topics */
			rstmbk();
			poll[usrnum].start_chan=1;
			poll[usrnum].end_chan=numchatchannels-1;
			poll[usrnum].cur_chan=1;
			poll[usrnum].action=CLEAR_CHAT;
			begin_polling(usrnum, poll_routine);
			return(-1);
			}
		if ( margc == 3 )
			{       /* clear a specific channel number topic */
			lowclear=atoi(margv[2]);        /* channel number to clear */
			if ( ( lowclear < 1 ) || ( lowclear > (numchatchannels-1) ) )
				{       /* bogus channel number entered */
				chanselecterror();
				return(1);
				}
			dfaSetBlk(globchatdata);
			if ( !sameas( topics[lowclear].chantopic, "" ) )
				{       /* clear the topic from memory and disk */
				setmem( topics[lowclear].chantopic, CHATTOPSIZ, 0 );
				dfaAcqEQ(NULL, &lowclear, 0);
				dfaDelete();
				}
			dfaRstBlk();
			prfmsg(ONECLRD, lowclear);
			doprintoutput();
			if ( topchginaudit == 1 )
				{
				shocst(spr("CHAT CHANNEL %d TOPIC CLEARED", lowclear),
				       "User-ID : %s", usaptr->userid);
				}
			return(1);
			}
		if ( margc == 4 )
			{       /* clear a range of channel topics */
			lowclear=atoi(margv[2]);
			highclear=atoi(margv[3]);
			if ( ( lowclear < 1 ) || ( lowclear > (numchatchannels-1) ) )
				{       /* bogus channel number entered */
				chanselecterror();
				return(1);
				}
			if ( ( highclear < 1 ) || ( highclear > (numchatchannels-1) ) )
				{       /* bogus channel number entered */
				chanselecterror();
				return(1);
				}
			if ( highclear < lowclear )
				{       /* exchange channel numbers since they are mixed up */
				highclear^=lowclear;
				lowclear^=highclear;
				highclear^=lowclear;
				}
			rstmbk();
			poll[usrnum].start_chan=lowclear;
			poll[usrnum].end_chan=highclear;
			poll[usrnum].cur_chan=lowclear;
			poll[usrnum].action=CLEAR_CHAT;
			begin_polling(usrnum, poll_routine);
			return(-1);
			}
		}
	if ( margc >= 2 && sameas( margv[1], topicstring ) )
		{       /* set channel topic channels 1 to numchatchannels - 1 */
		CHAR newtopic[INPSIZ];          /* new topic to set channel to */
		struct topicdata newtopicdata;  /* new topic data to save to disk */
		INT topicnumber;                /* set to globalchatchannel[usrnum], specifically for BTRIEVE key usage */

		if ( userinvis() && !haskey(chatinviskey) )
			{       /* can't do this while invisible */
			prfmsg(URINVIS);
			doprintoutput();
			return(1);
			}
		topicnumber=globalchatchannel[usrnum];
		if ( ( topicnumber < 1 ) || ( topicnumber >= numchatchannels ) )
			{
			prfmsg(TPCNMSG, numchatchannels-1);
			doprintoutput();
			return(1);
			}
		if ( margc == 2 )
			{       /* clear channel topic */
			if ( !sameas( topics[topicnumber].chantopic, "" ) )
				{       /* clear disk topic if needed */
				setmem( topics[topicnumber].chantopic, CHATTOPSIZ, 0 );
				dfaSetBlk(globchatdata);
				dfaAcqEQ(NULL, &topicnumber, 0);
				dfaDelete();
				dfaRstBlk();
				}
			prfmsg(TPCLMSG, topicnumber);
			if ( topchginaudit == 1 )
				{       /* display in audit trail */
				shocst(spr("CHANNEL %d TOPIC CLEARED", topicnumber),
				       spr("%s", usaptr->userid));
				}
			}
		else
			{
			getstring( newtopic, 2, (margc-1) );
			if ( strlen(newtopic) > CHATTOPSIZ-1 )
				{
				prfmsg(TPBIGMSG, (INT)CHATTOPSIZ-1);
				doprintoutput();
				return(1);
				}
			strcpy( topics[topicnumber].chantopic, newtopic );
			dfaSetBlk(globchatdata);
			if ( dfaAcqEQ(&newtopicdata, &topicnumber, 0) )
				{       /* update existing topic on disk */
				strcpy( newtopicdata.chantopic, newtopic );
				dfaUpdate(&newtopicdata);
				}
			else
				{       /* store new topic to disk */
				newtopicdata.channelnumber=(USHORT)topicnumber;
				strcpy( newtopicdata.chantopic, newtopic );
				dfaInsert(&newtopicdata);
				}
			dfaRstBlk();
			prfmsg(NWTPMSG, topicnumber, topics[topicnumber].chantopic);
			if ( topchginaudit == 1 )
				{       /* display in audit trail */
				shocst(spr("CHANNEL %d TOPIC CHANGED", topicnumber),
				       spr("%s : %s", usaptr->userid, topics[topicnumber].chantopic));
				}
			}
		outprf(usrnum);
		for ( othusn=0, othusp=usroff(othusn); othusn < nterms; othusn++, othusp=usroff(othusn) )
			{       /* show each user who changed topic */
			setuserpointers();
			if ( isauser() && ( othusn != usrnum ) && canbeseen() )
				{
				if ( ( globalchatchannel[othusn] == topicnumber ) && oktotell() )
					{
					if ( sameas( topics[topicnumber].chantopic, "" ) )
						{
						if ( dobeeps )
							{
							beep(1);
							}
						prfmsg(CLRTPMSG, topicnumber, usaptr->userid);
						}
					else
						{
						if ( dobeeps )
							{
							beep(1);
							}
						prfmsg(TPNWMSG, topicnumber, topics[topicnumber].chantopic, usaptr->userid);
						}
					injoth();
					}
				}
			}
		rstmbk();
		return(1);
		}
	if ( margc == 2 && sameas( margv[1], displaystring ) )
		{       /* display each user's chatchannel for Sysops */
		if ( !haskey(displaykey) )
			{
			cantuse();
			return(1);
			}
		prfmsg(CHANDIS);
		for ( othusn=0, othusp=usroff(othusn); othusn < nterms; othusn++, othusp=usroff(othusn) )
			{       /* show each user's chat channel and topic */
			setuserpointers();
			if ( isauser() && cansee() )
				{
				prfmsg(ALLCNMSG, othuap->userid, (globalchatchannel[othusn] == 0) ? "OFF" : spr("%d", globalchatchannel[othusn]), topics[globalchatchannel[othusn]].chantopic);
				}
			}
		doprintoutput();
		return(1);
		}
	if ( margc > 1 )
		{       /* display margv[1] to margv[margc-1] on chat channel */
		CHAR chatmessage[INPSIZ];       /* the message to broadcast */

		if ( userinvis() && !haskey(chatinviskey) )
			{       /* can't do this while invisible */
			prfmsg(URINVIS);
			doprintoutput();
			return(1);
			}
		if ( globalchatchannel[usrnum] == 0 )
			{       /* global chat is off */
			prfmsg(CNZMSG);
			doprintoutput();
			return(1);
			}
		getstring( chatmessage, 1, (margc-1) );
		for ( othusn=0, othusp=usroff(othusn); othusn < nterms; othusn++, othusp=usroff(othusn) )
			{       /* send out chat to users on channel */
			setuserpointers();
			if ( isauser() )
				{
				if ( ( globalchatchannel[othusn] == globalchatchannel[usrnum] ) && ( ( chatecho[usrnum] ) || ( othusn != usrnum ) ) && oktotell() && canbeseen() )
					{       /* both on same chat channel; don't show to usrnum */
					prfmsg(CHATOUT, globalchatchannel[usrnum], usaptr->userid);
					prfmsg(CTMSG, chatmessage);
					injoth();
					}
				}
			}
		rstmbk();
		return(1);
		}
	rstmbk();
	return(0);
	}

VOID transfer_credits( VOID )				/* transfer credits handler */
	{
	CHAR transferid[INPSIZ];/* user id to transfer credits to */
	LONG trancreds;         /* credits to transfer to account */
	struct usracc crd;      /* user account to transfer credits to (on disk) */
	struct usracc usrcrd;	/* user account to transfer credits from (on disk) */
	struct usracc *crdptr;  /* pointer to user account to transfer credits to */
	struct telegram notgm;  /* send telegram to user if not online */
	INT sendok;             /* 0 if notify by telegram, otherwise 1 */
	INT online;		/* 1 if user transferred to is online, 0 if not */

	INIT_STRUCT_TO_ZERO(crd);
	INIT_STRUCT_TO_ZERO(usrcrd);
	INIT_STRUCT_TO_ZERO(notgm);

	setmbk(glomb);
	if ( !haskey(help[30].key) )           /* check that user may use this global */
		{
		cantuse();
		return;
		}
	if ( margc < 3 )
		{       /* need command, userid and amount */
		prfmsg(TRNHLP, help[30].command, (haskey(unlimtrankey)) ? spr("%d", 1) : l2as(minimumtransfer), l2as(usaptr->creds));
		doprintoutput();
		return;
		}
	if ( !numok(margv[margc-1], 0) || strlen( margv[margc-1] ) > TRANCRDDIG )
		{       /* number too large or bogus input */
		prfmsg(BADTRN);
		doprintoutput();
		return;
		}
	trancreds=atol(margv[margc-1]);
	if ( trancreds == 0L )
		{       /* can't do this */
		prfmsg(NOZEROT);
		doprintoutput();
		return;
		}
	if ( ( trancreds < minimumtransfer ) && !haskey(unlimtrankey) )
		{       /* can't do this either */
		prfmsg(MINTRN, l2as(minimumtransfer));
		doprintoutput();
		return;
		}
	if ( trancreds > usaptr->creds )
		{       /* can't transfer more credits than you have */
		prfmsg(NOTENUF, l2as(usaptr->creds));
		doprintoutput();
		return;
		}
	getuserid( transferid, 1, (margc-2) );
	crdptr=&crd;
	dfaSetBlk(accbb);
	if ( !dfaAcqEQ( crdptr, transferid, 0 ) )
		{
		prfmsg(NOSCHGUY, transferid);
		doprintoutput();
		dfaRstBlk();
		return;
		}
	if ( taggedforkill(crdptr) )
		{       /* if tagged for delete don't transfer credits */
		prfmsg(PSTDLT, crdptr->userid);
		doprintoutput();
		dfaRstBlk();
		return;
		}
	if ( !uhsmkey(crdptr->userid, TRCRKEY) && !haskey(unlimtrankey) )
		{	/* user to receive transfer lacks key to receive */
		prfmsg(CANTRCV, crdptr->userid);
		doprintoutput();
		dfaRstBlk();
		return;
		}
	if ( trancreds > (LONG_MAX - crdptr->creds) )
		{       /* check for possible credit overflow */
		prfmsg(CROVFLW, crdptr->userid, l2as(LONG_MAX - crdptr->creds));
		doprintoutput();
		dfaRstBlk();
		return;
		}
	crdptr->creds+=trancreds;
	dfaUpdate(crdptr);
	usaptr->creds-=trancreds;
	if ( dfaAcqEQ(&usrcrd, usaptr->userid, 0) )
		{	/* update disk data immediately */
		usrcrd.creds-=trancreds;
		dfaUpdate(&usrcrd);
		}
	dfaRstBlk();
	prfmsg(TDNMSG, l2as(trancreds), crdptr->userid);
	outprf(usrnum);
	if ( crdptr->creds > 0L && (clsptr=fndcls(crdptr->curcls)) != NULL )
		{       /* if needed switch user's class */
		if ( clsptr->flags&HASCRD )
			{
			swtcls(crdptr, sameas(crdptr->curcls, crdptr->prmcls),
			       clsptr->nxtcls[DCREDIT],1,0);
			}
		}
	online=onsysn(crdptr->userid, 1);
	if ( online )
		{	/* update online data */
		othuap->creds+=trancreds;
		}
	sendok=( online && oktotell() && canbeseen() );
	if ( sendok )
		{       /* update online data and try to send injoth() */
		if ( dobeeps )
			{
			beep(1);
			}
		prfmsg(TRNINJ, usaptr->userid, l2as( trancreds ));
		sendok=injoth();
		}
	if ( !sendok )
		{       /* inform by telegram */
		dfaSetBlk(telegramdata);
		preparetelegram(&notgm, crdptr->userid);
		strcpy( notgm.telegramtext, spr(stgopt(TRNNOTE), l2as( trancreds )) );
		dfaInsert(&notgm);
		dfaRstBlk();
		if ( notifyoftelegram(crdptr->userid) )
			{       /* go ahead and let him know */
			telltelegram();
			}
		}
	shocst(spr("%s CREDITS TRANSFERRED", l2as(trancreds)),
		   "%s to %s", usaptr->userid, crdptr->userid);
	rstmbk();
	}

VOID transfer_days( VOID )				/* transfer days in class handler */
	{
	CHAR transferid[INPSIZ];/* user id to transfer days to */
	INT trandays;           /* days to transfer to account */
	struct usracc dayacc;   /* user account to transfer days to (on disk) */
	struct usracc usrday;	/* user account to transfer days from (on disk) */
	struct usracc *dayptr;  /* pointer to user account to transfer days to */
	struct telegram notgm;  /* send telegram to user if not online */
	INT sendok;             /* 0 if notify by telegram, otherwise 1 */
	INT online;		/* 1 if user transferred to is online, 0 if not */

	INIT_STRUCT_TO_ZERO(dayacc);
	INIT_STRUCT_TO_ZERO(usrday);
	INIT_STRUCT_TO_ZERO(notgm);

	setmbk(glomb);
	if ( !haskey(help[31].key) )		/* check that user may use this global */
		{
		cantuse();
		return;
		}
	if ( margc < 3 )
		{	/* need command, userid and amount */
		prfmsg(TDYHLP, help[31].command, (haskey(nomindaykey)) ? 1 : minimumtrandays, (INT)MAXDAYSPOST); /* print transfer days help */
		doprintoutput();
		return;
		}
	if ( !usesdays(usaptr) )
		{	/* user's class doesn't use days in class, or can't find class data */
		prfmsg(NOUSEDAY, strupr(usaptr->curcls));
		doprintoutput();
		return;
		}
	if ( !numok(margv[margc-1], 0) || strlen(margv[margc-1]) > TRANCRDDIG )
		{	/* number too large or bogus input */
		prfmsg(BADTDY, (haskey(nomindaykey)) ? 1 : minimumtrandays, MAXDAYSPOST);
		doprintoutput();
		return;
		}
	trandays=atoi(margv[margc-1]);
	if ( !trandays || ( ( trandays < minimumtrandays ) && !haskey(nomindaykey) ) || ( trandays > MAXDAYSPOST ) )
		{	/* can't do this */
		prfmsg(BADTDY, (haskey(nomindaykey)) ? 1 : minimumtrandays, MAXDAYSPOST);
		doprintoutput();
		return;
		}
	if ( usaptr->daystt <= trandays )
		{	/* can't give away all days or more than we have */
		prfmsg(TOOMANY, usaptr->daystt);
		doprintoutput();
		return;
		}
	getuserid( transferid, 1, (margc-2) );
	dayptr=&dayacc;
	dfaSetBlk(accbb);
	if ( !dfaAcqEQ(dayptr, transferid, 0) )
		{
		prfmsg(NOSCHGUY, transferid);
		doprintoutput();
		dfaRstBlk();
		return;
		}
	if ( !sameas(usaptr->curcls, dayptr->curcls) )
		{	/* different classes, can't do this */
		prfmsg(DIFFCLS, dayptr->userid, strupr(dayptr->curcls));
		doprintoutput();
		dfaRstBlk();
		return;
		}
	if ( taggedforkill(dayptr) )
		{	/* if tagged for delete don't transfer days */
		prfmsg(PSTDLT, dayptr->userid);
		doprintoutput();
		dfaRstBlk();
		return;
		}
	dayptr->daystt+=(USHORT)trandays;
	dfaUpdate(dayptr);
	usaptr->daystt-=(USHORT)trandays;
	if ( dfaAcqEQ(&usrday, usaptr->userid, 0) )
		{	/* update disk data immediately */
		usrday.daystt-=(USHORT)trandays;
		dfaUpdate(&usrday);
		}
	dfaRstBlk();
	prfmsg(TDYMSG, trandays, dayptr->userid);
	outprf(usrnum);
	online=onsysn(dayptr->userid, 1);
	if ( online )
		{	/* update online data */
		othuap->daystt+=(USHORT)trandays;
		}
	sendok=( online && oktotell() && canbeseen() );
	if ( sendok )
		{       /* update online data and try to send injoth() */
		if ( dobeeps )
			{
			beep(1);
			}
		prfmsg(TDYINJ, usaptr->userid, trandays);
		sendok=injoth();
		}
	if ( !sendok )
		{       /* inform by telegram */
		dfaSetBlk(telegramdata);
		preparetelegram(&notgm, dayptr->userid);
		strcpy( notgm.telegramtext, spr(stgopt(TDYNOTE), trandays) );
		dfaInsert(&notgm);
		dfaRstBlk();
		if ( notifyoftelegram(dayptr->userid) )
			{       /* go ahead and let him know */
			telltelegram();
			}
		}
	shocst(spr("%d DAYS TRANSFERRED", trandays),
		   "%s to %s", usaptr->userid, dayptr->userid);
	rstmbk();
	}

VOID extend_time( VOID )				/* extend time online of user */
	{
	INT minutes;                     /* minutes to extend on-line time */
	struct clstab *tabptr;           /* user's class data */
	INT minthiscall;        	 /* minutes left in this call */
	INT mintoday;           	 /* minutes left today */
	INT calltimeadjusted=1000;	 /* minutes this call adjusted by (1000 means no adjustment made) */
	INT daytimeadjusted=1000;	 /* minutes time today adjusted by (1000 means no adjustment made) */
	struct usracc timeacc;		 /* user account info of user to extend */
	CHAR usertoextend[INPSIZ];	 /* userid of user to extend time on-line */
	INT online;			 /* 1 if user to extend is on-line, 0 if not */
	INT sendok=0;             	 /* 0 if send telegram to notify, otherwise 1; default to 0 */
	struct telegram notgm;  	 /* send telegram to user */
	GBOOL inform;			 /* TRUE if inform user of time extension, FALSE if don't */
	INT time_offset;		 /* margv[time_offset] == time to extend */
	INT name_offset;		 /* User-ID of user to extend time is from margv[1] to margv[name_offset] */

	setmbk(glomb);
	if ( !haskey(help[32].key) )           /* check that user may use this global */
		{
		cantuse();
		return;
		}
	inform=sameas(margv[margc-1], "+");
	if ( inform )
		{
		time_offset=margc-2;
		name_offset=margc-3;
		}
	else
		{
		time_offset=margc-1;
		name_offset=margc-2;
		}
	if ( (inform && (margc < 4)) || (margc < 3) )
		{       /* need command, userid and minutes, optional inform */
		prfmsg(TEXTHLP, (INT)MINTIMEEXT, (INT)MAXTIMEEXT, help[32].command);
		doprintoutput();
		return;
		}
	if ( !numok(margv[time_offset], 1) || strlen(margv[time_offset]) > MAXTIMEDIGITS )
		{       /* number too large or bogus input */
		prfmsg(BADTIME, (INT)MINTIMEEXT, (INT)MAXTIMEEXT);
		doprintoutput();
		return;
		}
	minutes=atoi(margv[time_offset]);
	if ( minutes == 0 )
		{	/* obviously a joker here */
		prfmsg(NOZEROM);
		doprintoutput();
		return;
		}
	if ( ( minutes > MAXTIMEEXT ) || ( minutes < MINTIMEEXT ) )
		{       /* make sure integer is valid */
		prfmsg(BADTIME, (INT)MINTIMEEXT, (INT)MAXTIMEEXT);
		doprintoutput();
		return;
		}
	getuserid(usertoextend, 1, name_offset);
	dfaSetBlk(accbb);
	if ( !dfaAcqEQ(&timeacc, usertoextend, 0) )
		{	/* no such user */
		prfmsg(NOSCHGUY, usertoextend);
		doprintoutput();
		dfaRstBlk();
		return;
		}
	online=( onsysn(timeacc.userid, 1) && cansee() );
	if ( (tabptr=fndcls(timeacc.curcls)) == NULL )
		{       /* class is not in table */
		if ( online )
			{	/* can use this info if we have to */
			tabptr=othusp->cltptr;
			}
		else
			{
			prfmsg(NOCLSDAT, strupr(timeacc.curcls));
			doprintoutput();
			dfaRstBlk();
			return;
			}
		}
	if ( online )
		{	/* check for minutes per call */
		minthiscall=calllimit( tabptr, othusp );
		if ( minthiscall != -1 )
			{	/* limit for call, adjust time */
			calltimeadjusted=adjustcalltime( othusp, tabptr, (othusp->minut4>>2), minutes );
			}
		}
	mintoday=daylimit( tabptr, &timeacc );
	if ( mintoday != -1 )
		{	/* limit for day, adjust time */
		daytimeadjusted=adjustdaytime( &timeacc, tabptr, (INT)(timeacc.timtdy/60), minutes, online );
		}
	if ( ( calltimeadjusted == 1000 ) && ( daytimeadjusted == 1000 ) )
		{	/* user could not have time extended */
		prfmsg(NOTMCHG, timeacc.userid);
		doprintoutput();
		dfaRstBlk();
		return;
		}
	dfaUpdate(&timeacc);
	dfaRstBlk();
	if ( online && ( calltimeadjusted != 1000 ) )
		{	/* tell about current call time */
		prfmsg(CALLTM, timeacc.userid, (calltimeadjusted < 0) ? "shortened" : "extended", abs(calltimeadjusted));
		outprf(usrnum);
		}
	if ( daytimeadjusted != 1000 )
		{
		prfmsg(DAYTM, timeacc.userid, (daytimeadjusted < 0) ? "shortened" : "extended", abs(daytimeadjusted));
		outprf(usrnum);
		}
	if ( inform && online && oktotell() && canbeseen() )
		{       /* try to send injoth() */
		if ( dobeeps )
			{
			beep(1);
			}
		if ( calltimeadjusted != 1000 )
			{
			prfmsg(CALLINJ, usaptr->userid, (calltimeadjusted < 0) ? "shortened" : "extended", abs(calltimeadjusted));
			}
		if ( daytimeadjusted != 1000 )
			{
			prfmsg(DAYINJ, usaptr->userid, (daytimeadjusted < 0) ? "shortened" : "extended", abs(daytimeadjusted));
			}
		sendok=injoth();
		}
	if ( inform && !sendok )
		{       /* inform by telegram */
		dfaSetBlk(telegramdata);
		preparetelegram(&notgm, timeacc.userid);
		strcpy( notgm.telegramtext, spr(stgopt(TEXTNOTE), (calltimeadjusted < 0) ? "shortened" : "extended", (calltimeadjusted == 1000) ? 0 : calltimeadjusted, (daytimeadjusted < 0) ? "shortened" : "extended", (daytimeadjusted == 1000) ? 0 : daytimeadjusted));
		dfaInsert(&notgm);
		dfaRstBlk();
		if ( notifyoftelegram(timeacc.userid) )
			{       /* go ahead and let him know */
			telltelegram();
			}
		}
	rstmbk();
	shocst("USER TIME EXTENDED",
	       "%s by %s", timeacc.userid, usaptr->userid);
	}

VOID post_days( VOID )					/* post days in class to a user */
	{
	CHAR postid[INPSIZ];    /* user id to post days to */
	INT daysposted;         /* days to post to account */
	struct usracc post;     /* user account to post days to (on disk) */
	struct usracc *postptr; /* pointer to user account to post days to */
	struct telegram notgm;  /* send telegram to user */
	INT sendok;             /* 0 if send telegram to notify, otherwise 1 */
	INT usersdays;          /* current days in class of user */
	INT online;		/* 1 if user posted to is online, 0 if not */
	GBOOL inform;		/* TRUE if tell user of posting, FALSE if not */
	INT day_offset;		/* margv[day_offset] is days to post */
	INT uid_offset;		/* margv[1] to margv[uid_offset] is user to post to */

	INIT_STRUCT_TO_ZERO(post);
	INIT_STRUCT_TO_ZERO(notgm);

	setmbk(glomb);
	if ( !haskey(help[21].key) )           /* check that user may use this global */
		{
		cantuse();
		return;
		}
	inform=sameas(margv[margc-1], "+");
	if ( (inform && (margc < 4)) || (margc < 3) )
		{       /* need command, userid and number */
		prfmsg(PDYHLP, (INT)MINDAYSPOST, (INT)MAXDAYSPOST, help[21].command);
		doprintoutput();
		return;
		}
	if ( inform )
		{
		day_offset=margc-2;
		uid_offset=margc-3;
		}
	else
		{
		day_offset=margc-1;
		uid_offset=margc-2;
		}
	if ( !numok(margv[day_offset], 1) || strlen(margv[day_offset]) > MAXPOSTDIGITS )
		{       /* number too large or bogus input */
		prfmsg(BADPDY, (INT)MINDAYSPOST, (INT)MAXDAYSPOST);
		doprintoutput();
		return;
		}
	daysposted=atoi(margv[day_offset]);
	if ( daysposted == 0 )
		{	/* obviously a joker here */
		prfmsg(NOZEROD);
		doprintoutput();
		return;
		}
	if ( ( daysposted > MAXDAYSPOST ) || ( daysposted < MINDAYSPOST ) )
		{       /* make sure integer is valid */
		prfmsg(BADPDY, (INT)MINDAYSPOST, (INT)MAXDAYSPOST);
		doprintoutput();
		return;
		}
	getuserid(postid, 1, uid_offset);
	postptr=&post;
	dfaSetBlk(accbb);
	if ( !dfaAcqEQ( postptr, postid, 0 ) )
		{
		prfmsg(NOSCHGUY, postid);
		doprintoutput();
		dfaRstBlk();
		return;
		}
	if ( !usesdays(postptr) )
		{	/* this guy doesn't use days in class */
		prfmsg(NOUSEDY, postptr->userid, postptr->curcls);
		doprintoutput();
		dfaRstBlk();
		return;
		}
	usersdays=postptr->daystt;
	if ( usersdays == 0 )
		{       /* user already has unlimited days in class */
		prfmsg(DYSUNLM, postptr->userid);
		doprintoutput();
		dfaRstBlk();
		return;
		}
	if ( (double)( usersdays + daysposted ) > UINT_MAX )
		{       /* can have only UINT_MAX days in class */
		prfmsg(DYOFLW, postptr->userid, (USHORT)(UINT_MAX-usersdays));
		doprintoutput();
		dfaRstBlk();
		return;
		}
	if ( taggedforkill(postptr) )
		{       /* if tagged for delete don't post credits */
		prfmsg(PSTDLT, postptr->userid);
		doprintoutput();
		dfaRstBlk();
		return;
		}
	if ( ( usersdays + daysposted ) <= 0 )
		{       /* can't have negative or zero days in class */
		prfmsg(NONEGZ, postptr->userid, postptr->daystt);
		doprintoutput();
		dfaRstBlk();
		return;
		}
	else
		{
		postptr->daystt+=(USHORT)daysposted;
		prfmsg(PDNMSG, postptr->userid, daysposted);
		outprf(usrnum);
		}
	dfaUpdate(postptr);
	dfaRstBlk();
	online=onsysn(postptr->userid, 1);
	if ( online )
		{	/* update on-line data */
		othuap->daystt+=(USHORT)daysposted;
		}
	sendok=( inform && online && oktotell() && canbeseen() );
	if ( sendok )
		{	/* try to send injoth() */
		if ( dobeeps )
			{
			beep(1);
			}
		prfmsg(PDYINJ, usaptr->userid, daysposted);
		sendok=injoth();
		}
	if ( inform && !sendok )
		{       /* inform by telegram */
		dfaSetBlk(telegramdata);
		preparetelegram(&notgm, postptr->userid);
		strcpy( notgm.telegramtext, spr(stgopt(PDYNOTE), daysposted) );
		dfaInsert(&notgm);
		dfaRstBlk();
		if ( notifyoftelegram(postptr->userid) )
			{       /* go ahead and let him know */
			telltelegram();
			}
		}
	shocst(spr("%d DAYS IN CLASS POSTED", daysposted),
		   "%s to %s", usaptr->userid, postptr->userid);
	rstmbk();
	}

VOID switch_class( VOID )				/* switch a user's class */
	{
	CHAR switchid[INPSIZ];          /* user-id to switch class */
	CHAR startclass[KEYSIZ];        /* user's class at start */
	INT online;                     /* 1 if user-id is on-line, 0 if not */
	struct usracc swccls;           /* user account to post credits to (on disk) */
	struct usracc *swcptr;          /* pointer to user account to post credits to */
	struct telegram notgm;          /* notify user of switch by telegram if needed */
	INT sendok;                     /* 0 if notify by telegram, otherwise 1 */
	INT classmarg;			/* class name to switch to is margv[classmarg] */
	INT useridarg;			/* userid to switch is margv[1] to margv[useridarg] */
	INT daystoswitch;		/* days in class to be switched for (0 for default) */
	GBOOL inform;			/* TRUE if inform user of class switch, FALSe if don't */
	INT day_offset;			/* margv[day_offset] is days in class to give */

	INIT_STRUCT_TO_ZERO(swccls);
	INIT_STRUCT_TO_ZERO(notgm);

	setmbk(glomb);
	if ( !haskey(help[27].key) )           /* check that user may use this global */
		{
		cantuse();
		return;
		}
	inform=sameas(margv[margc-1], "+");
	if ( inform )
		{
		day_offset=margc-2;
		}
	else
		{
		day_offset=margc-1;
		}
	if ( numok(margv[day_offset], 0) && strlen(margv[day_offset]) <= MAXPOSTDIGITS )
		{       /* number, so days to switch for are given */
		if ( inform )
			{
			classmarg=margc-3;
			useridarg=margc-4;
			}
		else
			{
			classmarg=margc-2;	/* class name is margv[margc-2] */
			useridarg=margc-3;
			}
		daystoswitch=atoi(margv[day_offset]);
		if ( ( daystoswitch < 1 ) || ( daystoswitch > (INT)MAXDAYSPOST ) )
			{	/* user wants to switch for some days, but bad number */
			prfmsg(BADDYNM, (INT)MAXDAYSPOST);
			doprintoutput();
			return;
			}
		}
	else
		{
		if ( inform )
			{
			classmarg=margc-2;
			useridarg=margc-3;
			}
		else
			{
			classmarg=margc-1;
			useridarg=margc-2;
			}
		daystoswitch=0;
		}
	if ( ( (daystoswitch && inform) && (margc < 5) ) || (daystoswitch && (margc < 4)) || (inform && (margc < 4)) || ( margc < 3 ) )
		{       /* need command, userid and class, possibly days and whether to inform user */
		prfmsg(SWCHLP, help[27].command, (INT)MAXDAYSPOST);
		doprintoutput();
		return;
		}
	if ( !fndcls(margv[classmarg]) )
		{       /* bogus class name given */
		prfmsg(BDCLNM, strupr(margv[classmarg]));
		doprintoutput();
		return;
		}
	getuserid(switchid, 1, useridarg);
	online=onsysn( switchid, 1 );
	if ( online )       /* online user, so modify account in memory */
		{
		swcptr=othuap;
		}
	else
		{
		swcptr=&swccls;
		dfaSetBlk(accbb);
		if ( !dfaAcqEQ( swcptr, switchid, 0 ) )
			{
			prfmsg(NOSCHGUY, switchid);
			doprintoutput();
			dfaRstBlk();
			return;
			}
		dfaRstBlk();
		}
	strcpy( startclass, swcptr->curcls );
	swtcls( swcptr, 1, margv[classmarg], 1, daystoswitch );
	if ( sameas( startclass, swcptr->curcls ) )
		{       /* no change made to class */
		prfmsg(CLSNTCH, swcptr->userid, swcptr->curcls);
		doprintoutput();
		return;
		}
	sendok=( inform && onsysn(swcptr->userid, 1) && oktotell() && canbeseen() );
	if ( sendok )
		{       /* try to send injoth() */
		if ( dobeeps )
			{
			beep(1);
			}
		prfmsg(CLSINJ, usaptr->userid, swcptr->curcls, usesdays(swcptr) ? spr("%d", swcptr->daystt) : "UNLIMITED");
		sendok=injoth();
		}
	if ( inform && !sendok )
		{       /* inform by telegram */
		dfaSetBlk(telegramdata);
		preparetelegram(&notgm, swcptr->userid);
		strcpy( notgm.telegramtext, spr(stgopt(CLSNOTE), swcptr->curcls, usesdays(swcptr) ? spr("%d", swcptr->daystt) : "UNLIMITED") );
		dfaInsert(&notgm);
		dfaRstBlk();
		if ( notifyoftelegram(swcptr->userid) )
			{       /* go ahead and let him know */
			telltelegram();
			}
		}
	shocst(spr("CLASS SWITCH:%s", swcptr->curcls),
	       "%s by %s", swcptr->userid, usaptr->userid);
	if ( !sameas( swcptr->curcls, margv[classmarg] ) )
		{	/* different class than requested */
		prfmsg(SWCDIFF, swcptr->userid, swcptr->curcls, usesdays(swcptr) ? spr("%d", swcptr->daystt) : "UNLIMITED");
		}
	else
		{
		prfmsg(SWCDUN, swcptr->userid, swcptr->curcls, usesdays(swcptr) ? spr("%d", swcptr->daystt) : "UNLIMITED");
		}
	doprintoutput();
	}

VOID key_handler( VOID )				/* give/take user key */
	{
	CHAR keyid[INPSIZ];             /* user-id to give/take key */
	INT online;                     /* 1 if keyid is online, 0 if not */
	struct usracc uakey;            /* user account of user to give/take key */
	struct usracc *ukptr;           /* pointer to user account */
	CHAR userskeys[RINGSZ];         /* buffer of user's current keys */
	CHAR classkeys[RINGSZ];         /* buffer of keys in user's class */
	CHAR keystg[KEYSIZ+1];          /* name to use to get keys in user's class */
	struct telegram notgm;          /* telegram message to user getting/losing key */
	INT sendok;                     /* 0 if send telegram to notify, otherwise 0 */
	INT addkey;                     /* 0 if removed key, 1 if added key */
	CHAR keyname[INPSIZ];		/* name of key to give or take */
	struct notkey keydat;		/* data for giving or taking a negative key */
	INT found;			/* 1 if found negative key in database, 0 if not */
	INT removed=0;			/* 1 if user has key, 0 if not */
	GBOOL inform;			/* TRUE if tell user, FALSe if don't tell */
	INT key_offset;			/* margv[key_offset] is key to give/take */
	INT uid_offset;			/* user to receive/lose key is margv[1] to margv[uid_offset] */

	INIT_STRUCT_TO_ZERO(uakey);
	INIT_STRUCT_TO_ZERO(notgm);
	INIT_STRUCT_TO_ZERO(keydat);

	setmbk(glomb);
	if ( !haskey(help[33].key) )           /* check that user may use this global */
		{
		cantuse();
		return;
		}
	inform=sameas(margv[margc-1], "+");
	if ( inform )
		{
		key_offset=margc-2;
		uid_offset=margc-3;
		}
	else
		{
		key_offset=margc-1;
		uid_offset=margc-2;
		}
	if ( (inform && (margc < 4)) || margc < 3 )
		{
		prfmsg(UKHLP, help[33].command, help[33].command);
		doprintoutput();
		return;
		}
	if ( !keynam(margv[key_offset]) && !sameas(margv[key_offset], "?") )
		{       /* bogus key name */
		prfmsg(UKNBAD, margv[key_offset]);
		doprintoutput();
		return;
		}
	getuserid(keyid, 1, (uid_offset));
	online=onsysn( keyid, 1 );
	if ( online )       /* online user, so keyid is ok */
		{
		ukptr=othuap;
		}
	else
		{       /* check for user in account file */
		ukptr=&uakey;
		dfaSetBlk(accbb);
		if ( !dfaAcqEQ( ukptr, keyid, 0 ) )
			{
			prfmsg(NOSCHGUY, keyid);
			doprintoutput();
			dfaRstBlk();
			return;
			}
		dfaRstBlk();
		}
	if ( ukptr->flags&HASMST )
		{       /* Sysop, has every key by default */
		prfmsg(UKSYS, ukptr->userid);
		doprintoutput();
		return;
		}
	setmem( keystg, KEYSIZ+1, 0 );
	keystg[0]=RINGID;
	strcat( keystg, ukptr->curcls );
	setmem( userskeys, RINGSZ, 0 );
	setmem( classkeys, RINGSZ, 0 );
	getlst(ukptr->userid, userskeys);
	getlst(keystg, classkeys);
	if ( sameas( margv[margc-1], "?" ) )
		{       /* list user's current keys */
		prfmsg(CLKMSG, ukptr->userid, ukptr->curcls, classkeys);
		listnotkeys(ukptr->userid);
		if ( !sameas( userskeys, "" ) )
			{
			prfmsg(SKYMSG, userskeys);
			}
		doprintoutput();
		return;
		}
	strcpy(keyname, margv[key_offset]);
	if ( hasnotkey( ukptr->userid, keyname ) )
		{	/* has negative key, so remove */
		dfaSetBlk(notkeydata);
		found=dfaAcqEQ(&keydat, ukptr->userid, 0);
		while ( found && !removed )
			{
			if ( sameas( keydat.keyname, keyname ) )
				{
				removed=1;
				dfaDelete();
				}
			found=dfaAcqNX(&keydat);
			}
		dfaRstBlk();
		updateonline(ukptr->userid, keyname, 1);
		addkey=1;
		}
	else
		if ( findkey(keyname, classkeys, 0) )
			{	/* give negative key */
			dfaSetBlk(notkeydata);
			strcpy(keydat.keyid, ukptr->userid);
			strcpy(keydat.keyname, keyname);
			dfaInsert(&keydat);
			dfaRstBlk();
			updateonline(ukptr->userid, keyname, 0);
			addkey=0;
			}
		else
			{	/* check user's personal key data */
			if ( findkey(keyname, userskeys, 0) )
				{       /* has key, so remove it */
				rmvkey(ukptr->userid, keyname);
				addkey=0;
				}
			else
				{       /* doesn't have key, so give it */
				givkey(ukptr->userid, keyname);
				addkey=1;
				}
			}
	sendok=( inform && onsysn(ukptr->userid, 1) && oktotell() && canbeseen() );
	if ( sendok )
		{       /* try to send injoth() */
		if ( dobeeps )
			{
			beep(1);
			}
		if ( addkey )
			{
			prfmsg(GKYINJ, usaptr->userid, strupr(keyname));
			}
		else
			{
			prfmsg(RKYINJ, usaptr->userid, strupr(keyname));
			}
		sendok=injoth();
		}
	if ( inform && !sendok )
		{       /* inform by telegram */
		dfaSetBlk(telegramdata);
		preparetelegram(&notgm, ukptr->userid);
		if ( addkey )
			{	/* key given */
			strcpy( notgm.telegramtext, spr(stgopt(GKYNOTE), strupr(keyname)) );
			}
		else
			{	/* key removed */
			strcpy( notgm.telegramtext, spr(stgopt(RKYNOTE), strupr(keyname)) );
			}
		dfaInsert(&notgm);
		dfaRstBlk();
		if ( notifyoftelegram(ukptr->userid) )
			{       /* go ahead and let him know */
			telltelegram();
			}
		}
	if ( addkey )
		{	/* key was given */
		prfmsg(UKADD, ukptr->userid, strupr(keyname));
		outprf(usrnum);
		shocst(spr("%s KEY GIVEN", keyname),
		       "%s by %s", ukptr->userid, usaptr->userid);
		}
	else
		{	/* key was removed */
		prfmsg(UKRMV, strupr(keyname), ukptr->userid);
		outprf(usrnum);
		shocst(spr("%s KEY TAKEN", keyname),
		       "%s by %s", ukptr->userid, usaptr->userid);
		}
	rstmbk();
	}

VOID toggle_invis( VOID )				/* toggle invisibility for a user */
	{
	CHAR invisid[INPSIZ];           /* user to toggle invisible */
	struct userdata invdat;		/* global data of user to toggle invisible */
	struct usracc invacc;		/* account data of user to toggle invisible */
	struct telegram notgm;          /* notify user about change */
	INT sendok;                     /* 0 if send telegram to notify, otherwise 1 */
	INT savenum;			/* save current usrnum while faking log-off message */
	GBOOL inform;			/* TRUE if tell user of toggle, FALSE if don't tell */

	setmbk(glomb);
	if ( !haskey(help[14].key) )           /* check that user may use this global */
		{
		cantuse();
		return;
		}
	if ( margc == 1 )
		{       /* toggle user invisible */
		if ( invisnotify && userinvis() )
			{	/* set user's minutes on-line to 0 */
			usrptr->minut4=0;
			}
		if ( userinvis() )
			{	/* invisible, toggle off */
			toggleinvis( usrptr, usaptr, (INT)OFF);
			}
		else
			{	/* visible, so toggle on */
			toggleinvis( usrptr, usaptr, (INT)ON);
			}
		minuteson[usrnum]=usaptr->timtdy/60;
		dfaSetBlk(accbb);
		if ( dfaAcqEQ(&invacc, usaptr->userid, 0) )
			{
			dfaUpdate(&invacc);
			}
		dfaRstBlk();
		dfaSetBlk(userglobdata);
		if ( dfaAcqEQ(&invdat, usaptr->userid, 0) )
			{	/* this MUST always be true */
			cvtDataIP(&invdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
			invdat.invis^=1;
			cvtDataIP(&invdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
			dfaUpdate(&invdat);
			}
		dfaRstBlk();
		prfmsg(INVTOG, userinvis() ? "ON" : "OFF");
		outprf(usrnum);
		if ( invisnotify )
			{	/* alert other users */
			if ( userinvis() )
				{	/* user toggled invisibility */
				if ( logchannotice )
					{
					dochannotice(LEFTCHAN);
					}
				useralrt(&invdat, (INT)OFF, (INT)FAKE);
				}
			else
				{	/* user is now visible again */
				if ( logchannotice )
					{
					dochannotice(JOINCHAN);
					}
				useralrt(&invdat, (INT)ON, (INT)FAKE);
				}
			}
		rstmbk();
		return;
		}
	else
		{       /* toggle another user invisible */
		inform=sameas(margv[margc-1], "+");
		if ( inform )
			{
			if ( margc < 3 )
				{
				prfmsg(INVHLP, help[14].command, userinvis() ? "ON" : "OFF", help[14].command); /* print invisible help */
				doprintoutput();
				return;
				}
			getuserid(invisid, 1, margc-2);
			}
		else
			{
			getuserid(invisid, 1, margc-1);
			}
		if ( onsysn(invisid, 1) )
			{       /* user is on-line */
			if ( othmaster() && !hasmaster() )
				{	/* non-MASTER key holders can't toggle a MASTER key holder */
				prfmsg(NOTOG, othuap->userid);
				doprintoutput();
				return;
				}
			if ( invisusernotify && otherinvis() )
				{	/* set user's minutes on-line to 0 */
				othusp->minut4=0;
				}
			strcpy( invisid, othuap->userid );
			if ( otherinvis() )
				{	/* invisible, so toggle off */
				toggleinvis( othusp, othuap, (INT)OFF);
				}
			else
				{	/* visible, so toggle on */
				toggleinvis( othusp, othuap, (INT)ON);
				}
			minuteson[othusn]=othuap->timtdy/60;
			dfaSetBlk(accbb);
			if ( dfaAcqEQ(&invacc, invisid, 0) )
				{
				dfaUpdate(&invacc);
				}
			dfaRstBlk();
			dfaSetBlk(userglobdata);
			if ( dfaAcqEQ(&invdat, othuap->userid, 0) )
				{	/* this MUST always be true */
				cvtDataIP(&invdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
				invdat.invis^=1;
				cvtDataIP(&invdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
				dfaUpdate(&invdat);
				}
			dfaRstBlk();
			savenum=usrnum;
			curusr(othusn);
			if ( invisusernotify )
				{	/* alert other users */
				if ( userinvis() )
					{	/* user toggled invisibility */
					if ( logchannotice )
						{
						dochannotice(LEFTCHAN);
						}
					useralrt(&invdat, (INT)OFF, (INT)FAKE);
					}
				else
					{	/* user is now visible again */
					if ( logchannotice )
						{
						dochannotice(JOINCHAN);
						}
					useralrt(&invdat, (INT)ON, (INT)FAKE);
					}
				}
			curusr(savenum);
			}
		else
			{	/* other user is not on-line */
			dfaSetBlk(accbb);
			if ( !dfaAcqEQ(&invacc, invisid, 0) )
				{	/* no such user */
				prfmsg(NOSCHGUY, invisid);
				doprintoutput();
				dfaRstBlk();
				return;
				}
			if ( (invacc.flags&HASMST) && !hasmaster() )
				{	/* non MASTER key holder can't toggle MASTER key holder */
				prfmsg(NOTOG, invacc.userid);
				doprintoutput();
				dfaRstBlk();
				return;
				}
			strcpy( invisid, invacc.userid );
			dfaSetBlk(userglobdata);
			if ( !dfaAcqEQ(&invdat, invisid, 0) )
				{	/* no such user to toggle */
				prfmsg(NODATA, invisid);
				doprintoutput();
				dfaRstBlk();
				dfaRstBlk();
				return;
				}
			cvtDataIP(&invdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
			invdat.invis^=1;
			cvtDataIP(&invdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
			dfaUpdate(&invdat);
			dfaRstBlk();
			invacc.flags^=GOINVB;
			dfaUpdate(&invacc);
			dfaRstBlk();
			}
		}
	prfmsg(INVOTOG, (invdat.invis) ? "ON" : "OFF", invdat.userid);
	outprf(usrnum);
	sendok=( inform && onsysn(invdat.userid, 1) && oktotell() && canbeseen() );
	if ( sendok )
		{       /* try to send injoth() */
		if ( dobeeps )
			{
			beep(1);
			}
		prfmsg(INVINJ, usaptr->userid, (invdat.invis) ? "now" : "not");
		sendok=injoth();
		}
	if ( inform && !sendok )
		{       /* inform by telegram */
		dfaSetBlk(telegramdata);
		preparetelegram(&notgm, invdat.userid);
		strcpy( notgm.telegramtext, spr(stgopt(INVNOTE), (invdat.invis) ? "now" : "not") );
		dfaInsert(&notgm);
		dfaRstBlk();
		if ( notifyoftelegram(invdat.userid) )
			{       /* go ahead and let him know */
			telltelegram();
			}
		}
	shocst(spr("USER MADE %sINVISIBLE", (invdat.invis) ? "" : "UN"),
	       "%s by %s", invdat.userid, usaptr->userid);
	rstmbk();
	}

VOID suspend_user( VOID )				/* suspend/unsuspend a user */
	{
	CHAR suspendid[INPSIZ];         /* user-id to suspend/unsuspend */
	struct usracc susacc;           /* user account of user to suspend/unsuspend */
	struct usracc *susptr;          /* pointer to user account */
	GBOOL inform;			/* TRUE if tell user who suspended him, FALSE if don't */

	INIT_STRUCT_TO_ZERO(susacc);

	setmbk(glomb);
	if ( !haskey(help[26].key) )           /* check that user may use this global */
		{
		cantuse();
		return;
		}
	inform=sameas(margv[margc-1], "+");
	if ( (inform && (margc < 3)) || (margc == 1) )
		{
		prfmsg(SUSHLP, help[28].command);
		doprintoutput();
		return;
		}
	if ( inform )
		{
		getuserid(suspendid, 1, (margc-2));
		}
	else
		{
		getuserid(suspendid, 1, (margc-1));
		}
	susptr=&susacc;
	dfaSetBlk(accbb);
	if ( !dfaAcqEQ(susptr, suspendid, 0) )
		{	/* user entered to suspend not found */
		prfmsg(NOSCHGUY, suspendid);
		doprintoutput();
		dfaRstBlk();
		return;
		}
	if ( uhskey(susptr->userid, stgopt(NSUSKEY)) )
		{       /* can't suspend this one! */
		prfmsg(NOSUS, susptr->userid);
		doprintoutput();
		dfaRstBlk();
		return;
		}
	strcpy( suspendid, susptr->userid );
	if ( onsysn(suspendid, 1) )       /* online user, so just suspend */
		{
		if ( inform )
			{
			if ( dobeeps )
				{
				beep(1);
				}
			prfmsg(BTNSUS, usaptr->userid);
			injoth();
			}
		if ( cansee() )
			{	/* user can "see" suspended user */
			prfmsg(BNSDUN, susptr->userid);
			shocst("USER BOOTED & SUSPENDED",
			       "%s by %s", susptr->userid, usaptr->userid);
			}
		else
			{	/* user can't "see" suspended user */
			prfmsg(SUSDN, susptr->userid);
			shocst("USER SUSPENDED",
			       "%s by %s", susptr->userid, usaptr->userid);
			}
		suspendtoggle(othuap);
		bbsdsc( othusn );
		}
	else
		{
		suspendtoggle(susptr);
		if ( susptr->flags&SUSPEN )
			{       /* user suspended */
			prfmsg(SUSDN, susptr->userid);
			shocst("USER SUSPENDED",
			       "%s by %s", susptr->userid, usaptr->userid);
			}
		else
			{       /* user unsuspended */
			prfmsg(UNSUS, susptr->userid);
			shocst("USER UNSUSPENDED",
			       "%s by %s", susptr->userid, usaptr->userid);
			}
	        dfaUpdate(susptr);
	        dfaRstBlk();
		}
	doprintoutput();
	dfaSetBlk(suspenddata);
	if ( dfaAcqEQ(NULL, suspendid, 0) )
		{       /* delete current suspension record */
		dfaDelete();
		}
	dfaRstBlk();
	}

INT autosuspend_user( VOID )				/* suspend user with automatic unsuspend */
	{
	CHAR suspendid[INPSIZ];         /* user-id to suspend/unsuspend */
	struct usracc susacc;           /* user account of user to suspend/unsuspend */
	struct usracc *susptr;          /* pointer to user account */
	struct autounsus susstrc;       /* user to unsuspend disk data */
	INT daystosuspend;              /* optional days to suspend for */
	USHORT todaysdate;              /* the date as returned by today() */
	GBOOL inform;			/* TRUE if inform user who suspended him, FALSE if don't */
	INT day_offset;			/* margv[day_offset] is days to suspend for */
	INT uid_offset;			/* margv[1] to margv[uid_offset] is user to suspend */

	INIT_STRUCT_TO_ZERO(susacc);
	INIT_STRUCT_TO_ZERO(susstrc);

	setmbk(glomb);
	if ( !haskey(help[26].key) )           /* check that user may use this global */
		{
		cantuse();
		return(1);
		}
	if ( margc == 2 && sameas( margv[1], "?" ) )
		{       /* list users auto-suspended */
		prfmsg(ASUSHDR);
		dfaSetBlk(suspenddata);
		if ( !dfaAcqLO(&susstrc, 0) )
			{	/* no one in database */
			prfmsg(NONESUS);
			doprintoutput();
			dfaRstBlk();
			return(1);
			}
		dfaRstBlk();
		prfmsg(SUSGUY, susstrc.susid, ncdate(susstrc.date));
		doprintoutput();
		poll[usrnum].action=LIST_SUSPEND;
		strcpy(poll[usrnum].saveid, susstrc.susid);
		begin_polling(usrnum, poll_routine);
		return(-1);
		}
	inform=sameas(margv[margc-1], "+");
	if ( (inform && (margc <  4)) || margc < 3 )
		{	/* did not use command properly */
		prfmsg(ASUSHLP, (INT)MAXSUS, help[26].command, help[26].command, help[28].command);
		doprintoutput();
		return(1);
		}
	if ( inform )
		{
		day_offset=margc-2;
		uid_offset=margc-3;
		}
	else
		{
		day_offset=margc-1;
		uid_offset=margc-2;
		}
	if ( !numok(margv[day_offset], 0) || strlen(margv[day_offset]) > MAXSUSDIG )
		{	/* cannot be a valid number */
		prfmsg(DYS2BIG, (INT)MAXSUS);
		doprintoutput();
		return(1);
		}
	daystosuspend=atoi(margv[day_offset]);
	if ( ( daystosuspend < 1 ) || ( daystosuspend > MAXSUS ) )
		{       /* bogus days */
		prfmsg(DYS2BIG, (INT)MAXSUS);
		doprintoutput();
		return(1);
		}
	getuserid(suspendid, 1, uid_offset);
	susptr=&susacc;
	dfaSetBlk(accbb);
	if ( !dfaAcqEQ(susptr, suspendid, 0) )
		{	/* no such user in database */
		prfmsg(NOSCHGUY, suspendid);
		doprintoutput();
		dfaRstBlk();
		return(1);
		}
	if ( uhskey(susptr->userid, stgopt(NSUSKEY)) )
		{       /* Sysop, can't suspend this one! */
		prfmsg(NOSUS, susptr->userid);
		doprintoutput();
		dfaRstBlk();
		return(1);
		}
	strcpy( suspendid, susptr->userid );
	if ( onsysn(susptr->userid, 1) )       /* online user, so suspendid is ok */
		{
		if ( inform )
			{
			if ( dobeeps )
				{
				beep(1);
				}
			prfmsg(BTNSUS, usaptr->userid);
			injoth();
			}
		if ( cansee() )
			{	/* user can "see" suspended user */
			prfmsg(AUTOBNS, othuap->userid, daystosuspend);
			shocst("USER BOOTED & SUSPENDED",
			       "%s by %s", othuap->userid, usaptr->userid);
			}
		else
			{	/* user can't "see" suspended user */
			prfmsg(AUTOUNS, othuap->userid, daystosuspend);
			shocst("USER SUSPENDED",
			       "%s by %s", othuap->userid, usaptr->userid);
			}
		outprf(usrnum);
		suspendem( othuap );
		bbsdsc( othusn );
		}
	else
		{
		prfmsg(AUTOUNS, susptr->userid, daystosuspend);
		outprf(usrnum);
		shocst("USER SUSPENDED",
		       "%s by %s", susptr->userid, usaptr->userid);
		suspendem( susptr );
		dfaUpdate(susptr);
		dfaRstBlk();
		}
	dfaSetBlk(suspenddata);
	if ( dfaAcqEQ(NULL, suspendid, 0) )
		{       /* delete current suspension record */
		dfaDelete();
		}
	strcpy( susstrc.susid, suspendid );
	todaysdate=today();
	susstrc.date=getfdate((USHORT)daystosuspend, todaysdate);
	dfaInsert(&susstrc);
	dfaRstBlk();
	rstmbk();
	return(1);
	}

VOID delete_user( VOID )				/* delete/undelete a user */
	{
	CHAR deleteid[INPSIZ];          /* user-id to delete/undelete */
	struct usracc delacc;           /* user account of user to delete/undelete */
	struct usracc *delptr;          /* pointer to user account */
	GBOOL inform;			/* TRUE if tell user who deleted him, FALSE if don't */

	INIT_STRUCT_TO_ZERO(delacc);

	setmbk(glomb);
	if ( !haskey(help[15].key) )           /* check that user may use this global */
		{
		cantuse();
		return;
		}
	inform=sameas(margv[margc-1], "+");
	if ( (inform && (margc < 3)) || (margc == 1) )
		{
		prfmsg(DLUHLP, help[15].command);
		doprintoutput();
		return;
		}
	if ( inform )
		{
		getuserid(deleteid, 1, (margc-2));
		}
	else
		{
		getuserid(deleteid, 1, (margc-1));
		}
	delptr=&delacc;
	dfaSetBlk(accbb);
	if ( !dfaAcqEQ( delptr, deleteid, 0 ) )
		{	/* invalid userid given */
		prfmsg(NOSCHGUY, deleteid);
		doprintoutput();
		dfaRstBlk();
		return;
		}
	if ( uhskey(delptr->userid, stgopt(NDELKEY)) )
		{       /* can't delete this one! */
		prfmsg(NODLU, delptr->userid);
		doprintoutput();
		dfaRstBlk();
		return;
		}
	if ( usrprotected(delptr) )
		{       /* this account is protected */
		prfmsg(NODLA, delptr->userid);
		doprintoutput();
		dfaRstBlk();
		return;
		}
	if ( onsysn(deleteid, 1) )       /* online user, so zap */
		{
		if ( inform )
			{
			if ( dobeeps )
				{
				beep(1);
				}
			prfmsg(BTNDLU, usaptr->userid);
			injoth();
			}
		if ( cansee() )
			{	/* user can "see" deleted user */
			prfmsg(BNDDUN, delptr->userid);
			shocst("USER BOOTED & DELETED",
			       "%s by %s", delptr->userid, usaptr->userid);
			}
		else
			{	/* user can't "see" deleted user */
			prfmsg(DLUDN, delptr->userid);
			shocst("USER DELETED",
			       "%s by %s", delptr->userid, usaptr->userid);
			}
		outprf(usrnum);
		othuap->flags|=DELTAG;
		bbsdsc( othusn );
		}
	else
		{
		delptr->flags^=DELTAG;
		dfaUpdate(delptr);
		if ( taggedforkill(delptr) )
			{       /* user deleted */
			prfmsg(DLUDN, delptr->userid);
			shocst("USER DELETED",
			       "%s by %s", delptr->userid, usaptr->userid);
			}
		else
			{       /* user undeleted */
			prfmsg(UNDLU, delptr->userid);
			shocst("USER UNDELETED",
			       "%s by %s", delptr->userid, usaptr->userid);
			}
		outprf(usrnum);
		}
	dfaRstBlk();
	rstmbk();
	}

VOID block_page( VOID )					/* block/unblock a user's pages */
	{
	CHAR checkcount=0;              /* counter for comparing input with blocked user-ids */
	CHAR blockid[INPSIZ];           /* user-id to block/unblock */
	INT found=0;                    /* 1 if user-id match found, otherwise 0 */
	CHAR numblocked=0;              /* number of users currently blocked from paging */
	CHAR isBlocked=0;                 /* 1 if there is a user blocked, otherwise 0 */
	INT lower;			/* offset to copy string from */
	INT upper;			/* offset to copy string to */
	struct usracc forgetacc;	/* account data of user to block */
	struct userdata ourdat;		/* global user data of blocking/unblocking user */

	setmbk(glomb);
	if ( !haskey(help[19].key) )
		{
		cantuse();
		return;
		}
	if ( margc == 1 )
		{       /* print message on usage */
		prfmsg(PFUSG, pageforgetstring, pageforgetstring);
		doprintoutput();
		return;
		}
	if ( margc == 2 && sameas( margv[1], "?" ) )
		{       /* list user-ids blocked from paging */
		while ( ( checkcount < NUMFORGETS ) && (isBlocked == 0 ) )
			{       /* check for any blocked users so we can print correct header */
			if ( !sameas( userpage[usrnum].forget[checkcount], "" ) )
				{
				isBlocked =1;
				}
			checkcount++;
			}
		if (isBlocked == 0 )
			{       /* no one is blocked from paging user */
			prfmsg(NUBLOCK);
			}
		else
			{       /* list users blocked from paging */
			prfmsg(NOBLOCK1);
			for ( checkcount=0; checkcount < NUMFORGETS; checkcount++ )
				{
				if ( !sameas( userpage[usrnum].forget[checkcount], "" ) )
					{
					prfmsg(NOBLOCK2, userpage[usrnum].forget[checkcount]);
					}
				}
			}
		doprintoutput();
		return;
		}
	getuserid( blockid, 1, (margc-1) );
	while ( ( checkcount < NUMFORGETS ) && !found )
		{       /* check for user-id blocked */
		if ( !sameas( userpage[usrnum].forget[checkcount], "" ) )
			{
			numblocked++;
			found=sameas( blockid, userpage[usrnum].forget[checkcount] );
			}
		if ( !found )
			{       /* check next position */
			checkcount++;
			}
		}
	if ( found )
		{       /* user-id is blocked, so unblock */
		prfmsg(UNBLOCK, userpage[usrnum].forget[checkcount]);
		outprf(usrnum);
		setmem( userpage[usrnum].forget[checkcount], UIDSIZ, 0 );
		lower=checkcount;
		upper=checkcount+1;
		while ( ( upper < NUMFORGETS ) && !sameas( userpage[usrnum].forget[upper], "" ) )
			{	/* copy any strings at higher offsets down 1 to eliminate "holes" */
			strcpy( userpage[usrnum].forget[lower++], userpage[usrnum].forget[upper++] );
			}
		setmem( userpage[usrnum].forget[lower], UIDSIZ, 0 );	/* clear so we don't have it twice */
		dfaSetBlk(userglobdata);
		if ( dfaAcqEQ(&ourdat, usaptr->userid, 0) )
			{	/* this must be true */
			cvtDataIP(&ourdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
			for ( checkcount=0; checkcount < NUMFORGETS; checkcount++ )
				{
				strcpy( ourdat.pageforgets[checkcount], userpage[usrnum].forget[checkcount] );
				}
			cvtDataIP(&ourdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
			dfaUpdate(&ourdat);
			}
		dfaRstBlk();
		rstmbk();
		return;
		}
	if ( numblocked == NUMFORGETS )
		{       /* already have five people blocked from paging */
		prfmsg(CNTBLK, (INT)NUMFORGETS);
		prfmsg(NOBLOCK1);
		for ( checkcount=0; checkcount < NUMFORGETS; checkcount++ )
			{
			prfmsg(NOBLOCK2, userpage[usrnum].forget[checkcount]);
			}
		doprintoutput();
		return;
		}
	dfaSetBlk(accbb);
	if ( !dfaAcqEQ(&forgetacc, blockid, 0) )
		{	/* no such person to block */
		prfmsg(NOSCHGUY, blockid);
		doprintoutput();
		dfaRstBlk();
		return;
		}
	dfaRstBlk();
	if ( uhskey(forgetacc.userid, pagethrukey) )
		{       /* can't block a Sysop */
		prfmsg(NBKSYS, forgetacc.userid);
		doprintoutput();
		return;
		}
	checkcount=0;
	while ( ( checkcount < NUMFORGETS ) && !sameas( userpage[usrnum].forget[checkcount], "" ) )
		{
		checkcount++;
		}
	strcpy( userpage[usrnum].forget[checkcount], forgetacc.userid );
	dfaSetBlk(userglobdata);
	if ( dfaAcqEQ(&ourdat, usaptr->userid, 0) )
		{	/* this must be true */
		cvtDataIP(&ourdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
		for ( checkcount=0; checkcount < NUMFORGETS; checkcount++ )
			{
			strcpy( ourdat.pageforgets[checkcount], userpage[usrnum].forget[checkcount] );
			}
		cvtDataIP(&ourdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
		dfaUpdate(&ourdat);
		}
	dfaRstBlk();
	prfmsg(UPGBLK, forgetacc.userid);
	doprintoutput();
	}

VOID handle_page( VOID )				/* handle paging service */
	{
	CHAR checkuserid[INPSIZ];       /* store possible userid here */
	CHAR pagemessage[INPSIZ];       /* store the page message here */
	CHAR founduserid[INPSIZ];       /* user-id we have matched for page */
	INT attempts=1;                 /* number of attempts to find valid userid */
	INT found;                      /* -1 if exact match, 0 if no such user, otherwise # of matches */
	struct userdata pagdat;         /* data structure to store change of userpage[].pagesetting */
	struct telegram sendgram;       /* telegram user wishes to send */
	USHORT currenttime;       	/* current time returned from now() */
	USHORT currentdate;       	/* current date returned from today() */
	INT match;                      /* 1 if input matches exact user-id, otherwise 0 */
	INT attemptsend=1;		/* 1 if try injoth(), 0 if don't */
	INT pagesent;			/* 1 if page sent successfully, 0 if not, so make telegram */

	setmbk(glomb);
	if ( !haskey(help[19].key) )           /* check that user may use this global */
		{
		cantuse();
		return;
		}
	if ( margc == 1 )
		{
		prfmsg(PGHLP, help[19].command, help[19].command, help[19].command, repagestring, pageforgetstring, pageforgetstring);
		doprintoutput();
		return;
		}
	if ( margc == 2 )
		{       /* check special page settings (on, off, ok) */
		if ( sameas( margv[1], "?" ) )
			{       /* display page setting */
			prfmsg(PAGSET, ( userpage[usrnum].pagesetting > POFF ) ? ( ( userpage[usrnum].pagesetting == POK ) ? "OK" : "ON" ) : "OFF");
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], "off" ) )
			{       /* turn off pages */
			userpage[usrnum].pagesetting=POFF;
			dfaSetBlk(userglobdata);
			if ( dfaAcqEQ(&pagdat, usaptr->userid, 0) )
				{
				cvtDataIP(&pagdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
				pagdat.pagemode=POFF;
				cvtDataIP(&pagdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
				dfaUpdate(&pagdat);
				}
			dfaRstBlk();
			prfmsg(PGOFFMSG);
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], "on" ) )
			{       /* turn on pages */
			userpage[usrnum].pagesetting=PON;
			dfaSetBlk(userglobdata);
			if ( dfaAcqEQ(&pagdat, usaptr->userid, 0) )
				{
				cvtDataIP(&pagdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
				pagdat.pagemode=PON;
				cvtDataIP(&pagdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
				dfaUpdate(&pagdat);
				}
			dfaRstBlk();
			prfmsg(PGONMSG);
			doprintoutput();
			return;
			}
		if ( sameas( margv[1], "ok" ) )
			{       /* allow pages every two minutes */
			userpage[usrnum].pagesetting=POK;
			userpage[usrnum].nextpage=now();
			userpage[usrnum].pagedate=today();
			dfaSetBlk(userglobdata);
			if ( dfaAcqEQ(&pagdat, usaptr->userid, 0) )
				{
				cvtDataIP(&pagdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTPACKED, CVTSERVER, usrnum);
				pagdat.pagemode=POK;
				cvtDataIP(&pagdat, sizeof(struct userdata), sizeof(struct userdata), userdataFDA, CVTSERVER, CVTPACKED, usrnum);
				dfaUpdate(&pagdat);
				}
			dfaRstBlk();
			prfmsg(PGOKMSG);
			doprintoutput();
			return;
			}
		}
	if ( userinvis() && !haskey(pageinviskey) )
		{       /* can't do this while invisible */
		prfmsg(URINVIS);
		doprintoutput();
		return;
		}
	strcpy( checkuserid, margv[attempts] );
	strcpy( founduserid, margv[attempts] );
	do
		{
		found=uidsearch(checkuserid);
		switch ( found )
			{
			case 0 :        /* no such user on */
				{
				if ( attempts > 1 )
					{       /* coming here after multiple possibilities */
					if ( !onsysn(founduserid, 1) )
						{       /* cannot determine user-id */
						prfmsg(MULTUSR, checkuserid, help[19].command, help[19].command);
						doprintoutput();
						return;
						}
					else
						{       /* founduserid is our user */
						strcpy( checkuserid, founduserid );
						attempts--;
						found=1;
						break;
						}
					}
				else
					{       /* first check and no such user */
					prfmsg(NODUDE, checkuserid);
					}
				doprintoutput();
				return;
				}
			case 1 :        /* can only be one user */
				{
				if ( ( attempts + 1 ) < margc )
					{       /* check for only one input word */
					do
						{       /* get full handle from the input if there */
						strcat( founduserid, " " );
						strcat( founduserid, margv[++attempts] );
						match=sameto( founduserid, checkuserid );
						}
					while ( ( ( attempts + 1 ) < margc ) && match );
					if ( !match )
						{       /* index last word in handle */
						attempts--;
						}
					}
				break;
				}
			default :       /* multiple possibilities */
				{
				if ( ++attempts < margc )
					{       /* add more to possible user-id */
					strcpy( founduserid, checkuserid );
					strcat( checkuserid, " " );
					strcat( checkuserid, margv[attempts] );
					}
				else
					{
					if ( !onsysn(checkuserid, 1) )
						{       /* cannot determine user-id */
						prfmsg(MULTUSR, checkuserid, help[19].command, help[19].command);
						doprintoutput();
						return;
						}
					else
						{       /* founduserid is our user */
						found=1;
						attempts--;
						}
					}
				}
			}
		}
	while ( found > 1 );
	onsysn(checkuserid, 1);       /* set othusn, othusp, and othuap */
	if ( otherinvis() && ( !haskey(pageinviskey) || !cansee() ) )
		{	/* trying to page invisible user without the power to do it */
		prfmsg(NODUDE, checkuserid);
		doprintoutput();
		return;
		}
	if ( !oktotell() && !haskey(pagebusykey) )
		{	/* user is in the Editor and does not wish to be disturbed */
		attemptsend=0;
		}
	if ( ( userpage[othusn].pagesetting == POFF ) && ( !haskey(pagethrukey) ) )
		{       /* user has pages turned off */
		prfmsg(PGOFF, othuap->userid);
		doprintoutput();
		return;
		}
	if ( userpage[othusn].pagesetting == POK && ( !haskey(pagethrukey) ) )
		{       /* check if user has been paged in last two minutes */
		currenttime=now();
		currentdate=today();
		if ( ( currenttime < userpage[usrnum].nextpage ) || ( userpage[usrnum].pagedate > currentdate ) )
			{       /* user has recently been paged */
			prfmsg(PAGED, othuap->userid);
			doprintoutput();
			return;
			}
		}
	if ( !blockpage( othusn, usaptr->userid ) )
		{       /* pages are blocked */
		prfmsg(INGPGE, othuap->userid);
		doprintoutput();
		return;
		}
	rstrin();
	strcpy( userpage[usrnum].lastpage, input );    /* save page */
	parsin();
	setmem( pagemessage, INPSIZ, 0 );
	if ( ++attempts < margc )
		{       /* get first word in page */
		strcpy( pagemessage, margv[attempts] );
		}
	while ( ++attempts < margc )
		{       /* copy page */
		strcat( pagemessage, " " );
		strcat( pagemessage, margv[attempts] );
		}
	if ( sameas( pagemessage, "" ) )
		{       /* blank page */
		if ( dobeeps )
			{
			beep(1);
			}
		prfmsg(BLNKOUT, usaptr->userid, module[usrptr->state]->descrp);
		}
	else
		{
		if ( dobeeps )
			{
			beep(1);
			}
		prfmsg(PGOUT, usaptr->userid, module[usrptr->state]->descrp, pagemessage);
		}
	if ( usrbusy(othusp) && sendthrubusy && haskey(pagebusykey) && sendtobusy(othuap) )
		{	/* page thru "Busy" */
		togglebusy(othusp);
		pagesent=injoth();
		togglebusy(othusp);
		}
	else
		{	/* try to send injoth() */
		if ( attemptsend )
			{	/* try to send the page */
			pagesent=injoth();
			}
		else
			{	/* dont bother */
			pagesent=0;
			}
		}
	if ( pagesent )
		{       /* page reached user */
		if ( userpage[othusn].pagesetting == POK )
			{       /* get time and date for next allowed page */
			currenttime=now();
			currentdate=today();
			userpage[othusn].nextpage=(USHORT)mjrtime( PAGEOK, currenttime );
			if ( userpage[othusn].nextpage < currenttime )
				{       /* clock rolled over to 0 at midnight, so adjust next page date */
				userpage[usrnum].pagedate=getfdate(1, currentdate);
				}
			else
				{       /* update the date */
				userpage[usrnum].pagedate=currentdate;
				}
			}
		prfmsg(PGSNT, othuap->userid);
		}
	else
		{       /* couldn't receive page, so send a telegram */
		if ( !sameas( pagemessage, "" ) )
			{	/* message is not blank */
			preparetelegram( &sendgram, othuap->userid );
			strcpy( sendgram.telegramtext, pagemessage );
			dfaSetBlk(telegramdata);
			dfaInsert(&sendgram);
			dfaRstBlk();
			dotelalert( othusn, (INT)ON );
			prfmsg(CNTPGE, othuap->userid);
			}
		else
			{	/* blank message, don't bother with telegram */
			prfmsg(CNTPGENT, othuap->userid);
			}
		}
	doprintoutput();
	}

VOID poll_routine( VOID )				/* our polled routine for disk accesses */
	{
	INT gotrecord;            /* 1 if user record obtained, 0 if none */
	struct usracc newacc;     /* structure for user's account data */
	CHAR callingfrom[NADSIZ]; /* user's city and state with ZIP code removed */
	struct autounsus susstrc; /* user to unsuspend disk data */

	switch ( poll[usrnum].action )
		{
		case LIST_NEW :
			{	/* list new users */
			dfaSetBlk(accbb);
			gotrecord=dfaAcqGT(&newacc, poll[usrnum].saveid, 0);   /* get newest record */
			if ( gotrecord )
				{	/* more records */
				if ( (INT)newacc.credat == poll[usrnum].date )
					{	/* new sign-up */
					stripnonalppun( newacc.usrad3, callingfrom );
					setmbk(glomb);
					if ( poll[usrnum].newusers == 0 )
						{	/* first user we've listed */
						prfmsg(NEWHDR, ncdate(poll[usrnum].date));
						poll[usrnum].newusers=1;
						}
					prfmsg(NWSMSG, newacc.userid, callingfrom);
					doprintoutput();
					}
				strcpy(poll[usrnum].saveid, newacc.userid);
				}
			else
				{	/* no more records */
				stop_polling(usrnum);
				if ( poll[usrnum].newusers == 0 )
					{
					setmbk(glomb);
					prfmsg(SGNMSG);
					doprintoutput();
					}
				injacr();
				}
			dfaRstBlk();
			break;
			}
		case CLEAR_CHAT :
			{	/* clear global chat channel topics */
			if ( poll[usrnum].cur_chan <= poll[usrnum].end_chan )
				{	/* more to check and clear */
				if ( !sameas( topics[poll[usrnum].cur_chan].chantopic, "" ) )
					{       /* clear the topic from memory and disk */
					setmem( topics[poll[usrnum].cur_chan].chantopic, CHATTOPSIZ, 0 );
					dfaSetBlk(globchatdata);
					if ( dfaAcqEQ(NULL, &poll[usrnum].cur_chan, 0) )
						{
						dfaDelete();
						}
					dfaRstBlk();
					}
				poll[usrnum].cur_chan++;
				}
			else
				{	/* done clearing channels */
				stop_polling(usrnum);
				setmbk(glomb);
				if ( (poll[usrnum].start_chan == 1) && (poll[usrnum].end_chan == (numchatchannels-1)) )
					{	/* all channels cleared */
					prfmsg(ALLCLRD);
					if ( topchginaudit == 1 )
						{
						shocst("ALL CHAT CHANNEL TOPICS CLEARED",
						       "User-ID : %s", usaptr->userid);
						}
					}
				else
					{	/* range of channels cleared */
					prfmsg(RNGCLRD, poll[usrnum].start_chan, poll[usrnum].end_chan);
					if ( topchginaudit == 1 )
						{
						shocst("CHAT CHANNEL TOPIC RANGE CLEARED",
							spr("User-ID : %-30.29s Channel %d to %d", usaptr->userid, poll[usrnum].start_chan, poll[usrnum].end_chan));
						}
					}
				doprintoutput();
				injacr();
				}
			break;
			}
		case LIST_SUSPEND :
			{	/* show suspended users with auto unsuspend */
			dfaSetBlk(suspenddata);
			if ( dfaAcqGT(&susstrc, poll[usrnum].saveid, 0) )
				{	/* more users to show */
				setmbk(glomb);
				prfmsg(SUSGUY, susstrc.susid, ncdate(susstrc.date));
				doprintoutput();
				strcpy(poll[usrnum].saveid, susstrc.susid);
				}
			else
				{	/* no more users */
				stop_polling(usrnum);
				injacr();
				}
			dfaRstBlk();
			break;
			}
		}
	}
