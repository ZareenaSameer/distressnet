/* gpsd_config.h.  Generated from gpsd_config.h.in by configure.  */
/* gpsd_config.h.in.  Generated from configure.ac by autoheader.  */

/* Define if building universal (internal helper macro) */
/* #undef AC_APPLE_UNIVERSAL_BUILD */

/* AIVDM protocol support) */
#define AIVDM_ENABLE 1

/* Allow gpsd to controlsend device */
#define ALLOW_CONTROLSEND 1

/* Allow gpsd to reconfigure device */
#define ALLOW_RECONFIGURE 1

/* Ashtech chipset support */
#define ASHTECH_ENABLE 1

/* client debugging support) */
#define CLIENTDEBUG_ENABLE 1

/* Define to one of `_getb67', `GETB67', `getb67' for Cray-2 and Cray-YMP
   systems. This function is required for `alloca.c' support on those systems.
   */
/* #undef CRAY_STACKSEG_END */

/* Define to 1 if using `alloca.c'. */
/* #undef C_ALLOCA */

/* DBUS support */
/* #undef DBUS_ENABLE */

/* DeLorme EarthMate Zodiac support */
#define EARTHMATE_ENABLE 1

/* EverMore binary support */
#define EVERMORE_ENABLE 1

/* Fixed port speed */
/* #undef FIXED_PORT_SPEED */

/* San Jose Navigation FV-18 support */
#define FV18_ENABLE 1

/* Garmin Simple Text support */
#define GARMINTXT_ENABLE 1

/* Garmin support */
#define GARMIN_ENABLE 1

/* GPSclock chipset support */
#define GPSCLOCK_ENABLE 1

/* GPSD privilege revokation group */
/* #undef GPSD_GROUP */

/* GPSD privilege revocation user */
/* #undef GPSD_USER */

/* Define to 1 if you have `alloca', as a function or macro. */
#define HAVE_ALLOCA 1

/* Define to 1 if you have <alloca.h> and it should be used (not on Ultrix).
   */
#define HAVE_ALLOCA_H 1

/* Define to 1 if you have the <arpa/inet.h> header file. */
#define HAVE_ARPA_INET_H 1

/* Define if you have the external 'daylight' variable. */
#define HAVE_DAYLIGHT 1

/* Define to 1 if you have the declaration of `tzname', and to 0 if you don't.
   */
/* #undef HAVE_DECL_TZNAME */

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <getopt.h> header file. */
#define HAVE_GETOPT_H 1

/* Define to 1 if you have the <grp.h> header file. */
#define HAVE_GRP_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* pthread libraries are present */
#define HAVE_LIBPTHREAD /**/

/* will link with -l$usb; */
/* #undef HAVE_LIBUSB */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <ncurses.h> header file. */
#define HAVE_NCURSES_H 1

/* Define to 1 if you have the <netdb.h> header file. */
#define HAVE_NETDB_H 1

/* Define to 1 if you have the <netinet/in.h> header file. */
#define HAVE_NETINET_IN_H 1

/* Define to 1 if you have the <netinet/in_systm.h> header file. */
#define HAVE_NETINET_IN_SYSTM_H 1

/* Define to 1 if you have the <netinet/ip.h> header file. */
/* #undef HAVE_NETINET_IP_H */

/* Define to 1 if you have the <netinet/tcp.h> header file. */
#define HAVE_NETINET_TCP_H 1

/* Define to 1 if you have the <pwd.h> header file. */
#define HAVE_PWD_H 1

/* Define to 1 if you have the <Python.h> header file. */
/* #undef HAVE_PYTHON_H */

/* Define to 1 if you have the `round' function. */
/* #undef HAVE_ROUND */

/* Define to 1 if you have the `setlocale' function. */
#define HAVE_SETLOCALE 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strlcat' function. */
/* #undef HAVE_STRLCAT */

/* Define to 1 if you have the `strlcpy' function. */
/* #undef HAVE_STRLCPY */

/* Define to 1 if you have the `strtonum' function. */
/* #undef HAVE_STRTONUM */

/* Define to 1 if `struct tm' is a member of `tm_zone'. */
#define HAVE_STRUCT_TM_TM_ZONE 1

/* Define to 1 if you have the <syslog.h> header file. */
#define HAVE_SYSLOG_H 1

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#define HAVE_SYS_IOCTL_H 1

/* Define to 1 if you have the <sys/ipc.h> header file. */
#define HAVE_SYS_IPC_H 1

/* Define to 1 if you have the <sys/modem.h> header file. */
/* #undef HAVE_SYS_MODEM_H */

/* Define to 1 if you have the <sys/select.h> header file. */
#define HAVE_SYS_SELECT_H 1

/* Define to 1 if you have the <sys/shm.h> header file. */
#define HAVE_SYS_SHM_H 1

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/termios.h> header file. */
#define HAVE_SYS_TERMIOS_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/un.h> header file. */
#define HAVE_SYS_UN_H 1

/* Define to 1 if you have the <termios.h> header file. */
#define HAVE_TERMIOS_H 1

/* Have timezone variable */
#define HAVE_TIMEZONE /**/

/* struct tm has tm_gmtoff */
/* #undef HAVE_TM_GMTOFF */

/* Define to 1 if your `struct tm' has `tm_zone'. Deprecated, use
   `HAVE_STRUCT_TM_TM_ZONE' instead. */
#define HAVE_TM_ZONE 1

/* Define to 1 if you don't have `tm_zone' but do have the external array
   `tzname'. */
/* #undef HAVE_TZNAME */

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `vsnprintf' function. */
#define HAVE_VSNPRINTF 1

/* Define to 1 if you have the <X11/xpm.h> header file. */
/* #undef HAVE_X11_XPM_H */

/* Xpm available */
/* #undef HAVE_XPM */

/* Define to 1 if you have the <xpm.h> header file. */
/* #undef HAVE_XPM_H */

/* IPv6 support */
#define IPV6_ENABLE 1

/* iTrax chipset support */
#define ITRAX_ENABLE 1

/* C++ support */
#define LIBGPSMM_ENABLE 1

/* Limited maximum clients */
/* #undef LIMITED_MAX_CLIENTS */

/* Maximum gps devices */
/* #undef LIMITED_MAX_DEVICES */

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* MTK-3301 support */
#define MTK3301_ENABLE 1

/* Navcom support */
#define NAVCOM_ENABLE 1

/* MTK-3301 requires NMEA support */
#define NMEA_ENABLE 1

/* NTP time hinting support */
#define NTPSHM_ENABLE 1

/* NTRIP support */
#define NTRIP_ENABLE 1

/* OceanServer support */
#define OCEANSERVER_ENABLE 1

/* oldstyle (pre-JSON) protocol support */
#define OLDSTYLE_ENABLE 1

/* Motorola OnCore chipset support */
#define ONCORE_ENABLE 1

/* Name of package */
#define PACKAGE "gpsd"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME ""

/* Define to the full name and version of this package. */
#define PACKAGE_STRING ""

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME ""

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION ""

/* PPS time syncing support */
#define PPS_ENABLE 1

/* PPS on CTS rather than DCD */
/* #undef PPS_ON_CTS */

/* profiling support */
/* #undef PROFILING */

/* Raw Measurement support */
#define RAW_ENABLE 1

/* rtcm104v2 binary support */
#define RTCM104V2_ENABLE 1

/* rtcm104v3 binary support */
#define RTCM104V3_ENABLE 1

/* SiRF chipset support */
#define SIRF_ENABLE 1

/* The size of `char', as computed by sizeof. */
#define SIZEOF_CHAR 1

/* The size of `double', as computed by sizeof. */
#define SIZEOF_DOUBLE 8

/* The size of `float', as computed by sizeof. */
#define SIZEOF_FLOAT 4

/* The size of `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG 8

/* The size of `long long', as computed by sizeof. */
#define SIZEOF_LONG_LONG 8

/* The size of `short', as computed by sizeof. */
#define SIZEOF_SHORT 2

/* Squelch logging and hexdumps */
/* #undef SQUELCH_ENABLE */

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at runtime.
	STACK_DIRECTION > 0 => grows toward higher addresses
	STACK_DIRECTION < 0 => grows toward lower addresses
	STACK_DIRECTION = 0 => direction of growth unknown */
/* #undef STACK_DIRECTION */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* SuperStarII chipset support */
#define SUPERSTAR2_ENABLE 1

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#define TIME_WITH_SYS_TIME 1

/* latency timing support) */
#define TIMING_ENABLE 1

/* Define to 1 if your <sys/time.h> declares `struct tm'. */
/* #undef TM_IN_SYS_TIME */

/* True North Technologies support */
#define TNT_ENABLE 1

/* DeLorme TripMate support */
#define TRIPMATE_ENABLE 1

/* Trimble TSIP support */
#define TSIP_ENABLE 1

/* UBX Protocol support */
#define UBX_ENABLE 1

/* Version number of package */
#define VERSION "2.94"

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

/* Define to 1 if the X Window System is missing or not being used. */
#define X_DISPLAY_MISSING 1

/* Some libc's don't have strlcat/strlcpy. Local copies are provided */
#ifndef HAVE_STRLCAT
# ifdef __cplusplus
extern "C" {
# endif
size_t strlcat(/*@out@*/char *dst, /*@in@*/const char *src, size_t size);
# ifdef __cplusplus
}
# endif
#endif
#ifndef HAVE_STRLCPY
# ifdef __cplusplus
extern "C" {
# endif
size_t strlcpy(/*@out@*/char *dst, /*@in@*/const char *src, size_t size);
# ifdef __cplusplus
}
# endif
#endif

#define GPSD_CONFIG_H

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */
