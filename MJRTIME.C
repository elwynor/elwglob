/*****************************************************************************
 *                                                                           *
 *       MJRTIME.C - Function to make working with time easier               *
 * Developed 07/25/94 by 'Easy Ed' Erdman.   Major BBS developer code 'EZE'. *
 *                                                                           *
 * Updates: 12/18/94  - 2.0                                                  *
 *           6/05/95  - 3.0                                                  *
 *           8/21/95  - 3.1                                                  *
 *           2/22/96  - 3.2                                                  *
 *           5/25/96  - 3.3                                                  *
 *            4/6/97  - 3.3W                                                 *
 *         5/19/2021  - 3.4                                                  *
 *          8/1/2024  - 3.41                                                 *
 *	                                                                         *
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

#include "mjrtime.h"

/****************************************************************************
* MJRTIME.C - A function for the Major BBS that gives you a time that you   *
* can do some activity; for example when you can next page the Sysop. It    *
* takes the number of seconds in the future (seconds) and the current time  *
* which is the value of the now() function (prestime) as input, and returns *
* the time the activity is next allowed in the now() format.                *
****************************************************************************/

INT mjrtime( INT seconds, INT prestime )
        {
        INT newtime;           /* time to return */
        INT hours;             /* hours in newtime */
        INT minutes;           /* minutes in newtime */
        INT preshours;         /* hours in current time */
        INT presmins;          /* minutes in current time */
        INT pressecs;          /* seconds in current time */
        
        preshours=( prestime >> 11 ) & 31;    /* get hours bits only */
        hours=seconds / 3600;                 /* convert seconds to hours */
        seconds=seconds % 3600; /* remaining seconds converted to now() format */
        hours=hours + preshours;
        if ( hours > 23 )       /* compensate for new day */
                {
                hours=24 - hours;/* compensates only for time not days! */
                }
        presmins=( prestime >> 5 ) & 63; /* get minutes bits only */
        minutes=seconds / 60;            /* convert seconds to minutes */
        seconds=seconds % 60;   /* remaining seconds */
        minutes=minutes + presmins;
        if ( minutes > 59 )     /* add another hour */
                {
                minutes=60 - minutes;
                hours++;
                if ( hours > 23 ) /* check for new day */
                        {
                        hours=24 - hours;
                        }
                }
        pressecs=prestime & 31; /* get seconds */
        seconds=seconds + pressecs;
        if ( seconds > 59 )     /* add another minute */
                {
                seconds=60 - seconds;
                minutes++;
                if ( minutes > 59 )     /* add another hour */
                        {
                        minutes=60 - minutes;
                        hours++;
                        if ( hours > 23 ) /* check for new day */
                                {
                                hours=24 - hours;
                                }
                        }
                }
        hours=hours << 11;      /* convert to now() format */
        minutes=minutes << 5;
        newtime=hours + minutes + seconds;
        return( newtime );
        }
