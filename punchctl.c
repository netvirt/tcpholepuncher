#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <thp.h>

#include <event2/event.h>

static void		 usage(void);
static void		 sighandler(int, short, void *);

struct event_base	*ev_base = NULL;

void
usage(void)
{
	extern char	*__progname;
	fprintf(stdout, "usage: %s\n"
	    "\t-c\thost address\n"
	    "\t-p\tports i.e: 80,81,443,5000-6000\n"
	    "\t-h\thelp\n", __progname);
	exit(1);
}

void
sighandler(int signal, short events, void *arg)
{
	(void)signal;
	(void)events;

	event_base_loopbreak(arg);
}

int main(int argc, char *argv[])
{
	struct event	*ev_sigint;
	struct event	*ev_sigterm;
	int		 ch;
	char		*host = NULL;
	char		*ports = NULL;

	while ((ch = getopt(argc, argv, "hc:p:")) != -1) {
		switch(ch) {
		case 'c':
			host = optarg;
			break;
		case 'p':
			ports = optarg;
			break;
		default:
			usage();
		}
	}
	argc -= optind;
	argv += optind;

	if (host == NULL || ports == NULL)
		usage();

#ifndef _WIN32
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
		fprintf(stderr, "%s: signal\n", __func__);
		exit(-1);
	}
#endif

	if ((ev_base = event_base_new()) == NULL) {
		fprintf(stderr, "%s: event_init\n", __func__);
		exit(-1);
	}

	if ((ev_sigint = evsignal_new(ev_base, SIGINT, sighandler, ev_base))
	    == NULL) {
		fprintf(stderr, "%s: evsignal_new\n", __func__);
		exit(-1);
	}
	event_add(ev_sigint, NULL);

	if ((ev_sigterm = evsignal_new(ev_base, SIGTERM, sighandler, ev_base))
	    == NULL) {
		fprintf(stderr, "%s: evsignal_new\n", __func__);
		exit(-1);
	}
	event_add(ev_sigterm, NULL);

	/* TODO:
	 * add callbacks
	 */
	thp_punch_start(ev_base, host, ports, NULL, NULL);

	event_base_dispatch(ev_base);

	event_free(ev_sigint);
	event_free(ev_sigterm);
	event_base_free(ev_base);
}
