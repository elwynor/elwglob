/* Build 26.10.11.4 07:59PM */
/*****************************************************************************
 *   GLOCONN.C                                  Globals - Connection type   *
 *                                                                           *
 *   Copyright (C) 2026 Elwynor Technologies.                                *
 *   Created by Mark Laudenbach.                                             *
 *                                                                           *
 *   How each user is connected, for the CONT column of Who's Online:       *
 *   TELNET, SSH, RLOGIN, FTP, WEB, LOCAL, or OTHER (WorldLink, MajorLink   *
 *   and anything else).                                                     *
 *                                                                           *
 *   GALTCPIP records, for every channel, the TCP/IP server that accepted   *
 *   the call (tcpipinf[].server, TCPIP.H: "server handling channel         *
 *   (NULL=none)").  Each daemon registers its server by name with           *
 *   regtcpsvr(): GALTNTD "Telnet" (TELNETD.H TNTNAME), the SSH Server       *
 *   module "SSH", GALRLGND "Rlogin", GALFTPD "FTP", GALWEBD "World-Wide     *
 *   Web".  So the name tells SSH from Telnet with no need to know whether  *
 *   the SSH Server module is installed: without it no call is accepted by  *
 *   a server named "SSH".  This is the field Total IP Control uses.        *
 *                                                                           *
 *   Globals is not linked with GALTCPIP, so tcpipinf and svrhead are found  *
 *   by name in GALTCPIP.DLL (as Total IP Control does).  If they cannot be  *
 *   found, every user shows as TELNET and nothing fails.                   *
 *                                                                           *
 *   Licensed under the GNU Affero General Public License v3.0.              *
 *****************************************************************************/

#include <winsock2.h>
#include <windows.h>
#include "gcomm.h"
#include "majorbbs.h"
/* tcpip.h defines the socket error codes (EWOULDBLOCK, ...) that the C     */
/* library's errno.h also defines; only the structure layouts are used here. */
#pragma warning(push)
#pragma warning(disable: 4005)
#include "tcpip.h"
#pragma warning(pop)
#include "GLOCONN.H"

static struct tcpipinf **pptcp = NULL;         /* GALTCPIP's tcpipinf       */
static struct svrinf **ppsvr = NULL;           /* GALTCPIP's svrhead        */

static const struct {
     const CHAR *server;             /* name the daemon registers            */
     const CHAR *shown;              /* what CONT shows                      */
} known[] = {
     { "Telnet",         "TELNET" },
     { "SSH",            "SSH"    },
     { "Rlogin",         "RLOGIN" },
     { "FTP",            "FTP"    },
     { "World-Wide Web", "WEB"    }
};

static GBOOL
resolve(VOID)                        /* find GALTCPIP's tables, once         */
{
     HMODULE h;

     if (pptcp != NULL) {
          return TRUE;
     }
     /* Looked up on first use, not at start-up: GALTCPIP may start after  */
     /* Globals.  A failed look-up is tried again next time.               */
     if ((h = GetModuleHandleA("GALTCPIP.DLL")) == NULL) {
          return FALSE;
     }
     ppsvr = (struct svrinf **)GetProcAddress(h, "_svrhead");
     pptcp = (struct tcpipinf **)GetProcAddress(h, "_tcpipinf");
     return pptcp != NULL;
}

const CHAR *
conntype(INT unum)                   /* how channel unum is connected        */
{
     struct svrinf *svr;
     INT i;

     if (!resolve() || *pptcp == NULL) {
          return "TELNET";
     }
     if (unum < 0 || unum >= nterms) {
          return "OTHER";
     }
     svr = (*pptcp)[unum].server;
     if (svr == NULL) {
          return "LOCAL";            /* no TCP/IP server: the console        */
     }
     if (svr->name != NULL) {
          for (i = 0 ; i < (INT)(sizeof(known) / sizeof(known[0])) ; i++) {
               if (sameas(svr->name, known[i].server)) {
                    return known[i].shown;
               }
          }
     }
     return "OTHER";
}

GBOOL
sshserver(VOID)                      /* is a server named SSH registered?    */
{
     struct svrinf *svr;

     if (!resolve() || ppsvr == NULL) {
          return FALSE;
     }
     for (svr = *ppsvr ; svr != NULL ; svr = svr->next) {
          if (svr->name != NULL && sameas(svr->name, "SSH")) {
               return TRUE;
          }
     }
     return FALSE;
}

VOID
connreport(VOID)                     /* rtkick: once, after start-up         */
{
     if (sshserver()) {
          shocst("GLOBALS CONNECTION TYPES",
                 "SSH Server found: SSH callers show as SSH in Who's Online");
     }
     else {
          shocst("GLOBALS CONNECTION TYPES",
                 "No SSH Server: callers show as TELNET in Who's Online");
     }
}
