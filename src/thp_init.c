#include <include/thp.h>

#include "log.h"

#include "thp_init.h"

void
thp_print(){
    log_init(4);
    printf("%s\n", MSG);
    printf("%s\n", ECHO);
    fatal("FATALX");
}