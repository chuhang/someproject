//A struct for storing all the buddies
//In the form of linked list
#include <string>
using namespace std;

struct BuddyNode
{
	string buddyJID;
	BuddyNode* next;

	BuddyNode();
};

struct BuddyList
{
	BuddyNode* head;
	BuddyNode* rear;

	BuddyList();
	void addBuddy(string jid);
};