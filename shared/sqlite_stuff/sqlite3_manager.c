#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "deobfuscate.h"
#include "log.h"
#include "sqlite3_manager.h"

static unsigned char libsqlite[] = "\x51\x0d\x44\x86\x22\x38\x22\x3f\x4c\x44\x86\x47\x48\x4d\x86\x47\x48\x4d\x22\x20\x47\x48\x3f\x4c\x81\x22\x46"; // "/system/lib/libsqlite.so"
static unsigned char open_str[] = "\x8a\x33\xb5\x1f\x1d\x2a\x25\x02\x11\xdf\x3b\x2b\x1e\x11\x24"; // "sqlite3_open"
static unsigned char exec_str[] = "\xaf\x79\xda\x2c\x2e\x45\x46\x2d\x3a\x6c\x10\x3a\x29\x3a\x3c"; // "sqlite3_exec"
static unsigned char close_str[] = "\xc1\xf6\x3a\x5e\x50\x55\x68\x5d\x6c\x1e\x62\x6e\x55\x52\x5e\x6c"; // "sqlite3_close"


typedef int (*sqlite3_open_t) (const char *, unsigned int *);
typedef int (*sqlite3_exec_t) (unsigned int, const char *, void *, void *, char *);
typedef int (*sqlite3_close_t) (unsigned int *);

void *sql_handle;
sqlite3_open_t  sqlite3_open;
sqlite3_exec_t  sqlite3_exec; 
sqlite3_close_t sqlite3_close; 
int init = 0;


// Initialize using dlopen and dlsyms
int sqlite_manager_init(void) {
  
  sql_handle = dlopen(deobfuscate(libsqlite), RTLD_LAZY);
  if(!sql_handle) {
    LOGD("[SQLManager] Unable to open the sqlite library.\n");
    return 0;
  }
  
  sqlite3_open  = (sqlite3_open_t) dlsym(sql_handle, deobfuscate(open_str));
  sqlite3_exec  = (sqlite3_exec_t) dlsym(sql_handle, deobfuscate(exec_str));
  sqlite3_close = (sqlite3_close_t) dlsym(sql_handle, deobfuscate(close_str));

  if(!sqlite3_open || !sqlite3_exec || !sqlite3_close) {
    LOGD("[SQLManager] Required symbols not found.\n");
    return 0;
  }

  LOGD("[SQLManager] Init completed.\n");
  init = 1;
  return init;
}


// Exec a query on a db
int sqlite_manager_query(char *db_path, char *query) { 
  unsigned int db = 0;
  int rc;
  char *sql;

  if(!init) {
    LOGD("[SQLManager] SqlManager not initialized.\n");
    return 0;
  }

  rc = sqlite3_open(db_path, &db);
  if(rc) {
    LOGD("[SQLManager] Error during open.\n");
    return 0;
  }

  rc = sqlite3_exec(db, query, NULL, 0, NULL);
  if(rc) {
    LOGD("[SQLManager] Error during exec.\n");
    return 0;
  }
    
  sqlite3_close(&db);

  LOGD("[SQLManager] Query performed.\n");

  return 1;

}
