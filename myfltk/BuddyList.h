//A struct for storing all the buddies
//In the form of linked list
#include <string>
#include <iostream>
#include <fstream>
#include "WindowList.h"
using namespace std;

struct BuddyNode
{
	string buddyJID;
	BuddyNode* next;
	WindowList onebuddywindowlist;

	BuddyNode();
	void AddWindowToBuddy(string newwindow,int ind);
};

struct BuddyList
{
	BuddyNode* head;
	BuddyNode* rear;

	BuddyList();
	void addBuddy(string jid);
	void readLog();
	int inList(string jid);
	void addWindow_BuddyID(int buddyid,string newwindow,int windowid);
};

extern BuddyList mybuddylist;