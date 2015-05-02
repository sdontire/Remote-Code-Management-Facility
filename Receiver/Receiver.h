#ifndef RECEIVER_H
#define RECEIVER_H
/////////////////////////////////////////////////////////////////////////
// Receiver.cpp- For receiving message from client ,server and interpreting them //
// ver 1.1                                                             //
// Dontireddy sneha reddy, CSE687 - Object Oriented Design, Spring 2015//
// Syracuse University, 315-380-0886, sdontire@syr.edu                 //
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
// Application: OOD Projects #4                                        //
// Platform:    Visual Studio 2013, Dell 2720, Win 8.1 pro             //
/////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  Receiver package receives messgae from serverExecutive and interpretes the
*  message. If message is file uploading, revceiver will write the messgae body to
*  the file name specified in the message. At the last message, it will close the file.
*
*
* Public Interface
* ------------------
*--------------------
*
* void start- start executition of receiver and check the command type
* void writeToFile- writes data received from the client to the newly created file
* setMessage- for setting acknowledgement to be sent to client
* recvAck- Client Waiting for the acknowledgement from the server
* showFiles- Retreives all the files form the given path.
* setFileMessage- Constructing message for textsearch with the result and sent to client gui
* showAllDirectories- Function to perfroming recursive directory search
* recvAck1- Client waits for acknowledgement
* recvCat- Client wating for results for Category Retreival from the server
* writeToFile1- Writes to the file in the client..
* downloadMessage- Function for retreving the content for downloading in client
* filesearchMessage- Function for reterieving files from the path specified in the message which matches the given patterns
* performTextSearch- Function for reterieving files which contains search text specified in the message
* performRetrieveCategories- Function for reterieving Categories from the path specified in the message
* performRetrieveFiles- Function for reterieving files from the path specified in the message
*
*
*  Required Files:
*  ---------------
*  Receiver.h, Receiver.cpp, BlockingQueue.h, BlockingQueue.cpp, Sockets.h, Sockets.cpp
*  FileSystem.h, FileSystem.cpp, AppHelpers.h, AppHelpers.cpp, Display.h, Display.cpp,
*  Message.h, Message.cpp
*
*  Build Command:
*-------------------------
* devenv Sockets.sln /rebuild debug
*
*  Maintenance History:
*  --------------------
* version 1.1- created Receiver Package
*/

#pragma once
#include "../BlockingQueue/BlockingQueue.h"
#include "../Sockets/Sockets.h"
#include "../Threads/Threads.h"
#include "../locks/locks.h"
#include "../Message/Message.h"
#include <string>
#include<fstream>
#include "../Display/Display.h"
//#include "../ChronoTimer/ChronoTimer.h"
using namespace std;
using Files = std::vector<std::string>;
using Path1 = std::string;
using Patterns = std::vector<std::string>;


class Receiver
{
public:
	Receiver();
	~Receiver();
	string start(string msg, Socket &si);
	string writeToFile(std::string filemsg, bool type, Socket &si); 
	void recvAck(Socket &si);
	string setMessage(std::string filemsg);
	string ReadfromFile(std::string filemsg, bool type, Socket &si);
	Files showFiles(const Path1& path, Patterns& patts);
	string setFileMessage(std::string filemsg, string resultfiles,string &result);
	std::vector<std::string> showAllDirectories(std::string & baseDirectory, std::vector<std::string> & directories);
	void recvAck1(Socket &si);
	void recvCat(Socket &si);
	string writeToFile1(std::string filemsg, bool type, Socket &si);
	string downloadMessage(string msg1,Message *msgobj,Socket &si);
	string filesearchMessage(string msg1, Message *msgobj, Socket &si);
	void recvfilesearch(Socket &si);

	string performTextSearch(string msg1, string &result);
	string performRetrieveCategories(string msg1, string &result);
	string performRetrieveFiles(string msg1,string &result);
private:
	BlockingQueue<string> q_;
	bool type=true;
	bool text=false;
	using Files = std::vector<std::string>;
	using Path1 = std::string;
	using Patterns = std::vector<std::string>;
};
#endif