#include "WindowList.h"

WindowList::WindowList()
{
	win_num=0;
	content=new string[10];
}

void WindowList::initialize()
{
	win_num=0;
}

void WindowList::addwindow(string newwindow)
{
	content[win_num]=newwindow;
	win_num++;
	if (win_num==10)
	{
		win_num=0;
	}
}

void WindowList::addwindow_ind(string newwindow,int ind)
{
	if ((ind<10)&&(ind>=0))
	{
		content[ind]=newwindow;
		if ((ind+1)>win_num)
		{
			win_num=(ind+1);
		}
	}
}