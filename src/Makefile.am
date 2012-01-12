## Process this file with automake to produce Makefile.in

# Set default values for user variables
CFLAGS = -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter

AM_CPPFLAGS = \
	-I$(top_builddir) \
	-isystem $(top_srcdir)/include/bsd/ \
	-include $(top_builddir)/config.h \
	-DLIBBSD_OVERLAY -DLIBBSD_DISABLE_DEPRECATED \
	-D__REENTRANT

EXTRA_DIST = \
	libbsd.map \
	libbsd.pc.in \
	libbsd-overlay.pc.in \
	hash/helper.c \
	$(nil)

CLEANFILES = \
	hash/md5hl.c \
	$(nil)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = \
	libbsd.pc \
	libbsd-overlay.pc \
	$(nil)

lib_LTLIBRARIES = libbsd.la

hash/md5hl.c: $(srcdir)/hash/helper.c
	$(MKDIR_P) hash
	$(AM_V_GEN) sed -e 's:hashinc:bsd/md5.h:g' -e 's:HASH:MD5:g' $< > $@

libbsd_la_DEPENDENCIES = \
	libbsd.map
libbsd_la_LDFLAGS = \
	-Wl,--version-script=$(srcdir)/libbsd.map \
	-version-number 0:3:0
libbsd_la_SOURCES = \
	arc4random.c \
	bsd_getopt.c \
	closefrom.c \
	dehumanize_number.c \
	err.c \
	expand_number.c \
	fgetln.c \
	flopen.c \
	fmtcheck.c \
	fpurge.c \
	getpeereid.c \
	hash/md5.c \
	hash/md5hl.c \
	heapsort.c \
	humanize_number.c \
	inet_net_pton.c \
	local-elf.h \
	merge.c \
	nlist.c \
	pidfile.c \
	progname.c \
	radixsort.c \
	readpassphrase.c \
	reallocf.c \
	setmode.c \
	setproctitle.c \
	strlcat.c \
	strlcpy.c \
	strmode.c \
	strtonum.c \
	unvis.c \
	vis.c \
	$(nil)

install-exec-hook:
	if [ "$(libdir)" != "$(runtimelibdir)" ]; then \
		mkdir -p $(DESTDIR)$(runtimelibdir); \
		mv $(DESTDIR)$(libdir)/libbsd*.so.* \
		   $(DESTDIR)$(runtimelibdir)/; \
		soname=`readlink $(DESTDIR)$(libdir)/libbsd.so`; \
		sorelprefix=`echo $(libdir) | sed -r -e 's:(^/)?[^/]+:..:g'`; \
		ln -sf $$sorelprefix$(runtimelibdir)/$$soname \
		       $(DESTDIR)$(libdir)/libbsd.so; \
	fi

uninstall-hook:
	rm -f $(DESTDIR)$(runtimelibdir)/libbsd*.so*