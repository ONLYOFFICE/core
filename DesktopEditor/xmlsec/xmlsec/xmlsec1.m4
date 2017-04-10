#
# Configure paths for xmlsec1
# H. Ronsdorf 2006-02-03
# Adapted from:
#
dnl AM_PATH_XMLSEC1([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Test for libxmlsec1, and define XMLSEC1_CFLAGS and XMLSEC1_LIBS
dnl
AC_DEFUN([AM_PATH_XMLSEC1],[

  AC_ARG_WITH(xmlsec1-prefix,
    AC_HELP_STRING(
      [--with-xmlsec1-prefix=PFX],
      [Prefix where libxmlsec1 is installed (optional)]
    ),
    xmlsec1_config_prefix="$withval", xmlsec1_config_prefix=""
  )

  AC_ARG_WITH(xmlsec1-exec-prefix,
    AC_HELP_STRING(
      [--with-xmlsec1-exec-prefix=PFX],
      [Exec prefix where libxmlsec1 is installed (optional)]
    ),
    xmlsec1_config_exec_prefix="$withval", xmlsec1_config_exec_prefix=""
  )

  AC_ARG_ENABLE(xmlsec1test,
    AC_HELP_STRING(
      [--disable-xmlsec1test],
      [Do not try to compile and run a test cSOAP program]
    ),,
    enable_xmlsec1test=yes
  )

  if test x$xmlsec1_config_exec_prefix != x ; then
     xmlsec1_config_args="$xmlsec1_config_args --exec-prefix=$xmlsec1_config_exec_prefix"
     if test x${XMLSEC1_CONFIG+set} != xset ; then
        XMLSEC1_CONFIG=$xmlsec1_config_exec_prefix/bin/xmlsec1-config
     fi
  fi
  if test x$xmlsec1_config_prefix != x ; then
     xmlsec1_config_args="$xmlsec1_config_args --prefix=$xmlsec1_config_prefix"
     if test x${XMLSEC1_CONFIG+set} != xset ; then
        XMLSEC1_CONFIG=$xmlsec1_config_prefix/bin/xmlsec1-config
     fi
  fi


  AC_PATH_PROG(XMLSEC1_CONFIG, xmlsec1-config, no)
  min_xmlsec1_version=ifelse([$1], ,1.0.0,[$1])
  AC_MSG_CHECKING(for libxmlsec1 - version >= $min_xmlsec1_version)
  no_xmlsec1=""
  if test "$XMLSEC1_CONFIG" = "no" ; then
    no_xmlsec1=yes
  else
    XMLSEC1_CFLAGS=`$XMLSEC1_CONFIG $xmlsec1_config_args --cflags`
    XMLSEC1_LIBS=`$XMLSEC1_CONFIG $xmlsec1_config_args --libs`
    xmlsec1_config_major_version=`$XMLSEC1_CONFIG $xmlsec1_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    xmlsec1_config_minor_version=`$XMLSEC1_CONFIG $xmlsec1_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    xmlsec1_config_micro_version=`$XMLSEC1_CONFIG $xmlsec1_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    if test "x$enable_xmlsec1test" = "xyes" ; then
      ac_save_CFLAGS="$CFLAGS"
      ac_save_LIBS="$LIBS"
      CFLAGS="$CFLAGS $XMLSEC1_CFLAGS"
      LIBS="$XMLSEC1_LIBS $LIBS"
dnl
dnl Now check if the installed libxmlsec1 is sufficiently new.
dnl (Also sanity checks the results of xmlsec1-config to some extent)
dnl
      rm -f conf.xmlsec1test
      AC_TRY_RUN([
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libxml/tree.h>
#include <xmlsec/xmlsec.h>

int main(int argc, char **argv) {

	int major, minor, micro;
	char *tmp_version;

	system("touch conf.xmlsec1test");

	/* Capture xmlsec1-config output via autoconf/configure variables */
	/* HP/UX 9 (%@#!) writes to sscanf strings */
	tmp_version = (char *)strdup("$min_xmlsec1_version");
	if (sscanf(tmp_version, "%d.%d.%d", &major, &minor, &micro) != 3) {

		printf("%s, bad version string from xmlsec1-config\n", "$min_xmlsec1_version");
		exit(1);
	}
	free(tmp_version);

	/* Test that the library is greater than our minimum version */
	if (($xmlsec1_config_major_version < major) ||
		(($xmlsec1_config_major_version == major) && ($xmlsec1_config_minor_version < minor)) ||
		(($xmlsec1_config_major_version == major) && ($xmlsec1_config_minor_version == minor) && ($xmlsec1_config_micro_version < micro))) {

		printf("\n*** An old version of libxmlsec1 (%d.%d.%d) was found.\n", $xmlsec1_config_major_version, $xmlsec1_config_minor_version, $xmlsec1_config_micro_version);
		printf("*** You need a version of libxmlsec1 newer than %d.%d.%d. The latest version of\n", major, minor, micro);
		printf("*** libxmlsec1 is always available from http://xmlsec1.sf.net.\n\n");
		printf("*** If you have already installed a sufficiently new version, this error\n");
		printf("*** probably means that the wrong copy of the xmlsec1-config shell script is\n");
		printf("*** being found. The easiest way to fix this is to remove the old version\n");
		printf("*** of libxmlsec1, but you can also set the XMLSEC1_CONFIG environment to point to the\n");
		printf("*** correct copy of xmlsec1-config. (In this case, you will have to\n");
		printf("*** modify your LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf\n");
		printf("*** so that the correct libraries are found at run-time))\n");
		return 1;
	}
	else {

		return 0;
	}
}
],, no_xmlsec1=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
       CFLAGS="$ac_save_CFLAGS"
       LIBS="$ac_save_LIBS"
     fi
  fi

  if test "x$no_xmlsec1" = x ; then
     AC_MSG_RESULT(yes (version $xmlsec1_config_major_version.$xmlsec1_config_minor_version.$xmlsec1_config_micro_version))
     ifelse([$2], , :, [$2])     
  else
     AC_MSG_RESULT(no)
     if test "$XMLSEC1_CONFIG" = "no" ; then
       echo "*** The xmlsec1-config script installed by LIBXMLSEC1 could not be found"
       echo "*** If libxmlsec1 was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the XMLSEC1_CONFIG environment variable to the"
       echo "*** full path to xmlsec1-config."
     else
       if test -f conf.xmlsec1test ; then
        :
       else
          echo "*** Could not run libxmlsec1 test program, checking why..."
          CFLAGS="$CFLAGS $XMLSEC1_CFLAGS"
          LIBS="$LIBS $XMLSEC1_LIBS"
          AC_TRY_LINK([
#include <libxmlsec1/version.h>
#include <stdio.h>
],      [ soap_client_destroy(); return 0;],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding LIBXMLSEC1 or finding the wrong"
          echo "*** version of LIBXMLSEC1. If it is not finding LIBXMLSEC1, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
          echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH" ],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means LIBXMLSEC1 was incorrectly installed"
          echo "*** or that you have moved LIBXMLSEC1 since it was installed. In the latter case, you"
          echo "*** may want to edit the xmlsec1-config script: $XMLSEC1_CONFIG" ])
          CFLAGS="$ac_save_CFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi

     XMLSEC1_CFLAGS=""
     XMLSEC1_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  AC_SUBST(XMLSEC1_CFLAGS)
  AC_SUBST(XMLSEC1_LIBS)
  rm -f conf.xmlsec1test
])
