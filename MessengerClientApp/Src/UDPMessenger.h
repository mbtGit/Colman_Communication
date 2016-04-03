//============================================================================
// Name        : UDPMessenger.h
// Author      : Matan Borenkraout And Or Shapira
// Version     :
// Copyright   : Your copyright notice
// Description : UDP Messenger application
//============================================================================

#ifndef UDPMESSENGER_H_
#define UDPMESSENGER_H_

#include <pthread.h>
#include "UDPSocket.h"
#include "MThread.h"
using namespace std;


class OnRecieveClbk {
public:
	virtual void handleMessage(string msg)=0;
	virtual ~OnRecieveClbk(){}
};

class UDPMessenger: public MThread{
	//TODO: declare the class properties
private:
	OnRecieveClbk* rcvClbk;
	UDPSocket* sckSocket;
	bool bIsRunning;
public:
	UDPMessenger(OnRecieveClbk* clbk, int p_nPort);

	/**
	 * sends the given message to the given peer specified by IP
	 */
	void sendTo(string msg,string ip, int p_nPort);

	/**
	 * reply to an incoming message, this method will send the given message
	 * the peer from which the last message was received.
	 */
	void reply(string msg);

	/**
	 * close the messenger and all related objects (socket)
	 */
	void close();

	/**
	 * This method runs in a separate thread, it reads the incoming messages
	 * from the socket and prints the content on the terminal.
	 * The thread should exist when the socket is closed
	 */
	void run();

};

#endif
