#include "BuddyList.h"

BuddyList mybuddylist;

BuddyNode::BuddyNode()
{
	next=NULL;
}

BuddyList::BuddyList()
{
	head=new BuddyNode();
	rear=head;
}

void BuddyList::addBuddy(string jid)
{
	rear->buddyJID=jid;
	rear->next=new BuddyNode();
	rear=rear->next;
}

void BuddyList::readLog()
{
	ifstream buddylog("mybuddies.logfile");
	string tmp;
	while (getline(buddylog,tmp))
	{
		cout<<tmp<<endl;
		mybuddylist.addBuddy(tmp);
	}
}