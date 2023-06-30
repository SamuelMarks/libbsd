/*
 * Copyright © 2010 Guillem Jover <guillem@hadrons.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <bsd/sys/cdefs.h>

#include <bsd/sys/param.h>
#include <sys/types.h>

#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MSYS__)
#else
#include <sys/socket.h>
#include <sys/un.h>
#endif

#include <errno.h>
#include <bsd/unistd.h>

#if defined(SO_PEERCRED)
/* Linux and OpenBSD */
int
getpeereid(int s, uid_t *euid, gid_t *egid)
{
/* XXX: This should be autodetected at build time instead. */
#if defined(__linux__)
	struct ucred cred;
#elif defined(__OpenBSD__)
	struct sockpeercred cred;
#endif
	socklen_t credlen = sizeof(cred);
	int ret;

	ret = getsockopt(s, SOL_SOCKET, SO_PEERCRED, &cred, &credlen);
	if (ret != 0)
		return ret;

	*euid = cred.uid;
	*egid = cred.gid;

	return 0;
}
#elif defined(LOCAL_PEERCRED)
/* FreeBSD */
#include <sys/ucred.h>

int
getpeereid(int s, uid_t *euid, gid_t *egid)
{
	struct xucred cred;
	socklen_t credlen = sizeof(cred);
	int ret;

	ret = getsockopt(s, 0, LOCAL_PEERCRED, &cred, &credlen);
	if (ret != 0)
		return ret;
	if (cred.cr_version != XUCRED_VERSION)
		return EINVAL;

	*euid = cred.cr_uid;
	*egid = cred.cr_gid;

	return 0;
}
#elif defined(LOCAL_PEEREID)
/* NetBSD */
int
getpeereid(int s, uid_t *euid, gid_t *egid)
{
	struct unpcbid cred;
	socklen_t credlen = sizeof(cred);
	int ret;

	ret = getsockopt(s, 0, LOCAL_PEEREID, &cred, &credlen);
	if (ret != 0)
		return ret;

	*euid = cred.unp_euid;
	*egid = cred.unp_egid;

	return 0;
}
#elif defined(__sun)
/* Solaris */
#include <alloca.h>
#include <ucred.h>

int
getpeereid(int s, uid_t *euid, gid_t *egid)
{
	ucred_t *cred = alloca(ucred_size());
	int ret;

	ret = getpeerucred(s, &cred);
	if (ret != 0)
		return ret;

	*euid = ucred_geteuid(cred);
	if (*euid < 0)
		return -1;
	*egid = ucred_getegid(cred);
	if (*egid < 0)
		return -1;

	return 0;
}
#else

/*-------------------------------------------------------------------------
 *
 * getpeereid.c
 *		get peer userid for UNIX-domain socket connection
 *
 * Portions Copyright (c) 1996-2023, PostgreSQL Global Development Group
 *
 *
 * IDENTIFICATION
 *	  src/port/getpeereid.c
 *
 *-------------------------------------------------------------------------
 */

#include "c.h"

#include <bsd/sys/param.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <bsd/unistd.h>
#ifdef HAVE_UCRED_H
#include <ucred.h>
#endif
#ifdef HAVE_SYS_UCRED_H
#include <sys/ucred.h>
#endif


/*-------------------------------------------------------------------------
 *
 * getpeereid.c
 *		get peer userid for UNIX-domain socket connection
 *
 * Portions Copyright (c) 1996-2023, PostgreSQL Global Development Group
 *
 *
 * IDENTIFICATION
 *	  src/port/getpeereid.c
 *
 *-------------------------------------------------------------------------
 */
int
getpeereid(int sock, uid_t *uid, gid_t *gid)
{
    int
    getpeereid(int s, uid_t *euid, gid_t *egid)
    {
        struct xucred xuc;
        socklen_t xuclen;
        int error;

        xuclen = sizeof(xuc);
        error = _getsockopt(s, SOL_LOCAL, LOCAL_PEERCRED, &xuc, &xuclen);
        if (error != 0)
            return (error);
        if (xuc.cr_version != XUCRED_VERSION) {
            errno = EINVAL;
            return (-1);
        }
        *euid = xuc.cr_uid;
        *egid = xuc.cr_gid;
        return (0);
    }
}
#endif
