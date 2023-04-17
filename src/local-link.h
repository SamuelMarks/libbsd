/*
 * Copyright © 2015 Guillem Jover <guillem@hadrons.org>
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

#ifndef LIBBSD_LOCAL_LINK_H
#define LIBBSD_LOCAL_LINK_H

#include <sys/cdefs.h>

#ifdef __ELF__
#define libbsd_link_warning(symbol, msg) \
	static const char libbsd_emit_link_warning_##symbol[] \
		__attribute__((__used__,__section__(".gnu.warning." #symbol))) = msg
#else
#define libbsd_link_warning(symbol, msg)
#endif

#if defined(__APPLE__)
#define libbsd_strong_alias(alias, symbol) \
	__asm__(".globl _" #alias); \
	__asm__(".set _" #alias ", _" #symbol); \
	extern __typeof(symbol) alias
#elif !defined(_MSC_VER)
#define libbsd_strong_alias(alias, symbol) \
	extern __typeof__(symbol) alias __attribute__((__alias__(#symbol)))
#endif

#ifdef __ELF__
#  if __has_attribute(symver)
/* The symver attribute is supported since gcc 10.x. */
#define libbsd_symver_default(alias, symbol, version) \
	extern __typeof__(symbol) symbol \
		__attribute__((__symver__(#alias "@@" #version)))
#define libbsd_symver_variant(alias, symbol, version) \
	extern __typeof__(symbol) symbol \
		__attribute__((__symver__(#alias "@" #version)))

#define libbsd_symver_weak(alias, symbol, version) \
	extern __typeof__(symbol) symbol \
		__attribute__((__symver__(#alias "@" #version), __weak__))
#  else
#define libbsd_symver_default(alias, symbol, version) \
	__asm__(".symver " #symbol "," #alias "@@" #version)

#define libbsd_symver_variant(alias, symbol, version) \
	__asm__(".symver " #symbol "," #alias "@" #version)

#define libbsd_symver_weak(alias, symbol, version) \
	libbsd_symver_variant(alias, symbol, version); \
	extern __typeof__(symbol) alias \
		__attribute__((__weak__))
#  endif
#else
#define libbsd_symver_default(alias, symbol, version) \
	libbsd_strong_alias(alias, symbol)

#define libbsd_symver_variant(alias, symbol, version)

#define libbsd_symver_weak(alias, symbol, version)
#endif

#endif
