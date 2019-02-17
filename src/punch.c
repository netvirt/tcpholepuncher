#include <stdlib.h>
#include <string.h>

#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/listener.h>

#include <thp.h>

#include "log.h"
#include "queue.h"

LIST_HEAD(port_list, port);

struct port {
	LIST_ENTRY(port)	 entry;
	struct evconnlistener	*listener;
	struct bufferevent	*client_bev;
	unsigned long		 num;
	char			 str[6];
	void			*arg;
};

struct thp_punch {
	struct port_list	*ports;
	struct event		*timeout;
	thp_punch_cb		 cb;
};

static struct event_base	*ev_base = NULL;

static void		 port_free();
static struct port	*port_new(const char *);
static void		 port_list_free(struct port_list *);
static struct port_list *port_list_new(char *);
static void		 punch_timeout_cb(int, short, void *);
static void		 punch_free();
static struct thp_punch	*punch_new();
static void		 punch_stop(struct thp_punch *);
static void		 peer_event_cb(struct bufferevent *, short, void *);
static void		 listen_error_cb(struct evconnlistener *, void *);
static void		 listen_conn_cb(struct evconnlistener *, int,
			    struct sockaddr *, int, void *);

struct port *
port_new(const char *port_str)
{
	struct port	*p = NULL;
	const char	*errstr;

	if ((p = malloc(sizeof(*p))) == NULL) {
		log_warn("%s: malloc", __func__);
		goto error;
	}
	p->listener = NULL;

	if (port_str != NULL) {
		p->num = strtonum(port_str, 1, 65535, &errstr);
		if (errstr != NULL)
			goto error;

		if (snprintf(p->str, sizeof(p->str), port_str)
		    >= sizeof(p->str))
			goto error;
	}

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
port_list_new(char *ports)
{
	struct port_list	*l = NULL;
	struct port		*p;
	char			*port, *last;

	if ((l = malloc(sizeof(*l))) == NULL) {
		log_warn("%s: malloc", __func__);
		goto error;
	}
	LIST_INIT(l);

	for ((port = strtok_r(ports, ",", &last)); port;
	    (port = strtok_r(NULL, ",", &last))) {
		if ((p = port_new(port)) == NULL)
			goto error;
		LIST_INSERT_HEAD(l, p, entry);
	}

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
punch_stop(struct thp_punch *thp)
{
	struct port	*p;

	LIST_FOREACH(p, thp->ports, entry) {
		evconnlistener_disable(p->listener);
	}
}

void
peer_event_cb(struct bufferevent *bev, short events, void *arg)
{

	if (events & BEV_EVENT_CONNECTED) {

		printf("connected\n");

	} else if (events & (BEV_EVENT_TIMEOUT | BEV_EVENT_EOF | BEV_EVENT_ERROR)) {

		printf("peer event problem\n");
	}
}

void
listen_error_cb(struct evconnlistener *l, void *arg)
{
	struct port	*p = arg;

	/* TODO stop that listener */

	return;
}

void
listen_conn_cb(struct evconnlistener *l, int fd,
    struct sockaddr *address, int socklen, void *arg)
{

	struct port		*p = arg;
	struct thp_punch	*thp = NULL;

	thp = p->arg;

	punch_stop(thp);

	if (thp->cb != NULL)
		thp->cb(0, fd, arg); /* event is 0 for now */
}

struct thp_punch *
thp_punch_new(struct event_base *evb, const char *ip, char *ports,
	    thp_punch_cb cb, void *arg)
{
	struct port		*p;
	struct thp_punch	*thp = NULL;
	struct addrinfo		 hints, *ai = NULL;
	int			 ret, flag, sock;

	ev_base = evb;

	if ((thp = punch_new()) == NULL) {
		log_warnx("%s: punch_new", __func__);
		goto error;
	}

	if ((thp->ports = port_list_new(ports)) == NULL) {
		log_warnx("%s: port_list_new", __func__);
		goto error;
	}

	thp->cb = cb;

	LIST_FOREACH(p, thp->ports, entry) {

		p->arg = thp;

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;

		/* listen on every interfaces */
		if ((ret = getaddrinfo("0.0.0.0", p->str, &hints, &ai)) < 0) {
			log_warnx("%s: getaddrinfo: %s", __func__,
			    gai_strerror(ret));
			goto error;
		}

		if ((p->listener = evconnlistener_new_bind(ev_base,
		    listen_conn_cb, p, LEV_OPT_REUSEABLE, -1,
		    ai->ai_addr, ai->ai_addrlen)) == NULL) {
			log_warnx("%s: evconnlistener_new_bind", __func__);
			goto error;
		}
		evconnlistener_set_error_cb(p->listener, listen_error_cb);
		freeaddrinfo(ai);

		/* connect to ever port */
		if ((ret = getaddrinfo(ip, p->str, &hints, &ai)) < 0) {
			log_warnx("%s: getaddrinfo: %s", __func__,
			    gai_strerror(ret));
			goto error;
		}

		if ((sock = socket(ai->ai_family, SOCK_STREAM, IPPROTO_TCP)) < 0) {
			log_warn("%s: socket", __func__);
			goto error;
		}

		flag = 1;
		if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(flag)) < 0) {
			log_warn("%s: setsockopt", __func__);
			goto error;
		}

		if (evutil_make_socket_nonblocking(sock) < 0) {
			log_warn("%s: evutil_make_socket_nonblocking", __func__);
			goto error;
		}

		if ((p->client_bev = bufferevent_socket_new(ev_base, sock, 0)) == NULL) {
			log_warnx("%s: bufferevent_socket_new", __func__);
			goto error;
		}
		bufferevent_setcb(p->client_bev, NULL, NULL, peer_event_cb, thp);

		if (bufferevent_socket_connect(p->client_bev, ai->ai_addr, ai->ai_addrlen) < 0) {
			log_warnx("%s: bufferevent_socket_connected", __func__);
			goto error;
		}
		freeaddrinfo(ai);
	}

	return (thp);

error:
	freeaddrinfo(ai);
	punch_free(thp);
        return (NULL);
}

void
thp_punch_free(struct thp_punch *thp)
{
	if (thp == NULL)
		return;

	punch_stop(thp);
	punch_free(thp);

        return;
}
