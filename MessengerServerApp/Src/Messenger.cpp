/*
 * TCPMessenger.cpp
 *
 *  Created on: Jan 27, 2016
 *      Author: Matan Borenkraout and Or Shapira
 */

#include "Messenger.h"
#include "TCPMessengerProtocol.h"
#include <fstream>
#include <iostream>
#include <string>

/***********************   TCPMessengerServer implementation ******************************/

Messenger::Messenger(){
	msnDispatcher = new TCPMsnDispatcher();
	msnServer = new TCPMsnServer(msnDispatcher);
}

void Messenger::close(){
	map<string, TCPSocket*>::iterator itrStart = msnDispatcher->getSockets().begin();
	map<string, TCPSocket*>::iterator itrEnd = msnDispatcher->getSockets().end();

	// Close all the sockets
	for(;itrStart != itrEnd;itrStart++){
		((*itrStart).second)->cclose();
	}

	// Delete all the sockets
	itrStart = msnDispatcher->getSockets().begin();
	itrEnd = msnDispatcher->getSockets().end();
	for(;itrStart != itrEnd;itrStart++){
		delete (*itrStart).second;
	}

	delete this;
}

Messenger::~Messenger(){
	delete msnServer;
	delete msnDispatcher;
}

int Messenger::readCommandFromPeer(TCPSocket* peer){
	//TODO: read a command from socket
	int command = 0;
	int retVal = peer->recv((char *) &command, 4);
	if (retVal < 1){
		return retVal;
	}

	command = ntohl(command);
	return command;
}

string Messenger::readDataFromPeer(TCPSocket* peer){
	string dataFromPeer;

	int msgLength;

	// Get the message length
	peer->recv((char*)&msgLength,4);
	msgLength = ntohl(msgLength);
	char* dataBuffer = new char[msgLength + 1];
	peer->recv(dataBuffer, msgLength);
	dataBuffer[msgLength] = 0;
	dataFromPeer = dataBuffer;

	return dataFromPeer;
}

void Messenger::sendCommandToPeer(TCPSocket* peer, int command){
	//TODO: send command to socket
	command = htonl(command);
	peer->send((char*) &command, 4);
}

void Messenger::sendDataToPeer(TCPSocket* peer, string msg){
	//TODO: send string to socket
	int msgLength = msg.length();
	msgLength = htonl(msgLength);
	peer->send((char*)&msgLength,4);
	peer->send(msg.data(),msg.length());
}

void Messenger::listPeers(){
	//TODO: print the connected peers
	for(map<string, TCPSocket*>::iterator itr = msnDispatcher->getSockets().begin();
			itr != msnDispatcher->getSockets().end(); itr++) {
		cout << itr->first << ":" << itr->second << endl;
	}
}



void Messenger::RegisterUser(string p_strUserName, string p_strPassword){
	fstream fsUserList;
	fsUserList.open("UserList.txt", ios::app | ios::out);

	if(fsUserList.is_open()){
		fsUserList << p_strUserName + " " + p_strPassword << endl;
		fsUserList.close();
	}
}

bool Messenger::LoginUser(string p_strUserName, string p_strPassword){
	fstream fsUserList;
	fsUserList.open("UserList.txt");
	string strLine;
	string strWantedUser = p_strUserName + " " + p_strPassword;
	if (fsUserList.is_open()){
		while (getline(fsUserList,strLine)){
			if (strLine.compare(strWantedUser) == 0){
				return true;
			}
		}

		fsUserList.close();
	}

	return false;
}

