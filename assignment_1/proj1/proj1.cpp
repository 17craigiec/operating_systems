#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/time.h>

using namespace std;

// NAME: Connor Craigie

/*
Build command: $ g++ -std=c++11 proj1.cpp -o doit

This program is broken into two parts essentially. If any arguments are given to the system, the program begins running
runExecutable(). However, if you input nothing you get dumped into this lovely homemade shell. The program should support
all 3 levels of the project including '&' operations and 'jobs'. This shell is certainly something! After finishing the project
I can think of so many way I could've made this code cleaner but hindsight is 20-20. Regardless, its quite impressie what can
be done with a few fork operations! Its quite easy to tell its no compairision to the quality of the standard shell. I can't
even tell you how many times ive tried to press the up arrow and spammed '[^]A' or something like that into the command line.
But it certainly got the job done, and I feel it was quite an engaging first project! Please enjoy using "cat foo.txt" and
watching the complete works of William Shakespeare speed by your screen.
*/

// simple get timestamp fcn
long int getTime()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
}



// runExecutable will be called if the argument number is above 0
void runExecutable(int argc, char *argv[])
{
    // define variable to input in execvp
    char *exe_input[argc];

    // DISPLAY ARGUMENTS
    cout << endl;

    cout << "Number of arguments: " << argc-1 << endl;
    for(int i = 0; i < argc; i++)
    {
        if(!i==0)
        {
            cout << i << ".  " << argv[i] << endl;
            exe_input[i-1] = argv[i];
        }
        exe_input[argc-1] = NULL;
    }
    cout << endl;

    // init a pid variable 
    // As a side note every time I see the letters PID, I immediately think of feedback control)
    int pid;

    if ((pid = fork()) < 0) 
    {
	    cerr << "Fork error\n";
        exit(1);
    }
    else if (pid == 0) 
    {
        /* child process */

        // DISPLAY EXECUTED COMMAND
        cout << "Execution output: " << endl;
        if(execvp(exe_input[0], exe_input) < 0)
        {
            cerr << "Error attempting execution" << endl;
            exit(1);
        }
    }
    else 
    {
        /* parent process */
        int returnStatus;   

        // record the process execution time 
        long int process_start_time = getTime();
        waitpid(pid, &returnStatus, 0);
        long int process_wall_clock_time = getTime() - process_start_time;

        int who = RUSAGE_CHILDREN;
        struct rusage usage;

        // DISPLAY RESOURCE STATISTICS
        cout << endl << "Resource statistics: " << endl;
        getrusage(who, &usage);
        
        cout << "1.  User CPU Time Used:            " << usage.ru_utime.tv_usec/1000 << " ms" << endl;
        cout << "2.  System CPU Time Used:          " << usage.ru_stime.tv_usec/1000 << " ms" << endl;
        cout << "3.  Wall Clock Execution Time:     " << process_wall_clock_time << " ms" << endl;
        cout << "3.  Voluntary Context Switches:    " << usage.ru_nvcsw << endl;
        cout << "4.  Involuntary Context Switches:  " << usage.ru_nivcsw << endl;
        cout << "5.  Number of Major Page Faults:   " << usage.ru_majflt << endl;
        cout << "6.  Number of Minor Page Faults:   " << usage.ru_minflt << endl;
    }

    cout << endl << endl;

    return;
}



// initShell starts a very basic shell script
void initShell()
{
    // initialize variables here outside the while
    bool done = 0;
    int pid;

    string prompt = "==>";
    string command;
    string command_prev = "";

    while(!done)
    {
        cout << prompt;
        getline(cin, command);

        // parse the command into an array of char
        int argc = 0;
        char **argv;

        // count number of spaces
        int spaces = 0;
        // space reset checks for double space inputs eg '  ' not expected ' '
        bool space_reset = false;
        for(int i = 0; i < command.size(); i++)
        {
            if(command[i] == ' ')
            {
                if(space_reset)
                {
                    spaces++;
                }
                space_reset = false;
            }
            else
            {
                space_reset = true;
            }
            
        }
        // from spaces, we can determine number of inputs
        argc = spaces + 2;

        // setup variables to later assign to a double pointer
        vector<char*> param_list(argc);
        vector<string> param_strings(argc);
        string current_param = "";
        int param_index = 0;

        // param_list always begins with the script label
        char* scpt = new char[6];
        strcpy(scpt, ".doit");
        param_list[param_index] = scpt;

        // track if its a paralell operation
        bool start_fork = false;

        for(int i = 0; i < command.size(); i++)
        {
            if(command[i] == ' ' || i == command.size()-1)
            {
                if(i == command.size()-1 && command[i] != '&')
                {
                    current_param.push_back(command[i]);
                }
                else if(command[i] == '&')
                {
                    start_fork = true;
                    argc = argc - 1;
                }
                

                char* p = new char[current_param.length() + 1];
                strcpy(p, current_param.c_str());

                if(current_param != "")
                {
                    param_index++;
                    param_list[param_index] = p;
                    param_strings[param_index] = current_param;
                }

                current_param = "";
            }
            else
            {
                current_param.push_back(command[i]);
            }
        }
        // from this param parse we have constructed the args
        argv = &param_list[0];

        string command = param_strings[1];
        cout << endl;

        // check to see if this script will be run in the background
        if(command == "set")
        {
            prompt = param_strings[param_index];
        }
        else if(command == "cd" && param_strings.size() > 2)
        {
            char* dir = new char[param_strings[2].size()];
            cout << "Transferring directory to: " << param_strings[2] << endl << endl;
            strcpy(dir, param_strings[2].c_str());
            chdir(dir);
        }
        else if(command == "jobs")
        {
            if(command_prev == "")
            {
                cout << "No processes running, try calling a command with '&'" << endl;
            }
            else
            {
                cout << "Process Number: [" << pid << "]   " << "Process Name: [" << command_prev << "]" << endl;
            }
        }
        else if(start_fork)
        {
            cout << "Initializing Background Task......" << endl << endl;
            // now that the command is identified as a paralell process, fork the process

            if ((pid = fork()) < 0) 
            {
                cerr << "Fork error\n";
                exit(1);
            }
            else if (pid == 0) 
            {
                /* child process */
                cout << "[" << getpid() << "] " << "PROCESS STARTING" << endl;
                runExecutable(argc, argv);
                cout << "[" << getpid() << "] " << "PROCESS COMPLETE!" << endl << endl;
                cout << prompt;
                return;
            }
            else 
            {
                /* parent process */
                usleep(750);
                //if the exit command is called, wait for all other processes to finish
                if(command == "exit")
                {
                    int returnStatus; 
                    waitpid(pid, &returnStatus, 0);
                    cout << "Command Terminating......" << endl;
                    return;
                }
                
            }
        }
        else if(command == "exit")
        {
            done = true;

            int returnStatus;
            waitpid(pid, &returnStatus, 0);
            cout << "Command Terminating......" << endl;
            return;
        }
        else
        {
            runExecutable(argc, argv);
        }

        command_prev = command;
    }
}



int main(int argc, char *argv[])
{
    // check num arguments
    if(argc > 1)
    {
        // if arguments are passed
        runExecutable(argc, argv);
    }
    else
    {
        // if arguments are not passed
        initShell();
    }
    
    return 0;
}
