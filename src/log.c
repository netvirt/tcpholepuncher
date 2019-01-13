#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <thp.h>

static thp_logcb	 cb_log_warn = NULL;

void
thp_log_setcb(thp_logcb log_warn)
{
	cb_log_warn = log_warn;
}

void
log_warnx(const char *format, ...)
{
	va_list		 list;
	static char	 buff[512];
	int		 len;

	va_start(list, format);
	len = vsnprintf(buff, sizeof(buff), format, list);
	snprintf(buff + len, sizeof(buff) - len, "\n");
	va_end(list);

	if (cb_log_warn)
		cb_log_warn(buff);
	else
		fprintf(stdout, "%s", buff);
}

void
log_warn(const char *format, ...)
{
	va_list		 list;
	static char	 buff[512];
	int		 len;

	va_start(list, format);
	len = vsnprintf(buff, sizeof(buff), format, list);
	snprintf(buff + len, sizeof(buff) - len, ": %s\n", strerror(errno));
	va_end(list);

	if (cb_log_warn)
		cb_log_warn(buff);
	else
		fprintf(stdout, "%s", buff);
}
