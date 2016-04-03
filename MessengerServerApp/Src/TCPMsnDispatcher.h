/*
 * TCPMsnDispatcher.h
 *
 *  Created on: Jan 27, 2016
 *      Author: Matan Borenkraout and Or Shapira
 */


#ifndef TCPMSNDISPATCHER_H_
#define TCPMSNDISPATCHER_H_

#include <strings.h>
#include <map>
#include <vector>
#include <set>
#include "MThread.h"
#include "MultipleTCPSocketsListener.h"
#include "TCPMessengerProtocol.h"
#include "Room.h"
#include "Utils.h"


class TCPMsnDispatcher :MThread{
private:
	map<string, TCPSocket*> mpSocketsMap;
	MultipleTCPSocketsListener* sckListener;
	map<string, User*> mpUsersMap;
	map<string, Room*> mpRoomsMap;

public:
	TCPMsnDispatcher();
	virtual ~TCPMsnDispatcher();
	void addSocket(TCPSocket* sock);
	void removeSocket(TCPSocket* sock);
	void addUser(User* usrNewUser);
	void removeUser(User* usrRemovedUser);
	void run();
	void ListAllConnectedUsers(User* usrCurrUser);
	void ListAllSessions(User* usrCurrUser);
	void ListAllRooms(User* usrCurrUser);
	void ListAllUsers(User* usrCurrUser);
	void ListAllUsersInSpecificRoom(string p_strRoomName, User* usrCurrUser);
	map<string, TCPSocket*> getSockets();
	map<string, User*> getUsers();
	map<string, Room*> getRooms();
	void RemoveRoom(Room* rmRoomToRemove);
	void CloseSession(User* usrCurrUser, TCPSocket* currSock);
	int GetUsersNum();
};

#endif /* TCPMSNDISPATCHER_H_ */
