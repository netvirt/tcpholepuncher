#include <stdlib.h>
#include <stdio.h>
#include <include/thp.h>

#include <sys/queue.h>
#include "log.h"

#include "thp_init.h"

void
thp_list_test(){
        struct entry {
                int a, b, c;
                LIST_ENTRY(entry) entries;
        } *i1, *i2, *ip, *ip_temp;
        LIST_HEAD(entry_list, entry) head;
        LIST_INIT(&head);

        i1 = malloc(sizeof(struct entry));
        i1->a = 2;
        i1->b = 13;
        i1->c = -1;
        LIST_INSERT_HEAD(&head, i1, entries);
        i2 = malloc(sizeof(struct entry));
        i2->a = 5;
        i2->b = 67;
        i2->c = 99;
        LIST_INSERT_HEAD(&head, i2, entries);
        LIST_FOREACH(ip, &head, entries)
                printf("%d\n%d\n%d\n", ip->a, ip->b, ip->c);

         while (!LIST_EMPTY(&head)) {            /* List Deletion. */
                ip = LIST_FIRST(&head);
                LIST_REMOVE(ip, entries);
                free(ip);
        }
}