/*
 * MultipleTCPSocketListener.cpp
 *
 *  Created on: Jan 27, 2016
 *      Author: Matan Borenkraout and Or Shapira
 */


#include "MultipleTCPSocketsListener.h"

using namespace std;



void MultipleTCPSocketsListener::addSocket(TCPSocket* socket){
	sockets.push_back(socket);
}

void MultipleTCPSocketsListener::addSockets(vector<TCPSocket*> socketVec){
	sockets = socketVec;
}

TCPSocket* MultipleTCPSocketsListener::listenToSocket(int timeout){

	// create local set for the select function (fd_set)
	fd_set fdSet, fdMaster;;
	FD_ZERO(&fdSet);
	FD_ZERO(&fdMaster);
	vector<TCPSocket*>::iterator itrFirst = sockets.begin();
	vector<TCPSocket*>::iterator itrLast = sockets.end();
	int countVec;
	int maxSock = 0;
	int sckCurrSock;
	struct timeval tv;
	tv.tv_sec=0;
	tv.tv_usec=100000;

	for(; itrFirst != itrLast; itrFirst++)
	{
		sckCurrSock = (*itrFirst)->getSocketFid();
		FD_SET(sckCurrSock, &fdMaster);
		countVec++;
		if(maxSock < sckCurrSock)
		{
			maxSock = sckCurrSock;
		}

	}

	fdSet = fdMaster;

	// perform the select
	int returned = select((maxSock + 1), &fdSet, NULL, NULL, &tv);

	// check the returned value from the select to find the socket that is ready
	if(returned)
	{
		for(int i=0; i<countVec; i++)
		{
			if(FD_ISSET((sockets[i])->getSocketFid(), &fdSet))
			{
				// if select return a valid socket return the matching TCPSocket object otherwise return NULL
				return sockets[i];
			}
		}
	}

	return NULL;
}

