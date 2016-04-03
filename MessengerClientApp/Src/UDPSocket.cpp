/*
 * UDPSocket.cpp
 *
 *  Created on: 16/11/2015
 *      Author: Or Shapira and Matan Borenkraout
 */

#include "UDPSocket.h"
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

UDPSocket::UDPSocket(int port){
	// Create a new socket
	nSocketFd = socket(AF_INET, SOCK_DGRAM, 0);

	// Create the structure containing an internet address
	struct sockaddr_in s_in;

	// Clear s_in struct
	bzero((char*) &s_in, sizeof(s_in));

	s_in.sin_family = (short)AF_INET;
	s_in.sin_addr.s_addr = htonl(INADDR_ANY);
	s_in.sin_port = htons(port);

	// Binds a socket to an address or returns error if didnt succeed
	if (bind(nSocketFd, (struct sockaddr*)&s_in, sizeof(s_in)) < 0){
		perror("Error naming channel");
	}

	// Reseting the last ip and last port members
	nLastPort = -99999;
	strLastIp = "";
}

int UDPSocket::recv(char* buffer, int length){
	sockaddr_in from;
	socklen_t fsize = sizeof(from);

	// Clear the sockaddr_in struct
	bzero(&from, sizeof(from));
	long int n = recvfrom(nSocketFd, buffer, length, 0, (sockaddr*)&from, &fsize);

	// Copy the IP into a string object
	strLastIp = inet_ntoa(from.sin_addr);
	nLastPort = (int)ntohs(from.sin_port);

	if (n < 0){
		perror("recv Error");
	}

	return n;
}

int UDPSocket::sendTo(string msg, string ip, int port){
	struct sockaddr_in s_out;

	// Clear the sockaddr_in struct
	bzero(&s_out, sizeof(s_out));
	s_out.sin_family = (short)AF_INET;
	s_out.sin_addr.s_addr = inet_addr(ip.c_str());
	s_out.sin_port = htons(port);

	// Send the message
	int n = sendto(nSocketFd,msg.data(), msg.length(),0,(struct sockaddr*)&s_out, sizeof(s_out));

	if (n < 0)
	{
		perror("sendTo Error");
	}
	return n;
}

int UDPSocket::reply(string msg){
	return this->sendTo(msg, this->fromAddr(), nLastPort);
}

void UDPSocket::cclose(){
	// Shut down all or part of the connection open on socket FD, receptions or
	// transmissions
	shutdown(nSocketFd, SHUT_RDWR);

	//Close the socket
	close(nSocketFd);
}

string UDPSocket::fromAddr(){
	return strLastIp;
}



