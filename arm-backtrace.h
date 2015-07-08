/********************************************
 *
 *	@file arm-backtrace.c
 *	@author wuwc.zeno@inesa-e.com
 *	@changelog:
 *		2014/06/24 create the file
 *
 *
 *******************************************/

#ifndef ARM_BACKTRACE_H__
#define ARM_BACKTRACE_H__

#ifdef __cplusplus
	extern "C"
	{
#endif	// __cplusplus

int backtrace_arm(void **array, int size);
char ** backtrace_symbols_arm(void *const *array, int size);

#ifdef __cplusplus
	}
#endif	// __cplusplus

#endif
