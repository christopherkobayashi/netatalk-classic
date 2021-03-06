dnl configure.in for netatalk-classic

AC_INIT([netatalk-classic], [m4_esyscmd([echo -n `cat VERSION`])])

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([subdir-objects])
AM_CONFIG_HEADER(config.h)
AC_CONFIG_MACRO_DIRS([macros])
AM_MAINTAINER_MODE([enable])

dnl Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CC_C99
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AC_PROG_GREP
AC_PROG_PS

AM_PROG_CC_C_O

dnl Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS(fcntl.h limits.h stdint.h strings.h time.h sys/param.h sys/fcntl.h sys/file.h sys/ioctl.h sys/time.h sys/mnttab.h sys/statvfs.h sys/stat.h sys/vfs.h mntent.h syslog.h unistd.h termios.h sys/termios.h netdb.h sgtty.h ufs/quota.h mount.h statfs.h sys/types.h dlfcn.h errno.h sys/errno.h sys/uio.h langinfo.h locale.h sys/filio.h)
AC_CHECK_HEADERS([sys/mount.h], , , 
[#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif
])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_UID_T
AC_C_INLINE
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_STRUCT_ST_RDEV
AC_HEADER_TIME
AC_STRUCT_TM

dnl --------------------------------------------------------------------------
dnl check if dlsym needs to add an underscore, uses libtool macros 
dnl --------------------------------------------------------------------------
AC_LTDL_DLLIB
AC_CHECK_FUNCS(dlopen dlsym dlclose)
AC_LTDL_DLSYM_USCORE
if test x"$libltdl_cv_need_uscore" = xyes; then
    AC_DEFINE(DLSYM_PREPEND_UNDERSCORE, 1, [BSD compatibility macro])
fi

dnl Checks for library functions.
AC_TYPE_GETGROUPS
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MEMCMP
AC_HEADER_MAJOR
AC_FUNC_MMAP
AC_TYPE_SIGNAL
AC_FUNC_UTIME_NULL
AC_CHECK_FUNCS(getcwd gethostname gettimeofday getusershell mkdir rmdir select socket strdup strcasestr strstr strtoul strchr memcpy)
AC_CHECK_FUNCS(backtrace_symbols setlocale nl_langinfo strlcpy strlcat setlinebuf dirfd pselect access pread pwrite)
AC_CHECK_FUNCS(waitpid getcwd strdup strndup strnlen strtoul strerror chown fchown chmod fchmod chroot link mknod mknod64)
ac_neta_haveatfuncs=yes
AC_CHECK_FUNCS(openat renameat fstatat unlinkat, , ac_neta_haveatfuncs=no)
if test x"$ac_neta_haveatfuncs" = x"yes" ; then
   AC_DEFINE([_ATFILE_SOURCE], 1, AT file source)
   AC_DEFINE([HAVE_ATFUNCS], 1, whether at funcs are available)
fi

AC_CHECK_MEMBERS(struct tm.tm_gmtoff,,, [#include <time.h>])

AC_CHECK_FUNC(gethostbyname,,[AC_CHECK_LIB(nsl,gethostbyname)])
AC_CHECK_FUNC(connect,,[AC_CHECK_LIB(socket,connect)])
dnl search for necessary libs for libpthread stuff
AC_SEARCH_LIBS(pthread_sigmask, pthread,, 
               [AC_MSG_ERROR([cannot find pthread_sigmask in libc or libpthread])])
if test x"$ac_cv_search_pthread_sigmask" != x"none required" ; then
   PTHREAD_LIBS=$ac_cv_search_pthread_sigmask
fi
AC_SUBST(PTHREAD_LIBS)
AC_CACHE_SAVE

dnl Checks for (v)snprintf
NETATALK_SNPRINTF_CHECK

dnl --------------------------------------------------------------------------
dnl 64bit platform check
dnl --------------------------------------------------------------------------

AC_MSG_CHECKING([whether to check for 64bit libraries])
# Test if the compiler is in 64bit mode
echo 'int i;' > conftest.$ac_ext
atalk_cv_cc_64bit_output=no
if AC_TRY_EVAL(ac_compile); then
    case `/usr/bin/file conftest.$ac_objext` in
    *"ELF 64"*)
      atalk_cv_cc_64bit_output=yes
      ;;
    esac
fi
rm -rf conftest*

case $host_cpu:$atalk_cv_cc_64bit_output in
powerpc64:yes | s390x:yes | sparc*:yes | x86_64:yes | i386:yes)
    case $target_os in
    *bsd* | dragonfly*)
        AC_MSG_RESULT([no])
        atalk_libname="lib"
        ;;
    *)
        AC_MSG_RESULT([yes])
        atalk_libname="lib64"
        ;;
    esac
    ;;
*:*)
    AC_MSG_RESULT([no])
    atalk_libname="lib"
    ;;
esac

dnl --------------------------------------------------------------------------
dnl specific configuration comes in here:
dnl --------------------------------------------------------------------------


netatalk_cv_admin_group=yes
AC_MSG_CHECKING([for administrative group support])
AC_ARG_ENABLE(admin-group,
 	[  --disable-admin-group   disable admin group],[
        if test x"$enableval" = x"no"; then
		AC_DEFINE(ADMIN_GRP, 0, [Define if the admin group should be enabled])
		netatalk_cv_admin_group=no
		AC_MSG_RESULT([no])
	else
		AC_DEFINE(ADMIN_GRP, 1, [Define if the admin group should be enabled])
		AC_MSG_RESULT([yes])
        fi],[
		AC_DEFINE(ADMIN_GRP, 1, [Define if the admin group should be enabled])
		AC_MSG_RESULT([yes])
	]
)

NETATALK_CONFIG_DIRS

netatalk_cv_with_cracklib=no
AC_ARG_WITH(cracklib,
	[  --with-cracklib=DICT    enable/set location of cracklib dictionary],[
	if test "x$withval" != "xno" ; then
		cracklib="$withval"
		AC_CHECK_LIB(crack, main, [
			AC_DEFINE(USE_CRACKLIB, 1, [Define if cracklib should be used])
			LIBS="$LIBS -lcrack"
			if test "$cracklib" = "yes"; then
				cracklib="/usr/$atalk_libname/cracklib_dict"
			fi
			AC_DEFINE_UNQUOTED(_PATH_CRACKLIB, "$cracklib",
				[path to cracklib dictionary])
			AC_MSG_RESULT([setting cracklib dictionary to $cracklib])
			netatalk_cv_with_cracklib=yes
			],[
			AC_MSG_ERROR([cracklib not found!])
			]
		)
	fi
	]
)
AC_MSG_CHECKING([for cracklib support])
AC_MSG_RESULT([$netatalk_cv_with_cracklib])

netatalk_cv_ddp_enabled=yes

AC_CHECK_ICONV

netatalk_cv_force_uidgid=no
AC_MSG_CHECKING([whether to enable forcing of uid/gid per volume])
AC_ARG_ENABLE(force-uidgid,
	[  --enable-force-uidgid   allow forcing of uid/gid per volume (BROKEN) ],[
	if test "$enableval" = "yes"; then
		AC_DEFINE(FORCE_UIDGID, 1, [Define if you want forcing of uid/gid per volume])
		AC_MSG_RESULT([enabling forcing of uid/gid per volume])
		AC_MSG_RESULT([yes])
		netatalk_cv_force_uidgid=yes
	else
		AC_MSG_RESULT([no])
	fi
	],[
		AC_MSG_RESULT([no])
	]
)

dnl Check for CNID database backends
bdb_required=no
AC_NETATALK_CNID([bdb_required=yes],[bdb_required=no])

dnl Check for quota support
AC_CHECK_QUOTA

dnl Check for PAM libs
netatalk_cv_use_pam=no
AC_PATH_PAM([
	use_pam_so=yes
	compile_pam=yes
	netatalk_cv_use_pam=yes
	AC_DEFINE(USE_PAM, 1, [Define to enable PAM support])
])

netatalk_cv_use_shadowpw=no
AC_ARG_WITH(shadow,
	[  --with-shadow           enable shadow password support [[auto]]],
	[netatalk_cv_use_shadowpw="$withval"],
	[netatalk_cv_use_shadowpw=auto]
)

if test "x$netatalk_cv_use_shadowpw" != "xno"; then
    AC_CHECK_HEADER([shadow.h])
    if test x"$ac_cv_header_shadow_h" = x"yes"; then
	netatalk_cv_use_shadowpw=yes
	AC_DEFINE(SHADOWPW, 1, [Define if shadow passwords should be used])
    else 
      if test "x$shadowpw" = "xyes"; then
        AC_MSG_ERROR([shadow support not available])
      else
       	netatalk_cv_use_shadowpw=no
      fi
    fi 
fi

AC_MSG_CHECKING([whether shadow support should be enabled])
if test "x$netatalk_cv_use_shadowpw" = "xyes"; then
	AC_MSG_RESULT([yes])
else
	AC_MSG_RESULT([no])
fi
	
netatalk_cv_use_shellcheck=yes
AC_MSG_CHECKING([whether checking for a valid shell should be enabled])
AC_ARG_ENABLE(shell-check,
	[  --disable-shell-check   disable checking for a valid shell],[
	if test "$enableval" = "no"; then 
		AC_DEFINE(DISABLE_SHELLCHECK, 1, [Define if shell check should be disabled])
		AC_MSG_RESULT([no])
		netatalk_cv_use_shellcheck=no
	else
		AC_MSG_RESULT([yes])
	fi
	],[
		AC_MSG_RESULT([yes])
	]
)

AC_ARG_ENABLE(netbsd,
	[  --enable-netbsd         use NetBSD-style rc.d configuration ],
	if test "x$enableval" = "xyes"; then
		sysv_style=netbsd
	fi
	AC_MSG_RESULT([enabling NetBSD-style rc.d support])
)

AC_ARG_ENABLE(systemd,
	[  --enable-systemd        use general systemd configuration],[
	if test "$enableval" = "yes"; then
		sysv_style=systemd
		systemdsystemunitdir=$(pkg-config --variable=systemdsystemunitdir systemd)
	fi
	AC_MSG_RESULT([enabling general systemd support])
	]
)

dnl ----- timelord compilation (disabled by default)
AC_MSG_CHECKING([whether timelord should be compiled])
compile_timelord=no
AC_ARG_ENABLE(timelord,
	[  --enable-timelord       enable compilation of timelord server],
	[compile_timelord="$enableval"],
	[compile_timelord="no"]
)
AC_MSG_RESULT([$compile_timelord])

dnl ----- a2boot compilation (enabled by default)
AC_MSG_CHECKING([whether a2boot should be compiled])
compile_a2boot=yes
AC_ARG_ENABLE(a2boot,
	[  --enable-a2boot         enable compilation of Apple2 boot server],
	[compile_a2boot="$enableval"],
	[compile_a2boot="yes"]
)
AC_MSG_RESULT([$compile_a2boot])

AC_ARG_WITH(uams-path,
	[  --with-uams-path=PATH   path to UAMs [[PKGCONF/uams]]],[
		uams_path="$withval"
	],[
		uams_path="${PKGCONFDIR}/uams"
	]
)

NETATALK_AC_CUPS

dnl check if we can use attribute unused (gcc only) from ethereal
AC_MSG_CHECKING(to see if we can add '__attribute__((unused))' to CFLAGS)
if test x$GCC != x ; then
  CFLAGS="-D_U_=\"__attribute__((unused))\" $CFLAGS"
  AC_MSG_RESULT(yes)
else
  CFLAGS="-D_U_=\"\" $CFLAGS"
  AC_MSG_RESULT(no)
fi

dnl --------------------------------------------------------------------------
dnl FHS stuff has to be done last because it overrides other defaults
dnl --------------------------------------------------------------------------

AC_MSG_CHECKING([whether to use Filesystem Hierarchy Standard (FHS) compatibility])
AC_ARG_ENABLE(fhs,
	[  --enable-fhs            use Filesystem Hierarchy Standard (FHS) compatibility],[
	if test "$enableval" = "yes"; then
		uams_path="${libdir}/netatalk"
		sysconfdir="/etc"
		PKGCONFDIR=${sysconfdir}/netatalk
		SERVERTEXT="${PKGCONFDIR}/msg"
		use_pam_so=yes
		mandir="/usr/share/man"
		AC_DEFINE(FHS_COMPATIBILITY, 1, [Define if you want compatibily with the FHS])
		AC_MSG_RESULT([yes])
	else
		AC_MSG_RESULT([no])
	fi
	],[
		AC_MSG_RESULT([no])
	]
)

dnl --------------------------------------------------------------------------
dnl post-FHS substitutions, etc
dnl --------------------------------------------------------------------------

dnl ***** UAMS_PATH
dnl AC_DEFINE_UNQUOTED(UAMS_PATH, "${uams_path}",
dnl 	[path to UAMs [default=PKGCONF/uams]])
UAMS_PATH="${uams_path}"
AC_SUBST(UAMS_PATH)

dnl --------------------------------------------------------------------------
dnl drop in includes for top level directory structures here...
dnl --------------------------------------------------------------------------

dnl Note: $(top_srcdir)/include should be added before all other includes
dnl       so that includes from that directory a preferred to includes from
dnl       /usr/include or similar places.
LIBS="$LIBS -L\$(top_srcdir)/libatalk"
CFLAGS="-I\$(top_srcdir)/include $CFLAGS -I\$(top_srcdir)/sys"

AC_DEFINE(OPEN_NOFOLLOW_ERRNO, ELOOP, errno returned by open with O_NOFOLLOW)

dnl --------------------------------------------------------------------------
dnl specific configuration comes in here:
dnl --------------------------------------------------------------------------

dnl ac_cv_target_os is now host_os, ac_cv_target_cpu is now host_cpu

dnl --------------------- determine operating system from "target"
case "$host_os" in
	*linux*)   			this_os=linux ;;
	*netbsd*) 			this_os=netbsd; CFLAGS="${CFLAGS} -I/usr/pkg/include"; LIBS="${LIBS} -L/usr/pkg/lib" ;;
esac

case "$host_cpu" in
	i386|i486|i586|i686|k7)		this_cpu=x86 ;;
	alpha)				this_cpu=alpha ;;
	mips)				this_cpu=mips ;;
	powerpc|ppc)			this_cpu=ppc ;;
esac

dnl --------------------- GNU source
case "$this_os" in
	linux)	AC_DEFINE(_GNU_SOURCE, 1, [Whether to use GNU libc extensions])
        ;;
esac

dnl --------------------- operating system specific flags (port from sys/*)

dnl ----- Linux specific -----
if test x"$this_os" = "xlinux"; then 
	AC_MSG_RESULT([ * Linux specific configuration])
	
	dnl ----- kernel 2.6 changed struct at_addr to atalk_addr
	AC_MSG_CHECKING([for struct atalk_addr])
dnl	AC_COMPILE_IFELSE([
	AC_TRY_COMPILE([
#include <sys/socket.h>
#include <asm/types.h>
#include <linux/atalk.h>

	struct atalk_addr foo;
],
[ ], [
		ac_have_atalk_addr=yes
		AC_MSG_RESULT([yes])
	], [
		AC_MSG_RESULT([no])
	])

if test "x$ac_have_atalk_addr" = "xyes"; then
	AC_DEFINE(HAVE_ATALK_ADDR, 1, [set if struct at_addr is called atalk_addr])
fi

fi

dnl ----- NetBSD specific -----
if test x"$this_os" = "xnetbsd"; then 
	AC_MSG_RESULT([ * NetBSD specific configuration])
	AC_DEFINE(BSD4_4, 1, [BSD compatiblity macro])
	AC_DEFINE(NETBSD, 1, [Define if OS is NetBSD])
	AC_DEFINE(OPEN_NOFOLLOW_ERRNO, EFTYPE, errno returned by open with O_NOFOLLOW)

	need_dash_r=yes 
fi

dnl ------ Check for sendfile() --------
netatalk_cv_search_sendfile=yes
AC_ARG_ENABLE(sendfile,
    [  --disable-sendfile       disable sendfile syscall],
    [if test x"$enableval" = x"no"; then
            netatalk_cv_search_sendfile=no
        fi]
)

if test x"$netatalk_cv_search_sendfile" = x"yes"; then
   case "$host_os" in
   *linux*)
        AC_DEFINE(SENDFILE_FLAVOR_LINUX,1,[Whether linux sendfile() API is available])
        AC_CHECK_FUNC([sendfile], [netatalk_cv_HAVE_SENDFILE=yes])
        ;;

    *)
        ;;

    esac

    if test x"$netatalk_cv_HAVE_SENDFILE" = x"yes"; then
        AC_DEFINE(WITH_SENDFILE,1,[Whether sendfile() should be used])
    fi
fi

dnl -- look for openssl, if found enables DHX UAM and Randnum UAM
AC_PATH_SSL

dnl Check for Berkeley DB library
if test "x$bdb_required" = "xyes"; then
	AC_PATH_BDB(, [
		AC_MSG_RESULT([])
		AC_MSG_RESULT([Make sure you have the required Berkeley DB libraries AND headers installed.])
		AC_MSG_RESULT([You can download the latest version from http://www.sleepycat.com.])
		AC_MSG_RESULT([If you have installed BDB in a non standard location use the])
		AC_MSG_RESULT([--with-bdb=/path/to/bdb configure option and make sure])
		AC_MSG_RESULT([your linker is configured to check for libraries there.])
		AC_MSG_ERROR([Berkeley DB library required but not found!])
	])
fi

dnl -- check for crypt
AC_CRYPT


dnl --------------------- overwrite the config files . . . or not.
AC_MSG_CHECKING([whether configuration files should be overwritten])
AC_ARG_ENABLE(overwrite,
	[  --enable-overwrite      overwrite configuration files during installation],
	[OVERWRITE_CONFIG="${enable_overwrite}"],
	[OVERWRITE_CONFIG="no"]
)
AC_MSG_RESULT([$OVERWRITE_CONFIG])

dnl --------------------- check for LDAP support, for client-side ACL visibility
AC_MSG_CHECKING(for LDAP (necessary for client-side ACL visibility))
AC_ARG_WITH(ldap,
    [AS_HELP_STRING([--with-ldap],
        [LDAP support (default=auto)])],
    [ case "$withval" in
      yes|no)
          with_ldap="$withval"
		  ;;
      *)
          with_ldap=auto
          ;;
      esac ])
AC_MSG_RESULT($with_ldap)

if test x"$with_ldap" != x"no" ; then
   	AC_CHECK_HEADER([ldap.h], with_ldap=yes,
        [ if test x"$with_ldap" = x"yes" ; then
            AC_MSG_ERROR([Missing LDAP headers])
        fi
		with_ldap=no
        ])
	AC_CHECK_LIB(ldap, ldap_init, with_ldap=yes,
        [ if test x"$with_ldap" = x"yes" ; then
            AC_MSG_ERROR([Missing LDAP library])
        fi
		with_ldap=no
        ])
fi

if test x"$with_ldap" = x"yes"; then
	AC_DEFINE(HAVE_LDAP,1,[Whether LDAP is available])
fi

dnl --------------------- check for ACL support
with_acl_support=no
AC_MSG_CHECKING(whether to support ACLs)
AC_ARG_WITH(acls,
    [AS_HELP_STRING([--with-acls],
        [Include ACL support (default=no)])],
    [ case "$withval" in
      yes|no)
          with_acl_support="$withval"
		  ;;
      *)
          with_acl_support=no
          ;;
      esac ],
    [with_acl_support=no])
AC_MSG_RESULT($with_acl_support)

if test x"$with_acl_support" = x"no"; then
	AC_MSG_RESULT(Disabling ACL support)
	AC_DEFINE(HAVE_NO_ACLS,1,[Whether no ACLs support should be built in])
else
    with_acl_support=yes
fi

if test x"$with_acl_support" = x"yes" ; then
	AC_MSG_NOTICE(checking whether ACL support is available:)
	case "$host_os" in
	*)
		AC_CHECK_LIB(acl,acl_get_file,[ACL_LIBS="$ACL_LIBS -lacl"])
		case "$host_os" in
		*linux*)
			AC_CHECK_LIB(attr,getxattr,[ACL_LIBS="$ACL_LIBS -lattr"])
			;;
		esac
		AC_CACHE_CHECK([for POSIX ACL support],netatalk_cv_HAVE_POSIX_ACLS,[
			acl_LIBS=$LIBS
			LIBS="$LIBS $ACL_LIBS"
			AC_TRY_LINK([
				#include <sys/types.h>
				#include <sys/acl.h>
			],[
				acl_t acl;
				int entry_id;
				acl_entry_t *entry_p;
				return acl_get_entry(acl, entry_id, entry_p);
			],
			[netatalk_cv_HAVE_POSIX_ACLS=yes],
			[netatalk_cv_HAVE_POSIX_ACLS=no
                with_acl_support=no])
			LIBS=$acl_LIBS
		])
		if test x"$netatalk_cv_HAVE_POSIX_ACLS" = x"yes"; then
			AC_MSG_NOTICE(Using POSIX ACLs)
			AC_DEFINE(HAVE_POSIX_ACLS,1,[Whether POSIX ACLs are available])
			AC_CACHE_CHECK([for acl_get_perm_np],netatalk_cv_HAVE_ACL_GET_PERM_NP,[
				acl_LIBS=$LIBS
				LIBS="$LIBS $ACL_LIBS"
				AC_TRY_LINK([
					#include <sys/types.h>
					#include <sys/acl.h>
				],[
					acl_permset_t permset_d;
					acl_perm_t perm;
					return acl_get_perm_np(permset_d, perm);
				],
				[netatalk_cv_HAVE_ACL_GET_PERM_NP=yes],
				[netatalk_cv_HAVE_ACL_GET_PERM_NP=no])
				LIBS=$acl_LIBS
			])
			if test x"$netatalk_cv_HAVE_ACL_GET_PERM_NP" = x"yes"; then
				AC_DEFINE(HAVE_ACL_GET_PERM_NP,1,[Whether acl_get_perm_np() is available])
			fi

			AC_CACHE_CHECK([for acl_from_mode], netatalk_cv_HAVE_ACL_FROM_MODE,[
				acl_LIBS=$LIBS
				LIBS="$LIBS $ACL_LIBS"
                AC_CHECK_FUNCS(acl_from_mode,
				[netatalk_cv_HAVE_ACL_FROM_MODE=yes],
				[netatalk_cv_HAVE_ACL_FROM_MODE=no])
				LIBS=$acl_LIBS
			])
			if test x"netatalk_cv_HAVE_ACL_FROM_MODE" = x"yes"; then
				AC_DEFINE(HAVE_ACL_FROM_MODE,1,[Whether acl_from_mode() is available])
			fi


		else
			AC_MSG_NOTICE(ACL support is not avaliable)
			AC_DEFINE(HAVE_NO_ACLS,1,[Whether no ACLs support is available])
		fi
		;;
    esac
fi

if test x"$with_acl_support" = x"yes" ; then
   AC_CHECK_HEADERS([acl/libacl.h])
    AC_DEFINE(HAVE_ACLS,1,[Whether ACLs support is available])
    AC_SUBST(ACL_LIBS)
fi

dnl --------------------- check for Extended Attributes support
neta_cv_eas="ad"
neta_cv_eas_sys_found=no
neta_cv_eas_sys_not_found=no

AC_CHECK_HEADERS(sys/attributes.h attr/xattr.h sys/xattr.h sys/extattr.h sys/uio.h sys/ea.h)

case "$this_os" in
  *)
	AC_SEARCH_LIBS(getxattr, [attr])

    if test "x$neta_cv_eas_sys_found" != "xyes" ; then
       AC_CHECK_FUNCS([getxattr lgetxattr fgetxattr listxattr llistxattr],
                      [neta_cv_eas_sys_found=yes],
                      [neta_cv_eas_sys_not_found=yes])
	   AC_CHECK_FUNCS([flistxattr removexattr lremovexattr fremovexattr],,
                      [neta_cv_eas_sys_not_found=yes])
	   AC_CHECK_FUNCS([setxattr lsetxattr fsetxattr],,
                      [neta_cv_eas_sys_not_found=yes])
    fi

    if test "x$neta_cv_eas_sys_found" != "xyes" ; then
	   AC_CHECK_FUNCS([getea fgetea lgetea listea flistea llistea],
                      [neta_cv_eas_sys_found=yes],
                      [neta_cv_eas_sys_not_found=yes])
	   AC_CHECK_FUNCS([removeea fremoveea lremoveea setea fsetea lsetea],,
                      [neta_cv_eas_sys_not_found=yes])
    fi

    if test "x$neta_cv_eas_sys_found" != "xyes" ; then
	   AC_CHECK_FUNCS([attr_get attr_list attr_set attr_remove],,
                      [neta_cv_eas_sys_not_found=yes])
       AC_CHECK_FUNCS([attr_getf attr_listf attr_setf attr_removef],,
                      [neta_cv_eas_sys_not_found=yes])
    fi
  ;;
esac

if test "x$neta_cv_eas_sys_found" = "xyes" ; then
   if test "x$neta_cv_eas_sys_not_found" != "xyes" ; then
      neta_cv_eas="$neta_cv_eas | sys"
   fi
fi
AC_DEFINE_UNQUOTED(EA_MODULES,["$neta_cv_eas"],[Available Extended Attributes modules])

dnl --------------------- last minute substitutions

dnl Comment/change this to deal with gcc "helpful" warnings

CFLAGS="-Wall -Werror $CFLAGS"

AC_SUBST(LIBS)
AC_SUBST(CFLAGS)
AC_SUBST(OVERWRITE_CONFIG)

AM_CONDITIONAL(COMPILE_TIMELORD, test x$compile_timelord = xyes)
AM_CONDITIONAL(COMPILE_A2BOOT, test x$compile_a2boot = xyes)
AM_CONDITIONAL(HAVE_OPENSSL, test x$neta_cv_have_openssl = xyes)
AM_CONDITIONAL(HAVE_ACLS, test x"$with_acl_support" = x"yes")
AM_CONDITIONAL(HAVE_LDAP, test x"$with_ldap" = x"yes")
AM_CONDITIONAL(USE_RANDNUM, test x$neta_cv_have_openssl = xyes)
AM_CONDITIONAL(USE_PAM_SO, test x$use_pam_so = xyes)
AM_CONDITIONAL(USE_PAM, test x$netatalk_cv_install_pam = xyes)
AM_CONDITIONAL(BUILD_PAM, test x$compile_pam = xyes)
AM_CONDITIONAL(DEFAULT_HOOK, test x$neta_cv_have_libgcrypt != xyes && test x$neta_cv_have_openssl != xyes)
AM_CONDITIONAL(USE_NETBSD, test x$sysv_style = xnetbsd)
AM_CONDITIONAL(USE_REDHAT_SYSV, test x$sysv_style = xredhat-sysv)
AM_CONDITIONAL(USE_SUSE_SYSV, test x$sysv_style = xsuse-sysv)
AM_CONDITIONAL(USE_SHADOWPW, test x$shadowpw = xyes)
AM_CONDITIONAL(USE_GENTOO, test x$sysv_style = xgentoo)
AM_CONDITIONAL(USE_DEBIAN, test x$sysv_style = xdebian)
AM_CONDITIONAL(USE_SYSTEMD, test x$sysv_style = xsystemd || test x$sysv_style = xredhat-systemd || test x$sysv_style = xsuse-systemd)
AM_CONDITIONAL(USE_UNDEF, test x$sysv_style = x)
AM_CONDITIONAL(USE_BDB, test x$bdb_required = xyes)

dnl --------------------- generate files

AC_OUTPUT([Makefile
	bin/Makefile
	bin/ad/Makefile
	bin/adv1tov2/Makefile
	bin/aecho/Makefile
	bin/afppasswd/Makefile
	bin/cnid/Makefile
	bin/cnid/cnid2_create
	bin/getzones/Makefile
	bin/megatron/Makefile
	bin/misc/Makefile
	bin/nbp/Makefile
	bin/pap/Makefile
	bin/psorder/Makefile
	bin/uniconv/Makefile
	config/Makefile
	config/pam/Makefile
	contrib/Makefile
	contrib/macusers/Makefile
	contrib/macusers/macusers
	contrib/misc/Makefile
	contrib/printing/Makefile
	contrib/shell_utils/Makefile
	contrib/shell_utils/apple_dump
	contrib/shell_utils/asip-status.pl
	contrib/timelord/Makefile
	contrib/a2boot/Makefile
	distrib/Makefile
	distrib/config/Makefile
	distrib/config/netatalk-config
	distrib/initscripts/Makefile
	distrib/m4/Makefile
	doc/Makefile
	etc/Makefile
	etc/afpd/Makefile
	etc/atalkd/Makefile
	etc/cnid_dbd/Makefile
	etc/macipgw/Makefile
	etc/uams/Makefile
	etc/papd/Makefile
	etc/psf/Makefile
	include/Makefile
	include/atalk/Makefile
	libatalk/Makefile
	libatalk/acl/Makefile
	libatalk/adouble/Makefile
	libatalk/asp/Makefile
	libatalk/atp/Makefile
	libatalk/bstring/Makefile
	libatalk/cnid/Makefile
	libatalk/cnid/last/Makefile
	libatalk/cnid/dbd/Makefile
	libatalk/nbp/Makefile
	libatalk/netddp/Makefile
	libatalk/util/Makefile
	libatalk/unicode/Makefile
	libatalk/unicode/charsets/Makefile
	libatalk/vfs/Makefile
	macros/Makefile
	man/Makefile
	man/man1/Makefile
	man/man3/Makefile
	man/man4/Makefile
	man/man5/Makefile
	man/man8/Makefile
	sys/Makefile
	sys/netatalk/Makefile
	test/Makefile
	test/afpd/Makefile
	],
	[chmod a+x distrib/config/netatalk-config contrib/shell_utils/apple_*]
)

AC_NETATALK_LIBS_SUMMARY
AC_NETATALK_CONFIG_SUMMARY
