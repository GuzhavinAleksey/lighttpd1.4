#include "first.h"

#undef NDEBUG
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "mod_access.c"

static void test_mod_access_check(void) {
    array *allow    = array_init(0);
    array *deny     = array_init(0);
    buffer *urlpath = buffer_init();
    int lc = 0;

    /* empty allow and deny lists */
    buffer_copy_string_len(urlpath, CONST_STR_LEN("/"));
    assert(1 == mod_access_check(allow, deny, urlpath, lc));

    array_insert_value(deny, CONST_STR_LEN("~"));
    array_insert_value(deny, CONST_STR_LEN(".inc"));

    /* deny */
    buffer_copy_string_len(urlpath, CONST_STR_LEN("/index.html~"));
    assert(0 == mod_access_check(allow, deny, urlpath, lc));
    lc = 1;
    buffer_copy_string_len(urlpath, CONST_STR_LEN("/index.INC"));
    assert(0 == mod_access_check(allow, deny, urlpath, lc));
    lc = 0;

    array_insert_value(allow, CONST_STR_LEN(".txt"));
    array_insert_value(deny, CONST_STR_LEN(".txt"));/* allow takes precedence */

    /* explicitly allowed */
    buffer_copy_string_len(urlpath, CONST_STR_LEN("/ssi-include.txt"));
    assert(1 == mod_access_check(allow, deny, urlpath, lc));
    lc = 1;
    buffer_copy_string_len(urlpath, CONST_STR_LEN("/ssi-include.TXT"));
    assert(1 == mod_access_check(allow, deny, urlpath, lc));
    lc = 0;

    /* allow not empty and urlpath not explicitly allowed */
    buffer_copy_string_len(urlpath, CONST_STR_LEN("/cgi.pl"));
    assert(0 == mod_access_check(allow, deny, urlpath, lc));

    array_free(allow);
    array_free(deny);
    buffer_free(urlpath);
}

int main (void) {
    test_mod_access_check();

    return 0;
}

/*
 * stub functions
 */

int config_plugin_values_init(server *srv, void *p_d, const config_plugin_keys_t *cpk, const char *mname) {
    UNUSED(srv);
    UNUSED(p_d);
    UNUSED(cpk);
    UNUSED(mname);
    return 0;
}

int config_check_cond(request_st *r, int context_ndx) {
    UNUSED(r);
    UNUSED(context_ndx);
    return 0;
}
