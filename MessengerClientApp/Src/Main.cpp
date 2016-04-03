/*
 * main.cpp
 *
 *  Created on: Feb 6, 2013
 *      Author: Eliav Menachi
 */

#ifndef MAIN_CPP_
#define MAIN_CPP_

#include <iostream>
#include <string.h>
#include "MessengerClient.h"
#include <stdlib.h>

using namespace std;

void printInstructions(){
	cout<<"To open connect to server type: c <server ip>"<<endl
	<<"To print the user list type: lu" <<endl
	<<"To print the connected user list type: lcu" <<endl
	<<"To print all rooms type: lr" <<endl
	<<"To print all users in a specific room type: lru <room name>" <<endl
	<<"To login to the server type: login <user> <password>" <<endl
	<<"To register to the server type: register <user> <password>" <<endl
	<<"To open session with user type: o <username>"<<endl
	<<"To enter a room type: or <room name>"<<endl
	<<"Opening session will close any previously active sessions"<<endl
	<<"After the session is opened, to send message type: s <message>"<<endl
	<<"To print current status type: l"<<endl
	<<"To close opened session or exit a room type: cs"<<endl
	<<"To disconnect from server type: d"<<endl
	<<"To exit type: x"<<endl;
}

int main()
{
	cout<<"Welcome to TCP messenger"<<endl;
	printInstructions();
	MessengerClient* messenger = new MessengerClient();
	while(true){
		string msg;
		string command;
		cin >> command;
		if(command == "c"){
			string ip;
			cin >> ip;
			messenger->connect(ip);
		}else if(command == "lu"){
			messenger->PrintUsersFromServer();
		}else if(command == "lcu"){
			messenger->PrintConnectedUsersFromServer();
		}else if(command == "lr"){
			messenger->PrintRoomsFromServer();
		}else if(command == "lru"){
			cout << "Insert The Room Name:" << endl;
			string strRoomName;
			cin >> strRoomName;
			messenger->PrintUsersInRoom(strRoomName);
		}else if(command == "login"){
			cout << "Insert the username and password for login: " << endl;
			string strUserName;
			string strPassword;
			cin >> strUserName;
			cin >> strPassword;
			messenger->loginIndication = messenger->LoginUser(strUserName, strPassword);
		}else if(command == "register"){
			cout << "Insert the username and password for signup: " << endl;
			string strUserName;
			string strPassword;
			cin >> strUserName;
			cin >> strPassword;
			messenger->RegisterUser(strUserName, strPassword);
		}else if(command == "o"){
			string strUserToConnect;
			cout << "Insert the user you want to connect with: "<< endl;
			cin >> strUserToConnect;

			messenger->ConnectToUser(strUserToConnect);

		}else if(command == "or"){
			string strRoomToEnter;
			cout << "Insert the room name: " << endl;
			cin >> strRoomToEnter;
			messenger->EnterRoom(strRoomToEnter);
		}else if(command == "cr"){
			cout << "Insert the room name: " << endl;
			string strRoomToCreate;
			cin >>strRoomToCreate;
			messenger->CreateRoom(strRoomToCreate);
		}else if(command == "s"){
			cout << "Insert The message" << endl;
			string strMessage;
			std::getline(cin, strMessage);
			messenger->SendMsgToFriend(strMessage);
		}else if(command == "l"){
			messenger->PrintStatus();
		}else if(command == "cs"){
			messenger->closeActiveSession();
		}else if(command == "d"){
			messenger->disconnectUser();
		}else if(command == "x"){
			break;
		}else{
			cout<<"wrong input man"<<endl;
			printInstructions();
		}

		//printInstructions();
	}
	messenger->disconnect();
	delete messenger;
	cout<<"messenger was closed"<<endl;
	return 0;
}


#endif /* MAIN_CPP_ */
