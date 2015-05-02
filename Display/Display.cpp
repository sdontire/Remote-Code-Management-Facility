
/////////////////////////////////////////////////////////////////////////
// Display.cpp- Displaying messages received from clientExecutive and Server// 
// Executive                                                           //
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
* Display package display's the messages or strings received from clientExecutive
* and serverExecutive to the console using functions.
*
* Public Interface
* ------------------
*--------------------
* void Exeception(std::exception ex)- Prints Exception
* void cosmtic()- Prints cout messages
* void show(std::string str)- Prints the string passed to it
*
*  Required Files:
*  ---------------
*  Display.cpp,Display.h, AppHelpers.cpp, AppHelpers.h, WindowsHelpers.cpp,
*  WindowsHelpers
*
*  Build Command:
*-------------------------
* devenv Sockets.sln /rebuild debug
*
*  Maintenance History:
*  --------------------
* version 1.1- created Display Package
*/

#include "Display.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "../WindowsHelpers/WindowsHelpers.h"
#include <iostream>
#include <string>
using namespace ApplicationHelpers;

void Display::Exeception(std::exception ex)
{
	std::cout<<" Exception caught:";
	std::string exec= std::string("\n  ") + ex.what() + "\n\n";
	std::cout << exec;
}
void Display::cosmtic()
{
	std::cout << "\n  press key to exit: ";
	std::cin.get();
	std::cout << "\n\n";
}

void Display::show(std::string str)
{
	std::cout << str;
}

#ifdef TEST_DISPLAY
int main(int argc, char *argv[])
{
	string pmsg="Client sending Message";
	string ex="File not found";
	Exeception(ex);
	cosmtic();
	string str="File uploaded";
	show(str)
}
#endif
