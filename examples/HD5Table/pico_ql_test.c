/*
 *   Set up local query preparation and execution 
 *   environment for testing purposes.
 *   Execute the queries user has included and write the 
 *   output in test_current.txt.
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
 *   distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 *   express or implied.
 *   See the License for the specific language governing
 *   permissions and limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico_ql_test.h"
#include "pico_ql_internal.h"

/* Takes care of query preparation and execution. 
 * Writes results to file.
 */
int test_prep_exec(FILE *f, sqlite3 *db, const char *q) {
  sqlite3_stmt  *stmt;
  int result, col, prepare;
  if ((prepare = sqlite3_prepare_v2(db, q, -1, &stmt, 0)) == SQLITE_OK) {
    fprintf(f,"Statement prepared.\n");
    for (col = 0; col < sqlite3_column_count(stmt); col++) {
      fprintf(f, "%s ", sqlite3_column_name(stmt, col));
    }
    fprintf(f, "\n");
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
      fprintf(f, "\n");
      for (col = 0; col < sqlite3_column_count(stmt); col++) {
	switch (sqlite3_column_type(stmt, col)) {
	case 1:
	  fprintf(f, "%i ", sqlite3_column_int(stmt, col));
	  break;
	case 2:
	  fprintf(f, "%f ", 
		  sqlite3_column_double(stmt, col));
	  break;
	case 3:
	  fprintf(f, "%s ", 
		  sqlite3_column_text(stmt, col));
	  break;
	case 4:
	  fprintf(f, "%s ", 
		  (char *)sqlite3_column_blob(stmt, col));
	  break;
	case 5:
	  fprintf(f, "(null) ");
	  break;
	}
      }
    }
    switch (result) {
    case SQLITE_DONE:
      fprintf(f, "\n\nDone\n");
      break;
    case SQLITE_OK:
      fprintf(f, "\n\nOK\n");
      break;
    case SQLITE_ERROR:
      fprintf(f, "\n\nSQL error or missing database\n");
      break;
    case SQLITE_MISUSE:
      fprintf(f, "\n\nLibrary used incorrectly\n");
      break;
    default:
      fprintf(f, "\n\nError code: %i.\nPlease advise Sqlite error codes (http://www.sqlite.org/c3ref/c_abort.html)", result);
    }
    fprintf(f, "\n");
  } else {
    fprintf(f, "Error in preparation of query: error no %i\n", prepare);
    fprintf(f, "\nExtended error code %i.\n", sqlite3_extended_errcode(db));
    fprintf(f, "\nExtended error message:\n%s\n\n", sqlite3_errmsg(db));
    return prepare;
  }
  deinit_temp_structs();
  sqlite3_finalize(stmt);
  return result;
}

/* Executes test queries. */
int call_test(sqlite3 *db) {
  FILE *f;
  f = fopen("pico_ql_test_current.txt", "w");
  int result, i = 0;
  char *q;

  q = "SELECT * FROM Sensor;";
  fprintf(f, "Query %i:\n %s\n\n", i++, q);
  result = test_prep_exec(f, db, q);

  deinit_vt_selectors();
  sqlite3_close(db);
  fclose(f);
  if (system("./pico_ql_diff_test.sh")) {
    printf("Invoking pico_ql_diff_test script failed.\n");
    exit(1);
  }
  return SQLITE_DONE;
}
