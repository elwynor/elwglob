/*****************************************************************************
 *                                                                           *
 *       DATEROUS.H - Header file for daterous.c                             *
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
 *          8/4/2024  - 3.42                                                 *
 *          3/5/2024  - 3.43                                                 *
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

#ifndef __GCTYPDEF_H
#include "gctypdef.h"
#endif

#include <stdlib.h>
#include <limits.h>

#define YEARMASK 0xFE00          // 7 bits for the year
#define YEARSHIFT 9              // The number of bits to shift to get the year
#define MONTHMASK 0x01E0         // 4 bits for the month
#define MONTHSHIFT 5             // The number of bits to shift to get the month
#define DAYMASK 0x001F           // 5 bits for the day

extern USHORT getfdate( USHORT daysahead, USHORT datenow );           /* returns date daysahead days in future */
extern USHORT getpdate( USHORT daysbehind, USHORT datenow );          /* returns a date daysbehind days in the past */
extern USHORT valdate(  USHORT datetocheck );                         /* returns 1 if date valid, else 0 */
extern USHORT daysbtwn( USHORT startdate, USHORT enddate );           /* returns the number of days between dates startdate and enddate */
extern USHORT getdpart( USHORT length, CHAR *date, CHAR start );      /* returns a portion of a date */
extern USHORT getage( USHORT todaysdate, CHAR *birthdate );           /* returns the age of a user */
