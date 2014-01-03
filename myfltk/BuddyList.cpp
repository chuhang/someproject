#include "BuddyList.h"

BuddyList mybuddylist;

BuddyNode::BuddyNode()
{
	next=NULL;
}

void BuddyNode::AddWindowToBuddy(string newwindow,int ind)
{
	onebuddywindowlist.addwindow_ind(newwindow,ind);
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

int BuddyList::inList(string jid)
{
	if (jid==head->buddyJID)
	{
		return 0;
	}
	else
	{
		if (!head->next)
		{
			return -1;
		}
		else
		{
			int returnval=0;
			BuddyNode* p=head;
			while (p->next)
			{
				returnval++;
				p=p->next;
				if (p->buddyJID==jid)
				{
					return returnval;
				}
			}
			return -1;
		}
	}
}

void BuddyList::addWindow_BuddyID(int buddyid,string newwindow,int windowid)
{
	BuddyNode* p=head;
	if (buddyid>0)
	{
		for (int i=0;i<buddyid;i++)
		{
			p=p->next;
		}
	}
	p->AddWindowToBuddy(newwindow,windowid);
}