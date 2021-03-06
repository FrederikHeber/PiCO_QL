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
 *   distributed on an "AS IS" BASIS.
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 *   express or implied.
 *   See the License for the specific language governing
 *  permissions and limitations under the License.
 */

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <sstream>
using namespace std;

#include "pico_ql.h"

namespace picoQL {

int exec_tests() {
  stringstream s;
  fstream fs;
  fs.open("chess_test_current.txt", fstream::out);
  int i = 1;
  char q[200];
  fs << s.str();
  s.str("");

  strcpy(q, "select rownum from ChessBoard;");
  fs << "Query " << i++ << ":\n " << q << endl << endl;
  exec_query(q, s, step_text);
  fs << s.str();
  s.str("");

  strcpy(q, "select * from ChessRow;");
  fs << "Query " << i++ << ":\n " << q << endl << endl;
  exec_query(q, s, step_text);
  fs << s.str();
  s.str("");

  strcpy(q, "select ChessBoard.rownum,ChessRow.rownum,color,name from ChessBoard,ChessRow where ChessRow.base=ChessBoard.row_id");
  fs << "Query " << i++ << ":\n " << q << endl << endl;
  exec_query(q, s, step_text);
  fs << s.str();
  s.str("");

  strcpy(q, "select ChessBoard.rownum,ChessRow.rownum,color,name from ChessBoard,ChessRow where ChessRow.base=ChessBoard.row_id and color=\"white\" and name=\"bishop\";");
  fs << "Query " << i++ << ":\n " << q << endl << endl;
  exec_query(q, s, step_text);
  fs << s.str();
  s.str("");

  strcpy(q, "select * from ReducedBoard;");
  fs << "Query " << i++ << ":\n " << q << endl << endl;
  exec_query(q, s, step_text);
  fs << s.str();
  s.str("");

  //deinit_vt_selectors();
  //sqlite3_close(db);
  fs.close();
  return SQLITE_DONE;
}

} // namespace picoQL
