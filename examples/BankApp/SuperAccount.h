/*
 *   Declare the interface to SuperAccount class.
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

#ifndef SUPERACCOUNT_H
#define SUPERACCOUNT_H

double get_math_root(int i);

class SuperAccount {

 protected:
  const char *account_no;
  float balance;
  int isbn;

 public:
  SuperAccount(const char *acc_no, float b, int i);
  SuperAccount(const char *acc_no, float b);
  float get_balance() const;
  const char * get_account_no() const;
  int get_isbn() const;
  void set_isbn(int i);
  virtual double get_rate();

};

#endif
