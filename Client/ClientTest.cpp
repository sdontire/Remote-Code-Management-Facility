
/////////////////////////////////////////////////////////////////////////////
// 
//clientExecutive.cpp - Test passing strings, blocks, and buffers          //
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

#include "ClientTest.h"
#include "../FileSystem/FileSystem.h"

using namespace FileSystem;

// Function for Uploading File
void performUpload(string filename, string client_address, int clientport, string server_address, int serverport, Socket &si)
{
	Display *disp = new Display();
	Sender sd;
	if (filename.find(".exe") != std::string::npos || filename.find(".bin") != std::string::npos)
		disp->show("Not demonstrating exe files");
	else
		sd.sendTextFile(client_address, clientport, server_address, serverport, filename, si, "true");
	Receiver *rs = new Receiver();
	disp->show("Uploading File:" + filename + " to server\n");
	rs->recvAck(si);
}

// Function for TextSearch
void performTextSearch(string client_address, int clientport, string server_address, int serverport, string textsearch, string filepattern, string filename, string filevalue, Socket &si)
{
	Receiver *rs = new Receiver();
	Sender s;
	if (filevalue == "false")
		filename = "../../../ServerFiles";
	else
		filename = "../ServerFiles";
	s.sendTextSearchMessage(client_address, clientport, server_address, serverport, textsearch, filepattern, filename, si);
	rs->recvAck1(si);
}

// Function for File Search
void performFileSearch(string client_address, int clientport, string server_address, int serverport, string path, string patterns, string value, Socket &si)
{
	Receiver *rs = new Receiver();
	Sender s;
	if (value == "false")
		path = "../../" + path;
	s.fileSearch(client_address, clientport, server_address, serverport, path, patterns, si);
	rs->recvfilesearch(si);
}

// Initializing varaiables
void initialize(string &type, string &client_address, int &clientport, string &server_address, int &serverport, string &client_port, string &server_port, char *argv[])
{
	type = argv[1];
	client_address = argv[2];
	client_port = argv[3];
	server_address = argv[4];
	server_port = argv[5];
	clientport = atoi(client_port.c_str());
	serverport = atoi(server_port.c_str());
}

// Initializing Text Search varaiables
void initializeTextSearch(string &filepattern, string &textsearch, string &filevalue, char * argv[])
{
	filepattern = argv[7];
	textsearch = argv[6];
	filevalue = argv[8];
}

int main(int argc, char *argv[])
{
  Display *disp = new Display();
  try
  {
    SocketSystem ss;
	Sender s;
	string stringval = "", filename,filevalue="", client_address, client_port, server_address, server_port, textsearch, filepattern, category, type, path, patterns;
	Receiver *rs = new Receiver();
    SocketConnecter si;
	int clientport=0,serverport=0;
	std::vector<std::string> file_patterns;
		initialize(type, client_address, clientport, server_address, serverport, client_port, server_port,argv);
		while (!si.connect(server_address, serverport))
			::Sleep(100);
		if (type.compare("upload") == 0)
		{
			filename = argv[6];
			performUpload(filename, client_address, clientport, server_address, serverport, si);
		}
		else if (strcmp(argv[1], "textsearch")==0)
		{
			initializeTextSearch(filepattern, textsearch, filevalue,argv);
			performTextSearch(client_address, clientport, server_address, serverport, textsearch, filepattern, filename, filevalue,si);
		}
		else if (strcmp(argv[1], "retrieve_files") == 0)
		{
			category = argv[6];
			s.retrieveFiles(client_address, clientport, server_address, serverport, category, si);
			rs->recvCat(si);
		}
		else if (strcmp(argv[1], "file_search") == 0)
		{
			path = argv[6];
			patterns = argv[7];
			string value = argv[8];
			performFileSearch(client_address, clientport, server_address, serverport, path,patterns,value, si);
		}
		else if (type.compare("retrieve_categories") == 0)
		{
			s.retrieveCategories(client_address, clientport, server_address, serverport, si);
			rs->recvCat(si);
		}
    si.shutDownSend();
  }
  catch (std::exception& ex)
  {
	  disp->Exeception(ex);
  }
}


