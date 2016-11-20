#include <list>
#include <vector>
#include <fstream>
#include <sstream>

#ifndef PICO_QL_SINGLE_THREADED
#include <pthread.h>
#endif
#include "pico_ql.h"
#include "Child.h"
#include "Parent.h"
/*------------*/

using namespace std;
using namespace picoQL;


int main()
{

	Parent p1;
	p1.m_data = "parent_1";
	Child c1, c2, c3;
	c1.m_data = "child_1";
	c2.m_data = "child_2";
	c3.m_data = "child_3";
	p1.m_children.push_back(c1);
	p1.m_children.push_back(c2);
	p1.m_children.push_back(c3);

	Parent p2;
	p2.m_data = "parent_2";
	Child c4, c5;
	c4.m_data = "child_4";
	c5.m_data = "child_5";
	p2.m_children.push_back(c4);
	p2.m_children.push_back(c5);

	Parent p3;
	p3.m_data = "parent_3";

	vector<Parent> p;
	p.push_back(p1);
	p.push_back(p2);
	p.push_back(p3);

	pico_ql_register((const void *)&p, "parent");

	const char *pragmas[2];
	pragmas[0] = "PRAGMA synchronous = OFF";
	pragmas[1] = "PRAGMA journal_mode = OFF";

	int re;
	void *exit_status = NULL;
#ifndef PICO_QL_SINGLE_THREADED
	pthread_t t;
	re = pico_ql_init(pragmas, 2, 8083, &t);
	pthread_join(t, &exit_status);
#else
	re = pico_ql_init(pragmas, 2, 8083, NULL);
#endif

	if (re)
		fprintf(stderr, "pico_ql_init() failed with code %d\n", re);

	stringstream s;
	fstream fs;
	fs.open("parentchild_resultset", fstream::out);
	pico_ql_exec_query("select * from parent;", s, pico_ql_step_text);
	fs << s.str() << endl;
	fs.close();

	pico_ql_shutdown();

	return 0;
}
