## Makefile.am -- Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = gnu

SUBDIRS = src data doc tests

## We want these in the dist tarball
EXTRA_DIST = autogen.sh \
             pixmaps

ACLOCAL = aclocal -I $(ac_aux_dir)

CLEANFILES = *~

DISTCLEANFILES = .deps/*.P

MAINTAINERCLEANFILES = Makefile.in aclocal.m4 configure config-h.in \
                       stamp-h.in $(ac_aux_dir)/depcomp \
                       $(ac_aux_dir)/install-sh $(ac_aux_dir)/missing \
                       $(ac_aux_dir)/mkinstalldirs $(ac_aux_dir)/config.guess \
                       $(ac_aux_dir)/config.sub $(ac_aux_dir)/ltmain.sh \
                       $(ac_aux_dir)/compile \
                       $(ac_aux_dir)/test-driver
