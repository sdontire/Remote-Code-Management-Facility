
/////////////////////////////////////////////////////////////////////////
// Sender.cpp- For receiving message from client and interpreting them //
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
#include "Sender.h"
#include "../Message/Message.h"
#include "../FileSystem/FileSystem.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "../locks/locks.h"
#include "../Threads/Threads.h"
#include "../Display/Display.h"
using namespace ApplicationHelpers;
using namespace FileSystem;
using namespace std;

//sending text file to the server
void Sender::sendTextFile(std::string clientip, int clientport, std::string serverip, int serverport, string filename, Socket& si, string textfile1)
{
	Path *p = new Path;
	string path = p->getName(filename, true);
	File::direction dir;
	File::type type;
	File f(filename);
	string line;
	dir = File::direction::in;
	type = File::type::text;
	f.open(dir, type);
	line = f.getLine();
	Message msg2;
		msg2 = getFileFirstMessage(clientip, clientport,serverip,serverport,line, path);
	q_.enQ(msg2.getMessage());
	string sendstring = q_.deQ();
	si.sendString(sendstring);
	while (f.getEof() == false && f.isGood())
	{
		Message msg;
		string line;
		line = f.getLine();
		msg = getOtherMessage(clientip, clientport,serverip,serverport, line, path);
		q_.enQ(msg.getMessage());
		sendstring = q_.deQ();
		si.sendString(sendstring);
	}
	string endline = "endfile";
	Message msg1;

	msg1 = getOtherMessage(clientip, clientport,serverip,serverport, endline, path);
	q_.enQ(msg1.getMessage());
	sendstring = q_.deQ();
	si.sendString(sendstring);
	size_t size = q_.size();
}

//Construct text file's first message
Message Sender::getFileFirstMessage(string clientip, int clientport, string serverip, int serverport, string line, string path)
{
	Message msg;
	msg.setCommand("uploadfile");
	string messagebody = line + '\0';
	msg.setsendAddr(clientip + "," + std::to_string(clientport));
	msg.setrecAddr(serverip + "," + std::to_string(serverport));
	msg.setmsgLen(std::to_string(sizeof(string)*messagebody.size()));
	msg.setFileAttrs(path + ";1");
	msg.setmsgBody(messagebody);
	return msg;
}

// Message constructing for download message
Message Sender::getFileFirstMessage1(string clientip, int clientport, string serverip, int serverport, string line, string path)
{
	Message msg;
	msg.setCommand("downloadfile");
	string messagebody = line + '\0';
	msg.setsendAddr(clientip + "," + std::to_string(clientport));
	msg.setrecAddr(serverip + "," + std::to_string(serverport));
	msg.setmsgLen(std::to_string(sizeof(string)*messagebody.size()));
	msg.setFileAttrs(path + ";1");
	msg.setmsgBody(messagebody);
	return msg;
}

//Construct Text file's other messages
Message Sender::getOtherMessage(string clientip, int clientport, string serverip, int serverport, string line, string path)
{
	Message msg;
	msg.setCommand("uploadfile");
	string messagebody = line + '\0';
	msg.setsendAddr(clientip + "," + std::to_string(clientport));
	msg.setrecAddr(serverip + "," + std::to_string(serverport));
	msg.setmsgLen(std::to_string(sizeof(string)*messagebody.size()));
	msg.setFileAttrs(path + ";0");
	msg.setmsgBody(messagebody);
	return msg;
}
// constructing other messages for download
Message Sender::getOtherMessage1(string clientip, int clientport, string serverip, int serverport, string line, string path)
{
	Message msg;
	msg.setCommand("downloadfile");
	string messagebody = line + '\0';
	msg.setsendAddr(clientip + "," + std::to_string(clientport));
	msg.setrecAddr(serverip + "," + std::to_string(serverport));
	msg.setmsgLen(std::to_string(sizeof(string)*messagebody.size()));
	msg.setFileAttrs(path + ";0");
	msg.setmsgBody(messagebody);
	return msg;
}

//Construct the last message
Message Sender::getEndMessage(string ip, int port)
{
	Message msg;
	msg.setCommand("EndMessage");
	string messagebody = "LastMessage\t";
	msg.setsendAddr(ip + "," + std::to_string(port));
	msg.setrecAddr(ip + "," + std::to_string(port));
	msg.setmsgLen(std::to_string(sizeof(string)*messagebody.size()));
	msg.setmsgBody(messagebody);
	return msg;

}

//Constructing and Sending download message
void Sender::sendDownloadMessage(string clientip, int clientport, string serverip, int serverport, string arg, Socket& si)
{
	Message msg;
	msg.setCommand("download");
	string messagebody = arg;
	msg.setsendAddr(clientip + "," + std::to_string(clientport));
	msg.setrecAddr(serverip + "," + std::to_string(serverport));
	msg.setmsgLen(std::to_string(sizeof(string)*messagebody.size()));
	msg.setmsgBody1(messagebody);
	string msg1 = msg.getMessage();
	si.sendString(msg1);
}

//Constructing and Sending textsearch message
void Sender::sendTextSearchMessage(string clientip, int clientport, string serverip, int serverport, string arg,string patterns,string filepath,Socket& si)
{
	Message msg;
	msg.setCommand("textsearch");
	string messagebody = arg;
	msg.setsendAddr(clientip + "," + std::to_string(clientport));
	msg.setrecAddr(serverip + "," + std::to_string(serverport));
	msg.setmsgLen(std::to_string(sizeof(string)*messagebody.size()));
	msg.setFileAttrs(filepath + ";");
	msg.setPatterns(patterns);
	msg.setmsgBody(messagebody);
	string msg1 = msg.getMessage();
	si.sendString(msg1);
}

void Sender::fileSearch(string clientip, int clientport, string serverip, int serverport, string path, string filepatterns, Socket& si)
{
	Message msg;
	msg.setCommand("filesearch");
	string messagebody = filepatterns;
	msg.setFileAttrs(path);
	msg.setsendAddr(clientip + "," + std::to_string(clientport));
	msg.setrecAddr(serverip + "," + std::to_string(serverport));
	msg.setmsgLen(std::to_string(sizeof(string)*messagebody.size()));
	msg.setmsgBody(messagebody);
	string msg1 = msg.getMessage();
	si.sendString(msg1);
}
//Sending Acknowledgement to client
void Sender::sendAck(Socket& si,string ack)
{
	si.sendString(ack);
}

// function for costructing and sending the message for retrieving the Categories.
void Sender::retrieveCategories(string clientip, int clientport, string serverip, int serverport, Socket& si)
{
	Message msg;
	msg.setCommand("retrieve_categories");
	msg.setsendAddr(clientip + "," + std::to_string(clientport));
	msg.setrecAddr(serverip + "," + std::to_string(serverport));
	string msg1 = msg.getMessage();
	si.sendString(msg1);

}

// function for costructing and sending the message for retrieving the Files.
void Sender::retrieveFiles(string clientip, int clientport, string serverip, int serverport, string category, Socket& si)
{
	Message msg;
	msg.setCommand("retrieve_files");
	msg.setsendAddr(clientip + "," + std::to_string(clientport));
	msg.setrecAddr(serverip + "," + std::to_string(serverport));
	msg.setmsgBody(category);
	string msg1 = msg.getMessage();
	si.sendString(msg1);

}

#ifdef TEST_SENDER

int main(int argc, char *argv[])
{
	string output="";
	for (int i = 0; i < argc; i++)
	{
		output += argv[i];
	}

	cout << output;
	string ip="localhost";
	int port=9080;
	filename= "../TestFolder/Test.txt";
	Socket si;
	sendBinarytFile(ip,port,filename,si)
		path="..TestFolder/Test.txt";
	string line="Project 3 OOd"
	Message msg= getFileFirstMessage(ip,port,line,path);
	Message msg1= getFileFirstMessage1(ip,port,line,path);
	Message msg2=getOtherMessage(ip,port,line,path);
	Message msg3=getOtherMessage(ip,port,line,path);
	Message endmsg=getEndMessage(ip,port);
	string ack="File uploaded successfully";
	sendAck(si,ack);
	string arg=1;
	sendNormalMessage(ip,port,arg);
	string ack="File uploaded successfully";
	retrieveFiles(ip,port,ip,port,"../ServerFiles",si);
	retrieveCategories(ip,port,ip,port,si);
	fileSearch(ip,port,ip,port,"../ServerFiles","*.cs,*.txt",si);
	sendTextSearchMessage(ip,port,ip,port,"GPA","*.cs,*.txt","../ServerFiles",si);

	sendDownloadMessage(ip,port,ip,port,"../ServerFiles/Text.txt",si);
	sendAck(si,ack);
	return 1;
}
#endif