//============================================================================
// Name        : TCP Messenger Server
// Author      : Eliav Menachi
// Version     :
// Copyright   :
// Description : TCP Messenger application
//============================================================================
#include <strings.h>
#include <map>
#include <vector>
#include "MThread.h"
#include "TCPSocket.h"
#include "TCPMessengerProtocol.h"
#include "UDPMessenger.h"
#include "Friend.h"

using namespace std;

class OnRecieveClbkImpl : public OnRecieveClbk{
	void handleMessage(string msg){
		cout<<">"<<msg<<endl;
	}
};
/**
 * The TCP Messenger client class
 */
class MessengerClient: public MThread{
	TCPSocket* sckServerSocket;
	bool running;

	bool connected; //indicate that there is an active connection to the server
	string serverAddress; // the connected server address
	bool sessionActive; // indicate that a session with a peer is active
	string peerAddress; // the session peer address
	string strUserName;
	UDPMessenger* udpmsnMessenger;
	OnRecieveClbkImpl* clbk;
	Friend* frdFriend;
	map<string, Friend*> mpFriendsInRoom;

public:
	/**
	 * initialize all properties
	 */
	MessengerClient();
	bool loginIndication;
	/**
	 * client receive loop, activated once a connection to the server is established
	 */
	void run();

	/**
	 * connect to the given server ip (the port is defined in the protocol header file)
	 */
	bool connect(string ip);

	/**
	 * open session with the given peer address (ip:port)
	 */
	bool open(string address);

	/**
	 * close active session
	 */
	bool closeActiveSession();

	/**
	 * disconnect from messenger server
	 */
	bool disconnect();

	/**
	 * send the given message to the connected peer
	 */
	bool send(string msg);

	/**
	 * return true if a session is active
	 */
	bool isActiveClientSession();

	/**
	 * return true if connected to the server
	 */
	bool isConnected();


	void PrintUsersFromServer();

	bool LoginUser(string strUsername, string strPassword);

	void RegisterUser(string strUsername, string strPassword);

	void PrintConnectedUsersFromServer();

	void PrintRoomsFromServer();

	void PrintUsersInRoom(string p_strRoomName);

	void ConnectToUser(string p_strUsername);

	void EnterRoom(string p_strRoomToEnter);

	void UpdateUsersInChat();

	void SendMsgToFriend(string p_strMessage);

	void PrintStatus();

	void ExitRoom();

	void CreateRoom(string p_strRoomToCreate);

	bool disconnectUser();
private:

	/**
	 * read incoming command
	 */
	int readCommand();

	/**
	 * read incoming data
	 */
	string readDataFromPeer();

	/**
	 * send given command
	 */
	void sendCommand(int command);

	/**
	 * send given message
	 */
	void sendData(string msg);

	void PrintDataFromServer();

};

