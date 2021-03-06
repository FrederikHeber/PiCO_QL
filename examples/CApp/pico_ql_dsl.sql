//.cpp #include <vector>
#include "Monetary_System.h"
#include "MoneyArray.h"
#include "Money.h"
//.cpp using namespace std;
#define Money_decl(X) struct Money *X
#define MoneyList_decl(X) struct Money *X
#define MoneyArray_decl(X) struct Money *X; int mar = 0
#define MoneyArray_advance(X, Y, Z) X = Y[Z]
#define Number_decl(X) int *X; int num = 0
#define Number_advance(X, Y, Z) X = &Y[Z]
#define ENumber_decl(X) int *X; int enm = 0
$

CREATE UNION VIEW price (
       CASE price_mode
       WHEN 0 THEN main DOUBLE FROM main
       WHEN 1 THEN name TEXT FROM name
       WHEN 2 THEN sub INT FROM sub
)$

CREATE STRUCT VIEW Price (
       union_mode INT FROM price_mode,
       price union FROM p
)$

CREATE VIRTUAL TABLE Price
USING STRUCT VIEW Price
WITH REGISTERED C NAME price
WITH REGISTERED C TYPE price$

CREATE STRUCT VIEW GoldEquivalent (
       pound DOUBLE FROM pound,
       ounce DOUBLE FROM ounce,
       grain DOUBLE FROM grain,
       exchange_rate DOUBLE FROM exchange_rate
)$

CREATE VIRTUAL TABLE GoldEquivalent
USING STRUCT VIEW GoldEquivalent
WITH REGISTERED C TYPE goldEquivalent$

CREATE UNION VIEW weight (
       CASE weight_mode
       WHEN 0 THEN name TEXT FROM name
       WHEN 1 THEN kg DOUBLE FROM kg
       WHEN 2 THEN gr INT FROM gr
       WHEN 3 THEN FOREIGN KEY(goldEquivalent_id) 
       	    FROM g_weight REFERENCES GoldEquivalent
)$

CREATE STRUCT VIEW Money (
       name TEXT FROM name,
       price_mode INT FROM price_mode,
       price UNION FROM prc,
       weight_mode INT FROM weight_mode,
       weight UNION FROM wgt
)$

CREATE VIRTUAL TABLE Money
USING STRUCT VIEW Money
//WITH REGISTERED C NAME money
WITH REGISTERED C TYPE struct Money*
USING LOOP for(tuple_iter = base; tuple_iter != NULL; tuple_iter = tuple_iter->next)$
//.cpp C TYPE vector<Money>;

CREATE VIRTUAL TABLE MoneyList
USING STRUCT VIEW Money
WITH REGISTERED C NAME money
WITH REGISTERED C TYPE struct Money*
USING LOOP for(tuple_iter = base; tuple_iter != NULL; tuple_iter = tuple_iter->next)$ 
//.cpp C TYPE vector<Money>;

CREATE STRUCT VIEW MonetarySystem (
       nCurrency INT FROM nCurrency,
       FOREIGN KEY(currency) FROM root REFERENCES Money POINTER
)$

CREATE VIRTUAL TABLE MonetarySystem
USING STRUCT VIEW MonetarySystem
WITH REGISTERED C NAME monetary_system
WITH REGISTERED C TYPE Monetary_System$

CREATE VIRTUAL TABLE MoneyArray
USING STRUCT VIEW Money
WITH REGISTERED C NAME money_array
WITH REGISTERED C TYPE MoneyArray:struct Money *
USING LOOP for(tuple_iter = base->mArray[mar]; mar < base->mArraySize; MoneyArray_advance(tuple_iter, base->mArray, ++mar))$

CREATE STRUCT VIEW Number (
	number INT FROM tuple_iter
)$

CREATE VIRTUAL TABLE Number
USING STRUCT VIEW Number
WITH REGISTERED C NAME numbers
WITH REGISTERED C TYPE int*
USING LOOP for(tuple_iter = &base[num]; num < 8; Number_advance(tuple_iter, base, ++num))$


CREATE VIRTUAL TABLE ENumber
USING STRUCT VIEW Number
WITH REGISTERED C NAME int_array
WITH REGISTERED C TYPE MoneyArray:int*
USING LOOP for(tuple_iter = &base->intArray[enm]; enm < base->iArraySize; Number_advance(tuple_iter, base->intArray, ++enm))$
