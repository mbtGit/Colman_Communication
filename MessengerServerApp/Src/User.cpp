/*
 * User.cpp
 *
 *  Created on: Feb 8, 2016
 *      Author: user
 */

#include "User.h"
#include "Messenger.h"
User::~User() {
	// TODO Auto-generated destructor stub
}

User::User(TCPSocket* p_sckUserSocket, string p_strUserName, string p_strPassword)
{
	this->sckUserSocket = p_sckUserSocket;
	this->strUserName = p_strUserName;
	this->strPassword = p_strPassword;
	this->strChattingUser = "";
	this->strRoomName = "";
}

void User::SetRoomName(string p_strRoomName)
{
	this->strRoomName = p_strRoomName;
}
void User::SetChattingUser(string p_strChattingUser)
{
	this->strChattingUser = p_strChattingUser;
}

string User::GetUserName()
{
	return this->strUserName;
}

string User::GetChattingUser()
{
	return this->strChattingUser;
}

TCPSocket* User::GetUserSocket()
{
	return this->sckUserSocket;
}

string User::GetUserRoom(){
	return this->strRoomName;
}

void User::LogUserToSession(User* usrUserToLoginTo){
	this->strChattingUser = usrUserToLoginTo->GetUserName();
	Messenger::sendDataToPeer(this->sckUserSocket,usrUserToLoginTo->GetUserName());
	Messenger::sendDataToPeer(this->sckUserSocket,usrUserToLoginTo->GetUserSocket()->destIpAndPort());
	Messenger::sendDataToPeer(this->sckUserSocket, this->GetUserSocket()->destIpAndPort());
}

void User::DisconnectUserFromSession(User* usrUserToDisconnectFrom) {
	this->strChattingUser = "";

	Messenger::sendCommandToPeer(usrUserToDisconnectFrom->GetUserSocket(), SESSION_CLOSED);
}

