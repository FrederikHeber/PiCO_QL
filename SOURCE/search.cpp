#include "search.h"
#include <string>
#include <vector>
#include <vector>
#include "Truck.h"

using namespace std;


    

int get_datastructure_size(void *st){
    stlTable *stl = (stlTable *)st;
    if( !strcmp(stl->zName, "Customers") ){
	
        vector<Customer*> *any_dstr = (vector<Customer*> *)stl->data;
        return ((int)any_dstr->size());
    }
    if( !strcmp(stl->zName, "Trucks") ){
        vector<Truck*> *any_dstr = (vector<Truck*> *)stl->data;
        return ((int)any_dstr->size());
    }
}


int traverse(long int dstr_value, int op, sqlite3_int64 value){
    switch( op ){
    case 0:
        return dstr_value<value;
    case 1:
        return dstr_value<=value;
    case 2:
        return dstr_value==value;
    case 3:
        return dstr_value>=value;
    case 4:
        return dstr_value>value;
    }
}

int traverse(int dstr_value, int op, int value){
    switch( op ){
    case 0:
        return dstr_value<value;
    case 1:
        return dstr_value<=value;
    case 2:
        return dstr_value==value;
    case 3:
        return dstr_value>=value;
    case 4:
        return dstr_value>value;
    }
}



int traverse(double dstr_value, int op, double value){
    switch( op ){
    case 0:
        return dstr_value<value;
    case 1:
        return dstr_value<=value;
    case 2:
        return dstr_value==value;
    case 3:
        return dstr_value>=value;
    case 4:
        return dstr_value>value;
    }
}

// compare addresses???
int traverse(const void *dstr_value, int op, const void *value){
    switch( op ){
    case 0:
        return dstr_value<value;
    case 1:
        return dstr_value<=value;
    case 2:
        return dstr_value==value;
    case 3:
        return dstr_value>=value;
    case 4:
        return dstr_value>value;
    }
}


int traverse(const unsigned char *dstr_value, int op, 
    		   const unsigned char *value){
    switch( op ){
    case 0:
        return strcmp((const char *)dstr_value,(const char *)value)<0;
    case 1:
        return strcmp((const char *)dstr_value,(const char *)value)<=0;
    case 2:
        return strcmp((const char *)dstr_value,(const char *)value)==0;
    case 3:
        return strcmp((const char *)dstr_value,(const char *)value)>=0;
    case 4:
        return strcmp((const char *)dstr_value,(const char *)value)>0;
    }
}




void Customers_search(void *stc, char *constr, sqlite3_value *val){
    sqlite3_vtab_cursor *cur = (sqlite3_vtab_cursor *)stc;
    stlTable *stl = (stlTable *)cur->pVtab;
    stlTableCursor *stcsr = (stlTableCursor *)stc;
    vector<Customer*> *any_dstr;
    any_dstr = (vector<Customer*> *)stl->data;
    vector<Customer*>:: iterator iter;
    
//    Type value;
    int op, count = 0;
// val==NULL then constr==NULL also
        switch( constr[0] - 'A' ){
        case 0:
            op = 0;
            break;
        case 1:
            op = 1;
            break;
        case 2:
            op = 2;
            break;
        case 3:
            op = 3;
            break;
        case 4:
            op = 4;
            break;
        case 5:
            op = 5;
            break;
        default:
            break;
        }

        int iCol, arraySize; 
// size, init_res_max_size, current, isEof, nByte;
	int *res;
	sqlite3_vtab_cursor *vtab_c;
        iCol = constr[1] - 'a' + 1;
            switch( iCol ){
// i=0. search using PK?memory location?or no PK?
// no can't do.PK will be memory location. PK in every table
// PK search
            case 0: 
		stl->data = (void *)sqlite3_value_int64(val);
		any_dstr = (vector<Customer*> *)stl->data;
		arraySize=get_datastructure_size(stl);
/*
		vtab_c = stcsr->vtab;
		nByte = stcsr->nByte;
		size = stcsr->size;
		current = stcsr->current;
		isEof = stcsr->isEof;
		init_res_max_size = stcsr->init_res_max_size;
*/
		res = (int *)sqlite3_realloc(stcsr->resultSet, sizeof(int) * arraySize);
		if (res!=NULL){
		    stcsr->resultSet = res;
		    memset(stcsr->resultSet, -1, 
			   sizeof(int) * arraySize);
		}else{
		    free(res);
		    printf("Error (re)allocating memory\n");
		    exit(1);
		}
                for(int i=0;i<(int)any_dstr->size();i++){
		    stcsr->resultSet[count++] = i;
                    iter++;
                }
                stcsr->size += count;
		break;
            case 1:
                iter=any_dstr->begin();
                for(int i=0;i<(int)any_dstr->size();i++){
                    if( traverse((*iter)->get_demand(), op, sqlite3_value_int(val)) )
                        stcsr->resultSet[count++] = i;
                    iter++;
                }
                stcsr->size += count;
                break;
            case 2:
                iter=any_dstr->begin();
                for(int i=0;i<(int)any_dstr->size();i++){
                    if( traverse((const unsigned char *)(*iter)->get_code().c_str(), op, sqlite3_value_text(val)) )
                        stcsr->resultSet[count++] = i;
                    iter++;
                }
                stcsr->size += count;
                break;
            case 3:
                iter=any_dstr->begin();
                for(int i=0;i<(int)any_dstr->size();i++){
                    if( traverse((*iter)->get_serviced(), op, sqlite3_value_int(val)) )
                        stcsr->resultSet[count++] = i;
                    iter++;
                }
                stcsr->size += count;
                break;
            case 4:
                iter=any_dstr->begin();
                for(int i=0;i<(int)any_dstr->size();i++){
                    if( traverse((*iter)->get_pickdemand(), op, sqlite3_value_int(val)) )
                        stcsr->resultSet[count++] = i;
                    iter++;
                }
                stcsr->size += count;
                break;
            case 5:
                iter=any_dstr->begin();
                for(int i=0;i<(int)any_dstr->size();i++){
                    if( traverse((*iter)->get_starttime(), op, sqlite3_value_int(val)) )
                        stcsr->resultSet[count++] = i;
                    iter++;
                }
                stcsr->size += count;
                break;
            case 6:
                iter=any_dstr->begin();
                for(int i=0;i<(int)any_dstr->size();i++){
                    if( traverse((*iter)->get_servicetime(), op, sqlite3_value_int(val)) )
                        stcsr->resultSet[count++] = i;
                    iter++;
                }
                stcsr->size += count;
                break;
            case 7:
                iter=any_dstr->begin();
                for(int i=0;i<(int)any_dstr->size();i++){
                    if( traverse((*iter)->get_finishtime(), op, sqlite3_value_int(val)) )
                        stcsr->resultSet[count++] = i;
                    iter++;
                }
                stcsr->size += count;
                break;
            case 8:
                iter=any_dstr->begin();
                for(int i=0;i<(int)any_dstr->size();i++){
                    if( traverse((*iter)->get_revenue(), op, sqlite3_value_int(val)) )
                        stcsr->resultSet[count++] = i;
                    iter++;
                }
                stcsr->size += count;
                break;
// more datatypes and ops exist
         
	    }
}


void Trucks_search(void *stc, char *constr, sqlite3_value *val){
    sqlite3_vtab_cursor *cur = (sqlite3_vtab_cursor *)stc;
    stlTable *stl = (stlTable *)cur->pVtab;
    stlTableCursor *stcsr = (stlTableCursor *)stc;
    vector<Truck*> *any_dstr = (vector<Truck*> *)stl->data;
    vector<Truck*>:: iterator iter;
//    Type value;
    int op, count = 0;
// val==NULL then constr==NULL also
    if ( val==NULL ){
        for (int j=0; j<get_datastructure_size((void *)stl); j++){
            stcsr->resultSet[j] = j;
            stcsr->size++;
        }
    }else{
        switch( constr[0] - 'A' ){
        case 0:
            op = 0;
            break;
        case 1:
            op = 1;
            break;
        case 2:
            op = 2;
            break;
        case 3:
            op = 3;
            break;
        case 4:
            op = 4;
            break;
        case 5:
            op = 5;
            break;
        default:
            break;
        }

        int iCol;
        iCol = constr[1] - 'a' + 1;
        char *colName = stl->azColumn[iCol];
// handle colName
            switch( iCol ){
// i=0. search using PK?memory location?or no PK?
// no can't do.PK will be memory location. PK in every table
// PK search
            case 0: 
                iter=any_dstr->begin();
                for(int i=0; i<(int)any_dstr->size(); i++){
                    if( traverse((long int)&(*iter), op, sqlite3_value_int64(val)) )
                        stcsr->resultSet[count++] = i;
                    iter++;
                }
                stcsr->size += count;
                break;
            case 1:
                iter=any_dstr->begin();
                for(int i=0;i<(int)any_dstr->size();i++){
                    if( traverse((*iter)->get_cost(), op, sqlite3_value_double(val)) )
                        stcsr->resultSet[count++] = i;
                    iter++;
                }
                stcsr->size += count;
                break;
            case 2:
                iter=any_dstr->begin();
                for(int i=0;i<(int)any_dstr->size();i++){
                    if( traverse((*iter)->get_delcapacity(), op, sqlite3_value_int(val)) )
                        stcsr->resultSet[count++] = i;
                    iter++;
                }
                stcsr->size += count;
                break;
            case 3:
                iter=any_dstr->begin();
                for(int i=0;i<(int)any_dstr->size();i++){
                    if( traverse((*iter)->get_pickcapacity(), op, sqlite3_value_int(val)) )
                        stcsr->resultSet[count++] = i;
                    iter++;
                }
                stcsr->size += count;
                break;
            case 4:
                iter=any_dstr->begin();
                for(int i=0;i<(int)any_dstr->size();i++){
                    if( traverse((*iter)->get_rlpoint(), op, sqlite3_value_int(val)) )
                        stcsr->resultSet[count++] = i;
                    iter++;
                }
                stcsr->size += count;
                break;
            case 5:
                iter=any_dstr->begin();
                for(int i=0;i<(int)any_dstr->size();i++){
                    if( traverse((long int)(*iter)->get_Customers(), op, sqlite3_value_int64(val)) )
                        stcsr->resultSet[count++] = i;
                    iter++;
                }
                stcsr->size += count;
                break;
// more datatypes and ops exist
            }
       
    }
}

// reallocate resultset for embedded
// fill resultset in join and see what happens
// might have to shrink resultset in next turns (further constraint dissatisfaction)


int Customers_retrieve(void *stc, int n, sqlite3_context *con){
    sqlite3_vtab_cursor *cur = (sqlite3_vtab_cursor *)stc;
    stlTable *stl = (stlTable *)cur->pVtab;
    stlTableCursor *stcsr = (stlTableCursor *)stc;
    vector<Customer*> *any_dstr = (vector<Customer*> *)stl->data;
    vector<Customer*>:: iterator iter;
    char *colName = stl->azColumn[n];
    int index = stcsr->current;
// iterator implementation. serial traversing or hit?
    iter = any_dstr->begin();
// serial traversing. simple and generic. visitor pattern is next step.
    for(int i=0; i<stcsr->resultSet[index]; i++){
        iter++;
    }
// int datatype;
// datatype = stl->colDataType[n];
        switch ( n ){
            case 0:
//		sqlite3_result_int64(con, (sqlite3_int64)&(*iter));
//		printf("memory location of Customer PK: %x\n", &(*iter));
		break;
            case 1:
                    sqlite3_result_int(con, (*iter)->get_demand());
                    break;
            case 2:
                    sqlite3_result_text(con, (const char *)(*iter)->get_code().c_str(), -1, SQLITE_STATIC);
                    break;
            case 3:
                    sqlite3_result_int(con, (*iter)->get_serviced());
                    break;
            case 4:
                    sqlite3_result_int(con, (*iter)->get_pickdemand());
                    break;
            case 5:
                    sqlite3_result_int(con, (*iter)->get_starttime());
                    break;
            case 6:
                    sqlite3_result_int(con, (*iter)->get_servicetime());
                    break;
            case 7:
                    sqlite3_result_int(con, (*iter)->get_finishtime());
                    break;
            case 8:
                    sqlite3_result_int(con, (*iter)->get_revenue());
                    break;
        }
    return SQLITE_OK;
}


int Trucks_retrieve(void *stc, int n, sqlite3_context *con){
    sqlite3_vtab_cursor *cur = (sqlite3_vtab_cursor *)stc;
    stlTable *stl = (stlTable *)cur->pVtab;
    stlTableCursor *stcsr = (stlTableCursor *)stc;
    vector<Truck*> *any_dstr = (vector<Truck*> *)stl->data;
    vector<Truck*>:: iterator iter;
    char *colName = stl->azColumn[n];
    int index = stcsr->current;
// iterator implementation. serial traversing or hit?
    iter = any_dstr->begin();
// serial traversing. simple and generic. visitor pattern is next step.
    for(int i=0; i<stcsr->resultSet[index]; i++){
        iter++;
    }
// int datatype;
// datatype = stl->colDataType[n];
    const char *pk = "pk";
    const char *fk = "fk";
//    printf("%s\n",stl->azColumn[0]);
    switch ( n ){
    case 0:
	sqlite3_result_int64(con, (sqlite3_int64)&(*iter));
	break;
    case 1:
	sqlite3_result_double(con, (*iter)->get_cost());
	break;
    case 2:
	sqlite3_result_int(con, (*iter)->get_delcapacity());
	break;
    case 3:
	sqlite3_result_int(con, (*iter)->get_pickcapacity());
	break;
    case 4:
	sqlite3_result_int(con, (*iter)->get_rlpoint());
	break;
    case 5:
	sqlite3_result_int64(con, (sqlite3_int64)(*iter)->get_Customers());
	break;
    }
    return SQLITE_OK;
}


void search(void* stc, char *constr, sqlite3_value *val){
    sqlite3_vtab_cursor *cur = (sqlite3_vtab_cursor *)stc;
    stlTable *stl = (stlTable *)cur->pVtab;
    if( !strcmp(stl->zName, "Customers") )
        Customers_search(stc, constr, val);
    if( !strcmp(stl->zName, "Trucks") )
        Trucks_search(stc, constr, val);
}

int retrieve(void* stc, int n, sqlite3_context *con){
    sqlite3_vtab_cursor *cur = (sqlite3_vtab_cursor *)stc;
    stlTable *stl = (stlTable *)cur->pVtab;
    if( !strcmp(stl->zName, "Customers") )
        return Customers_retrieve(stc, n, con);
    if( !strcmp(stl->zName, "Trucks") )
        return Trucks_retrieve(stc, n, con);
}

