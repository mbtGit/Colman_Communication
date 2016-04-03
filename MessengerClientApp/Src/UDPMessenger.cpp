//============================================================================
// Name        : UDPMessenger
// Author      : Matan Borenkraout And Or Shapira
// Version     :
// Copyright   : Your copyright notice
// Description : UDP Messenger application
//============================================================================

#include "UDPMessenger.h"

using namespace std;



UDPMessenger::UDPMessenger(OnRecieveClbk* clbk, int p_nPort){
	//TODO: initiate local arguments (if needed)
	bIsRunning = true;
	rcvClbk = clbk;
	//TODO: create the socket
	this->sckSocket = new UDPSocket(p_nPort);

	//TODO: start the thread to receive incoming messages
	this->start();
}

void UDPMessenger::run(){
	char buffer[100];

	while (this->bIsRunning)
	{
		for(int i = 0;i<100;i++,buffer[i] = 0);
		this->sckSocket->recv(buffer, 100);
		cout << buffer << endl;
	}

}

void UDPMessenger::sendTo(string msg,string ip, int p_nPort){
	//TODO: send the given message to the given destination
	this->sckSocket->sendTo(msg, ip, p_nPort);
}

void UDPMessenger::reply(string msg){
	//TODO: send the message to the address of the last received message
	this->sckSocket->reply(msg);
}

void UDPMessenger::close(){
	//TODO: close the thread
	this->bIsRunning = false;
	//TODO: close the socket
	this->sckSocket->cclose();
	//TODO: wait for thread to exit
	this->waitForThread();
	//TODO: delete and free any allocated resources
	delete this->sckSocket;
	delete this->rcvClbk;
}

