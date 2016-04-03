/*
 * TCPMsnDispatcher.cpp
 *
 *  Created on: Jan 27, 2016
 *      Author: Matan Borenkraout and Or Shapira
 */


#include "TCPMsnDispatcher.h"
#include "Messenger.h"
#include <fstream>

TCPMsnDispatcher::TCPMsnDispatcher() {
	this->sckListener = new MultipleTCPSocketsListener();
	this->start();
}

void TCPMsnDispatcher::addSocket(TCPSocket* sock) {
	this->mpSocketsMap.insert(std::pair<string, TCPSocket*>(sock->destIpAndPort(), sock));
	this->sckListener->addSocket(sock);
}

void TCPMsnDispatcher::removeSocket(TCPSocket* sock)
{
	// Remove the socket from the peers list
	this->mpSocketsMap.erase(sock->destIpAndPort());

	// Remove it also from the multiple sockets listener
	vector<TCPSocket*> newVector;

	// Go over each socket and push it to the new sockets vector of the listener.
	// We do this because there is no pretty way to remove from sockets vector of the listener.
	for (map<string, TCPSocket*>::iterator iterator = this->mpSocketsMap.begin();
			iterator != this->mpSocketsMap.end(); iterator++) {

		newVector.push_back(iterator->second);
	}

	sckListener->addSockets(newVector);
}

void TCPMsnDispatcher::RemoveRoom(Room* rmRoomToRemove){
	this->mpRoomsMap.erase(rmRoomToRemove->GetRoomName());
}
void TCPMsnDispatcher::addUser(User* usrNewUser)
{
	string strUser = usrNewUser->GetUserName();
	this->mpUsersMap.insert(std::pair<string, User*>(usrNewUser->GetUserName(), usrNewUser));
	this->addSocket(usrNewUser->GetUserSocket());
}
void TCPMsnDispatcher::removeUser(User* usrRemovedUser)
{
	// Remove the socket from the peers list
	this->mpUsersMap.erase(usrRemovedUser->GetUserName());
	this->removeSocket(usrRemovedUser->GetUserSocket());
}
map<string, TCPSocket*> TCPMsnDispatcher::getSockets() {
	return this->mpSocketsMap;
}

map<string, User*> TCPMsnDispatcher::getUsers(){
	return this->mpUsersMap;
}

map<string, Room*> TCPMsnDispatcher::getRooms(){
	return this->mpRoomsMap;
}

TCPMsnDispatcher::~TCPMsnDispatcher() {
	delete this->sckListener;
}

void TCPMsnDispatcher::ListAllConnectedUsers(User* usrCurrUser){
	for(map<string, User*>::iterator itr = this->mpUsersMap.begin();
			itr != this->mpUsersMap.end(); itr++) {
		if (usrCurrUser){
			Messenger::sendDataToPeer(usrCurrUser->GetUserSocket(), itr->first);
		}
		else{
			cout << itr->first << endl;
		}
	}
}
void TCPMsnDispatcher::ListAllSessions(User* usrCurrUser){
	for(map<string, User*>::iterator itr = this->mpUsersMap.begin();
			itr != this->mpUsersMap.end(); itr++) {
		if (itr->second->GetChattingUser().compare("") != 0)
		{
			cout << "User : "<< itr->first << " Is chatting with user: " << itr->second->GetChattingUser() << endl;
		}
	}
}
void TCPMsnDispatcher::ListAllRooms(User* usrCurrUser){
	for(map<string, Room*>::iterator itr = this->mpRoomsMap.begin();
				itr != this->mpRoomsMap.end(); itr++) {
		if(usrCurrUser){
			string strLine = "Room: " + itr->first + "\n";
			Messenger::sendDataToPeer(usrCurrUser->GetUserSocket(), strLine);
		}
		else{
			cout << "Room: "<< itr->first << endl;
		}
	}
}
void TCPMsnDispatcher::ListAllUsersInSpecificRoom(string p_strRoomName, User* usrCurrUser){
	for(map<string, Room*>::iterator itr = this->mpRoomsMap.begin();
					itr != this->mpRoomsMap.end(); itr++) {
			if (itr->first.compare(p_strRoomName) == 0){
				Messenger::sendCommandToPeer(usrCurrUser->GetUserSocket(), itr->second->GetAllUsersInRoom().size());
				cout << "In room " << p_strRoomName << " We have:" << endl;
				vector<User*> vcUsersInRoom = itr->second->GetAllUsersInRoom();
				for(std::vector<User*>::iterator it = vcUsersInRoom.begin();
						it != vcUsersInRoom.end(); ++it){
					if (usrCurrUser){
						string strLine = (*it)->GetUserName() + ", ";
						Messenger::sendDataToPeer(usrCurrUser->GetUserSocket(), strLine);
					}
					else{
						cout << (*it)->GetUserName() << ", ";
					}
				}
			}
		}
}
void TCPMsnDispatcher::ListAllUsers(User* usrCurrUser){
	fstream fsUserList;
	fsUserList.open("UserList.txt");
	string strLine;

	if (fsUserList.is_open()){
		while (getline(fsUserList,strLine)){
			if (usrCurrUser){
				const char* c = strLine.c_str();
				vector<string> vcStrUsers = Utils::split(c, ' ');
				Messenger::sendDataToPeer(usrCurrUser->GetUserSocket(), vcStrUsers[0]);
			}
			else{
				cout << strLine << '\n';
			}
		}

		fsUserList.close();
	}
}

int TCPMsnDispatcher::GetUsersNum(){
	fstream fsUserList;
		fsUserList.open("UserList.txt");
		string strLine;
		int nCounter = 0;
		if (fsUserList.is_open()){
			while (getline(fsUserList,strLine)){
				nCounter++;
			}

			fsUserList.close();
		}

		return nCounter;
}

void TCPMsnDispatcher::run() {
	while (true) {
		TCPSocket* currSock = NULL;

		while (currSock == NULL) {
			currSock = this->sckListener->listenToSocket(0);
		}

		int command;
		User* usrCurrUser = NULL;

		while (usrCurrUser == NULL){
			command = Messenger::readCommandFromPeer(currSock);


			// Check if this socket is already listed as a user
			for(map<string, User*>::iterator itr = this->mpUsersMap.begin();
								itr != this->mpUsersMap.end(); itr++) {
				if (itr->second->GetUserSocket() == currSock)
				{
					usrCurrUser = itr->second;
				}
			}

			if (usrCurrUser == NULL){

				switch(command){
					case REGISTER: {
						string strUserNname = Messenger::readDataFromPeer(currSock);
						string strPassword = Messenger::readDataFromPeer(currSock);
						// Register the user and log him in
						Messenger::RegisterUser(strUserNname, strPassword);
						//usrCurrUser = new User(currSock, strUserNname, strPassword);
						//this->addUser(usrCurrUser);

						Messenger::sendCommandToPeer(currSock, REGISTER_SUCCEEDED);
						break;
					}
					case LOGIN: {
						string strUserNname = Messenger::readDataFromPeer(currSock);
						string strPassword = Messenger::readDataFromPeer(currSock);
						// Log the user in
						if (Messenger::LoginUser(strUserNname, strPassword)){
							usrCurrUser = new User(currSock, strUserNname, strPassword);
							map<string,User*>::iterator it = this->mpUsersMap.end();
							it = this->mpUsersMap.find(strUserNname);

							if (it == this->mpUsersMap.end()) {
								this->addUser(usrCurrUser);
								Messenger::sendCommandToPeer(currSock, LOGIN_GRANTED);
							}
							else{
								Messenger::sendCommandToPeer(currSock, LOGIN_DENIED);
							}
						}
						else{
							Messenger::sendCommandToPeer(currSock, LOGIN_DENIED);
						}
						break;
					}
					case DISCONNECT:
						Messenger::sendCommandToPeer(currSock, DISCONNECT_FAILED);
				}

				cout << "command was inserted" << endl;
			}
		}

		//command = Messenger::readCommandFromPeer(currSock);

		switch (command) {
			case LOGIN: {
				if (usrCurrUser == NULL){
					string strUserNname = Messenger::readDataFromPeer(currSock);
					string strPassword = Messenger::readDataFromPeer(currSock);
					// Log the user in
					if (Messenger::LoginUser(strUserNname, strPassword)){
						User* usrCurrUser = new User(currSock, strUserNname, strPassword);
						this->addUser(usrCurrUser);
						Messenger::sendCommandToPeer(currSock, LOGIN_GRANTED);
					}
					else{
						Messenger::sendCommandToPeer(currSock, LOGIN_DENIED);
					}
				}
				break;
			}
			case OPEN_SESSION_WITH_PEER: {
				if (usrCurrUser->GetChattingUser().compare("") != 0 ||
						usrCurrUser->GetUserRoom().compare("") != 0	){
					this->CloseSession(usrCurrUser, currSock);
				}
				string strUsernameToConnect = Messenger::readDataFromPeer(currSock);

				map<string,User*>::iterator it = this->mpUsersMap.end();
				it = this->mpUsersMap.find(strUsernameToConnect);

				if (it != this->mpUsersMap.end()) {

					if (usrCurrUser->GetUserRoom().compare("") == 0 || usrCurrUser->GetChattingUser().compare("") == 0) {
						this->CloseSession(usrCurrUser, currSock);
					}
					// From user
					Messenger::sendCommandToPeer(usrCurrUser->GetUserSocket(), SESSION_ESTABLISHED);
					cout << "session established from " << usrCurrUser->GetUserName() << endl;
					usrCurrUser->LogUserToSession(it->second);

					// To user
					Messenger::sendCommandToPeer(it->second->GetUserSocket(), SESSION_ESTABLISHED);
					cout << " to " << it->second->GetUserName() << endl;
					it->second->LogUserToSession(usrCurrUser);


				} else {
					cout << "user not exist" << endl;
					Messenger::sendCommandToPeer(currSock, SESSION_CREATE_REFUSED);
				}

				break;
			}
			case EXIT: {
				removeUser(usrCurrUser);
				cout << "socket exit session" << endl;
				currSock->cclose();
				break;
			}
			case CLOSE_SESSION_WITH_PEER: {
				cout << "socket " << currSock->destIpAndPort() << " closed session with peer" << endl;
				currSock->cclose();
				break;
			}
			case CHAT_ROOM_CREATE: {
				if (usrCurrUser->GetChattingUser().compare("") != 0 ||
						usrCurrUser->GetUserRoom().compare("") != 0	){
					this->CloseSession(usrCurrUser, currSock);
				}
				string strRoomName = Messenger::readDataFromPeer(currSock);
				Room* newRoom = new Room(strRoomName, usrCurrUser);
				this->mpRoomsMap.insert(std::pair<string, Room*>(strRoomName, newRoom));
				usrCurrUser->SetRoomName(strRoomName);
				Messenger::sendCommandToPeer(currSock, CHAT_ROOM_CREATED);
				Messenger::sendDataToPeer(currSock, currSock->destIpAndPort());

				break;
			}
			case CHAT_ROOM_ENTER:{
				if (usrCurrUser->GetChattingUser().compare("") != 0 ||
						usrCurrUser->GetUserRoom().compare("") != 0	){
					this->CloseSession(usrCurrUser, currSock);
				}

				string strRoomName = Messenger::readDataFromPeer(currSock);
				for(map<string, Room*>::iterator itr = this->mpRoomsMap.begin();
									itr != this->mpRoomsMap.end(); itr++) {
					if (itr->first.compare(strRoomName) == 0)
					{
						(itr->second)->AddUserToRoom(usrCurrUser);
						usrCurrUser->SetRoomName(strRoomName);
					}
				}

				Messenger::sendCommandToPeer(currSock, CHAT_ROOM_LOGGED_IN);
				Messenger::sendDataToPeer(currSock, currSock->destIpAndPort());

				break;
			}
			case CHAT_ROOM_EXIT: {
				for(map<string, Room*>::iterator itr = this->mpRoomsMap.begin();
									itr != this->mpRoomsMap.end(); itr++) {
					if (itr->first.compare(usrCurrUser->GetUserRoom()) == 0)
					{
						(itr->second)->RemoveUserFromRoom(usrCurrUser);
					}
				}
				break;
			}
			case CLOSE_SESSION: {
				this->CloseSession(usrCurrUser, currSock);
				break;
			}
			case LIST_ALL_USERS: {
				Messenger::sendCommandToPeer(usrCurrUser->GetUserSocket(), LIST_ALL_USERS);
				Messenger::sendCommandToPeer(usrCurrUser->GetUserSocket(),this->GetUsersNum());
				this->ListAllUsers(usrCurrUser);
				break;
			}
			case GET_CONNECTED_USER_LIST:{
				Messenger::sendCommandToPeer(usrCurrUser->GetUserSocket(), GET_CONNECTED_USER_LIST);
				Messenger::sendCommandToPeer(usrCurrUser->GetUserSocket(),this->mpUsersMap.size());
				this->ListAllConnectedUsers(usrCurrUser);
				break;
			}
			case LIST_ROOMS:{
				Messenger::sendCommandToPeer(usrCurrUser->GetUserSocket(), LIST_ROOMS);
				Messenger::sendCommandToPeer(usrCurrUser->GetUserSocket(),this->mpRoomsMap.size());
				this->ListAllRooms(usrCurrUser);
				break;
			}
			case LIST_USERS_IN_ROOM: {
				string strRoomName = Messenger::readDataFromPeer(currSock);
				Messenger::sendCommandToPeer(usrCurrUser->GetUserSocket(), LIST_USERS_IN_ROOM);
				this->ListAllUsersInSpecificRoom(strRoomName, usrCurrUser);
				break;
			}
			case DISCONNECT: {
				this->removeUser(usrCurrUser);
				this->CloseSession(usrCurrUser, currSock);
				Messenger::sendCommandToPeer(currSock, DISCONNECTED);
				break;
			}

		}
	}
}

void TCPMsnDispatcher::CloseSession(User* usrCurrUser, TCPSocket* currSock){
	if (usrCurrUser->GetChattingUser().compare("") == 0){
		for(map<string, Room*>::iterator itr = this->mpRoomsMap.begin();
							itr != this->mpRoomsMap.end(); itr++) {
			if (itr->first.compare(usrCurrUser->GetUserRoom()) == 0)
			{
				(itr->second)->RemoveUserFromRoom(usrCurrUser);
				usrCurrUser->SetRoomName("");
				if ((itr->second)->GetManager()->GetUserName().compare(usrCurrUser->GetUserName()) == 0)
				{
					this->RemoveRoom(itr->second);
					delete itr->second;
				}
			}
		}
	}
	else{
		// From user
		cout << "session disconnecting from " << usrCurrUser->GetUserName() << endl;
		map<string,User*>::iterator it = this->mpUsersMap.end();
		it = this->mpUsersMap.find(usrCurrUser->GetChattingUser());

		if (it != this->mpUsersMap.end()) {
			usrCurrUser->DisconnectUserFromSession(it->second);

			// To user
			cout << " to " << it->second->GetUserName() << endl;
			it->second->DisconnectUserFromSession(usrCurrUser);
		}
	}
}

