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

#include <fstream>
#include "Receiver.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "../FileSystem/FileSystem.h"
#include "../Message/Message.h"
#include "../Sender/Sender.h"
using namespace FileSystem;
using namespace ApplicationHelpers;


// Class for TextSearch
class TextSearch
{
public:
	// Fucntion for searching the string.
	bool TextSearch::search(string sstr, string path)
	{
		std::ostringstream stext;
		std::ofstream fs;
		Path *p = new Path();
		fs.open(path, std::fstream::in);
		if (fs.good())
		{
			stext << fs.rdbuf();
			std::string text = stext.str();
			size_t pos = text.find(sstr);
			if (pos < text.size())
				return true;
		}
		return false;
	}
private:
	mutex file_mutex;
	using Files = std::vector<std::string>;
	using SearchString = std::string;
};

Receiver::Receiver()
{

}


Receiver::~Receiver()
{
}
// Starts the receiver
string Receiver::start(string msg1,Socket &si)
{
	try
		{
			    Message *msgobj = new Message();
				msgobj->setMessage(msg1);
				string msgstr="";
				if (msg1.find("command:uploadfile") != std::string::npos)
					msgstr=writeToFile(msg1, type,si);
				if (msg1.find("command:download") != std::string::npos)
					msgstr=downloadMessage(msg1, msgobj,si);
				else if (msg1.find("command:filesearch") != std::string::npos)
					msgstr = filesearchMessage(msg1, msgobj, si);
				else if (msg1.find("command:textsearch") != std::string::npos)
				{
					string result = "";
					result = performTextSearch(msg1, result);
					msgstr = result;
					//si.sendString(result);
				}
				else if (msg1.find("command:retrieve_categories") != std::string::npos)
				{
					string result = "";
					result=performRetrieveCategories(msg1,result);
					msgstr = result;
					//si.sendString(result);
				}
				else if (msg1.find("command:retrieve_files") != std::string::npos)
				{
					string result = "";
					result = performRetrieveFiles(msg1, result);
					msgstr = result;
					//si.sendString(result);
				}
				return msgstr;
		}

		catch (std::exception& ex)
		{
			Display *disp = new Display();
			disp->Exeception(ex);
			return "";
		}
		
}

// Function for reterieving files from the path specified in the message
string Receiver::performRetrieveFiles(string msg1, string &result)
{
	Message *m = new Message();
	Message *msgobj = new Message();
	msgobj->setMessage(msg1);
	std::vector<string> all_files;
	string category_path = "../ServerFiles/" + m->getmsgBody1(msg1);
	std::vector<string> pattern;
	pattern.push_back("*.*");
	all_files = showFiles(category_path, pattern);
	string files = "";
	for (size_t k = 0; k < all_files.size(); k++)
	{
		files += all_files[k];
		if (k != all_files.size() - 1)
			files += ',';
	}
	Message filemsg;
	filemsg.setCommand("command:retrieve_files");
	filemsg.setsendAddr(msgobj->getrecvAddr(msg1));
	filemsg.setrecAddr(msgobj->getsendAddr(msg1));
	filemsg.setmsgBody(files);
	result = filemsg.getMessage();
	return result;
}

// Function for reterieving Categories from the path specified in the message
string Receiver::performRetrieveCategories(string msg1, string &result)
{
	Path *p = new Path();
	Message *msgobj = new Message();
	msgobj->setMessage(msg1);
	std::vector<std::string> temp_directories = FileSystem::Directory::getDirectories(p->getFullFileSpec("../ServerFiles"));
	string directories = "";
	for (size_t i = 2; i < temp_directories.size(); i++)
	{
		directories += temp_directories[i];
		if (i != temp_directories.size() - 1)
			directories += ',';
	}
	Message Catmsg;
	Catmsg.setCommand("command:retrieve_categories");
	Catmsg.setsendAddr(msgobj->getrecvAddr(msg1));
	Catmsg.setrecAddr(msgobj->getsendAddr(msg1));
	Catmsg.setmsgBody(directories);
	result = Catmsg.getMessage();
	return result;
}

// Function for reterieving files which contains search text specified in the message
string Receiver::performTextSearch(string msg1, string &result)
{
	Display * disp = new Display();
	TextSearch *ts = new TextSearch();
	Message *m = new Message();
	BlockingQueue<std::string> resultfiles;
	std::vector<string> file_patterns, all_files;  
	string sstr = m->getmsgBody1(msg1), file_path = m->getFileName(msg1), filepattern = m->getPatterns(msg1), token, resultfiles1;
	disp->show("\n*******************************************\n");
	string prtmsg ="Performing Text Search for string: "+ sstr + "  on files with patterns " + filepattern+"\n";
	disp->show(prtmsg);
	std::istringstream ss(filepattern);
	while (std::getline(ss, token, ',')) 
	{
		file_patterns.push_back(token);
	}
	all_files = showFiles(file_path, file_patterns);
	for each (string file in all_files)
	{
		if (ts->search(sstr, file))
			resultfiles.enQ(file);
	}
	size_t vecsize = resultfiles.size();
	for (size_t i = 0; i < vecsize; i++)
	{
		resultfiles1 = resultfiles1 + resultfiles.deQ();
		resultfiles1.append(";");
	}
	disp->show("Sending Result File list to client \n");
	disp->show("*******************************************\n\n");
	Message *resmsg = new Message();
	Message orgmsg;
	orgmsg.setMessage(msg1);
	resmsg->setCommand("textsearch");
	resmsg->setFileAttrs(orgmsg.getFileName(msg1));
	resmsg->setsendAddr(orgmsg.getrecvAddr(msg1));
	resmsg->setrecAddr(orgmsg.getsendAddr(msg1));
	resmsg->setmsgBody(resultfiles1);
	result = resmsg->getMessage();
	return result;
}

// Function for retreving the content for downloading in client
string Receiver::downloadMessage(string msg1,Message *msgobj,Socket &si)
{
	string msgstr = "";
	Sender sd;
	Message *m = new Message();
	string filename = m->getmsgBody1(msg1), sendaddres = msgobj->getsendAddr(msg1), clientaddres = msgobj->getrecvAddr(msg1);
	string client_port, client_addrs, server_port, server_addrs, parsed, parsed1;
	stringstream input_stringstream1(sendaddres), input_stringstream(clientaddres);

	if (getline(input_stringstream1, parsed, ','))
		client_addrs = parsed;
	if (getline(input_stringstream1, parsed, ' '))
		client_port = parsed;
	if (getline(input_stringstream, parsed1, ','))
		server_addrs = parsed1;
	if (getline(input_stringstream, parsed1, ' '))
		server_port = parsed1;
	int clientport = atoi(client_port.c_str());
	int serverport = atoi(server_port.c_str());
	sd.sendTextFile(client_addrs, clientport, server_addrs, serverport, filename, si, "false");
	Message ackmsg;
	ackmsg.setCommand("Acknowledgement");
	ackmsg.setsendAddr(msgobj->getrecvAddr(msg1));
	ackmsg.setrecAddr(msgobj->getsendAddr(msg1)); 
	string s = "Message :" + msgobj->getmsgBody1(msg1) + "Received by client with address: " + msgobj->getrecvAddr(msg1);
	ackmsg.setmsgBody(s);
    msgstr = ackmsg.getMessage();
	si.sendString(msgstr);
	return msgstr;

}

// Function for reterieving files from the path specified in the message which matches the given patterns
string Receiver::filesearchMessage(string msg1, Message *msgobj, Socket &si)
{
	string msgstr = "";
	Message *m = new Message();
	std::vector<string> file_patterns, search_results;
	m->setMessage(msg1);
	string patterns = m->getmsgBody1(msg1),file_path = m->getFileName(msg1),sendaddres = m->getsendAddr(msg1),clientaddres = m->getrecvAddr(msg1);
	std::istringstream ss(patterns);
	std::string token, result = "";
	while (std::getline(ss, token, ',')) {
		file_patterns.push_back(token);
	}
	search_results = showFiles(file_path, file_patterns);
	for (size_t i = 0; i < search_results.size(); i++)
	{
		result += search_results.at(i);
		if (i != search_results.size() - 1)
			result += ",";
	}
	Display *disp = new Display();
	disp->show("\n*****************************************************\n");
	if (result == "")
		disp->show("\nNo Files Found With Specified Patterns At Given Path\n");
	else
	{
		disp->show("Performing File search with patterns: " + patterns + " at path: " + file_path+"\n");
		disp->show("Sending Result list of files to client\n");
		disp->show("**************************************************\n\n");
	}
	Message Catmsg;
	Catmsg.setCommand("command:filesearch");
	Catmsg.setsendAddr(msgobj->getrecvAddr(msg1));
	Catmsg.setrecAddr(msgobj->getsendAddr(msg1));
	Catmsg.setmsgBody(result);
	msgstr = Catmsg.getMessage();
	si.sendString(msgstr);
	return msgstr;
}

// Writes to the file in the server..
string Receiver::writeToFile(std::string filemsg, bool type,Socket &si)
{
	Display *disp = new Display();
	Message *m = new Message();
	char filestart = m->getFileStart(filemsg);
	string filename = m->getFileName(filemsg),fullpath= "../UploadedFolder/" + filename,msgbody = m->getmsgBody(filemsg),msgstr="";
	std::ofstream o;
	try
	{
		if (type == true)
		{
			if (filestart == '1')
			{
				o.open(fullpath);
				o << msgbody << endl;
			}
			else if (filemsg.find("Body:endfile") != string::npos)
			{
				o.close();
				string uploadpath = "../UploadedFolder/" + m->getFileName(filemsg);
				disp->show("\n**************************************************\n");
				disp->show("Requirement 1\n");
				string uploadstring = "Recieved File: " + m->getFileName(filemsg)+" And stored at path: " +uploadpath;
				disp->show(uploadstring);
				string pmsg = "Sending Acknowledgement to Client with address: " + m->getsendAddr(filemsg);
				disp->show(pmsg);
				disp->show("\n**************************************************\n");
				msgstr = setMessage(filemsg);
			}
			else
			{
				o.open(fullpath,ios::app);
				if (o.is_open())
					o << msgbody << endl;
			}

		}
		return msgstr;
	}
	catch (exception ex)
	{
		disp->Exeception(ex);
		return "";
	}
}

// Sets the message for the acknowledgement
string Receiver::setMessage(std::string filemsg)
{
	Message *msg = new Message();
	Message m;
	m.setMessage(filemsg);
	msg->setCommand("Acknowledgement");
	msg->setFileAttrs(m.getFileName(filemsg));
	msg->setsendAddr(m.getrecvAddr(filemsg));
	msg->setrecAddr(m.getsendAddr(filemsg));
	msg->setmsgBody("File Uploaded Successfully");
	string msgstr = msg->getMessage();
	return msgstr;
}

//Constructing message for textsearch with the result and sent to client gui
string Receiver::setFileMessage(std::string filemsg, string resultfiles, string &result)
{
	Message *msg = new Message();
	Message m;
	m.setMessage(filemsg);
	msg->setCommand("textsearch");
	msg->setFileAttrs(m.getFileName(filemsg));
	msg->setsendAddr(m.getrecvAddr(filemsg));
	msg->setrecAddr(m.getsendAddr(filemsg));
	msg->setmsgBody(resultfiles);
	result = msg->getMessage();
	return result;
}

// Waits for acknowledgment from the server
void Receiver::recvAck(Socket &si)
{
	Display *disp = new Display();
	if (si.waitForData(10000, 10))
	{
		string ack;
		while (ack.length() == 0)
		{
			ack = si.recvString();
			Message *m1 = new Message();
			m1->setMessage(ack);
			string result=m1->getmsgBody1(ack);

			if (ack.size() == 0 || ack.find("TEST_END") < std::string::npos)
				break;
			string msgdisp = "Acknowledgment: " + result;
			//t.stop();
			disp->show("\n");
			disp->show(msgdisp);
			//disp->show("Processing Time is: " + t.elapsedTime<std::chrono::milliseconds>());
		}
	}
	else
	{
		disp->show("client wait for server timed-out");
	}

}


//Client waits for acknowledgement
void Receiver::recvAck1(Socket &si)
{
	Display *disp = new Display();
	if (si.waitForData(10000, 10))
	{
		string ack;
		std::vector<string> file_patterns;
		while (ack.length() == 0)
		{
			ack = si.recvString();
			Message *m1 = new Message();
			m1->setMessage(ack);
			string result = m1->getmsgBody1(ack);
			if (ack.size() == 0 || ack.find("TEST_END") < std::string::npos)
				break;
			if (result != "")
			{
				std::istringstream ss(result);
				std::string token;

				while (std::getline(ss, token, ';'))
				{
					file_patterns.push_back(token);
				}
				disp->show("Requirement 6\n");
				disp->show("--------------------------------------\n");
				disp->show("Given Text is present in following files with details:\n");

				disp->show("File Name           FilePath            Machine Address\n");
				disp->show("*************************************\n");
				Path *p = new Path();
				for (size_t i = 0; i < file_patterns.size(); i++)
				{
					disp->show("\n");
					string name = p->getName(file_patterns.at(i));
					disp->show(name + "  ");
					string msgdisp = file_patterns.at(i);
					disp->show(msgdisp + "  ");
					disp->show("localhost,9080");
				}
			}
			else
				disp->show("Given Text not found in files with specified patterns \n");
		}
	}
	else
		disp->show("client wait for server timed-out");
}

void Receiver::recvfilesearch(Socket &si)
{
	Display *disp = new Display();
	if (si.waitForData(10000, 10))
	{
		string ack;
		std::vector<string> file_patterns;
		while (ack.length() == 0)
		{
			ack = si.recvString();
			Message *m1 = new Message();
			m1->setMessage(ack);
			string result = m1->getmsgBody1(ack);   
			if (ack.size() == 0 || ack.find("TEST_END") < std::string::npos)
				break;

			if (result != "")
			{
				std::istringstream ss(result);
				std::string token;

				while (std::getline(ss, token, ',')) {
					file_patterns.push_back(token);
				}

				disp->show("Requirement 7\n");
				disp->show("--------------------------------------\n");
				disp->show("Files with specified patterns are found in listed path below:\n");

				disp->show("File Name             FilePath       \n");
				disp->show("*************************************\n");
				Path *p = new Path();
				for (size_t i = 0; i < file_patterns.size(); i++)
				{
					disp->show("\n");
					string name = p->getName(file_patterns.at(i));
					disp->show(name);
					string msgdisp = file_patterns.at(i);
					disp->show("             " + msgdisp + "  ");
				}
			}
			else
				disp->show("Files with specified patterns are not found in the given path\n");
		}
	}
	else
		disp->show("client wait for server timed-out");
}

//Client wating for results for Category Retreival from the server
void Receiver::recvCat(Socket &si)
{
	Display *disp = new Display();
	if (si.waitForData(10000, 10))
	{
		string ack;
		std::vector<string> Categories;
		while (ack.length() == 0)
		{
			ack = si.recvString();
			Message *m1 = new Message();
			m1->setMessage(ack);
			string result = m1->getmsgBody1(ack);    
			if (ack.size() == 0 || ack.find("TEST_END") < std::string::npos)
				break;

				disp->show(result);
		}
	}
	else
	{
		disp->show("client wait for server timed-out");
	}

}

// Function to perfroming recursive directory search 
std::vector<std::string> Receiver::showAllDirectories(std::string & baseDirectory, std::vector<std::string> & directories)
{
	std::vector<std::string> temp_directories = FileSystem::Directory::getDirectories(baseDirectory);
	if (temp_directories.size() < 2){
		return directories;
	}
	else{
		directories.insert(directories.end(), temp_directories.begin() + 2, temp_directories.end());
		size_t count = 2;                                                             
		while (count < temp_directories.size()){;
			showAllDirectories(temp_directories[count], directories);
			count++;
		}
	}
	return directories;

}


// Function for retreiving all the files in the given path including sub directories
Files Receiver::showFiles(const Path1& path, Patterns& patts)
{
	Files *files = new Files();
	Path *p=new Path();
	string fullpath = p->getFullFileSpec(path);
	if (!FileSystem::Directory::exists(path))
	{
		std::ostringstream out;
		out << "path " << path << "does not exist";
		throw std::exception(out.str().c_str());
	}
	std::vector<std::string> directories;
	directories.push_back(fullpath);

	showAllDirectories((fullpath), directories);

	for (auto direc : directories){
		FileSystem::Directory::setCurrentDirectory(direc);
		for (auto patt : patts)
		{
			std::string fp = FileSystem::Directory::getCurrentDirectory();
			std::vector<std::string> temp = FileSystem::Directory::getFiles(FileSystem::Directory::getCurrentDirectory(), patt);
			for (size_t i = 0; i < temp.size(); i++){
				temp[i] = fp + "\\" + temp[i];      
			}
			files->insert(files->end(), temp.begin(), temp.end());
		}
	}
	return *files;
}

#ifdef TEST_RECEIVER
int main(int argc, char *argv[])
{
	string msg1="command:fileupload;\n;FileName:test.txt;\n;body:sneha;"
	start(msg1);
	writeToFile(msg1,true);
	writeToFile1(msg1,true);
	string body=getmsgBody(msg1);
	string body1=getbinarymsgBody(msg1);
	char c=getFileStart(msg1);
	string filename=getFileName(msg1);

	bool value= search("GPA","../ServerFiles");
	Message *msgobj=new Message();
	msgobj->setMessage(msg1);
	string msgstr="";

	msgstr=downloadMessage(msg1, msgobj,si);
	msgstr=filesearchMessage(msg1, msgobj, si);

	string result = "";
	result1=performRetrieveCategories(msg1,result);
	si.sendString(result1);
	result2=performTextSearch(msg1,"");
	si.sendString(result2);
	result3 = performRetrieveFiles(msg1, result);
	si.sendString(result3);

}
#endif