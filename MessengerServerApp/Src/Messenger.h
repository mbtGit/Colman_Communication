/*
 * TCPMessenger.h
 *
 *  Created on: Jan 27, 2016
 *      Author: Matan Borenkraout and Or Shapira
 */


#ifndef TCPMSN_H__
#define TCPMSN_H__

#include <strings.h>
#include <map>
#include <vector>
#include <set>
#include "MThread.h"
#include "TCPSocket.h"
#include "MultipleTCPSocketsListener.h"
#include "TCPMessengerProtocol.h"
#include "TCPMsnDispatcher.h"
#include "TCPMsnServer.h"

using namespace std;


class Messenger{
	//TODO: add class properties
private:
	TCPMsnServer* msnServer;
public:
	TCPMsnDispatcher* msnDispatcher;
	/**
	 * Build the Messenger server
	 */
	Messenger();
	~Messenger();

	/**
	 * print out the list of connected clients (for debug)
	 */
	void listPeers();

	/**
	 * close the server
	 */
	void close();

	/**
	 * read command from peer
	 */
	static int readCommandFromPeer(TCPSocket* peer);

	/**
	 * read data from peer
	 */
	static string readDataFromPeer(TCPSocket* peer);

	/**
	 * send command to peer
	 */
	static void sendCommandToPeer(TCPSocket* peer, int command);

	/**
	 * send data to peer
	 */
	static void sendDataToPeer(TCPSocket* peer, string msg);
	static void ListAllUsers(string usrCurrUser);
	static void RegisterUser(string p_strUserName, string p_strPassword);
	static bool LoginUser(string p_strUserName, string p_strPassword);

};
#endif
