/* Build 26.10.11.4 07:59PM */
/*****************************************************************************
 *   GLOUSER.H                                  Globals - User menu         *
 *                                                                           *
 *   Copyright (C) 2026 Elwynor Technologies.                                *
 *   Created by Mark Laudenbach.                                             *
 *                                                                           *
 *   Licensed under the GNU Affero General Public License v3.0.              *
 *****************************************************************************/

#ifndef GLOUSER_H
#define GLOUSER_H

VOID  gu_begin(VOID);                /* the user chose Globals from the menu */
INT   gu_input(VOID);                /* a line while in the menu, or 0       */
GBOOL gu_active(INT unum);           /* is channel unum in the user menu?    */
VOID  gu_userreset(INT unum);        /* log-on/hang-up: not in the menu      */

#endif /* GLOUSER_H */
