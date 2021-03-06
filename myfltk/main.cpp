#include <iostream>
#include <string>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Input.H>
#include "Bot.h"
#include "config.h"
#include "threads.h"

using namespace std;
using namespace gloox;

//global variables. Obviously the best way is define a class for everything in the main()
//however for now using global variables is the way for sharing contents
//will change later
Fl_Window* addbuddywindowglobal;
Fl_Input* addinputglobal;

Fl_Thread recv_thread;
Fl_Thread myfunc_thread;
int n=0;
void* thread_periodical(void* p);
void* thread_periodical2(void* p);

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
	ofstream buddylog("mybuddies.logfile",ios_base::app);
	buddylog<<newbuddy<<endl;
	buddylog.close();
	mybuddylist.addBuddy(newbuddy);
}
//**********Section FLTK**********

int main() 
{
	MyFunc();
	cout<<"win_num = "<<openedwindows.win_num<<endl;
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
	mybuddylist.readLog();

	Fl::lock();
	fl_create_thread(recv_thread,thread_periodical,&b);
	fl_create_thread(myfunc_thread,thread_periodical2,&b);
	Fl::run();
	return 0;
}

void* thread_periodical(void* p)
{
	Bot* b = (Bot*) p;

	while (1)
	{
		Sleep(1000);
		cout<<"sleep ends!"<<endl;
		Fl::lock();
		//MyFunc();
		b->receiveMsg();
		Fl::awake();
		Fl::unlock();
	}
	return 0L;
}

void* thread_periodical2(void* p)
{
	Bot* b = (Bot*) p;

	while (1)
	{
		Sleep(5000);
		cout<<"sleep 2 ends!"<<endl;
		WindowList formerwindows;
		formerwindows.copy(&openedwindows);
		openedwindows.initialize();
		MyFunc();
		int intarray[10];
		if (!formerwindows.compare(&openedwindows,intarray))
		{
			cout<<"your window list changes!"<<endl;
			for (int i=0;i<10;i++)
			{
				if (intarray[i]==1)
				{
					cout<<"This window chages: "<<i<<endl;
					//send the change to all friends
					//haven't finished
				}
			}
		}
	}
	return 0L;
}