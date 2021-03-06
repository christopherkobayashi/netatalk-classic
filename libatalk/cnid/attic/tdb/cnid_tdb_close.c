/*
 * $Id: cnid_tdb_close.c,v 1.3 2009-11-21 13:38:11 didg Exp $
 */

#include "config.h"

#ifdef CNID_BACKEND_TDB

#include "cnid_tdb.h"

void cnid_tdb_close(struct _cnid_db *cdb)
{
    struct _cnid_tdb_private *db;

    free(cdb->volpath);
    db = (struct _cnid_tdb_private *)cdb->_private;
    tdb_close(db->tdb_cnid);
    free(cdb->_private);
    free(cdb);
}

#endif
