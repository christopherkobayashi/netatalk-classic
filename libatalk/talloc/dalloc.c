/*
  Copyright (c) 2012 Frank Lahm <franklahm@gmail.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <inttypes.h>

#include <atalk/errchk.h>
#include <atalk/util.h>
#include <atalk/logger.h>
#include <atalk/talloc.h>
#include <atalk/bstrlib.h>
#include <atalk/dalloc.h>

/* Use dalloc_add_copy() macro, not this function */
int dalloc_add_talloc_chunk(DALLOC_CTX *dd, void *talloc_chunk, void *obj, size_t size)
{
    if (talloc_chunk) {
        /* Called from dalloc_add_copy() macro */
        dd->dd_talloc_array = talloc_realloc(dd,
                                             dd->dd_talloc_array,
                                             void *,
                                             talloc_array_length(dd->dd_talloc_array) + 1);
        memcpy(talloc_chunk, obj, size);
        dd->dd_talloc_array[talloc_array_length(dd->dd_talloc_array) - 1] = talloc_chunk;
    } else {
        /* Called from dalloc_add() macro */
        dd->dd_talloc_array = talloc_realloc(dd,
                                             dd->dd_talloc_array,
                                             void *,
                                             talloc_array_length(dd->dd_talloc_array) + 1);
        dd->dd_talloc_array[talloc_array_length(dd->dd_talloc_array) - 1] = obj;

    }
    return 0;
}

/* Get number of elements, returns 0 if the structure is empty or not initialized */
int dalloc_size(DALLOC_CTX *d)
{
    if (!d || !d->dd_talloc_array)
        return 0;
    return talloc_array_length(d->dd_talloc_array);
}

void *dalloc_get(const DALLOC_CTX *d, ...)
{
    EC_INIT;
    void *p = NULL;
    va_list args;
    const char *type;
    int elem;
    const char *elemtype;

    va_start(args, d);
    type = va_arg(args, const char *);

    if (STRCMP(type, !=, "DALLOC_CTX"))
        EC_FAIL;

    while (STRCMP(type, ==, "DALLOC_CTX")) {
        elem = va_arg(args, int);
        AFP_ASSERT(elem < talloc_array_length(d->dd_talloc_array));
        d = d->dd_talloc_array[elem];
        type = va_arg(args, const char *);
    }

    elem = va_arg(args, int);
    AFP_ASSERT(elem < talloc_array_length(d->dd_talloc_array));

    p = talloc_check_name(d->dd_talloc_array[elem], type);

    va_end(args);

EC_CLEANUP:
    if (ret != 0)
        p = NULL;
    return p;
}

void *dalloc_value_for_key(const DALLOC_CTX *d, ...)
{
    EC_INIT;
    void *p = NULL;
    va_list args;
    const char *type;
    int elem;
    const char *elemtype;
    char *s;

    va_start(args, d);
    type = va_arg(args, const char *);

    if (STRCMP(type, !=, "DALLOC_CTX"))
        EC_FAIL;

    while (STRCMP(type, ==, "DALLOC_CTX")) {
        elem = va_arg(args, int);
        AFP_ASSERT(elem < talloc_array_length(d->dd_talloc_array));
        d = d->dd_talloc_array[elem];
        type = va_arg(args, const char *);
    }

    for (elem = 0; elem + 1 < talloc_array_length(d->dd_talloc_array); elem += 2) {
        if (STRCMP(talloc_get_name(d->dd_talloc_array[elem]), !=, "char *")) {
            LOG(log_error, logtype_default, "dalloc_value_for_key: key not a string: %s",
                talloc_get_name(d->dd_talloc_array[elem]));
            EC_FAIL;
        }
        if (STRCMP((char *)d->dd_talloc_array[elem], ==, type)) {
            p = d->dd_talloc_array[elem + 1];
            break;
        }            
    }
    va_end(args);

EC_CLEANUP:
    if (ret != 0)
        p = NULL;
    return p;
}

char *dalloc_strdup(const void *ctx, const char *string)
{
    EC_INIT;
    char *p;

    EC_NULL( p = talloc_strdup(ctx, string) );
    talloc_set_name(p, "char *");

EC_CLEANUP:
    if (ret != 0) {
        if (p)
            talloc_free(p);
        p = NULL;
    }
    return p;
}

char *dalloc_strndup(const void *ctx, const char *string, size_t n)
{
    EC_INIT;
    char *p;

    EC_NULL( p = talloc_strndup(ctx, string, n) );
    talloc_set_name(p, "char *");

EC_CLEANUP:
    if (ret != 0) {
        if (p)
            talloc_free(p);
        p = NULL;
    }
    return p;
}