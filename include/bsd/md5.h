/*
 * Copyright © 2021 Guillem Jover <guillem@hadrons.org>
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

#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MSYS__)

/*	$NetBSD: md5.h,v 1.5 2008/10/08 14:28:14 joerg Exp $	*/

/*
 * This branch is derived from the RSA Data Security, Inc. MD5 Message-Digest
 * Algorithm and has been modified by Jason R. Thorpe <thorpej@NetBSD.ORG>
 * for portability and formatting.
 */

/*
 * Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
 * rights reserved.
 *
 * License to copy and use this software is granted provided that it
 * is identified as the "RSA Data Security, Inc. MD5 Message-Digest
 * Algorithm" in all material mentioning or referencing this software
 * or this function.
 *
 * License is also granted to make and use derivative works provided
 * that such works are identified as "derived from the RSA Data
 * Security, Inc. MD5 Message-Digest Algorithm" in all material
 * mentioning or referencing the derived work.
 *
 * RSA Data Security, Inc. makes no representations concerning either
 * the merchantability of this software or the suitability of this
 * software for any particular purpose. It is provided "as is"
 * without express or implied warranty of any kind.
 *
 * These notices must be retained in any copies of any part of this
 * documentation and/or software.
 */

#include <sys/cdefs.h>
#include <stdint.h>

#define MD5_DIGEST_LENGTH		16
#define	MD5_BLOCK_LENGTH		64
#define MD5_DIGEST_STRING_LENGTH	33

/* MD5 context. */
typedef struct MD5Context {
    uint32_t state[4];	/* state (ABCD) */
    uint32_t count[2];	/* number of bits, modulo 2^64 (lsb first) */
    unsigned char buffer[64]; /* input buffer */
} MD5_CTX;

__BEGIN_DECLS
void	MD5Init(MD5_CTX *);
void	MD5Update(MD5_CTX *, const unsigned char *, unsigned int);
void	MD5Final(unsigned char[16], MD5_CTX *);
char	*MD5End(MD5_CTX *, char *);
char	*MD5File(const char *, char *);
char	*MD5Data(const unsigned char *, unsigned int, char *);
void   MD5Pad (MD5_CTX *);
void MD5Transform(uint32_t [4], const unsigned char [64]);
__END_DECLS

#else
#ifdef LIBBSD_OVERLAY
#include_next <md5.h>
#else
#include <md5.h>
#endif
#endif
