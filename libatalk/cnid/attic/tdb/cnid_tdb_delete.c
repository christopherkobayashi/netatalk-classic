/*
 * $Id: cnid_tdb_delete.c,v 1.4 2009-11-20 19:25:05 didg Exp $
 *
 * Copyright (c) 1999. Adrian Sun (asun@zoology.washington.edu)
 * All Rights Reserved. See COPYRIGHT.
 *
 * cnid_delete: delete a CNID from the database 
 */

#include "config.h"

#ifdef CNID_BACKEND_TDB

#include "cnid_tdb.h"

int cnid_tdb_delete(struct _cnid_db *cdb, const cnid_t id)
{
    struct _cnid_tdb_private *db;
    TDB_DATA key, data;

    if (!cdb || !(db = cdb->_private) || !id) {
        return -1;
    }
    memset(&key, 0, sizeof(key));
    memset(&data, 0, sizeof(data));

    key.dptr  = (char *)&id;
    key.dsize = sizeof(cnid_t);
    data = tdb_fetch(db->tdb_cnid, key);
    if (!data.dptr)
    {
        return 0;
    }
    
    tdb_delete(db->tdb_cnid, key); 

    key.dptr = data.dptr +CNID_DEVINO_OFS;
    key.dsize = CNID_DEVINO_LEN;
    tdb_delete(db->tdb_devino, key); 

    key.dptr = (char *)data.dptr +CNID_DID_OFS;
    key.dsize = data.dsize -CNID_DID_OFS;
    tdb_delete(db->tdb_didname, key); 

    free(data.dptr);
    return 0;
}

#endif 
