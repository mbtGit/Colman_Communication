/*
 * Friend.h
 *
 *  Created on: Feb 18, 2016
 *      Author: user
 */

#ifndef FRIEND_H_
#define FRIEND_H_

#include <iostream>
#include <strings.h>

using namespace std;

class Friend {
public:
	Friend();
	virtual ~Friend();
	string strPartnerName;
	string strPartnerIp;
	string strPartnerPort;
};

#endif /* FRIEND_H_ */
