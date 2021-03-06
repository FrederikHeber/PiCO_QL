/*
 *   Manage database connections and pass queries to SQLite.
 *
 *   Copyright 2012 Marios Fragkoulis
 *
 *   Licensed under the Apache License, Version 2.0
 *   (the "License");you may not use this file except in
 *   compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in
 *   writing, software distributed under the License is
 *   distributed on an "AS IS" BASIS.
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 *   express or implied.
 *   See the License for the specific language governing
 *   permissions and limitations under the License.
 */

#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "pico_ql.h" /* pico_ql_exec_query() */

#ifdef PICO_QL_SWILL
#include "pico_ql_swill.h"
#endif
#include "pico_ql_vt.h"
#include "pico_ql_swill_access_func.h"

sqlite3 *db = NULL;

/* Executes the SQL CREATE queries, opens the sqlite 
 * database connection and calls swill or pico_ql_test 
 * depending on the compile flag TEST.
 */
int register_table(int argc,
		   int view_index, 
		   const char **q, 
		   const char **sqlite_names, 
		   int port_number,
		   sqlite3 *this_db) {
  /* This definition implicitly constraints a table name 
   * to 140 characters. It should be more than enough.
   */
  char sqlite_query[200];
  int re, i = 0, output;
  sqlite3_module *mod;

  db = this_db;
#ifdef PICO_QL_DEBUG
  for (i = 0; i < argc; i++) {
    printf("\nquery to be executed: %s.\n", q[i]);
  }
#endif
  mod = (sqlite3_module *)sqlite3_malloc(sizeof(sqlite3_module));
  fill_module(mod);
  output = sqlite3_create_module(db, "PicoQL", mod, NULL);
  if (output == 1) 
    printf("Error while registering module\n");
#ifdef PICO_QL_DEBUG
  else if (output == 0) 
    printf("Module registered successfully\n");
#endif
  /* sqlite3_create_function() calls */
  for (i = 0; i < argc; i++) {
    char sqlite_type[10];
    if (i < view_index)
      strcpy(sqlite_type, "table");
    else
      strcpy(sqlite_type, "view");
    sprintf(sqlite_query, "SELECT * FROM sqlite_master WHERE type='%s' AND name='%s';", sqlite_type, sqlite_names[i]);
    if (pico_ql_exec_query((const char *)sqlite_query, NULL, pico_ql_step_mute) != SQLITE_ROW) {
      re = pico_ql_exec_query((const char *)q[i], NULL, pico_ql_step_mute);
#ifdef PICO_QL_DEBUG
      printf("Query %s returned: %i\n", q[i], re);
#endif
      if ((re = sqlite3_extended_errcode(db)) != SQLITE_DONE && re != SQLITE_OK) {
	printf("Extended error code: %i.\n", sqlite3_extended_errcode(db));
	printf("Extended error message:\n%s.\n", sqlite3_errmsg(db));
	return re;
      }
    }
  }
  start_serving();
#ifndef PICO_QL_TEST
  if (port_number == -1)
    return re;
#ifdef PICO_QL_SWILL
  else {
    printf("Please visit http://localhost:%i to be served\n", port_number);
    init_pico_ql_swill(port_number);
  }
#endif
#else
  re = exec_tests();
#endif
  sqlite3_free(mod);
  return re;
}
