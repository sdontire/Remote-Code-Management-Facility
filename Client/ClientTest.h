#ifndef CLIENTTEST_H
#define CLIENTTEST_H

/////////////////////////////////////////////////////////////////////////////
// 
//clientExecutive.h - Test passing strings, blocks, and buffers          //
// Ver 1.2                                                                 //
// Dontireddy Sneha Reddy, CSE#687 - Object Oriented Design, Spring 2015   //
// Syracuse University, 315 380-0886, sdontire@syr.edu                     //
//-------------------------------------------------------------------------//
//                                                                         //
//-------------------------------------------------------------------------//
// Application: Demo for Project #3, Communication Channel                 //
// Platform:    Dell 2720, Win Pro 8.1, Visual Studio 2013                 //
/////////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides tests for Sockets-4.7:
*  - passing byte buffers
*  - passing strings
*  - This package passes Filename of the file to be uploaded to the server and
*    message to be sent to server using sender and receives acknowledgment from
*    the server.
*
*  Public Interface
*=--------------------------
* performUpload- Function for Uploading File Task
* performTextSearch- Function for TextSearch Task
* performFileSearch- Function for File Search Task
* initialize- Function For Initializing variables
* initializeTextSearch- Function for Initializing TextSearch variables

*  Required Files:
*  ---------------
*  clientExecutive.cpp, ServerExecutive.cpp, Message.h, Message.cpp
*  Sockets.h, Sockets.cpp, AppHelpers.h, AppHelpers.cpp, Sender.h, Sender.cpp,
*  Receiver.h, Receiver.cpp
*
*  Build Command:
*  --------------
*  devenv Sockets.sln /rebuild debug
*
*  Maintenance History:
*  --------------------
* ver 1.3- 4/28
*  ver 1.2: 4/11
*  Added Additional features- Sending messages to the server
*  ver 1.1 : 30 Mar 15
*  - minor modification to comments, above
*  ver 1.0 : 30 Mar 15
*  - first release
*/

#include "../ApplicationHelpers/AppHelpers.h"
#include "../Sockets/Sockets.h"
#include "../Sender/Sender.h"
#include "../Message/Message.h"
#include "../Receiver/Receiver.h"
#include <iostream>

class CLientTest
{
public:
	CLientTest(){}
	~CLientTest(){}
	void initializeTextSearch(string &filepattern, string &textsearch, string &filevalue, char * argv[]);
	void initialize(string &type, string &client_address, int &clientport, string &server_address, int &serverport, string &client_port, string &server_port, char *argv[]);
	void performFileSearch(string client_address, int clientport, string server_address, int serverport, string path, string patterns, string value, Socket &si);
	void performTextSearch(string client_address, int clientport, string server_address, int serverport, string textsearch, string filepattern, string filename, string filevalue, Socket &si);
	void performUpload(string filename, string client_address, int clientport, string server_address, int serverport, Socket &si);
};
#endif