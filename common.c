
/************************************************************************************
 * @file    : common.c 
 * @brief   : 
 *           
 *           
 * @version : 2.0
 * @date    : 2014/06/30
 * @author  : Public author 
 *            tp.joo@daum.net
 ************************************************************************************/


#include <stdio.h>
#include <stdarg.h>

#define STR_LEN 		256 // 129

/* error messages with function name and argument list */
void errormsg(const char *func, int line, const char *str, ...) 
{
	char msg[STR_LEN];
	va_list argptr;	
	char tmp[STR_LEN];

	va_start(argptr,str);	
	vsnprintf(msg, STR_LEN-1, str, argptr);	

#if 0
	fprintf(stderr, "%-12s:%5d: %s\n", func, line, msg);	
#else
	fprintf(stderr, "%-12s: %s\n", func, msg);	
#endif

	va_end(argptr);
}








