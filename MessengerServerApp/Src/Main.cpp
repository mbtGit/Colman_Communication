/*
 * Main.cpp
 *
 *  Created on: Jan 27, 2016
 *      Author: Matan Borenkraout and Or Shapira
 */

#include <iostream>
#include "Messenger.h"
using namespace std;

void printInstructions(){
	cout<<"Welcome to the messenger server side"<<endl;
	cout<<"Insert the command you want"<<endl;
	cout<<"-----------------------"<<endl;
	cout<<"lu - List all users"<<endl;
	cout<<"lcu - List all connected users"<<endl;
	cout<<"ls - List all sessions"<<endl;
	cout<<"lr - List all rooms"<<endl;
	cout<<"lru <room name> - List all rooms"<<endl;
	cout<<"x - Shutdown server"<<endl;
	cout<<"-----------------------"<<endl;
}

int main(){
	cout<<"Welcome to TCP messenger Server"<<endl;
	printInstructions();
	Messenger* msngr = new Messenger();
	while(true){
		string msg;
		string command;
		cin >> command;
		if (command == "lu"){
			msngr->msnDispatcher->ListAllUsers(NULL);
		}
		else if(command == "lcu"){
			msngr->msnDispatcher->ListAllConnectedUsers(NULL);
		}
		else if(command == "ls"){
			msngr->msnDispatcher->ListAllSessions(NULL);
		}
		else if(command == "lr"){
			msngr->msnDispatcher->ListAllRooms(NULL);
		}
		else if(command == "lru"){
			string strRoomName;
			cin >> strRoomName;
			msngr->msnDispatcher->ListAllUsersInSpecificRoom(strRoomName, NULL);
		}
		else if(command == "x"){
			break;
		}
		else{
			cout << "Wrong input man" << endl;
		}

		printInstructions();
	}
	msngr->close();
	cout<<"messenger was closed"<<endl;
	return 0;
}
