#ifndef THP_H
#define THP_H

#include <stdarg.h>

typedef void (*thp_logcb)(const char *);

void	 thp_log_setcb(thp_logcb);

#endif
