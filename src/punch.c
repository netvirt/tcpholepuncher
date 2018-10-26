#include <stdlib.h>
#include <string.h>

#include <event2/event.h>
#include <event2/listener.h>

#include <thp.h>

#include "log.h"

struct thp_punch {
	struct event	*timeout;
};

static struct evconnlistener	*listener = NULL;
static struct event_base	*ev_base = NULL;

static void		 thp_punch_timeout_cb(int, short, void *);
static void		 thp_punch_free();
static struct thp_punch	*thp_punch_new();
static void		 listen_error_cb(struct evconnlistener *, void *);
static void		 listen_conn_cb(struct evconnlistener *, int,
			    struct sockaddr *, int, void *);
void
thp_punch_timeout_cb(int fd, short event, void *arg)
{

}

void
thp_punch_free(struct thp_punch *thp)
{
	if (thp == NULL)
		return;

	event_free(thp->timeout);

	free(thp);
}

struct thp_punch *
thp_punch_new()
{
	struct thp_punch	*thp;

	if ((thp = malloc(sizeof(*thp))) == NULL) {
		log_warn("%s: malloc", __func__);
		goto error;
	}
	thp->timeout = NULL;

	if ((thp->timeout = evtimer_new(ev_base,
	    thp_punch_timeout_cb, thp)) == NULL) {
		log_warnx("%s: evtimer_new", __func__);
		goto error;
	}

	return (thp);

error:
	thp_punch_free(thp);
	return (NULL);
}

void
listen_error_cb(struct evconnlistener *l, void *arg)
{
	return;
}

void
listen_conn_cb(struct evconnlistener *l, int fd,
    struct sockaddr *address, int socklen, void *arg)
{

}

struct thp_punch *
thp_punch_start(struct event_base *evb, const char *ip, const char *ports,
	    thp_punch_cb cb, void *data)
{
	struct thp_punch	*thp = NULL;
	struct addrinfo		 hints, *ai = NULL;
	int			 ret;
	const char		*port;

	ev_base = evb;

	if ((thp = thp_punch_new()) == NULL) {
		log_warnx("%s: thp_punch_new", __func__);
		goto error;
	}

	/* TODO:
	 * parse ports and create a list
	 * iterate the list and create listener for every port
	 */
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if ((ret = getaddrinfo(ip, port, &hints, &ai)) != 0) {
		log_warnx("%s: getaddrinfo: %s", __func__, gai_strerror(ret));
		goto error;
	}

	if ((listener = evconnlistener_new_bind(ev_base, listen_conn_cb, thp,
	    LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1,
	    ai->ai_addr, ai->ai_addrlen)) == NULL) {
		log_warnx("%s: evconnlistener_new_bind", __func__);
		goto error;
	}

	evconnlistener_set_error_cb(listener, listen_error_cb);
	freeaddrinfo(ai);
	return (thp);

error:
	freeaddrinfo(ai);
	thp_punch_free(thp);
        return (NULL);
}

void
thp_punch_stop(struct thp_punch *p)
{
        return;
}
