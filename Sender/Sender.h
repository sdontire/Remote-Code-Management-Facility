#ifndef SENDER_H
#define SENDER_H

/////////////////////////////////////////////////////////////////////////
// Sender.h- For receiving message from client and interpreting them //
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
*  Sender Packages sends the constructed messages to the server and also
*  sendes the acknowledegement after the server receives the messages to the client.
*  Sender also sends messages to the client form the server.
*  Sender is used by both Server and Client
*
* Public Interface
* ------------------
*--------------------
*
* void sendTextFile- Sends the constructed message for text file to server.
* void sendBinarytFile- Sends the constructed message for binary file to server.
* Message getFileFirstMessage- Constructs the first message to be sent.
* Message getOtherMessage- Constructs the messages other than first message to be sent.
* Message getEndMessage- Constructs the last message and sends to the server
* void sendNormalMessage- Constructs simple messages.
* void sendAck- Sends Acknowledgement to the server from client.
* void sendDownloadMessage- Constructs and sends the message for download
* void sendTextSearchMessage- Constructs and sends the Text Search message.
* void retrieveCategories- Constructs and sends the message for retreiving Categories.
* void retrieveFiles- Constructs and sends the message for retreiving files from server.
* void fileSearch-  Constructs and sends the message for File Search
* Message getFileFirstMessage1- Constructs the first message for the download.
* Message getOtherMessage1- Constructs the messages other than first message to be sent for download purpose.
*
*
*  Required Files:
*  ---------------
*  Sender.h, Sender.cpp, BlockingQueue.h, BlockingQueue.cpp, Sockets.h, Sockets.cpp
*  FileSystem.h, FileSystem.cpp
*
*  Build Command:
*-------------------------
* Sockets.sln
*
*  Maintenance History:
*  --------------------
* version 1.1- created Sender Package
*/


#pragma once
#include <string>
#include "../BlockingQueue/BlockingQueue.h"
#include "../Sockets/Sockets.h"
#include "../Message/Message.h"
#include "../Threads/Threads.h"
#include "../locks/locks.h"
#include "../Display/Display.h"
using namespace std;

class Sender
{
public:
	Sender(){}
	~Sender(){}

	void sendTextFile(std::string clientip, int clientport, std::string serverip, int serverport, string filename, Socket& si, string textfile1);
	void sendBinarytFile(std::string ip, int port, string filename, Socket& si);
	void sendNormalMessage(std::string ip, int port, string filename, Socket& si, bool textfile1);
	Message getFileFirstMessage(string clientip, int clientport, string serverip, int serverport, string line, string path);
	Message getOtherMessage(string clientip, int clientport, string serverip, int serverport, string line, string path);
	Message getEndMessage(string ip, int port);
	void sendNormalMessage(string clientip, int clientport, string serverip, int serverport, string arg, Socket& si);
	void sendAck(Socket& si,string ack);
	void sendDownloadMessage(string clientip, int clientport, string serverip, int serverport, string arg, Socket& si);
	void sendTextSearchMessage(string clientip, int clientport, string serverip, int serverport, string arg, string patterns, string filepath,Socket& si);
	void retrieveCategories(string clientip, int clientport, string serverip, int serverport, Socket& si);
	void retrieveFiles(string clientip, int clientport, string serverip, int serverport,string category, Socket& si);
	void fileSearch(string clientip, int clientport, string serverip, int serverport, string path, string filepatterns, Socket& si);
	Message getFileFirstMessage1(string clientip, int clientport, string serverip, int serverport, string line, string path);
	Message getOtherMessage1(string clientip, int clientport, string serverip, int serverport, string line, string path);
private:
	string TestFolderPath = "../TestFolder/";
	BlockingQueue<string> q_;
	string textfile="true";
};
#endif
