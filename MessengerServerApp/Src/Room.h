/*
 * Room.h
 *
 *  Created on: Feb 11, 2016
 *      Author: user
 */

#ifndef ROOM_H_
#define ROOM_H_

#include "User.h"
#include <vector>

class Room {
private:
	vector<User*> vcUsersInRoom;
	string strRoomName;
	User* usrManager;
public:
	Room(string p_strRoomName, User* p_usrUserToAdd);
	virtual ~Room();
	vector<User*> GetAllUsersInRoom();
	string GetRoomName();
	void AddUserToRoom(User* p_usrUserToAdd);
	void RemoveUserFromRoom(User* p_strUserToRemove);
	void NotifyChangesInRoom();
	User* GetManager();
};

#endif /* ROOM_H_ */
