/*
 * TCPMsnServer.cpp
 *
 *  Created on: Jan 27, 2016
 *      Author: Matan Borenkraout and Or Shapira
 */


#include "TCPMsnServer.h"
#include "TCPMessengerProtocol.h"

TCPMsnServer::TCPMsnServer(TCPMsnDispatcher* dispatcher) {
	_dispatcher = dispatcher;
	_sock = new TCPSocket(MSNGR_PORT);
	this->start();

}
void TCPMsnServer::run(){
	while (true){
		TCPSocket* newSock = _sock->listenAndAccept();
		cout << "socket added " << newSock->destIpAndPort() << endl;
		_dispatcher->addSocket(newSock);
	}
}

TCPMsnServer::~TCPMsnServer() {
	delete _sock;
	delete _dispatcher;
}


