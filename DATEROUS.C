/*****************************************************************************
 *                                                                           *
 *       DATEROUS.C - A collection of functions to work with dates           *
 * Developed 07/25/94 by 'Easy Ed' Erdman.   Major BBS developer code 'EZE'. *
 *                                                                           *
 * Updates: 12/18/94  - 2.0                                                  *
 *		       6/05/95  - 3.0                                                  *
 *           8/21/95  - 3.1                                                  *
 *		       2/22/96  - 3.2                                                  *
 *           5/25/96  - 3.3                                                  *
 *		        4/6/97  - 3.3W                                                 *
 *		     5/19/2021  - 3.4                                                  *
 *          8/1/2024  - 3.41                                                 *
 *									                                                  *
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

#include "daterous.h"
#include "gcomm.h"
#include "majorbbs.h"

USHORT daysinmonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; /* number of days in the months */

/*****************************************************************************
* GETFDATE - A function that returns a date in the future as an integer in   *
* the Major BBS format. daysahead is the number of days in the future to get *
* as our date, and datenow is today's date as returned by today().           *
*****************************************************************************/

USHORT getfdate( USHORT daysahead, USHORT datenow )      /* returns a date dayshead days in the future */
	{
	USHORT curyear;                    /* year in datenow */
	USHORT curmonth;                   /* month in datenow */
	USHORT curday;                     /* day in datenow */
	USHORT leapyear;                   /* 1 if a leapyear, 0 otherwise */
	USHORT daysthismonth;              /* days in the present month */
	USHORT returndate;                 /* date returned by function */

	curyear=datenow >> 9;
	curmonth=( datenow >> 5 ) & 15;
	curday=datenow & 31;
	while ( daysahead > 0 )
		{
		if ( ( curyear % 4 ) == 0 )
			leapyear=1;
		else
			leapyear=0;
		if ((curmonth == 2) && leapyear)
			daysthismonth = 29;
		else if (curmonth <= 11)
			daysthismonth = daysinmonth[curmonth - 1];
		else {
		    daysthismonth = 30; // this is an error really.
			shocst(spr("ELWGLOB: error curmonth %d",curmonth), "Overflow in getfdate()");
		}
		daysthismonth-=curday;  /* compensate for any days in first month already gone */
		if ( daysahead > daysthismonth )
			{       /* we will go past end of month */
			daysahead-=daysthismonth;
			curday=0;               /* compensate only for first month */
			if ( curmonth < 12 )
				curmonth++;
			else
				{
				curyear++;      /* start with a new year and first month */
				curmonth=1;
				}
			}
		else
			{
			curday=daysahead + curday;
			daysahead=0;
			}
		}
	returndate=( curyear << 9 ) + ( curmonth << 5 ) + curday;
	return( returndate );
	}

/*****************************************************************************
* GETPDATE - A function that returns a date in the past as an integer in     *
* the Major BBS format. daysbehind is the number of days in the past to get  *
* as our date, and datenow is today's date as returned by today().           *
*****************************************************************************/

USHORT getpdate( USHORT daysbehind, USHORT datenow )      /* returns a date daysbehind days in the past */
	{
	USHORT curyear;                    /* year in datenow */
	USHORT curmonth;                   /* month in datenow */
	USHORT curday;                     /* day in datenow */
	USHORT leapyear;                   /* 1 if a leapyear, 0 otherwise */
	USHORT daysthismonth;              /* days in the present month */
	USHORT returndate;                 /* date returned by function */

	curyear=datenow >> 9;
	curmonth=( datenow >> 5 ) & 15;
	curday=datenow & 31;
	while ( daysbehind > 0 )
		{
		if ( ( curyear % 4 ) == 0 )
			leapyear=1;
		else
			leapyear=0;
		if ( ( curmonth == 2 ) && leapyear )
			daysthismonth=29;
		else if (curmonth <= 11) 
			daysthismonth=daysinmonth[curmonth-1];
		else {
			daysthismonth = 30; // this is an error patch
			shocst(spr("ELWGLOB: error curmonth %d", curmonth), "Overflow in getpdate()");
		}
		if ( curday != 0 )
			{       /* deal only with part of starting month */
			daysthismonth=curday;
			}
		if ( daysbehind > daysthismonth )
			{       /* we will go past end of month */
			daysbehind-=daysthismonth;
			curday=0;               /* compensate only for first month */
			if ( curmonth > 1 )
				curmonth--;
			else
				{
				curyear--;      /* start with a new year and first month */
				curmonth=12;
				}
			}
		else
			{
			curday=daysthismonth - daysbehind;
			daysbehind=0;
			}
		}
	returndate=( curyear << 9 ) + ( curmonth << 5 ) + curday;
	return( returndate );
	}

/*****************************************************************************
* VALDATE - A function that accepts a date passed as an integer in the       *
* Major BBS date format and returns 1 if the date is valid, or 0 if the date *
* is not valid. Currently only detects use of leap year day in invalid       *
* years.                                                                     *
*****************************************************************************/

USHORT valdate( USHORT datetocheck )                /* returns 1 if date valid, else 0 */
	{
	USHORT curyear;                    /* year in datetocheck */
	USHORT curmonth;                   /* month in datetocheck */
	USHORT curday;                     /* day in datetocheck */
	USHORT leapyear;                   /* 1 if a leapyear, 0 otherwise */

	curyear=datetocheck >> 9;
	curmonth=( datetocheck >> 5 ) & 15;
	curday=datetocheck & 31;
	if ( ( curyear % 4 ) == 0 )
		{
		leapyear=1;
		}
	else
		{
		leapyear=0;
		}
	if ( ( curmonth == 2 ) && ( curday == 29 ) )
		{	/* special check for this date */
		if ( !leapyear )
			{	/* can't have a February 29th */
			return(0);
			}
		else
			{	/* it's ok */
			return(1);
			}
		}
	if ( curmonth > 12 )
		{	/* Bogus month */
		return(0);
		}
	if ( curday > daysinmonth[curmonth-1] )
		{	/* Bogus day */
		return(0);
		}
	return(1);
	}

/*****************************************************************************
* DAYSBTWN - A function that returns the number of days between two given    *
* dates passed as integers according to the Major BBS format. startdate is   *
* the date to start with, and enddate is the date to end with.               *
* NOTE: This routine is accurate to INT_MAX days. If you need to work with   *
* larger amounts of days than this, you will need to edit this function.     *
*****************************************************************************/

USHORT daysbtwn( USHORT startdate, USHORT enddate )      /* returns the number of days between dates startdate and enddate */
	{
	USHORT startyear;                  /* year to start counting with */
	USHORT endyear;                    /* year to stop counting with */
	USHORT startmonth;                 /* month to start counting with */
	USHORT endmonth;                   /* month to stop counting with */
	USHORT startday;                   /* day to start counting with */
	USHORT endday;                     /* day to stop counting with */
	USHORT leapyear;                   /* 1 if a leapyear, 0 otherwise */
	USHORT days=0;                     /* count of days between the dates */
	USHORT daysthismonth;              /* days in the present month */
	USHORT workyear;                   /* counter for looping thru years */
	USHORT workmonth;                  /* counter for looping thru months */
	USHORT workday;                    /* counter for looping thru days */

	startyear=startdate >> 9;
	endyear=enddate >> 9;
	startmonth=( startdate >> 5 ) & 15;
	endmonth=( enddate >> 5 ) & 15;
	startday=startdate & 31;
	endday=enddate & 31;
	workyear=startyear;     /* start with years if not the same */
	workday=startday;       /* start with first day in date */
	while ( workyear < endyear )
		{
		if ( ( workyear % 4 ) == 0 )
			leapyear=1;
		else
			leapyear=0;
		for ( workmonth=startmonth; workmonth < 13; workmonth++ )
			{       /* get days in each month of year */
			if ( ( workmonth == 2 ) && leapyear )
				daysthismonth=29;
			else
				daysthismonth=daysinmonth[workmonth-1];
			days+=daysthismonth-workday;
			workday=0;     /* make sure we get each day after first month */
			}
		startmonth=1;   /* start at January and work up to December */
		workyear++;     /* go to the next year */
		}
	if ( startyear == endyear )
		{       /* start with startmonth, same years */
		workmonth=startmonth;
		}
	else
		{       /* start with January, different years */
		workmonth=1;
		}
	while ( workmonth < endmonth )
		{       /* get days in the months */
		if ( ( workyear % 4 ) == 0 )
			leapyear=1;
		else
			leapyear=0;
		if ( ( workmonth == 2 ) && leapyear )
			daysthismonth=29;
		else if (workmonth <= 11) 
			daysthismonth=daysinmonth[workmonth-1];
		else {
			daysthismonth = 30; // this is an error patch
			shocst(spr("ELWGLOB: error workmonth %d", workmonth), "Overflow in daysbtwn()");
		}
		days+=daysthismonth-workday;
		workday=0;     /* make sure we get each day after first month */
		workmonth++;
		}
	if ( ( startyear == endyear ) && ( startmonth == endmonth ) )
		{       /* only days between dates in one month */
		days=endday-startday;
		}
	else
		{       /* get days starting from first day in month to endday */
		days+=endday;
		}
	return( days );
	}

/*****************************************************************************
* GETAGE - A function which takes the current date as integer in the form    *
* YYYYYYYMMMMDDDDD where Ys are year bits, Ms are month bits and Ds are day  *
* bits in parameter todaysdate, and the user's birthday in the format of a   *
* string, MM/DD/YY where the letters are as above, and returns the age of    *
* user.									     *
*****************************************************************************/

USHORT getage( USHORT todaysdate, char *birthdate )
	{
	USHORT byear;			/* numeric value of year of birth */
	USHORT bmonth;			/* numeric value of month of birth */
	USHORT bday;			/* numeric value of day of birth */
	CHAR yearstring[BDYSIZ] = { 0 };	/* string of year of birth */
	CHAR monthstring[BDYSIZ] = { 0 };	/* string of month of birth */
	CHAR daystring[BDYSIZ] = { 0 };		/* string of day of birth */
	USHORT usersage;		/* the user's age we return */
	USHORT thismonth;		/* the current month converted to 1 to 12 */
	USHORT thisday;			/* the current day converted to 1 to 31 */

	monthstring[0]=birthdate[0];
	monthstring[1]=birthdate[1];
	monthstring[2]='\0';
	daystring[0]=birthdate[3];
	daystring[1]=birthdate[4];
	daystring[2]='\0';
	yearstring[0]=birthdate[6];
	yearstring[1]=birthdate[7];
	yearstring[2]='\0';
	bmonth=(USHORT)atoi(monthstring);
	bday=(USHORT)atoi(daystring);
	byear=(USHORT)atoi(yearstring);
	usersage=(((todaysdate&YEARMASK)>>YEARSHIFT)+80)-byear;
	thismonth=((todaysdate&MONTHMASK)>>MONTHSHIFT);
	thisday=(todaysdate&DAYMASK);
	if ( ( bmonth > thismonth ) || ( ( bmonth == thismonth ) && ( bday > thisday ) ) )
		{	/* user has not reached this year's birthday, so subtract one from age */
		if ( usersage )
			{	/* obviously we can't have a negative age */
			usersage--;
			}
		}
	return(usersage);
	}
