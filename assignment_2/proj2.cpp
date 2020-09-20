#include <map>
#include <iostream>
#include <fstream>

using namespace std;

void printMap(map<string, int> calls)
{
    map<string, int>::iterator it;

    for ( it = calls.begin(); it != calls.end(); it++ )
    {
        std::cout << it->first  // string (key)
                << ": "
                << it->second   // string's value 
                << std::endl ;
    }
}

string parseSystemCall(string str)
{
    string system_call;

    for (int i = 0; i < str.length(); i++)
    {
        if(str[i] == '(')
        {
            //cout << "SYSCALL: " << system_call << " ";
            return system_call;
        }
        else
        {
            system_call.push_back(str[i]);
        }
    }

    return "N/A";
}

int main(int argc, char const *argv[])
{
    ifstream in;
    string str;
    string cur_call;

    int individual_calls = 0;
    int unique_calls = 0;

    in.open("ls.slog");

    map<string, int> calls;

    while(getline(in,str))
    {
        cur_call = parseSystemCall(str);
        individual_calls++;

        if(calls.count(cur_call))
        {
            calls[cur_call] = calls[cur_call] + 1;
        }
        else
        {
            unique_calls++;
            calls[cur_call] = 1;
        }
    }

    cout << "AAA: " << individual_calls << " invoked system call instances from " << unique_calls << " unique system calls" << endl;

    printMap(calls);

    return 0;
}
