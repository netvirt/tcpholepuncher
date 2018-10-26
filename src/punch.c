#include <stdlib.h>
#include <thp.h>

struct thp_punch {

};

struct thp_punch *
thp_punch_start(const char *address, const char *ports, thp_punch_cb cb,
	    void *data)
{
        return (NULL);
}

void
thp_punch_stop(struct thp_punch *p)
{
        return;
}
