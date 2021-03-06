//
// debug.cpp
//
// (c) 2007 Prof Dr Andreas Mueller, Hochschule Rapperswil
// $Id: debug.cpp,v 1.3 2008/12/05 18:08:25 afm Exp $
//
#include <debug.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <cerrno>
#include <cstdarg>
#include <cstring>
#include <ctime>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */

#ifdef HAVE_SYSLOG_H
#include <syslog.h>
#endif /* HAVE_SYSLOG_H */

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif /* HAVE_SYS_TIME_H */

#include <pthread.h>

int	debuglevel = LOG_ERR;

int	debugtimeprecision = 0;

int	debugthreads = 0;

extern "C" void	debug(int loglevel, const char *file, int line,
	int flags, const char *format, ...) {
	va_list ap;
	if (loglevel > debuglevel) { return; }
	va_start(ap, format);
	vdebug(loglevel, file, line, flags, format, ap);
	va_end(ap);
}

#define	MSGSIZE	1024

extern "C" void vdebug(int loglevel, const char *file, int line,
	int flags, const char *format, va_list ap) {
	//time_t		t;
	struct tm	*tmp;
	char	msgbuffer[MSGSIZE], prefix[MSGSIZE],
		msgbuffer2[MSGSIZE], tstp[MSGSIZE],
		threadid[20];
	int	localerrno;

	if (loglevel > debuglevel) { return; }

	// message content
	localerrno = errno;
	vsnprintf(msgbuffer2, sizeof(msgbuffer2), format, ap);
	if (flags & DEBUG_ERRNO) {
		snprintf(msgbuffer, sizeof(msgbuffer), "%s: %s (%d)",
			msgbuffer2, strerror(localerrno), localerrno);
	} else {
		strcpy(msgbuffer, msgbuffer2);
	}

	// get time
	struct timeval	tv;
	gettimeofday(&tv, NULL);
	tmp = localtime(&tv.tv_sec);
	size_t	bytes = strftime(tstp, sizeof(tstp), "%b %e %H:%M:%S", tmp);

	// high resolution time
	if (debugtimeprecision > 0) {
		if (debugtimeprecision > 6) {
			debugtimeprecision = 6;
		}
		unsigned int	u = tv.tv_usec;
		int	p = 6 - debugtimeprecision;
		while (p--) { u /= 10; }
		snprintf(tstp + bytes, sizeof(tstp) - bytes, ".%0*u",
			debugtimeprecision, u);
	}

	// find the current thread id if necessary
	if (debugthreads) {
		snprintf(threadid, sizeof(threadid), "/%04lx",
			((unsigned long)pthread_self()) % 0x10000);
	} else {
		threadid[0] = '\0';
	}

	// get prefix
	if (flags & DEBUG_NOFILELINE) {
		snprintf(prefix, sizeof(prefix), "%s %s[%d%s]:",
			tstp, "astro", getpid(), threadid);
	} else {
		snprintf(prefix, sizeof(prefix), "%s %s[%d%s] %s:%03d:",
			tstp, "astro", getpid(), threadid, file, line);
	}

	// format log message
	fprintf(stderr, "%s %s\n", prefix, msgbuffer);
	fflush(stderr);
}
