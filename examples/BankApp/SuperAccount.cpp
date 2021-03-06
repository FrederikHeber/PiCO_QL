/*
 *   Implement the member methods of SuperAccount class.
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

#include <math.h>
#include "SuperAccount.h"

SuperAccount::SuperAccount(const char *acc_no, float b, 
			   int i) {
    account_no = acc_no;
    balance = b;
    isbn = i;
}

SuperAccount::SuperAccount(const char *acc_no, float b) {
    account_no = acc_no;
    balance = b;
}

float SuperAccount::get_balance() const {
    return balance;
}

const char * SuperAccount::get_account_no() const {
    return account_no;
}

int SuperAccount::get_isbn() const {
    return isbn;
}

void SuperAccount::set_isbn(int i) {
    isbn = i;
}
double SuperAccount::get_rate() {
    return balance / 100;
}

double get_math_root(int i) {
  return sqrt(i);
}
