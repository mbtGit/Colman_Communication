/*
 * TCPMsnServer.h
 *
 *  Created on: Jan 27, 2016
 *      Author: Matan Borenkraout and Or Shapira
 */


#ifndef TCPMSNSERVER_H_
#define TCPMSNSERVER_H_

#include "MThread.h"
#include "TCPMsnDispatcher.h"
#include "TCPSocket.h"

class TCPMsnServer : MThread {
private:
	TCPMsnDispatcher* _dispatcher;
	TCPSocket* _sock;
public:
	TCPMsnServer(TCPMsnDispatcher* msnDispatcher);
	virtual ~TCPMsnServer();
	void run();
};

#endif /* TCPMESSENGER_H_ */
