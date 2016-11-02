/*
 *   Setup the user interface (html pages that the local 
 *   server (SWILL) will serve.
 *   Manage database connections, pass queries to SQLite 
 *   and format resultset for appropriate presentation.
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
#include <swill.h>
#include <string.h>
#include "pico_ql_interface.h"
#include "pico_ql_vt.h"
#include "pico_ql_test.h"
#include "pico_ql_swill_access_func.h"

/* Forwards  a query for execution to sqlite and 
 * presents the resultset of a query.
 */
int step_query(FILE *f, sqlite3_stmt *stmt, const char *response_type) {
  int col, result, rows = 0;
  if ((!strcmp(response_type, "text/html"))) {
    swill_fprintf(f, "<table>");
    swill_fprintf(f, "</tr>");
  } else
    swill_fprintf(f, "[");
  if (!strcmp(response_type, "text/html")) {
    for (col = 0; col < sqlite3_column_count(stmt); col++) {
      swill_fprintf(f, "<td><b>%s</td></b>",
		    sqlite3_column_name(stmt, col));
    }
    swill_fprintf(f, "</tr>");
  }
  while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
    rows++;
    if (!strcmp(response_type, "text/html"))
      swill_fprintf(f, "<tr>");
    else
      if (rows == 1)
        swill_fprintf(f, " {");
      else
        swill_fprintf(f, ", {");
    for (col = 0; col < sqlite3_column_count(stmt); col++) {
        if (!strcmp(response_type, "application/json")) {
          if (col != 0)
            swill_fprintf(f, ",");
        }
      switch (sqlite3_column_type(stmt, col)) {
      case 1:
        if (!strcmp(response_type, "text/html")) {
	  swill_fprintf(f, "<td><b>%li</b></td>",
		        (long)sqlite3_column_int64(stmt, col));
        } else {
	  swill_fprintf(f, " \"%s\" : %li",
                        sqlite3_column_name(stmt, col),
		        (long)sqlite3_column_int64(stmt, col));
        }
	break;
      case 2:
        if (!strcmp(response_type, "text/html")) {
	  swill_fprintf(f, "<td><b>%f</b></td>", 
		      sqlite3_column_double(stmt, col));
        } else {
	  swill_fprintf(f, " \"%s\" : %f",
                        sqlite3_column_name(stmt, col),
		        sqlite3_column_double(stmt, col));
        }
	break;
      case 3:
        if (!strcmp(response_type, "text/html")) {
	  swill_fprintf(f, "<td><b>%s</b></td>", 
		      sqlite3_column_text(stmt, col));
        } else {
	  swill_fprintf(f, " \"%s\" : \"%s\"",
                        sqlite3_column_name(stmt, col),
		        sqlite3_column_text(stmt, col));
        }
	break;
      case 4:
        if (!strcmp(response_type, "text/html")) {
	  swill_fprintf(f, "<td><b>%s</b></td>", 
		      (char *)sqlite3_column_blob(stmt, 
						  col));
        } else {
	  swill_fprintf(f, " \"%s\" : \"%s\"",
                        sqlite3_column_name(stmt, col),
		        (char *)sqlite3_column_blob(stmt, col));
        }
	break;
      case 5:
        if (!strcmp(response_type, "text/html")) {
	  swill_fprintf(f, "<td><b>(null)</td></b>");
        } else {
	  swill_fprintf(f, " \"%s\" : \"(null)\"",
                        sqlite3_column_name(stmt, col));
        }
	break;
      }
    }
    if (!strcmp(response_type, "text/html"))
      swill_fprintf(f, "</tr>");
    else
      swill_fprintf(f, " }");
  }
  if (!strcmp(response_type, "text/html")) {
    swill_fprintf(f,"</table>");
    swill_fprintf(f, "<br>");
    swill_fprintf(f, "<b>%i rows in result set.</b><br>", rows);
    swill_fprintf(f, "<br>");
  } else
    swill_fprintf(f, " ]");
  return result;
}

/* Calls step_query for query execution. 
 * Collects and acts on the result status of a query 
 * execution.
 */
int file_prep_exec(FILE *f, sqlite3_stmt *stmt, const char *response_type) {
  int result = 0;
  result = step_query(f, stmt, response_type);
  switch (result) {
  case SQLITE_DONE:
#ifdef PICO_QL_DEBUG
    if (!strcmp(response_type, "text/html"))
      swill_fprintf(f, "<b>DONE<br></b>");
    else
      swill_fprintf(f, "DONE\n");
#endif
    break;
  case SQLITE_OK:
#ifdef PICO_QL_DEBUG
    if (!strcmp(response_type, "text/html"))
      swill_fprintf(f, "<b>OK<br></b>");
    else
      swill_fprintf(f, "OK\n");
#endif
    break;
  case SQLITE_ERROR:
  if (!strcmp(response_type, "text/html"))
    swill_fprintf(f, "<b>SQL error or missing database.\n</b>");
  else
    swill_fprintf(f, "SQL error or missing database.\n");
    break;
  case SQLITE_MISUSE:
  if (!strcmp(response_type, "text/html"))
    swill_fprintf(f, "<b>Library used incorrectly.<br></b>");
  else
    swill_fprintf(f, "Library used incorrectly.\n");
    break;
  }
  return result;
}

// Takes care of query preparation and execution.
int prep_exec(FILE *f, sqlite3 *db, const char *q, const char *response_type) {
  sqlite3_stmt  *stmt;
  int result, prepare;
  if ((prepare = sqlite3_prepare_v2(db, q, -1, &stmt, 0)) == SQLITE_OK) {
    if (f) {
      result = file_prep_exec(f, stmt, response_type);
      fprintf(f, "\n");
    } else {
      /* Step only: queries with no resultset (check if 
       * table exists). For those queries preparation 
       * will always succeed.
       */ 
      result = sqlite3_step(stmt);
    }
  } else {
    if (f) {
      swill_fprintf(f, "Error in preparation of query: error no %i\n", prepare);
      swill_fprintf(f, "\nExtended error code %i.\n", sqlite3_extended_errcode(db));
      swill_fprintf(f, "\nExtended error message:\n%s\n\n", sqlite3_errmsg(db));
    }
    return prepare;
  }
  sqlite3_finalize(stmt);
  return result;
}


/* Calls the function that prints the PiCO QL error page (.html).
 */
void print_pico_ql_error_page(FILE *f) {
  error_page(f);
}

/* Calls the function that prints the PiCO QL logo (.PNG).
 */
void print_pico_ql_logo(FILE *f) {
  logo(f);
}

/* Builds the front page of the library's web interface, 
 * retrieves the database schema and promotes inputted 
 * queries to sqlite_engine.
 */
void app_index(FILE *f, sqlite3 *db) {
  swill_fprintf(f, "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"\n\"http://www.w3.org/TR/html4/loose.dtd\">"
		"<html>"
		"<head>"
		"<style type=\"text/css\">"
		"img{height:116px;width:109px;align:left}"
		".div_style{width:500px; border:2px solid; background-color:#ccc;margin:0px auto;margin-top:-40px;}"
		".top{border-bottom:1px solid;padding-top:10px;padding-left:10px;padding-bottom:2px;}"
		".middle{padding-top:5px;padding-left:9px; padding-bottom:5px;}"
		".bottom{border-top:1px solid;padding-top:5px; padding-bottom:10px; text-align:center;}"
		".button{height:7em; width:10em; font-size:22px;}"
		".style_text{font-family:Times New Roman;font-size:20px;}"
		".style_input{font-family:Times New Roman;font-size:15px;}"
		"table,td{border:1px double;}"
		".div_tbl{margin-top:20px;}"
		"p.aligned{text-align:left;}"
		"</style>"
		"</head>"
		"<body>");
  swill_fprintf(f, "<p><left><img src=\"pico_ql_logo.png\"></left>\n"
		"<div class=\"div_style\">"
		"<form action=\"serveQuery.html\" method=GET>"
		"<div class=\"top\">"
		"<span class=\"style_text\"><b>Input your SQL query:</b></span>"
		"</div>"
		"<div class=\"middle\">"
		"<textarea name=\"query\" cols=\"72\" rows=\"10\" class=\"style_input\"></textarea><br>"
  "</div>"
		"<div class=\"bottom\">"
		"<input type=\"submit\" value=\"Submit\" class=\"button\"></input>"
		"</div>"
		"</form>"
		"</div>"
		"<div class=\"div_tbl\">"
		"<span class=\"style_text\"><b>Your database schema is:</b></span>");
  prep_exec(f, db, "SELECT * FROM sqlite_master;", "text/html");
  swill_fprintf(f, "</div>"
		"<br>"
		"<p class=\"aligned\">");
  swill_fprintf(f,"<a href=\"");
  swill_printurl(f,"terminateConnection.html", "", 0);
  swill_fprintf(f,"\">[ Terminate Server Connection ]</a>"
		"</p>"
		"</body>"
		"</html>");
}


/* Builds the html page of the result set of a query 
 * along with the time it took to execute and the query 
 * itself.
 */
void serve_query(FILE *f, sqlite3 *db) {
  const char *query = "\0";
  char response_type[50];
  char *rt = swill_getheader("Http_Choose_Response_Type");
  if (rt)
    strcpy(response_type, rt);
  else
    strcpy(response_type, "text/html");   /* default */
  //swill_fprintf(f, "Response type should be: %s", response_type);
  if (!strcmp(response_type, "text/html"))
    swill_fprintf(f, "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"\n\"http://www.w3.org/TR/html4/loose.dtd\">"
		"<html>"
		"<head>"
		"<style type=\"text/css\">"
		"body{bgcolor=\"#ffffff\";}"
		"span.styled{color:blue;}"
		"table, td{border:1px double;}"
		"p.aligned{text-align:left;}"
		"</style>"
		"</head>"
		"<body>");
  if (swill_getargs("s(query)", &query)) {
    int rc = 0;
    clock_t start_clock,finish_clock;
    double c_time;
    start_clock = clock();
    int j = 0;
    if (!strcmp(response_type, "text/html")) {
      swill_fprintf(f, "<b>For SQL query: ");
      swill_fprintf(f, "<span class=\"styled\">%s</span><br><br>", query);
      swill_fprintf(f, "Result set is:</b><br><br>");
    }
    // j for debugging, execute the query multiple times.
    while (j < 1 && (rc = prep_exec(f, db, query, response_type)) == SQLITE_DONE) {
      j++;
    }
    if (rc == SQLITE_DONE) {
      finish_clock = clock();
      c_time = ((double)finish_clock - 
		(double)start_clock)/CLOCKS_PER_SEC;
      if (!strcmp(response_type, "text/html")) {
        swill_fprintf(f, "<b>\nQUERY SUCCESSFUL! </b><br><br>");
        swill_fprintf(f,"CPU time: <b>%f</b>s.<br><br>", c_time);
      }
    } else {
      if (!strcmp(response_type, "text/html")) {
        swill_fprintf(f, "<br><b>Extended error message:<br><b>%s</b><br><br>", sqlite3_errmsg(db));
        swill_fprintf(f, "Extended error code <b>%i.<br>Please advise </b><a href=\"", sqlite3_extended_errcode(db));
        swill_printurl(f, "pico_ql_error_page.html", "", 0);
        swill_fprintf(f,"\">SQLite error codes</a>.<br><br>");
      }
    }
    if (!strcmp(response_type, "text/html")) {
      swill_fprintf(f, "<p class=\"aligned\">");
      swill_fprintf(f, "<a href=\"");
      swill_printurl(f,"index.html", "", 0);
      swill_fprintf(f,"\">[ Input new Query ]</a>");
      swill_fprintf(f, "<a href=\"");
      swill_printurl(f,"terminateConnection.html", "", 0);
      swill_fprintf(f,"\">[ Terminate Server Connection ]</a>"
		  "</p>"
		  "</body>"
		  "</html>");
    }
  }
  clear_temp_structs();
}

// Terminates connection to the embedded web-server.
void terminate(FILE *f, sqlite3 *db) {
  char response_type[50];
  char *rt = swill_getheader("Http_Choose_Response_Type");
  if (rt)
    strcpy(response_type, rt);
  else
    strcpy(response_type, "text/html");   /* default */
  if (!strcmp(response_type, "text/html")) {
    swill_fprintf(f, "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"\n\"http://www.w3.org/TR/html4/loose.dtd\">"
		"<html>"
		"<head>"
		"<style type=\"text/css\">"
		"body{bgcolor=\"#ffffff\";}"
		"</style>"
		"</head>"
		"<body>");
  }
  deinit_selectors();
  sqlite3_close(db);
  if (!strcmp(response_type, "text/html")) {
    swill_fprintf(f, "<b>TERMINATED CONNECTION...</b>"
		"</body>"
		"</html>");
  }
  swill_close();
}

// Interface to the swill server functionality.
void call_swill(sqlite3 *db, int port_number) {
  swill_init(port_number);
  swill_handle("pico_ql_logo.png", print_pico_ql_logo, 0);
  swill_handle("pico_ql_error_page.html", print_pico_ql_error_page, 0);
  swill_handle("index.html", app_index, db);
  swill_handle("serveQuery.html", serve_query, db);
  swill_handle("terminateConnection.html", terminate, db);
  while (swill_serve()) {

  }
}

/* Executes the SQL CREATE queries, opens the sqlite 
 * database connection and calls swill or pico_ql_test 
 * depending on the compile flag TEST.
 */
int register_table(int argc,
		   int view_index, 
		   const char **q, 
		   const char **sqlite_names, 
		   int port_number,
		   sqlite3 *db) {
  /* This definition implicitly constraints a table name 
   * to 140 characters. It should be more than enough.
   */
  char sqlite_query[200];
  int re, i = 0;

#ifdef PICO_QL_DEBUG
  for (i = 0; i < argc; i++) {
    printf("\nquery to be executed: %s.\n", q[i]);
  }
#endif
  sqlite3_module *mod;
  mod = (sqlite3_module *)sqlite3_malloc(sizeof(sqlite3_module));
  fill_module(mod);
  int output = sqlite3_create_module(db, "PicoQL", mod, NULL);
  if (output == 1) 
    printf("Error while registering module\n");
#ifdef PICO_QL_DEBUG
  else if (output == 0) 
    printf("Module registered successfully\n");
#endif
  // sqlite3_create_function() calls
  for (i = 0; i < argc; i++) {
    char sqlite_type[10];
    if (i < view_index)
      strcpy(sqlite_type, "table");
    else
      strcpy(sqlite_type, "view");
    sprintf(sqlite_query, "SELECT * FROM sqlite_master WHERE type='%s' AND name='%s';", sqlite_type, sqlite_names[i]);
    if (prep_exec(NULL, db, (const char *)sqlite_query, NULL) != SQLITE_ROW) {
      re = prep_exec(NULL, db, (const char *)q[i], NULL);
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
  else {
    printf("Please visit http://localhost:%i to be served\n", port_number);
    call_swill(db, port_number);
  }
#else
  re = call_test(db);
#endif
  sqlite3_free(mod);
  return re;
}
