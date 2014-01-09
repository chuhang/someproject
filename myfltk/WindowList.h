//A struct for storing the Alt+Tab window list
//Assuming we have no more than 10 windows..
#include <string>
using namespace std;

struct WindowList
{
	string* content;
	int win_num;

	WindowList();
	void initialize();
	void addwindow(string newwindow);
	void addwindow_ind(string newwindow,int ind);
	void copy(WindowList* origin);
	bool compare(WindowList* p,int* whichchanges);
};