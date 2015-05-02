/////////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs- Displaying messages received from Sever         // 
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
* Display's Messages from the Server and contains Event Handlers
*
* Public Interface
* ------------------
*--------------------
*  public MainWindow()- Used for initializing Components in GUI.
*
*  Build Command:
*-------------------------
* devenv Sockets.sln /rebuild debug
*
*  Maintenance History:
*  --------------------
* version 1.1- created
*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Timers;
using System.Diagnostics;
using System.IO;

using Bridge;
namespace GUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        string stringval;
        string stringval1;
        public MainWindow()
        {
            InitializeComponent();
            addItemsToListBox();
        }

        void addItemsToListBox()
        {
            stringval = "true";
            stringval1 = "true";
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Resultslv.Items.Clear();
            uploadlv.Content = "";
            sendFiles();

        }

        void sendFiles()
        {
            Program testspawn = new Program();
            List<string> Results;
            uploadtb.Clear();
            string server_address = address.Text;

            if (TextSearch.Text == "")
                MessageBox.Show("Please Enter The Text String");
            else if (keywordstb.Text == "")
                MessageBox.Show("Please Enter The patterns");
            else if (server_address == "")
                MessageBox.Show("Please the enter the endpoint address");
            else
            {
                string[] serveraddress = server_address.Split(',');
                if (serveraddress[0] != "localhost")
                    MessageBox.Show("invalid format for end-point address");
                else if (serveraddress[1] != "9080")
                    MessageBox.Show("invalid format for end-point address");
                else
                {
                    Stopwatch st = new Stopwatch();
                    st.Start();
                    stringval1 = "false";  
                    string[] input_args = { "../../Debug/ClientExecutive.exe", "textsearch", "localhost", "9080", serveraddress[0], serveraddress[1], TextSearch.Text, keywordstb.Text,stringval };
                    testspawn.processing(input_args);
                    st.Stop();
                    Results = testspawn.retrieveResults();
                    stringval = "false";
                    //string search_result = testspawn.getTextSearch_result();
                //   string[] result = testspawn.getTextSearch_result().Split(',');
              
                    //foreach (string res in result)
                    //    Resultslv.Items.Add(res);
                    Resultslv.Items.Add(testspawn.getTextSearch_result());
                    Resultslv.Items.Add("\nPERFORMANCE INFORMATION\n");
                    Resultslv.Items.Add("Total Time Taken to process request and receiving reply:" + st.ElapsedMilliseconds +"\n");
                }
            }
        }
        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            // Create OpenFileDialog 
            Microsoft.Win32.OpenFileDialog Dialog = new Microsoft.Win32.OpenFileDialog();
            Nullable<bool> result = Dialog.ShowDialog();
            if (result == true)
            {
                string filename = Dialog.FileName;
                uploadtb.Text = filename;
            }
        }

        private void Button_Click_3(object sender, RoutedEventArgs e)
        {
            Resultslv.Items.Clear();
            uploadlv.Content = "";
            Program testspawn = new Program();
            testspawn.setFilesInputs(uploadtb.Text);
            testspawn.setType("Upload");
            if (uploadtb.Text == "")
                MessageBox.Show("Please Select File For Uploading");
            else
            {
                Stopwatch st = new Stopwatch();
                st.Start();
                string[] input_args = { "../../Debug/ClientExecutive.exe", "upload", "localhost", "9080", "localhost", "9080", uploadtb.Text };
                testspawn.processing(input_args);
                st.Stop();
                string result = testspawn.getTextSearch_result();
                Resultslv.Items.Add(result);
                uploadlv.Content = "File Uploaded Successfully!!";
                Resultslv.Items.Add("\nPERFORMANCE INFORMATION\n");
                Resultslv.Items.Add("Total Time Taken to process request and receiving reply:" + st.ElapsedMilliseconds + "\n");            
            }
        }

        private void keywordstb_TextChanged(object sender, TextChangedEventArgs e)
        {

        }


        private void ListView_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }

      
        private void Button_Click_4(object sender, RoutedEventArgs e)
        {
            Resultslv.Items.Clear();
            Program testspawn = new Program();
            if (file_path.Text == "")
                MessageBox.Show("Please Enter The Path");
            else if (file_patterns.Text == "")
                MessageBox.Show("Please Enter The Patterns");
            else
            {
                Stopwatch st = new Stopwatch();
                st.Start();
                stringval = "false";
                string[] input_args = { "../../Debug/ClientExecutive.exe", "file_search", "localhost", "9080", "localhost", "9080", file_path.Text, file_patterns.Text,stringval1 };
                testspawn.processing(input_args);
                st.Stop();
                string[] result = testspawn.getTextSearch_result().Split(',');
                stringval1 = "false"; 
                foreach (string res in result)
                    Resultslv.Items.Add(res);
                Resultslv.Items.Add("\nPERFORMANCE INFORMATION\n");
                Resultslv.Items.Add("Total Time Taken to process request and receiving reply:" + st.ElapsedMilliseconds + "\n");
            }

        }

        private void uploadtb_TextChanged(object sender, TextChangedEventArgs e)
        {

        }
    }
}
