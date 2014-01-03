#include <message.h>
#include <messagehandler.h>
#include <FL/Fl_Text_Display.H>
#include <string>
#include <client.h>
#include "ConnListener.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>
#include "BuddyList.h"
using namespace std;
using namespace gloox;

//global variables. Obviously the best way is define a class for everything in the main()
//however for now using global variables is the way for sharing contents
//will change later
extern HWND windowHandle;
extern WindowList openedwindows;
extern WindowList buddyswindows;

//some function found on the web for converting int to string
#pragma comment(lib, "user32.lib")
inline string to_string(int _Val);

//**********Section Alt+Tab**********
//These 3 functions gives a list of window names(in std::string) as Alt+Tab does
//To use this just call MyFunc() and the window list will be stored in global variable "openedwindows"
BOOL IsAltTabWindow(HWND hwnd);
BOOL CALLBACK MyEnumProc(HWND hWnd, LPARAM lParam);
void MyFunc(void);

//**********Section gloox**********
//gloox is a xmpp library, most of the following functions are copied from its provided examples
//All I did is: modified handleMessage(), and added receiveMsg() and sendChecking()
ostream& operator<<(ostream& os, Message::MessageType type);
ostream& operator<<(ostream& os, const Message& stanza);
class Bot : public MessageHandler 
{
public:
    Bot(Fl_Text_Buffer* o);
	ConnectionError receiveMsg();
    ~Bot();
    virtual void handleMessage( const Message& stanza, MessageSession* session = 0 );
	void sendChecking();
private:
    Client* client;
    ConnListener* connListener;
    Fl_Text_Buffer* displaywidget;
};