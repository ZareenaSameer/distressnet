/* This file is generated.  Do not hand-hack it! */
/* gpsd.h -- fundamental types and structures for the gpsd library
 *
 * This file is Copyright (c) 2010 by the GPSD project
 * BSD terms apply: see the file COPYING in the distribution root for details.
 */

#ifndef _GPSD_H_
#define _GPSD_H_

#include "gpsd_config.h"
#include <stdbool.h>
#include <stdio.h>
#ifdef HAVE_TERMIOS_H
#include <termios.h>
#endif
#ifdef HAVE_SYS_TERMIOS_H
#include <sys/termios.h>
#endif
#include "gps.h"

#ifndef GPSD_CONFIG_H
/* Feature configuration switches begin here */
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
/* Feature configuration switches end here
 *
 * This file is Copyright (c) 2010 by the GPSD project
 * BSD terms apply: see the file COPYING in the distribution root for details.
 */
#endif /* GPSD_CONFIG_H */

#ifdef _WIN32
typedef unsigned int speed_t;
#endif

/* constants for the VERSION response */
#define GPSD_PROTO_MAJOR_VERSION	3	/* bump on incompatible changes */
#define GPSD_PROTO_MINOR_VERSION	2	/* bump on compatible changes */

/* Some internal capabilities depend on which drivers we're compiling. */
#ifdef EARTHMATE_ENABLE
#define ZODIAC_ENABLE	
#endif
#if defined(ZODIAC_ENABLE) || defined(SIRF_ENABLE) || defined(GARMIN_ENABLE) || defined(TSIP_ENABLE) || defined(EVERMORE_ENABLE) || defined(ITRAX_ENABLE) || defined(UBX_ENABLE) || defined(SUPERSTAR2_ENABLE) || defined(ONCORE_ENABLE)
#define BINARY_ENABLE	
#endif
#if defined(TRIPMATE_ENABLE) || defined(BINARY_ENABLE)
#define NON_NMEA_ENABLE	
#endif
#if defined(TNT_ENABLE) || defined(OCEANSERVER_ENABLE)
#define COMPASS_ENABLE	
#endif

/* First, declarations for the packet layer... */

/*
 * For NMEA-conforming receivers this is supposed to be 82, but
 * some receivers (TN-200, GSW 2.3.2) emit oversized sentences.
 * The current hog champion is the Trimble BX-960 receiver, which
 * emits a 91-character GGA message.
 */
#define NMEA_MAX	91		/* max length of NMEA sentence */
#define NMEA_BIG_BUF	(2*NMEA_MAX+1)	/* longer than longest NMEA sentence */

/* a few bits of ISGPS magic */
enum isgpsstat_t {
    ISGPS_NO_SYNC, ISGPS_SYNC, ISGPS_SKIP, ISGPS_MESSAGE,
};
#define ISGPS_ERRLEVEL_BASE	5

#define RTCM_MAX	(RTCM2_WORDS_MAX * sizeof(isgps30bits_t))

/*
 * The packet buffers need to be as long than the longest packet we
 * expect to see in any protocol, because we have to be able to hold
 * an entire packet for checksumming...
 * First we thought it had to be big enough for a SiRF Measured Tracker
 * Data packet (188 bytes). Then it had to be big enough for a UBX SVINFO
 * packet (206 bytes). Now it turns out that a couple of ITALK messages are
 * over 512 bytes. I know we like verbose output, but this is ridiculous.
 */
#define MAX_PACKET_LENGTH	516	/* 7 + 506 + 3 */

struct gps_packet_t {
    /* packet-getter internals */
    int	type;
#define BAD_PACKET      	-1
#define COMMENT_PACKET  	0
#define NMEA_PACKET     	1
#define AIVDM_PACKET    	2
#define GARMINTXT_PACKET	3
#define MAX_TEXTUAL_TYPE	3	/* increment this as necessary */
#define SIRF_PACKET     	4
#define ZODIAC_PACKET   	5
#define TSIP_PACKET     	6
#define EVERMORE_PACKET 	7
#define ITALK_PACKET    	8
#define GARMIN_PACKET   	9
#define NAVCOM_PACKET   	10
#define UBX_PACKET      	11
#define SUPERSTAR2_PACKET	12
#define ONCORE_PACKET   	13
#define MAX_PACKET_TYPE		13	/* increment this as necessary */
#define RTCM2_PACKET    	14
#define RTCM3_PACKET    	15
#define TEXTUAL_PACKET_TYPE(n)	(((n)>=NMEA_PACKET) && ((n)<=MAX_TEXTUAL_TYPE))
#define GPS_PACKET_TYPE(n)	(((n)>=NMEA_PACKET) && ((n)<=MAX_PACKET_TYPE))
#define LOSSLESS_PACKET_TYPE(n)	(((n)>=RTCM2_PACKET) && ((n)<=RTCM3_PACKET))
#define PACKET_TYPEMASK(n)	(1 << (n))
#define GPS_TYPEMASK	(((2<<(MAX_PACKET_TYPE+1))-1) &~ PACKET_TYPEMASK(COMMENT_PACKET))
    unsigned int state;
    size_t length;
    unsigned char inbuffer[MAX_PACKET_LENGTH*2+1];
    size_t inbuflen;
    unsigned /*@observer@*/char *inbufptr;
    /* outbuffer needs to be able to hold 4 GPGSV records at once */
    unsigned char outbuffer[MAX_PACKET_LENGTH*2+1];
    size_t outbuflen;
    unsigned long char_counter;		/* count characters processed */
    unsigned long retry_counter;	/* count sniff retries */
    unsigned counter;			/* packets since last driver switch */
    /*
     * This is not conditionalized on RTCM104_ENABLE because we need to
     * be able to build gpsdecode even when RTCM support is not
     * configured in the daemon.
     */
    struct {
	/* ISGPS200 decoding */
	bool            locked;
	int             curr_offset;
	isgps30bits_t   curr_word;
	isgps30bits_t   buf[RTCM2_WORDS_MAX];
	unsigned int    bufindex;
    } isgps;
};

extern void packet_init(/*@out@*/struct gps_packet_t *);
extern void packet_reset(/*@out@*/struct gps_packet_t *);
extern void packet_pushback(struct gps_packet_t *);
extern void packet_parse(struct gps_packet_t *);
extern ssize_t packet_get(int, struct gps_packet_t *);
extern int packet_sniff(struct gps_packet_t *);
#define packet_buffered_input(lexer) ((lexer)->inbuffer + (lexer)->inbuflen - (lexer)->inbufptr)

extern void isgps_init(/*@out@*/struct gps_packet_t *);
enum isgpsstat_t isgps_decode(struct gps_packet_t *, 
			      bool (*preamble_match)(isgps30bits_t *),
			      bool (*length_check)(struct gps_packet_t *),
			      size_t,
			      unsigned int);
extern unsigned int isgps_parity(isgps30bits_t);
extern void isgps_output_magnavox(const isgps30bits_t *, unsigned int, FILE *);

extern enum isgpsstat_t rtcm2_decode(struct gps_packet_t *, unsigned int);
extern void rtcm2_sager_dump(const struct rtcm2_t *, /*@out@*/char[], size_t);
extern void rtcm2_json_dump(const struct rtcm2_t *, /*@out@*/char[], size_t);
extern int rtcm2_undump(/*@out@*/struct rtcm2_t *, char *);
extern void rtcm2_unpack(/*@out@*/struct rtcm2_t *, char *);
extern bool rtcm2_repack(struct rtcm2_t *, isgps30bits_t *);

extern void rtcm3_unpack(/*@out@*/struct rtcm3_t *, char *);
extern void rtcm3_dump(struct rtcm3_t *rtcm, FILE *);

extern size_t oncore_payload_cksum_length(unsigned char id1,unsigned char id2);

/* Next, declarations for the core library... */

/* factors for converting among confidence interval units */
#define CEP50_SIGMA	1.18
#define DRMS_SIGMA	1.414
#define CEP95_SIGMA	2.45

/* this is where we choose the confidence level to use in reports */
#define GPSD_CONFIDENCE	CEP95_SIGMA

#define NTPSHMSEGS	4		/* number of NTP SHM segments */

struct gps_context_t {
    int valid;				/* member validity flags */
    bool readonly;			/* if true, never write to device */
#define LEAP_SECOND_VALID	0x01	/* we have or don't need correction */
    /* DGPSIP status */
    bool sentdgps;			/* have we sent a DGPS report? */
    enum { netgnss_none, netgnss_dgpsip, netgnss_ntrip} netgnss_service;	/* type of GNSS service */
    int fixcnt;				/* count of good fixes seen */
    socket_t dsock;			/* socket to DGPSIP server/Ntrip caster */
    void *netgnss_privdata;		/* DGNSS service specific data */
    size_t rtcmbytes;			/* byte count of last RTCM104 report */
    char rtcmbuf[RTCM_MAX];		/* last RTCM104 report */
    double rtcmtime;			/* timestamp of last RTCM104 report */ 
    /* timekeeping */
    int leap_seconds;			/* Unix seconds to UTC */
    unsigned short gps_week;            /* GPS week, actually 10 bits */
    double gps_tow;                     /* GPS time of week, actually 19 bits */
    int century;			/* for NMEA-only devices without ZDA */
#ifdef NTPSHM_ENABLE
    bool enable_ntpshm;
    /*@reldef@*/struct shmTime *shmTime[NTPSHMSEGS];
    bool shmTimeInuse[NTPSHMSEGS];
# ifdef PPS_ENABLE
    bool shmTimePPS;
# endif /* PPS_ENABLE */
#endif /* NTPSHM_ENABLE */
};

struct aivdm_context_t {
    /* hold context for decoding AIDVM packet sequences */
    int part, await;		/* for tracking AIDVM parts in a multipart sequence */
    unsigned char *field[NMEA_MAX];
    unsigned char fieldcopy[NMEA_MAX+1];
    unsigned char bits[2048];
    char shipname[AIS_SHIPNAME_MAXLEN+1];
    size_t bitlen;
};

struct gps_device_t;

#if defined (HAVE_SYS_TERMIOS_H)
#include <sys/termios.h>
#else
#if defined (HAVE_TERMIOS_H)
#include <termios.h>
#endif
#endif

#define MODE_NMEA	0
#define MODE_BINARY	1

typedef enum {ANY, GPS, RTCM2, RTCM3, AIS} gnss_type;
typedef enum {
    event_wakeup,
    event_triggermatch,
    event_identified,
    event_configure, 
    event_driver_switch,
    event_deactivate,
    event_reactivate,
} event_t;

#define ONLINE_IS	0x00000001u
#define TIME_IS 	0x00000002u
#define TIMERR_IS	0x00000004u
#define LATLON_IS	0x00000008u
#define ALTITUDE_IS	0x00000010u
#define SPEED_IS	0x00000020u
#define TRACK_IS	0x00000040u
#define CLIMB_IS	0x00000080u
#define STATUS_IS	0x00000100u
#define MODE_IS 	0x00000200u
#define DOP_IS  	0x00000400u
#define HERR_IS 	0x00000800u
#define VERR_IS 	0x00001000u
#define PERR_IS 	0x00002000u
#define SATELLITE_IS	0x00004000u
#define RAW_IS  	0x00008000u
#define USED_IS 	0x00010000u
#define SPEEDERR_IS	0x00020000u
#define DEVICE_IS	0x00040000u
#define DEVICEID_IS	0x00100000u
#define ERROR_IS	0x00200000u
#define RTCM2_IS	0x00400000u
#define RTCM3_IS	0x00800000u
#define AIS_IS  	0x01000000u
#define ATT_IS		0x02000000u
#define PACKET_IS	0x04000000u
#define CLEAR_IS	0x08000000u	/* sentence starts a reporting cycle */
#define REPORT_IS	0x10000000u	/* sentence ends a reporting cycle */
#define DATA_IS	~(ONLINE_IS|PACKET_IS|CLEAR_IS|REPORT_IS)

struct gps_type_t {
/* GPS method table, describes how to talk to a particular GPS type */
    /*@observer@*/char *type_name;
    int packet_type;
    /*@observer@*//*@null@*/char *trigger;
    int channels;
    /*@null@*/bool (*probe_detect)(struct gps_device_t *session);
    /*@null@*/ssize_t (*get_packet)(struct gps_device_t *session);
    /*@null@*/gps_mask_t (*parse_packet)(struct gps_device_t *session);
    /*@null@*/ssize_t (*rtcm_writer)(struct gps_device_t *session, char *rtcmbuf, size_t rtcmbytes);
    /*@null@*/void (*event_hook)(struct gps_device_t *session, event_t event);
#ifdef ALLOW_RECONFIGURE 
    /*@null@*/bool (*speed_switcher)(struct gps_device_t *session, 
				     speed_t speed, char parity, int stopbits);
    /*@null@*/void (*mode_switcher)(struct gps_device_t *session, int mode);
    /*@null@*/bool (*rate_switcher)(struct gps_device_t *session, double rate);
    double min_cycle;
#endif /* ALLOW_RECONFIGURE */
#ifdef ALLOW_CONTROLSEND
    /*@null@*/ssize_t (*control_send)(struct gps_device_t *session, char *buf, size_t buflen);
#endif /* ALLOW_CONTROLSEND */
#ifdef NTPSHM_ENABLE
    /*@null@*/double (*ntp_offset)(struct gps_device_t *session);
#endif /* NTPSHM_ENABLE */
};

typedef enum {source_unknown,
	      source_blockdev,
	      source_rs232,
	      source_usb,
	      source_pty,
	      source_socket}
    sourcetype_t;

struct gps_device_t {
/* session object, encapsulates all global state */
    struct gps_data_t gpsdata;
    /*@relnull@*/const struct gps_type_t *device_type;
    struct gps_context_t	*context;
    bool is_serial;
    sourcetype_t sourcetype;
    double rtcmtime;	/* timestamp of last RTCM104 correction to GPS */
#ifndef _WIN32
    struct termios ttyset, ttyset_old;
#endif
    unsigned int baudindex;
    int saved_baud;
    struct gps_packet_t packet;
    char subtype[64];			/* firmware version or subtype ID */
    double opentime;
    double releasetime;
#ifdef NTPSHM_ENABLE
    int shmindex;
    double last_fixtime;		/* so updates happen once */
# ifdef PPS_ENABLE
    int shmTimeP;
# endif /* PPS_ENABLE */
#endif /* NTPSHM_ENABLE */
    double mag_var;			/* magnetic variation in degrees */  
    bool back_to_nmea;			/* back to NMEA on revert? */
    char msgbuf[MAX_PACKET_LENGTH*2+1];	/* command message buffer for sends */
    size_t msgbuflen;
    int observed;			/* which packet type`s have we seen? */
    bool cycle_end_reliable;		/* does driver signal REPORT_MASK */
    bool notify_clients;		/* ship DEVICE notification on poll? */
    struct gps_fix_t newdata;		/* where clients put their data */
    struct gps_fix_t oldfix;		/* previous fix for error modeling */
    /*
     * The rest of this structure is driver-specific private storage.
     * Because the Garmin driver uses a long buffer, you can have
     * up to 4096+12 bytes of private storage in your own union member
     * without making this structure larger or changing the API at all.
     */
    union {
#ifdef NMEA_ENABLE
	struct {
	    int part, await;		/* for tracking GSV parts */
	    struct tm date;		/* date part of last sentence time */
	    double subseconds;		/* subsec part of last sentence time */
	    char *field[NMEA_MAX];
	    unsigned char fieldcopy[NMEA_MAX+1];
	    /*
	     * State for the cycle-tracking machinery.
	     * The reason these timestamps are separate from the
	     * general sentence timestamps is that we can
	     * use the minutes and seconds part of a sentence
	     * with an incomplete timestasmp (like GGA) for
	     * end-cycle recognition, even if we don't have a previous
	     * RMC or ZDA that lets us get full time from it.
	     */
	    double this_frac_time, last_frac_time;
	    bool latch_frac_time;
	    unsigned int lasttag;
	    unsigned int cycle_enders;
#ifdef GPSCLOCK_ENABLE
	    bool ignore_trailing_edge;
#endif /* GPSCLOCK_ENABLE */
	} nmea;
#endif /* NMEA_ENABLE */
#ifdef GARMINTXT_ENABLE
	struct {
	    struct tm date;		/* date part of last sentence time */
	    double subseconds;		/* subsec part of last sentence time */
	} garmintxt;
#endif /* NMEA_ENABLE */
#ifdef BINARY_ENABLE
#ifdef SIRF_ENABLE
	struct {
	    unsigned int driverstate;	/* for private use */
#define SIRF_LT_231	0x01		/* SiRF at firmware rev < 231 */
#define SIRF_EQ_231     0x02            /* SiRF at firmware rev == 231 */
#define SIRF_GE_232     0x04            /* SiRF at firmware rev >= 232 */
#define UBLOX   	0x08		/* uBlox firmware with packet 0x62 */
	    unsigned long satcounter;
	    unsigned int time_seen;
#define TIME_SEEN_GPS_1	0x01	/* Seen GPS time variant 1? */
#define TIME_SEEN_GPS_2	0x02	/* Seen GPS time variant 2? */
#define TIME_SEEN_UTC_1	0x04	/* Seen UTC time variant 1? */
#define TIME_SEEN_UTC_2	0x08	/* Seen UTC time variant 2? */
	    /* fields from Navigation Parameters message */
	    bool nav_parameters_seen;	/* have we seen one? */
	    unsigned char altitude_hold_mode;
	    unsigned char altitude_hold_source;
	    int16_t altitude_source_input;
	    unsigned char degraded_mode;
	    unsigned char degraded_timeout;
	    unsigned char dr_timeout;
	    unsigned char track_smooth_mode;
	} sirf;
#endif /* SIRF_ENABLE */
#ifdef SUPERSTAR2_ENABLE
	struct {
	    time_t last_iono;
	} superstar2;
#endif /* SUPERSTAR2_ENABLE */
#ifdef TSIP_ENABLE
	struct {
	    bool superpkt;		/* Super Packet mode requested */
	    time_t last_41;		/* Timestamps for packet requests */
	    time_t last_48;
	    time_t last_5c;
	    time_t last_6d;
	    time_t last_46;
	    time_t req_compact;
	    unsigned int stopbits; /* saved RS232 link parameter */
	    char parity;
	} tsip;
#endif /* TSIP_ENABLE */
#ifdef GARMIN_ENABLE	/* private housekeeping stuff for the Garmin driver */
	struct {
	    unsigned char Buffer[4096+12];	/* Garmin packet buffer */
	    size_t BufferLen;		/* current GarminBuffer Length */
	} garmin;
#endif /* GARMIN_ENABLE */
#ifdef ZODIAC_ENABLE	/* private housekeeping stuff for the Zodiac driver */
	struct {
	    unsigned short sn;		/* packet sequence number */
	    /*
	     * Zodiac chipset channel status from PRWIZCH. Keep it so
	     * raw-mode translation of Zodiac binary protocol can send
	     * it up to the client.
	     */
#define ZODIAC_CHANNELS	12
	    unsigned int Zs[ZODIAC_CHANNELS];	/* satellite PRNs */
	    unsigned int Zv[ZODIAC_CHANNELS];	/* signal values (0-7) */
	} zodiac;
#endif /* ZODIAC_ENABLE */
#ifdef UBX_ENABLE
	struct {
	    bool have_port_configuration;
	    unsigned char original_port_settings[20];
	    unsigned char sbas_in_use;
    	} ubx;
#endif /* UBX_ENABLE */
#ifdef NAVCOM_ENABLE
	struct {
	    uint8_t physical_port;
	    bool warned;
	} navcom;
#endif /* NAVCOM_ENABLE */
#ifdef ONCORE_ENABLE
	struct {
#define ONCORE_VISIBLE_CH 12
	    int visible;
	    int PRN[ONCORE_VISIBLE_CH];		/* PRNs of satellite */
	    int elevation[ONCORE_VISIBLE_CH];	/* elevation of satellite */
	    int azimuth[ONCORE_VISIBLE_CH];	/* azimuth */
	    double pps_delay;
	} oncore;
#endif /* ONCORE_ENABLE */

	/*
	 * This is not conditionalized on RTCM104_ENABLE because we need to
	 * be able to build gpsdecode even when RTCM support is not
	 * configured in the daemon.  It doesn't take up extra space.
	 */
	struct {
	    /* ISGPS200 decoding */
	    bool            locked;
	    int             curr_offset;
	    isgps30bits_t   curr_word;
	    isgps30bits_t   buf[RTCM2_WORDS_MAX];
	    unsigned int    bufindex;
	} isgps;
#endif /* BINARY_ENABLE */
    } driver;
    /*
     * Auxiliary structures for parsing data that can be interleaved with
     * GPS sentences. Can't be in the driver union or it will get stepped on.
     * So far the only case of this is AIS reports, which in marine navigation
     * systems may come over the same wire with GPS NMEA sentences.
     */
#ifdef AIVDM_ENABLE
    struct aivdm_context_t aivdm;
#endif /* AIVDM_ENABLE */

#ifdef TIMING_ENABLE
    /* profiling data for last sentence */
    char tag[MAXTAGLEN+1];	/* tag of last sentence processed */
    double d_xmit_time;		/* beginning of sentence transmission */
    double d_recv_time;		/* daemon receipt time (-> E1+T1) */
    double d_decode_time;	/* daemon end-of-decode time (-> D1) */
    double emit_time;		/* emission time (-> E2) */
#endif /* TIMING_ENABLE */
};

/* logging levels */
#define LOG_ERROR 	0	/* errors, display always */
#define LOG_SHOUT	0	/* not an error but we should always see it */
#define LOG_WARN	1	/* not errors but may indicate a problem */
#define LOG_INF 	2	/* key informative messages */
#define LOG_DATA	3	/* log data management messages */
#define LOG_PROG	4	/* progress messages */
#define LOG_IO  	5	/* IO to and from devices */
#define LOG_SPIN	6	/* logging for catching spin bugs */
#define LOG_RAW 	7	/* raw low-level I/O */

#define IS_HIGHEST_BIT(v,m)	(v & ~((m<<1)-1))==0

/* here are the available GPS drivers */
extern const struct gps_type_t **gpsd_drivers;

/* gpsd library internal prototypes */
extern gps_mask_t nmea_parse_input(struct gps_device_t *);
extern gps_mask_t nmea_parse(char *, struct gps_device_t *);
extern ssize_t nmea_write(struct gps_device_t *session, char *buf, size_t len);
extern ssize_t nmea_send(struct gps_device_t *session, const char *, ... );
extern void nmea_add_checksum(char *);

ssize_t generic_get(struct gps_device_t *);
ssize_t pass_rtcm(struct gps_device_t *, char *, size_t);

extern gps_mask_t sirf_parse(struct gps_device_t *, unsigned char *, size_t);
extern gps_mask_t evermore_parse(struct gps_device_t *, unsigned char *, size_t);
extern gps_mask_t navcom_parse(struct gps_device_t *, unsigned char *, size_t);
extern gps_mask_t garmin_ser_parse(struct gps_device_t *);
extern gps_mask_t garmintxt_parse(struct gps_device_t *);
extern gps_mask_t aivdm_parse(struct gps_device_t *);

extern bool netgnss_uri_check(char *);
extern int netgnss_uri_open(struct gps_context_t *, char *);
extern int netgnss_poll(struct gps_context_t *);
extern void netgnss_report(struct gps_device_t *);
extern void netgnss_autoconnect(struct gps_context_t *, double, double);

extern void rtcm_relay(struct gps_device_t *);
extern void rtcm2_output_mag(isgps30bits_t *, FILE *);

extern int dgpsip_open(struct gps_context_t *, const char *);
extern void dgpsip_report(struct gps_device_t *);
extern void dgpsip_autoconnect(struct gps_context_t *, 
			       double, double, const char *);
extern int ntrip_open(struct gps_context_t *, char *);
extern void ntrip_report(struct gps_device_t *);

extern void gpsd_tty_init(struct gps_device_t *);
extern int gpsd_open(struct gps_device_t *);
extern bool gpsd_set_raw(struct gps_device_t *);
extern ssize_t gpsd_write(struct gps_device_t *, void const *, size_t);
extern bool gpsd_next_hunt_setting(struct gps_device_t *);
extern int gpsd_switch_driver(struct gps_device_t *, char *);
extern void gpsd_set_speed(struct gps_device_t *, speed_t, char, unsigned int);
extern speed_t gpsd_get_speed(const struct termios *);
extern void gpsd_assert_sync(struct gps_device_t *);
extern void gpsd_close(struct gps_device_t *);

extern void gpsd_zero_satellites(/*@out@*/struct gps_data_t *sp)/*@modifies sp@*/;
extern void gpsd_interpret_subframe(struct gps_device_t *, unsigned int[]);
extern int gpsd_interpret_subframe_raw(struct gps_device_t *, unsigned int[]);
extern int gpsd_hexdump_level;
extern /*@ observer @*/ char *gpsd_hexdump(/*@null@*/const void *, size_t);
extern /*@ observer @*/ char *gpsd_hexdump_wrapper(/*@null@*/const void *, size_t, int);
extern int gpsd_hexpack(/*@in@*/const char *, /*@out@*/char *, size_t);
extern int hex2bin(const char *);
extern ssize_t hex_escapes(/*@out@*/char *cooked, const char *raw);
extern void ntpd_link_activate(struct gps_device_t *session);
extern char /*@observer@*/ *gpsd_id(/*@in@*/struct gps_device_t *);
extern void gpsd_position_fix_dump(struct gps_device_t *, /*@out@*/char[], size_t);
extern void gpsd_clear_data(struct gps_device_t *);
extern socket_t netlib_connectsock(int, const char *, const char *, const char *);
extern char /*@observer@*/ *netlib_errstr(const int);
extern char /*@observer@*/ *netlib_sock2ip(int);

extern void nmea_tpv_dump(struct gps_device_t *, /*@out@*/char[], size_t);
extern void nmea_sky_dump(struct gps_device_t *, /*@out@*/char[], size_t);

extern void ntpshm_init(struct gps_context_t *, bool);
extern int ntpshm_alloc(struct gps_context_t *);
extern bool ntpshm_free(struct gps_context_t *, int);
extern int ntpshm_put(struct gps_device_t *, double, double);
extern int ntpshm_pps(struct gps_device_t *,struct timeval *);

extern void ecef_to_wgs84fix(/*@out@*/struct gps_fix_t *,
			     /*@out@*/double *,
			     double, double, double, 
			     double, double, double);
extern void clear_dop(/*@out@*/struct dop_t *);
extern gps_mask_t fill_dop(/*@in@*/const struct gps_data_t *gpsdata, 
			   struct dop_t *dop);

/* srecord.c */
extern void hexdump(size_t, unsigned char *, unsigned char *);
extern unsigned char sr_sum(unsigned int, unsigned int, unsigned char *);
extern int bin2srec(unsigned int, unsigned int, unsigned int, unsigned char *, unsigned char *);
extern int srec_hdr(unsigned int, unsigned char *, unsigned char *);
extern int srec_fin(unsigned int, unsigned char *);
extern unsigned char hc(unsigned char);

/* application interface */
extern void gpsd_init(struct gps_device_t *, 
		      struct gps_context_t *, 
		      /*@null@*/char *);
extern int gpsd_activate(struct gps_device_t *);
extern void gpsd_deactivate(struct gps_device_t *);
extern gps_mask_t gpsd_poll(struct gps_device_t *);
extern void gpsd_wrap(struct gps_device_t *);

/* exceptional driver methods */
#ifdef UBX_ENABLE
extern bool ubx_write(struct gps_device_t *, unsigned int, unsigned int, 
		      /*@null@*/unsigned char *, unsigned short);
#endif /* UBX_ENABLE */
#ifdef AIVDM_ENABLE
extern bool aivdm_decode(const char *, size_t, struct aivdm_context_t *, struct ais_t *);
extern void  aivdm_json_dump(const struct ais_t *, bool, /*@out@*/char *, size_t);
#endif /* AIVDM_ENABLE */
#ifdef MTK3301_ENABLE
extern gps_mask_t processMTK3301(int c UNUSED, char *field[], struct gps_device_t *session);
#endif /* MTK3301_ENABLE */

/* caller should supply this */
# if __GNUC__ >= 3 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 7)
__attribute__((__format__(__printf__, 2, 3))) void gpsd_report(int, const char *, ...);
# else /* not a new enough GCC, use the unprotected prototype */
void gpsd_report(int, const char *, ...);
#endif
extern /*@observer@*/const char *gpsd_maskdump(gps_mask_t);


#ifdef S_SPLINT_S
extern bool finite(double);
extern struct protoent *getprotobyname(const char *);
extern /*@observer@*/char *strptime(const char *,const char *tp,/*@out@*/struct tm *)/*@modifies tp@*/;
extern struct tm *gmtime_r(const time_t *,/*@out@*/struct tm *tp)/*@modifies tp@*/;
extern struct tm *localtime_r(const time_t *,/*@out@*/struct tm *tp)/*@modifies tp@*/;
extern float roundf(float x);
#endif /* S_SPLINT_S */

/*
 * How to mix together epx and epy to get a horizontal circular error
 * eph when reporting requires it. Most devices don't report these;
 * NMEA 3.x devices reporting $GPGBS are the exception.
 */
#define EMIX(x, y)	(((x) > (y)) ? (x) : (y))

/* some OSes don't have round(). fake it if need be */
#ifndef HAVE_ROUND
#define	round(x) ((double)rint(x))
#define roundf(x) ((float)rintf((double)x))
#endif /* !HAVE_ROUND */

#define NITEMS(x) (int)(sizeof(x)/sizeof(x[0]))

/* OpenBSD and FreeBSD and Cygwin don't seem to have NAN, NetBSD does, others? */
/* XXX test for this in configure? */
#if defined(__OpenBSD__) || defined(__FreeBSD__) || defined(__CYGWIN__)
#ifndef NAN
#define NAN (0.0/0.0)
#endif /* !NAN */
#endif /* list of Operating Systems */

/* Cygwin, in addition to NAN, doesn't have cfmakeraw */
#if defined(__CYGWIN__)
void cfmakeraw(struct termios *);
#endif /* defined(__CYGWIN__) */

#endif /* _GPSD_H_ */
// Local variables:
// mode: c
// end:
