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


/* Executes test queries. */
int exec_tests() {
  FILE *f;
  f = fopen("pico_ql_test_current.txt", "w");

  /*
  int result, i = 0;
  char *q;

  q = "select...;";
  fprintf(f, "Query %i:\n %s\n\n", i++, q);
  result = pico_ql_exec_query(q, f, pico_ql_step_text);

  */

  //pico_ql_shutdown() instead of sqlite3_close(db);
  fclose(f);
  return SQLITE_DONE;
}
