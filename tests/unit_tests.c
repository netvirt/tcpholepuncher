#include <stdio.h>
#include <include/thp.h>

static void unit_test_log_setup();
static void unit_test_list_setup();

int main()
{
        unit_test_log_setup();
        unit_test_list_setup();

        return 0;
}

/* 
 * Unit tests for validating setup and libs
 */

void 
unit_test_log_setup()
{

}

void 
unit_test_list_setup()
{
        thp_list_test();
}
