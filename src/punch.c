#include <stdlib.h>
#include <string.h>

#include <event2/event.h>
#include <event2/listener.h>

#include <thp.h>

#include "log.h"
#include "queue.h"

LIST_HEAD(port_list, port);

struct port {
	LIST_ENTRY(port)	 entry;
	struct evconnlistener	*listener;
	int			 num;
	char			*str;
};

struct thp_punch {
	struct port_list	*ports;
	struct event		*timeout;
};

static struct event_base	*ev_base = NULL;

static void		 port_free();
static struct port	*port_new();
static void		 port_list_free(struct port_list *);
static struct port_list *port_list_new(const char *);
static void		 punch_timeout_cb(int, short, void *);
static void		 punch_free();
static struct thp_punch	*punch_new();
static void		 listen_error_cb(struct evconnlistener *, void *);
static void		 listen_conn_cb(struct evconnlistener *, int,
			    struct sockaddr *, int, void *);

struct port *
port_new()
{
	struct port	*p = NULL;

	if ((p = malloc(sizeof(*p))) == NULL) {
		log_warn("%s: malloc", __func__);
		goto error;
	}
	p->listener = NULL;
	p->str = NULL;

	return (p);

error:
	port_free(p);
	return (NULL);
}

void
port_free(struct port *p)
{
	if (p == NULL)
		return;

	if (p->listener != NULL)
		evconnlistener_free(p->listener);
	free(p->str);
	free(p);
}

void
port_list_free(struct port_list *l)
{
	struct port	*p;

	if (l == NULL)
		return;

	while (!LIST_EMPTY(l)) {
		p = LIST_FIRST(l);
		LIST_REMOVE(p, entry);
		port_free(p);
	}

	free(l);
}

struct port_list *
port_list_new(const char *ports)
{
	struct port_list	*l = NULL;
	struct port		*p;

	if ((l = malloc(sizeof(*l))) == NULL) {
		log_warn("%s: malloc", __func__);
		goto error;
	}
	LIST_INIT(l);

	/* XXX hardcoded for testing */

	p = port_new();
	p->num = 8080;
	p->str = strdup("8080");

	LIST_INSERT_HEAD(l, p, entry);

	p = port_new();
	p->num = 9090;
	p->str = strdup("9090");

	LIST_INSERT_HEAD(l, p, entry);

	/* TODO:
	 * parse ports
	 * port_new() LIST_INSERT_HEAD()
	 */

	return (l);

error:
	port_list_free(l);
	return (NULL);
}

void
punch_timeout_cb(int fd, short event, void *arg)
{

}

void
punch_free(struct thp_punch *thp)
{
	if (thp == NULL)
		return;

	event_free(thp->timeout);
	port_list_free(thp->ports);

	free(thp);
}

struct thp_punch *
punch_new()
{
	struct thp_punch	*thp;

	if ((thp = malloc(sizeof(*thp))) == NULL) {
		log_warn("%s: malloc", __func__);
		goto error;
	}
	thp->timeout = NULL;
	thp->ports = NULL;

	if ((thp->timeout = evtimer_new(ev_base,
	    punch_timeout_cb, thp)) == NULL) {
		log_warnx("%s: evtimer_new", __func__);
		goto error;
	}

	return (thp);

error:
	punch_free(thp);
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
	struct port		*p;
	struct thp_punch	*thp = NULL;
	struct addrinfo		 hints, *ai = NULL;
	int			 ret;

	ev_base = evb;

	if ((thp = punch_new()) == NULL) {
		log_warnx("%s: punch_new", __func__);
		goto error;
	}

	if ((thp->ports = port_list_new(ports)) == NULL) {
		log_warnx("%s: port_list_new: %s", __func__);
		goto error;
	}

	LIST_FOREACH(p, thp->ports, entry) {

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;

		/* listen on every interfaces */
		if ((ret = getaddrinfo("0.0.0.0", p->str, &hints, &ai)) != 0) {
			log_warnx("%s: getaddrinfo: %s", __func__,
			    gai_strerror(ret));
			goto error;
		}

		if ((p->listener = evconnlistener_new_bind(ev_base,
		    listen_conn_cb, thp,
		    LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1,
		    ai->ai_addr, ai->ai_addrlen)) == NULL) {
			log_warnx("%s: evconnlistener_new_bind", __func__);
			goto error;
		}

		evconnlistener_set_error_cb(p->listener, listen_error_cb);
		freeaddrinfo(ai);
		ai = NULL;

		/* TODO: connect() */
	}

	return (thp);

error:
	freeaddrinfo(ai);
	punch_free(thp);
        return (NULL);
}

void
thp_punch_stop(struct thp_punch *thp)
{
	if (thp == NULL)
		return;

	/* XXX Stop everything */
	punch_free(thp);

        return;
}
