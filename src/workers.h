/*
 *   Declare interface to workers.cpp
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

#ifndef WORKERS_H
#define WORKERS_H

#include "sqlite3.h"
#include "stl_search.h"

int realloc_resultset(sqlite3_vtab_cursor *cur);
int compare(int dstr_value, int op, int value);
int compare(long int dstr_value, int op, long int value);
int compare(double dstr_value, int op, double value);
int compare(const void *dstr_value, int op, 
	    const void *value);
int compare(const unsigned char *dstr_value, int op,
	    const unsigned char *value);
int compare_res(int count, stlTableCursor *stcsr, 
		int *temp_res);
void check_alloc(const char *constr, int &op, int &iCol);

#endif