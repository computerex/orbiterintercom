#ifndef _H_HUD_SCROLL_COMPUTEREX_H_
#define _H_HUD_SCROLL_COMPUTEREX_H_

#include <string>
#include <vector>

class HUDSCROLL
{
public:
	HUDSCROLL()
	{
		lc=15;
		m.resize(lc);
		clear();
	}
	HUDSCROLL(int l)
	{
		lc=l;
		m.resize(lc);
		clear();
	}
	//void insert(char * msg);
	void insert(const char * fmt, ...);
	void clear(void)
	{
		for (unsigned int i = 0; i < lc; i++)
			m[i]="";
	}
	std::vector<std::string> m;
	unsigned long lc;
};
#endif
