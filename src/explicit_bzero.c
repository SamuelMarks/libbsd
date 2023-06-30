/*	$OpenBSD: explicit_bzero.c,v 1.4 2015/08/31 02:53:57 guenther Exp $ */
/*
 * Public domain.
 * Written by Matthew Dempsky.
 */

#include <string.h>

#ifndef BSD_WEAK
#  if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MSYS__)
#define BSD_WEAK /* __declspec(selectany)*/
#  else
#define BSD_WEAK __attribute__((__weak__))
#  endif
#endif /* !BSD_WEAK */

 void
__explicit_bzero_hook(void *, size_t);

BSD_WEAK void
__explicit_bzero_hook(void *buf, size_t len)
{
}

void
explicit_bzero(void *buf, size_t len)
{
	memset(buf, 0, len);
	__explicit_bzero_hook(buf, len);
}
