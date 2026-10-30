/* Build 26.10.11.4 07:59PM */
/*****************************************************************************
 *   GLOCONN.H                                  Globals - Connection type   *
 *                                                                           *
 *   Copyright (C) 2026 Elwynor Technologies.                                *
 *   Created by Mark Laudenbach.                                             *
 *                                                                           *
 *   Licensed under the GNU Affero General Public License v3.0.              *
 *****************************************************************************/

#ifndef GLOCONN_H
#define GLOCONN_H

const CHAR *conntype(INT unum);      /* TELNET, SSH, RLOGIN, ... for CONT    */
GBOOL sshserver(VOID);               /* is the SSH Server module running?    */
VOID connreport(VOID);               /* Audit Trail line, after start-up     */

#endif /* GLOCONN_H */
