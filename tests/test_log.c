#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <thp.h>

#include "log.h"

const char *msg_01 = "test";

void
log_01(const char *msg)
{
	if (strncmp(msg, msg_01, strlen(msg_01)) != 0)
		exit(-1);
}

int main()
{
	thp_log_setcb(log_01);
	log_warn("%s", msg_01);

        return 0;
}
