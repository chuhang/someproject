#include "ConnListener.h"

void ConnListener::onConnect()
{
	cout << "ConnListener::onConnect()" << endl;
	ontls=1;
}

void ConnListener::onDisconnect(ConnectionError e) 
{
    cout << "ConnListener::onDisconnect() " << e << endl;
}

bool ConnListener::onTLSConnect(const CertInfo& info) 
{
    cout << "ConnListener::onTLSConnect()" << endl;
    return true;
}

void ConnListener::initFlag()
{
	ontls=0;
}

int ConnListener::checkFlag()
{
	return ontls;
}