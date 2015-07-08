/********************************************
 *
 *	@file arm-backtrace.c
 *	@author wuwc.zeno@inesa-e.com
 *	@changelog:
 *		2014/06/24 create the file
 *
 *
 *******************************************/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ucontext.h>
#include "arm-backtrace.h"
#include <string.h>
#include <dlfcn.h>

int backtrace_arm(void **array, int size)
{
	if (size <= 0)
		return 0;

	int* fp = 0;
	int* nfp = 0;
	int ret = 0;
	int cnt = 0;

	__asm__ __volatile
	(
		"mov %0, fp\n" 
		: "=r"(fp)
	);

	//printf("fp: 0x%x\n", fp);
	//printf("*fp: 0x%x\n", *fp);
	//printf("*(fp-1): 0x%x\n", *(fp-1));
	//printf("*(fp-3): 0x%x\n", (*(fp-3)));

	if ((fp-1) != 0)
		array[cnt++] = (void *)(*(fp-1));
	else
	{
		printf("fp-1 assert....\n");
		return -1;
	}
	//printf("*(fp-3): 0x%x\n", (*(fp-3)));

	if ((fp-3) != 0)
		nfp = (int *)(*(fp-3));
	else
	{
		printf("fp-3 assert....\n");
		return -1;
	}
	//printf("nfp: 0x%x\n", nfp);
	//printf("nfp-3: 0x%x\n", nfp - 3);
	//printf("*nfp-3: 0x%x\n", *(nfp - 3));
	while((cnt <= size) && (nfp > 0))
	{
		array[cnt++] = (void *)*(nfp - 1);
		//char** strings = backtrace_symbols_ce123(array, cnt);
		//printf("%d: %s\n", cnt-1, strings[cnt-1]);
		//free(strings);
		if ((nfp-3) > 0 && *(nfp-3) > 0)
		{
			//printf("nfp - 3: %d\n", (int)(nfp - 3));
			//printf("*(nfp-3): %d\n", *(nfp-3));
			nfp = (int *)(*(nfp-3));
		}
		else
		{
			printf("nfp-3 assert....\n");
			break ;
		}
	}


	ret = ((cnt <= size) ? cnt : size);

	return ret;
}

char ** backtrace_symbols_arm(void *const *array, int size)
{
	Dl_info dlinfo[size];
	int dlinfop[size];
	int cnt;
	size_t total_size = 0;
	char **res;

	// 从函数返回地址获得函数名
	for (cnt = 0; cnt < size; ++cnt)
	{
		//dlinfop[cnt] = _dl_addr (array[cnt], &dlinfo[cnt]);
		dlinfop[cnt] = dladdr (array[cnt], &dlinfo[cnt]);
		if (dlinfop[cnt] && dlinfo[cnt].dli_fname && dlinfo[cnt].dli_fname[0] != '\0')
		// 文件名(入口)+偏移
		total_size += (strlen (dlinfo[cnt].dli_fname ?: "")
			+ (dlinfo[cnt].dli_sname ? strlen (dlinfo[cnt].dli_sname) + 3 + 8 + 3 : 1)
			+ 8 + 5);
		else
			total_size += 5 + 8;
	}


	// 返回值，需要用户free
	res = (char **) malloc (size * sizeof (char *) + total_size);
	if (res != NULL)
	{
		char *last = (char *) (res + size);

		for (cnt = 0; cnt < size; ++cnt)
		{
			res[cnt] = last;

			if (dlinfop[cnt] && dlinfo[cnt].dli_fname && dlinfo[cnt].dli_fname[0] != '\0')
			{
				char buf[20];

				if (array[cnt] >= (void *) dlinfo[cnt].dli_saddr)
					sprintf (buf, "+%#lx", \
						(unsigned long)(array[cnt] - dlinfo[cnt].dli_saddr));
				else
					sprintf (buf, "-%#lx", \
						(unsigned long)(dlinfo[cnt].dli_saddr - array[cnt]));

				last += 1 + sprintf (last, "%s%s%s%s%s[%p]",
					dlinfo[cnt].dli_fname ?: "??",
					dlinfo[cnt].dli_sname ? "(" : "",
					dlinfo[cnt].dli_sname ?: "?",
					dlinfo[cnt].dli_sname ? buf : "?",
					dlinfo[cnt].dli_sname ? ") " : " ",
					array[cnt]);
			}
			else
				last += 1 + sprintf (last, "[%p]", array[cnt]);
		}
		assert (last <= (char *) res + size * sizeof (char *) + total_size);
	}

	return res;
}
