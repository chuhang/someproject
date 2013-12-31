#include <iostream>
#include <string>
#include <client.h>
#include <message.h>
#include <messagehandler.h>
#include <connectionlistener.h>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include "WindowList.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Display.H>
#include "BuddyList.h"
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Input.H>

using namespace std;
using namespace gloox;

//some function found on the web for converting int to string
#pragma comment(lib, "user32.lib")
inline string to_string(int _Val)
{   
	// convert long long to string
    char _Buf[2 * _MAX_INT_DIG];
    sprintf(_Buf, "%d", _Val);
    return (string(_Buf));
}

//global variables. Obviously the best way is define a class for everything in the main()
//however for now using global variables is the way for sharing contents
//will change later
HWND windowHandle;
WindowList openedwindows;
WindowList buddyswindows;
int ontls=0;
BuddyList mybuddylist;
Fl_Window* addbuddywindowglobal;
Fl_Input* addinputglobal;

//**********Section Alt+Tab**********
//These 3 functions gives a list of window names(in std::string) as Alt+Tab does
//To use this just call MyFunc() and the window list will be stored in global variable "openedwindows"
BOOL IsAltTabWindow(HWND hwnd)
{
	if (hwnd == GetShellWindow())   //Desktop
		return false;
	// Start at the root owner
	HWND hwndWalk = GetAncestor(hwnd, GA_ROOTOWNER);

	// See if we are the last active visible popup
	HWND hwndTry;
	while ((hwndTry = GetLastActivePopup(hwndWalk)) != hwndTry) 
	{
		if (IsWindowVisible(hwndTry)) 
			break;
		hwndWalk = hwndTry;
	}
	return hwndWalk == hwnd;
}

BOOL CALLBACK MyEnumProc(HWND hWnd, LPARAM lParam)
{
    TCHAR title[500];
    ZeroMemory(title, sizeof(title));
    
    //string strTitle;
    
    GetWindowText(hWnd, title, sizeof(title)/sizeof(title[0]));
	
	if (IsAltTabWindow(hWnd) && IsWindowVisible(hWnd) && GetWindowTextLength(hWnd)>0)
	{
		_tprintf(_T("Window Found: %s\n"), title);
		string nowwindow=title;
		//cout<<"string nowwindow has value: "<<nowwindow<<endl;
		openedwindows.addwindow(nowwindow);
	}
    
    //strTitle += title; // Convert to std::string
    if(_tcsstr(title, _T("Excel")))
    {
        windowHandle = hWnd;
        return FALSE;
    }
    return TRUE;
}

void MyFunc(void)
{
    EnumWindows(MyEnumProc, 0);
}
//**********Section Alt+Tab**********

//**********Section gloox**********
//gloox is a xmpp library, most of the following functions are copied from its provided examples
//All I did is: modified handleMessage(), and added receiveMsg() and sendChecking()
ostream& operator<<(ostream& os, Message::MessageType type) 
{
    switch (type) 
	{
        case Message::Chat:
            os << "Chat";
            break;
        case Message::Error:
            os << "Error";
            break;
        case Message::Groupchat:
            os << "Groupchat";
            break;
        case Message::Headline:
            os << "Headline";
            break;
        case Message::Normal:
            os << "Normal";
            break;
        case Message::Invalid:
            os << "Invalid";
            break;
        default:
            os << "unknown type";
            break;
    }
	return os;
}

ostream& operator<<(ostream& os, const Message& stanza) 
{
    os << "type:'" << stanza.subtype() <<  "' from:'" << stanza.from().full() << "' body:'" << stanza.body() << "'";
    return os;
}

class ConnListener : public ConnectionListener 
{
public:
    virtual void onConnect() 
	{
        cout << "ConnListener::onConnect()" << endl;
		ontls=1;
    }
    virtual void onDisconnect(ConnectionError e) 
	{
        cout << "ConnListener::onDisconnect() " << e << endl;
    }
    virtual bool onTLSConnect(const CertInfo& info) 
	{
        cout << "ConnListener::onTLSConnect()" << endl;
        return true;
    }
};

class Bot : public MessageHandler 
{
public:
    Bot(Fl_Text_Buffer* o) 
	{
        displaywidget=o;
		JID jid("buddytestinghang@gmail.com");
        client = new Client( jid, "wochuhang" );
        connListener = new ConnListener();
        client->registerMessageHandler( this );
        client->registerConnectionListener(connListener);
        if (client->connect(false))
		{
			ConnectionError ce = ConnNoError;
			while( ontls==0 )
			{
				ce = client->recv();
			}
		}
    }

	ConnectionError receiveMsg()
	{
		ConnectionError ce = client->recv();
		return ce;
	}

	void sendChecking()
	{
		JID buddyjid("chuhang1122@gmail.com");
		Message checkmsg(Message::Chat,buddyjid,"C");
		client->send(checkmsg);
	}

    ~Bot() 
	{
        delete client;
        delete connListener;
    }
 
    virtual void handleMessage( const Message& stanza, MessageSession* session = 0 ) 
	{
        cout << "Received message: " << stanza << endl;
		int inMsgType=0;
		string inMsg=stanza.body();
		if (inMsg.length()>0)
		{
			if (inMsg[0]=='C')
			{
				inMsgType=1;
			}
			if (inMsg[0]=='T')
			{
				inMsgType=2;
			}
			if (inMsg[0]=='R')
			{
				inMsgType=3;
			}
		}
		if (inMsgType==1)
		{
			openedwindows.initialize();
			MyFunc();
			//Message msg(Message::Chat,stanza.from(),"I am doing: ");
			//client->send(msg);
			for (int i=0;i<openedwindows.win_num;i++)
			{
				Message newmsg(Message::Chat,stanza.from(),'T'+to_string(i)+": "+openedwindows.content[i]);
				cout<<"Message sent: "<<'T'+to_string(i)+": "+openedwindows.content[i]<<endl;
				client->send(newmsg);
			}
			Message msg(Message::Chat,stanza.from(),"R");
			client->send(msg);
		}
		if (inMsgType==2)
		{
			int inNum=inMsg[1]-'0';
			cout<<"Buddy is telling me something with inNum = "<<inNum<<endl;
			buddyswindows.addwindow_ind(inMsg,inNum);
		}
		if (inMsgType==3)
		{
			string uishow;
			for (int i=0;i<buddyswindows.win_num;i++)
			{
				cout<<"Buddys window"<<to_string(i)+": "+buddyswindows.content[i]<<endl;
				uishow=uishow+buddyswindows.content[i]+'\n';
			}
			const char *uishowchar = uishow.c_str();
			displaywidget->text(uishowchar);
		}
    }

private:
    Client* client;
    ConnListener* connListener;
    Fl_Text_Buffer* displaywidget;
};
//**********Section gloox**********

//**********Section FLTK**********
//These are the callback functions that menus and buttons in the FLTK interface use
//Again the best way (as Noah does in his projects) is to wrap them in a class, so that the global variables
//will no longer be needed for content sharing
void buttonRecv_cb(Fl_Widget* o,void* data) 
{
    Fl_Button* button=(Fl_Button*)o;
    Bot *tb=reinterpret_cast<Bot*>(data);
    tb->receiveMsg();
}

void buttonCheck_cb(Fl_Widget* o,void* data)
{
	Fl_Button* button=(Fl_Button*)o;
	Bot *tb=reinterpret_cast<Bot*>(data);
	tb->sendChecking();
}

void Quit_CB(Fl_Widget* o,void* data) 
{
    exit(0);
}

void AddBuddy_CB(Fl_Widget* o,void* data)
{
	addinputglobal->value("");
	addbuddywindowglobal->show();
}

void buttonAdd_CB(Fl_Widget* o,void* data)
{
	addbuddywindowglobal->hide();
	string newbuddy=addinputglobal->value();
	cout<<"This is what you typed in: "<<newbuddy<<endl;
	//add newbuddy to mybuddylist
	//then store mybuddylist in a local txt file
	//haven't finish yet
}
//**********Section FLTK**********

int main() 
{
	Fl_Window win(320,290,"Testing");
	win.begin();
		Fl_Button buttonRecv(10,240,140,30,"Receive Message");
		Fl_Button buttonCheck(170,240,140,30,"Check Buddies");
		Fl_Text_Buffer *buff=new Fl_Text_Buffer();
		Fl_Text_Display *disp=new Fl_Text_Display(20,50,280,170,"Buddy's Windows");
		Fl_Menu_Bar *menu = new Fl_Menu_Bar(0,0,320,30);
	win.end();

    disp->buffer(buff);
	Bot b(buff);
	buttonRecv.callback(buttonRecv_cb,&b);
	buttonCheck.callback(buttonCheck_cb,&b);
    win.resizable(*disp);
	buff->text("Empty Now");

	menu->add("Menu/Quit",FL_CTRL+'q',Quit_CB);
	menu->add("Buddys/Add Buddy",FL_CTRL+'a',AddBuddy_CB);
	static Fl_Window addbuddywin(250,90,"Add a new buddy");
	addbuddywin.begin();
		Fl_Input addInput(110,10,130,30,"Buddy's JID");
		Fl_Button buttonAdd(55,50,140,30,"Add!");
	addbuddywin.end();
	addbuddywindowglobal=&addbuddywin;
	addinputglobal=&addInput;
	buttonAdd.callback(buttonAdd_CB);

	win.show();
	return Fl::run();
}