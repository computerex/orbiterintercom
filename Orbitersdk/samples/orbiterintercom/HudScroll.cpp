#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "HudScroll.h"

void HUDSCROLL::insert(const char *fmt, ...)
{
	char cbuf[255];   
	va_list arg;
	va_start(arg,fmt);
	vsprintf_s(cbuf,fmt,arg);
	va_end(arg);

	for (unsigned int i = 0; i < m.size(); i++)
	{
		if (i == m.size()-1)
		{
			m[i]=cbuf;
			break;
		}
		m[i]=m[i+1];
	}
}
