/*
 * Room.cpp
 *
 *  Created on: Feb 11, 2016
 *      Author: user
 */

#include "Room.h"
#include "Messenger.h"

Room::Room(string p_strRoomName, User* p_usrUserToAdd) {
	this->strRoomName = p_strRoomName;
	this->vcUsersInRoom.push_back(p_usrUserToAdd);
	this->usrManager = p_usrUserToAdd;
}

Room::~Room() {
	// TODO: Tell all the users that the room is closing and let them handle it
	vector<User*>::iterator iter = this->vcUsersInRoom.begin();
	while (iter != this->vcUsersInRoom.end())
	{
		(*iter)->SetRoomName("");
		this->RemoveUserFromRoom(*iter);
	}

	this->vcUsersInRoom.clear();
}

void Room::AddUserToRoom(User* p_usrUserToAdd){
	this->vcUsersInRoom.push_back(p_usrUserToAdd);
	this->NotifyChangesInRoom();
}

string Room::GetRoomName(){
	return this->strRoomName;
}

void Room::RemoveUserFromRoom(User* p_strUserToRemove){
	// Remove the user from the list
	vector<User*> newVector;

	vector<User*>::iterator iter = this->vcUsersInRoom.begin();
	    while (iter != this->vcUsersInRoom.end())
	    {
	        if ((*iter)->GetUserName() != p_strUserToRemove->GetUserName())
	        {
	            newVector.push_back(*iter);
	        }

	        ++iter;
	    }

	    this->vcUsersInRoom = newVector;

	    this->NotifyChangesInRoom();
}

vector<User*> Room::GetAllUsersInRoom(){
	return this->vcUsersInRoom;
}

User* Room::GetManager(){
	return this->usrManager;
}
void Room::NotifyChangesInRoom(){
	vector<User*>::iterator iter = this->vcUsersInRoom.begin();
	vector<User*>::iterator end = this->vcUsersInRoom.end();
	for (; iter != end; iter++){
		// send update to user
		Messenger::sendCommandToPeer((*iter)->GetUserSocket(), CHAT_ROOM_CHANGED);

		// Send the number of users for the for loop in client minus one for the current user
		Messenger::sendCommandToPeer((*iter)->GetUserSocket(), this->vcUsersInRoom.size() - 1);

		// send all users
		for(vector<User*>::iterator usriter = this->vcUsersInRoom.begin(); usriter != this->vcUsersInRoom.end(); usriter++) {
			if((*usriter)->GetUserName() != (*iter)->GetUserName())
			{
				// send the name of the user
				Messenger::sendDataToPeer((*iter)->GetUserSocket(), (*usriter)->GetUserName());

				// send the ip and port of the user
				Messenger::sendDataToPeer((*iter)->GetUserSocket(), (*usriter)->GetUserSocket()->destIpAndPort());
			}
		}
	}
}
