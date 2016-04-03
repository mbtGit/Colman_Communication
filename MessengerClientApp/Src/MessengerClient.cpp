
#include "MessengerClient.h"
#include "TCPMessengerProtocol.h"
#include <stdlib.h>
#include "Utils.h"

/**
 * initialize all properties
 */
MessengerClient::MessengerClient(){
	sckServerSocket = NULL;
	loginIndication = false;
	running = false;
	peerAddress = "";
	sessionActive = false;
	connected = false;
	serverAddress = "";
	clbk = new OnRecieveClbkImpl();
	udpmsnMessenger = NULL;
	frdFriend = NULL;
}

/**
 * client receive loop, activated once a connection to the server is established
 */
void MessengerClient::run(){
	running = true;
	vector<string> str;
	string strCurrPort;
	while(running){
		int command = readCommand();
		string msg;
		switch (command){
		case SEND_MSG_TO_PEER:
			msg = readDataFromPeer();
			cout<<">>"<<msg<<endl;
			break;
		case SESSION_CLOSED:
			cout<<"Session was closed by remote peer"<<endl;
			break;
		case OPEN_SESSION_WITH_PEER:
			peerAddress = readDataFromPeer();
			cout<<"Session was opened by remote peer: "<<peerAddress<<endl;
			sessionActive = true;
			break;
		case SESSION_ESTABLISHED:
			cout<<"Session was established with remote peer"<<endl;
			sessionActive = true;

			frdFriend = new Friend();

			frdFriend->strPartnerName = this->readDataFromPeer();
			frdFriend->strPartnerIp = this->readDataFromPeer().c_str();
			str = Utils::split(frdFriend->strPartnerIp, ':');
			frdFriend->strPartnerIp = str[0];
			frdFriend->strPartnerPort = str[1];

			// My Port

			strCurrPort = this->readDataFromPeer();
			strCurrPort = Utils::split(strCurrPort.c_str(), ':')[1];

			udpmsnMessenger= new UDPMessenger(clbk, atoi(strCurrPort.c_str()));

			cout<< "Session opened with " + frdFriend->strPartnerName << endl;
			break;
		case LIST_ALL_USERS:
		case GET_CONNECTED_USER_LIST:
		case LIST_ROOMS:
		case LIST_USERS_IN_ROOM:
			this->PrintDataFromServer();
			break;
		case LOGIN_GRANTED:
			cout << "Logged in to " << strUserName << endl;
			break;
		case CHAT_ROOM_CHANGED:
			this->UpdateUsersInChat();
			break;
		case CHAT_ROOM_CREATED:
			cout << "The chat room you wanted was created" << endl;
			str = Utils::split(this->readDataFromPeer().c_str(), ':');
			udpmsnMessenger = new UDPMessenger(clbk, atoi(str[1].c_str()));
			break;
		case DISCONNECT_FAILED:
			cout << "You cant disconnect without login first" << endl;
			break;
		case DISCONNECTED:
			cout << "You have successfuly disconnected" << endl;
			this->disconnect();
			break;
		case CHAT_ROOM_LOGGED_IN:
			cout << "You have successfuly entered the room" << endl;
			str = Utils::split(this->readDataFromPeer().c_str(), ':');
			udpmsnMessenger = new UDPMessenger(clbk, atoi(str[1].c_str()));
			break;

		default:
			cout<<"communication with server was interrupted - connection closed"<<endl;
			running = false;
			sckServerSocket->cclose();
			connected = true;
			serverAddress = "";
			sessionActive = false;
			peerAddress = "";
			delete sckServerSocket;
			break;
		}
	}
}

/**
 * connect to the given server ip (the port is defined in the protocol header file)
 */
bool MessengerClient::connect(string ip){
	if(connected) disconnect();
	sckServerSocket = new TCPSocket(ip,MSNGR_PORT);
	if (sckServerSocket == NULL) return false;
	connected = true;
	serverAddress = ip;
	//start();
	return true;
}

/**
 * open session with the given peer address (ip:port)
 */
bool MessengerClient::open(string address){
	if(sessionActive) closeActiveSession();
	cout<<"Opening session with: "<<address<<endl;
	sendCommand(OPEN_SESSION_WITH_PEER);
	sendData(address);
	return true;
}

/**
 * close active session
 */
bool MessengerClient::closeActiveSession(){
	if (mpFriendsInRoom.size() != 0){
		this->ExitRoom();
	}
	else{
		sendCommand(CLOSE_SESSION);
		sessionActive = false;
		peerAddress = "";
	}
	return true;
}

/**
 * disconnect from messenger server
 */
bool MessengerClient::disconnect(){
	running = false;
	if (sckServerSocket != NULL){
		if(sessionActive) closeActiveSession();
		sckServerSocket->cclose();
		this->waitForThread();
	}
	return true;
}

bool MessengerClient::disconnectUser(){
	this->sendCommand(DISCONNECT);
	return true;
}

/**
 * send the given message to the connected peer
 */
bool MessengerClient::send(string msg){
	if(!sessionActive) return false;
	sendCommand(SEND_MSG_TO_PEER);
	sendData(msg);
	return true;
}

/**
 * return true if a session is active
 */
bool MessengerClient::isActiveClientSession(){
	return sessionActive;
}

/**
 * return true if connected to the server
 */
bool MessengerClient::isConnected(){
	return connected;
}

void MessengerClient::SendMsgToFriend(string p_strMessage)
{
	string msgFormat = ">[" + strUserName + "]" + p_strMessage;
	if(mpFriendsInRoom.size() != 0)
	{
		map<string, Friend*>::iterator iterStart = this->mpFriendsInRoom.begin();
		map<string, Friend*>::iterator iterEnd = this->mpFriendsInRoom.end();

		for (; iterStart != iterEnd; iterStart++)
		{
			Friend* frdCurrFriend = iterStart->second;
			cout << "try to send msg to " << frdCurrFriend->strPartnerName << endl;
			int nPort = atoi(frdCurrFriend->strPartnerPort.c_str());
			udpmsnMessenger->sendTo(msgFormat, frdCurrFriend->strPartnerIp, nPort);
		}
	}
	else
	{
		int nPort = atoi(frdFriend->strPartnerPort.c_str());
		udpmsnMessenger->sendTo(msgFormat, frdFriend->strPartnerIp, nPort);
	}
}


/**
 * read incoming command
 */
int MessengerClient::readCommand()
{
	int command;
	sckServerSocket->recv((char*)&command,4);
	command = ntohl(command);
	return command;
}

/**
 * read incoming data
 */
string MessengerClient::readDataFromPeer()
{
	string msg;
	char buff[1500];
	int msgLen;
	sckServerSocket->recv((char*)&msgLen,4);
	msgLen = ntohl(msgLen);
	int totalrc = 0;
	int rc;
	while (totalrc < msgLen)
	{
		rc = sckServerSocket->recv((char*)&buff[totalrc],msgLen-totalrc);
		if (rc>0)
		{
			totalrc += rc;
		}
		else
		{
			break;
		}
	}
	if (rc > 0 && totalrc == msgLen)
	{
		buff[msgLen] = 0;
		msg = buff;
	}
	else
	{
		sckServerSocket->cclose();
	}
	return msg;
}

/**
 * send given command
 */
void MessengerClient::sendCommand(int command)
{
	command = htonl(command);
	sckServerSocket->send((char*)&command,4);
}

/**
 * send given message
 */
void MessengerClient::sendData(string msg){
	int msgLen = msg.length();
	msgLen = htonl(msgLen);
	sckServerSocket->send((char*)&msgLen,4);
	sckServerSocket->send(msg.data(),msg.length());
}

bool MessengerClient::LoginUser(string strUsername, string strPassword)
{
	this->sendCommand(LOGIN);
	this->sendData(strUsername);
	this->sendData(strPassword);
	int resultCommand = this->readCommand();
	if(resultCommand == LOGIN_GRANTED)
	{
		cout<< "log In succeed!!! " << endl;
		connected = true;
		strUserName = strUsername;
		running = true;
		this->start();
		return true;
	}
	else{
		cout<< "log in denied" << endl;
		return false;
	}
}

void MessengerClient::RegisterUser(string strUsername, string strPassword){
	this->sendCommand(REGISTER);
	this->sendData(strUsername);
	this->sendData(strPassword);
	int resultCommand = this->readCommand();
	if(resultCommand == REGISTER_SUCCEEDED)
	{
		cout<< "register succeed!!! " << endl;
		LoginUser(strUsername, strPassword);
	}
	else{
		cout<< "register denied" << endl;
	}
}

void MessengerClient::PrintUsersFromServer()
{
	// Send the command to the server
	if (this->loginIndication)
		this->sendCommand(LIST_ALL_USERS);
}

void MessengerClient::PrintConnectedUsersFromServer()
{
	if (this->loginIndication)
		this->sendCommand(GET_CONNECTED_USER_LIST);
}

void MessengerClient::PrintRoomsFromServer()
{
	if (this->loginIndication)
		this->sendCommand(LIST_ROOMS);
}

void MessengerClient::PrintUsersInRoom(string p_strRoomName)
{
	if (this->loginIndication)
	{
		this->sendCommand(LIST_USERS_IN_ROOM);
		this->sendData(p_strRoomName);
	}
}

void MessengerClient::PrintDataFromServer()
{
	if (this->loginIndication)
	{
		int nNumberOfIterations = this->readCommand();

		for(int i = 0; i < nNumberOfIterations; i++){
			cout << this->readDataFromPeer() << endl;
		}
	}
}

void MessengerClient::ConnectToUser(string p_strUsername)
{
	if (this->loginIndication)
	{
		this->sendCommand(OPEN_SESSION_WITH_PEER);
		this->sendData(p_strUsername);
	}
}

void MessengerClient::EnterRoom(string p_strRoomToEnter)
{
	if (this->loginIndication)
	{
		this->sendCommand(CHAT_ROOM_ENTER);
		this->sendData(p_strRoomToEnter);
	}
}

void MessengerClient::ExitRoom()
{
	if (this->loginIndication)
		this->sendCommand(CHAT_ROOM_EXIT);
		this->mpFriendsInRoom.clear();
}

void MessengerClient::UpdateUsersInChat()
{
	if (this->loginIndication)
	{
		mpFriendsInRoom.clear();
		int nNumOfIterations;
		nNumOfIterations = this->readCommand();
		for (int i = 0 ; i< nNumOfIterations;i++)
		{
			Friend* frdCurrFriend = new Friend();

			frdCurrFriend->strPartnerName = this->readDataFromPeer();
			vector<string> strIpAndPort = Utils::split(this->readDataFromPeer().c_str(), ':');
			frdCurrFriend->strPartnerIp = strIpAndPort[0];
			frdCurrFriend->strPartnerPort = strIpAndPort[1];

			mpFriendsInRoom.insert(std::pair<string, Friend*>(frdCurrFriend->strPartnerName, frdCurrFriend));
		}
	}
}

void MessengerClient::PrintStatus()
{
	if (connected)
	{
		cout << "Your are connected";
		if (strUserName.compare("") != 0)
		{
			cout << "To User " << strUserName << endl;
		}
		else
		{
			cout << "You are connected to the host but not to a user" << endl;
		}
	}
	else
	{
		cout << "You are not connected" << endl;
	}
}

void MessengerClient::CreateRoom(string p_strRoomToCreate)
{
	if (this->loginIndication)
	{
		this->sendCommand(CHAT_ROOM_CREATE);
		this->sendData(p_strRoomToCreate);
	}
}
