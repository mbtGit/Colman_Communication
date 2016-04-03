/*
 * TCPMessengerProtocol.h
 *
 *  Created on: Feb 13, 2013
 *      Author: efi
 */

#ifndef TCPMESSENGERPROTOCOL_H_
#define TCPMESSENGERPROTOCOL_H_

/**
 * TCP Messenger protocol:
 * all messages are of the format [Command 4 byte int]
 * and optionally data follows in the format [Data length 4 byte int][ Data ]
 */
#define MSNGR_PORT 3346

#define CLOSE_SESSION_WITH_PEER 	1
#define OPEN_SESSION_WITH_PEER 		2
#define EXIT						3
#define SEND_MSG_TO_PEER			4
#define SESSION_REFUSED				5
#define SESSION_ESTABLISHED			6
#define GET_USER_LIST				7
#define GET_CONNECTED_USER_LIST		8
#define GET_ROOMS					9
#define GET_USERS_IN_ROOM			10
#define	LOGIN						11
#define REGISTER					12
#define CHAT_ROOM_CREATE			13
#define CHAT_ROOM_ENTER				14
#define CLOSE_SESSION				15
#define LIST_ALL_USERS				16
#define CHAT_ROOM_CREATED			17
#define LIST_SESSIONS				18
#define LIST_ROOMS					19
#define LIST_USERS_IN_ROOM			20
#define CHAT_ROOM_CHANGED			21
#define DISCONNECT					22
#define SESSION_CREATE_REFUSED		23
#define SESSION_CLOSED				24
#define LOGIN_GRANTED				25
#define REGISTER_SUCCEEDED			26
#define CHAT_ROOM_EXIT 				27
#define LOGIN_DENIED				28
#define DISCONNECT_FAILED			29
#define DISCONNECTED				30
#define CHAT_ROOM_LOGGED_IN			31
#define TEST_PEER_NAME "test"
#define SESSION_REFUSED_MSG "Connection to peer refused, peer might be busy or disconnected, try again later"



#endif /* TCPMESSENGERPROTOCOL_H_ */
