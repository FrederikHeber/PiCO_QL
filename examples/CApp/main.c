#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Monetary_System.h"
#include "Money.h"
#include "MoneyArray.h"

#ifndef PICO_QL_SINGLE_THREADED
#include <pthread.h>
#endif
#include "pico_ql.h"

/* .cpp
#include <vector>
using namespace std;
using namespace picoQL;
*/

int main() {
  int num[] = {1, 2, 3, 4, 5, 6, 7, 8};
  pico_ql_register_data(num, "numbers");

  Monetary_System ms;
  MoneyArray ma;
  ma.intArray = num;
  ma.iArraySize = 8;
  pico_ql_register_data(&ma, "int_array");
  ma.mArraySize = 3;
  ma.mArray = (struct Money **)malloc(sizeof(struct Money *) * ma.mArraySize);
  struct Money* M = (struct Money *)malloc(sizeof(struct Money));
  ma.mArray[0] = M;
  ms.root = M;
  ms.nCurrency = 1;
  //.cpp:  vector<money> mon;
  strcpy((char *)M->name, "Euro");
  M->prc.sub = 9;
  M->price_mode = 2;
  M->prc.main = 35.4;
  M->price_mode = 0;
  strcpy(M->prc.name, "Euro_price");
  M->price_mode = 1;

  price prc;
  prc.p.main = 12.5;
  prc.price_mode = 0;
  pico_ql_register_data(&prc, "price");

  M->wgt.gr = 612;
  M->weight_mode = 2;
  M->wgt.kg = 23.6;
  M->weight_mode = 1;
  strcpy(M->wgt.name, "euro_wgt");
  M->weight_mode = 0;
  M->wgt.g_weight.pound = 9.8;
  M->wgt.g_weight.ounce = 25.8;
  M->wgt.g_weight.grain = 5013.1;
  M->wgt.g_weight.exchange_rate = 1.36;
  M->weight_mode = 3;
  struct Money* N = (struct Money *)malloc(sizeof(struct Money));
  M->next = N;
  ma.mArray[1] = N;
  ms.nCurrency = 2;
  //.cpp:  vector<money> mon;
  strcpy((char *)N->name, "Pound");
  N->prc.sub = 43;
  N->price_mode = 2;
  N->prc.main = 78.1;
  N->price_mode = 0;
  strcpy(N->prc.name, "pnd_price");
  N->price_mode = 1;

  N->wgt.gr = 432;
  N->weight_mode = 2;
  N->wgt.kg = 12.3;
  N->weight_mode = 1;
  strcpy(N->wgt.name, "pound_wgt");
  N->weight_mode = 0;
  N->wgt.g_weight.pound = 11.3;
  N->wgt.g_weight.ounce = 53.5;
  N->wgt.g_weight.grain = 9045.2;
  N->wgt.g_weight.exchange_rate = 0.49;
  N->weight_mode = 3;
  N->next = NULL;
  struct Money* O = (struct Money *)malloc(sizeof(struct Money));
  N->next = O;
  ma.mArray[2] = O;
  ms.nCurrency = 3;
  //.cpp:  vector<money> mon;
  strcpy((char *)O->name, "Dollar");
  O->prc.sub = 23;
  O->price_mode = 2;
  O->prc.main = 61.3;
  O->price_mode = 0;
  strcpy(O->prc.name, "dlr_price");
  O->price_mode = 1;

  O->wgt.gr = 321;
  O->weight_mode = 2;
  O->wgt.kg = 6.7;
  O->weight_mode = 1;
  strcpy(O->wgt.name, "dlr_wgt");
  O->weight_mode = 0;
  O->wgt.g_weight.pound = 5.4;
  O->wgt.g_weight.ounce = 43.9;
  O->wgt.g_weight.grain = 5021.4;
  O->wgt.g_weight.exchange_rate = 0.32;
  O->weight_mode = 3;
  O->next = NULL;
  //.cpp:  mon.push_back(O);
  pico_ql_register_data(M, "money");
  pico_ql_register_data(&ms, "monetary_system");
  pico_ql_register_data(&ma, "money_array");

  int re;
#ifndef PICO_QL_SINGLE_THREADED
  void *exit_status = NULL;
  pthread_t t;
  re = pico_ql_init(NULL, 0, 8080, &t);
  pthread_join(t, &exit_status);
#else
  re = pico_ql_init(NULL, 0, 8080, NULL);
#endif

  if (re)
    fprintf(stderr, "pico_ql_init() failed with code %d", re);

  /*FILE *f = fopen("CApp_resultset", "w");
  pico_ql_exec_query("select * from Money;", f, pico_ql_step_text);
  fclose(f);
  */

  pico_ql_shutdown();

  printf("Money M Price main: %f\n", M->prc.main);
  printf("Money N Price main: %f\n", N->prc.main);
  printf("Money O Price main: %f\n", O->prc.main);
  //  printf("Money Price sub: %i\n", O->prc.sub);

  free(M);
  free(N);
  free(O);
  free(ma.mArray);
  return 0;
}
