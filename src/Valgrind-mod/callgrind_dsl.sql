#include "global.h"                 // typedef struct _thread_info thread_info,
		                    // CLG_(get_threads)(), Statistics, N_FN_ENTRIES
#include "pub_tool_threadstate.h"   // VG_N_THREADS

#define ApplicationThread_decl(X) thread_info *X;int i = 0
#define ApplicationThread_begin(X,Y,Z) X = Y[Z]
#define ApplicationThread_end(X,Y) X < Y
#define ApplicationThread_advance(X,Y,Z) X = Y[Z]

#define BasicBlockCostCenterRecursion_decl(X) BBCC *X;int i = 0 
#define BasicBlockCostCenterRecursion_begin(X,Y,Z) X = Y[Z]
#define BasicBlockCostCenterRecursion_end(X,Y) X < Y
#define BasicBlockCostCenterRecursion_advance(X,Y,Z) X = Y[Z]

#define BasicBlockCostCentersAll_decl(X) BBCC *X;int i = 0
#define BasicBlockCostCentersAll_begin(X,Y,Z) X = Y[Z]
#define BasicBlockCostCentersAll_end(X,Y) X < Y
#define BasicBlockCostCentersAll_advance(X,Y,Z) X = Y[Z]

#define JumpCallCostCentersAll_decl(X) jCC *X;int i = 0 
#define JumpCallCostCentersAll_begin(X,Y,Z) X = Y[Z]
#define JumpCallCostCentersAll_end(X,Y) X < Y
#define JumpCallCostCentersAll_advance(X,Y,Z) X = Y[Z]


static long get_cost_offset(FullCost *f, int index) {
  return (long)f[index];
};

$

CREATE STRUCT VIEW FnNodeV (
	codeLocationFunction TEXT FROM name,
	contextBaseNumber INT FROM number,
	codeLocationFile TEXT FROM file->name
)$

CREATE VIRTUAL TABLE FunctionNode
USING STRUCT VIEW FnNodeV
WITH REGISTERED C TYPE fn_node$


CREATE STRUCT VIEW BbV (
	codeLocationObject TEXT FROM obj->name,
	FOREIGN KEY(functionNodeId) FROM fn REFERENCES FunctionNode POINTER,
	memoryAddress BIGINT FROM bb_addr(tuple_iter),
	codeLocationLine INT FROM line,
	codeLocationObjectOffset BIGINT FROM offset,
	instructionCount INT FROM instr_count
)$
	
CREATE VIRTUAL TABLE BasicBlock
USING STRUCT VIEW BbV
WITH REGISTERED C TYPE BB$


CREATE STRUCT VIEW BbccV (
	executionCounterSum BIGINT FROM ecounter_sum,
	FOREIGN KEY(basicBlockId) FROM bb REFERENCES BasicBlock POINTER,
	FOREIGN KEY(basicBlockCostCenterRecursionId) FROM tuple_iter REFERENCES BasicBlockCostCenterRecursion POINTER,
	FOREIGN KEY(costId) FROM cost REFERENCES FullCost POINTER
)$


CREATE VIRTUAL TABLE BasicBlockCostCenterRecursion
USING STRUCT VIEW BbccV
WITH REGISTERED C TYPE BBCC*:BBCC*
USING LOOP for(BasicBlockCostCenterRecursion_begin(tuple_iter, base->rec_array, i); BasicBlockCostCenterRecursion_end(i, base->cxt->fn[0]->separate_recursions); BasicBlockCostCenterRecursion_advance(tuple_iter, base->rec_array, ++i))$

CREATE STRUCT VIEW JccV (
	callCounter BIGINT FROM call_counter,
	FOREIGN KEY(fromBasicBlockCostCenterId) FROM from REFERENCES BasicBlockCostCenter POINTER,
	FOREIGN KEY(toBasicBlockCostCenterId) FROM to REFERENCES BasicBlockCostCenter POINTER,
	FOREIGN KEY(costId) FROM cost REFERENCES FullCost POINTER
)$


CREATE VIRTUAL TABLE JumpCallCostCentersAll
USING STRUCT VIEW JccV
WITH REGISTERED C TYPE jcc_hash:jCC*
USING LOOP for(JumpCallCostCentersAll_begin(tuple_iter, base->table, i); JumpCallCostCentersAll_end(i, base->size); JumpCallCostCentersAll_advance(tuple_iter, base->table, ++i))$

CREATE VIRTUAL TABLE BasicBlockCostCentersAll
USING STRUCT VIEW BbccV
WITH REGISTERED C TYPE bbcc_hash:BBCC*
USING LOOP for(BasicBlockCostCentersAll_begin(tuple_iter, base->table, i); BasicBlockCostCentersAll_end(i, base->size); BasicBlockCostCentersAll_advance(tuple_iter, base->table, ++i)) {
               for(;tuple_iter != NULL; tuple_iter = tuple_iter->next)$
//                     for(BasicBlockCostCentersAll_begin(tuple_iter, tuple_iter->rec_array, h); BasicBlockCostCentersAll_end(h, tuple_iter->cxt->fn[0]->separate_recursions); BasicBlockCostCentersAll_advance(tuple_iter, tuple_iter->rec_array, ++h))$


CREATE VIRTUAL TABLE BasicBlockCostCenter
USING STRUCT VIEW BbccV
WITH REGISTERED C TYPE BBCC$



CREATE STRUCT VIEW FullCostV (
	//event TEXT FROM get
	instructionFetches BIGINT FROM {get_cost_offset(tuple_iter, 0)}
)$

CREATE VIRTUAL TABLE FullCost
USING STRUCT VIEW FullCostV
WITH REGISTERED C TYPE FullCost$
//WITH REGISTERED C TYPE FullCost:long
//USING LOOP for(FullCostVT_begin(tuple_iter, base, i); FullCostVT_end(i, CLG_(sets).full->size); FullCostVT_advance(tuple_iter, base, ++i))$

CREATE STRUCT VIEW ThreadV (
	FOREIGN KEY(jumpCallCostCentersAllId) FROM jccs REFERENCES JumpCallCostCentersAll,
	FOREIGN KEY(basicBlockCostCentersAllId) FROM bbccs REFERENCES BasicBlockCostCentersAll
)$

CREATE VIRTUAL TABLE ApplicationThread
USING STRUCT VIEW ThreadV
WITH REGISTERED C NAME all_threads
WITH REGISTERED C TYPE thread_info **:thread_info *
USING LOOP for(ApplicationThread_begin(tuple_iter, base, i); ApplicationThread_end(i, VG_N_THREADS); ApplicationThread_advance(tuple_iter, base, ++i))$


