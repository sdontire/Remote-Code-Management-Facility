
/////////////////////////////////////////////////////////////////////////
// Program.cs- Servers as a bridge between c++ and c# code            // 
//                                                                     //
// ver 1.1                                                             //
// Dontireddy sneha reddy, CSE687 - Object Oriented Design, Spring 2015//
// Syracuse University, 315-380-0886, sdontire@syr.edu                 //
// Refernce: Dr Jim Fawcett, Object Oriented Design, HelpCode- Spring 2014//
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
// Application: OOD Projects #4                                        //
// Platform:    Visual Studio 2013, Dell 2720, Win 8.1 pro             //
/////////////////////////////////////////////////////////////////////////
/*
 *  Operations:
 *  What you need to add to be compliant:
 *  - Build a WPF application that calls into these functions
 *  - Compile this as a library and the WPF will add a reference
 *    to the library.  Don't use the __declspec qualifiers.
 *    That's a native C\C++ protocol.  You can just change
 *    the applcation property to library in the Project Properties.
 *    
 *   Public Interface
 *   -----------------------------------
 *    getTextSearch_result()- To get results for Text Search
 *    setKeyWordsInputs()-To set patterns input from GUI
 *    retrieveResults()- To retrieve Results
 *    setFilesInputs() - To set inputs for files
 *    setType()- To set inputs to type variable
 *    startTestInWindow()- To intiate Client.exe
 *    startTestNoWindow()- To initiate Client without using console window
 *    processing()- Handling concurrent Requests
 *    
 *     Build Command:
*-------------------------
* devenv Sockets.sln /rebuild debug
 * 
 * *  Maintenance History:
*  --------------------
* version 1.1- created Display Package
 * 
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.IO;

namespace Bridge
{
    public class Program
    {
        object synch = new object();
        List<string> Results = new List<string>();
        string path = "..\\..\\..\\Debug";
        string keywords;
        string files;
        string type;
        string textsearch_result;

        // To get Results for Text Search
        public string getTextSearch_result()
        {
            return textsearch_result;
        }
       
        // To set patterns input from GUI
        public void setKeyWordsInputs(string inputs)
        {
            keywords = inputs;
        }

        // To retrieve Results
        public List<string> retrieveResults()
        {
            return Results;

        }

        // To set inputs for files
        public void setFilesInputs(string inputs)
        {
            files = inputs;
        }

        // To set inputs to type variable
        public void setType(string inputs)
        {
            type = inputs;
        }
        // To intiate Client.exe
        public void startTestInWindow()
        {
            ProcessStartInfo pInfo = new ProcessStartInfo();
            pInfo.FileName = path + "\\ClientTest.exe";
            pInfo.Arguments = "TextSearch 2 \"this is a search string\" dstMachine:127.0.0.1:9080 dstName:TextSearch";
            Process rpProcess = Process.Start(pInfo);
            rpProcess.WaitForExit();
            rpProcess.Dispose();
        }
        //----< Run RequestProcessor windowless >--------------------------


        // To initiate Client without using console window
        public void startTestNoWindow(string[] args)
        {
            ProcessStartInfo pInfo = new ProcessStartInfo();
            pInfo.FileName = path + "\\ClientExecutive.exe";
            string args_string="";
            for (int i = 1; i < args.Length;i++ )
            {
                args_string += args[i];
                if (i != args.Length-1)
                    args_string += " ";
            }
            pInfo.Arguments = args_string;
            pInfo.UseShellExecute = false;
            pInfo.CreateNoWindow = true;
            pInfo.RedirectStandardOutput = true;

            Stopwatch sw = new Stopwatch();
            sw.Start();
            using (Process rpProcess = Process.Start(pInfo))
            {
                using (StreamReader rdr = rpProcess.StandardOutput)
                {
                    while (true)
                    {
                        if (rdr.EndOfStream)
                            break;
                        string result = rdr.ReadToEnd();
                        textsearch_result = result;
                        lock (synch)
                        {
                            Results.Add(result);
                        }
                    }
                }
            }
            sw.Stop();
        }

        List<string> getResults() { return Results; }

        // Handling concurrent Requests
        public void processing(string[] args)
        {

            if (args.Count() == 0)
            {
                return;
            }
            // run specified number of RequestProcessors

            int numToStart = 1;

            List<Task> taskList = new List<Task>();
            for (int i = 0; i < numToStart; ++i)
            {

                taskList.Add(Task.Run(() => startTestNoWindow(args)));

            }
            // make sure we have all the results

            foreach (Task task in taskList)
            {

                task.Wait();

            }

            List<string> Results = getResults();
            foreach (string result in Results)
                Console.Write(result);
            Console.Write("\n  Test complete\n");
        }
        static void Main(string[] args)
        {
            Program ts = new Program();
            ts.processing(args);
        }
    }
}

