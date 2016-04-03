/*
 * User.h
 *
 *  Created on: Feb 8, 2016
 *      Author: user
 */

#ifndef USER_H_
#define USER_H_

#include "TCPSocket.h"

class User {
private:
	TCPSocket* sckUserSocket;
	string strUserName;
	string strPassword;
	string strRoomName;
	string strChattingUser;
public:
	User(TCPSocket* p_sckUserSocket, string p_strUserName, string p_strPassword);
	virtual ~User();
	void SetRoomName(string p_strRoomName);
	void SetChattingUser(string p_strChattingUser);
	string GetUserName();
	TCPSocket* GetUserSocket();
	string GetChattingUser();
	string GetUserRoom();
	void LogUserToSession(User* usrUserToLoginTo);
	void DisconnectUserFromSession(User* usrUserToDisconnectFrom);
};

#endif /* USER_H_ */
