/********************************************
 *
 *	@file arm-backtrace.c
 *	@author wuwc.zeno@inesa-e.com
 *	@changelog:
 *		2014/06/24 create the file
 *
 *
 *******************************************/

#include "arm-backtrace.h"
#include <stdio.h>
#include <stdlib.h>

void dump()
{
	void* array[128];
	int size = 0;
	int i = 0;
	char** strings = NULL;
	size = backtrace_arm(array, 128);
	strings = backtrace_symbols_arm(array, size);

	for (i = 0; i < size; i++)
		printf("%d: %s\n", i+1, strings[i]);

	free(strings);
}

class A
{
public:
	A(){;}
	~A(){;}
	void AFunc()
	{
		printf("Hello!\n");
		dump();
	}
};

class Caller
{
public:
	Caller(){;}
	~Caller(){;}
	void CFunc()
	{
		A a;
		a.AFunc();
	}
};

void f1() {Caller c;c.CFunc();};
void f2() {f1();}
void f3() {f2();}
void f4() {f3();}
void f5() {f4();}
void f6() {f5();}
void f7() {f6();}
void f8() {f7();}
void f9() {f8();}
void f10() {f9();}
void f11() {f10();}
void f12() {f11();}
void f13() {f12();}
void f14() {f13();}
void f15() {f14();}
void f16() {f15();}

int main()
{
	f16();
	return 0;
}
