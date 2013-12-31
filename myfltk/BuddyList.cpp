#include "BuddyList.h"

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