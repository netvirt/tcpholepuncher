#ifndef THP_H
#define THP_H

#include <stdarg.h>

void    thp_list_test();

/*
 * Call_backs for logging
 */
typedef void(* info_logger_cb)(const char *, ...);
typedef void(* warn_logger_cb)(const char *, ...);
typedef void(* debug_logger_cb)(const char *, ...);
typedef void(* fatal_logger_cb)(const char *);

extern const char	*log_procname;

void	log_init(int);
void	log_verbose(int);
void    vlog(int pri, const char *, va_list);
void    logit(int pri, const char *, ...);
void    log_warn(const char *, ...);
void    log_warnx(const char *, ...);
void    log_info(const char *, ...);
void    log_debug(const char *, ...);
void    fatal(const char *);
void    fatalx(const char *);

#endif /* THP_H */