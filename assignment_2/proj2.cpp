#include <map>
#include <iostream>
#include <fstream>

using namespace std;

// A struct used in the main hashmap Eg: map<string, call_info>
struct call_info
{
    int times_called = 0;
    map<string, int> prior_calls;
};

void printMap(map<string, int> calls, string seq)
{
    map<string, int>::iterator it;

    for ( it = calls.begin(); it != calls.end(); it++ )
    {
        std::cout << "  " << seq << ":" << it->first  // string (key)
                << " "
                << it->second   // string's value 
                << std::endl ;
    }
}

void printNestedMap(map<string, call_info> nested_map, bool is_seq)
{
    map<string, call_info>::iterator it;

    for ( it = nested_map.begin(); it != nested_map.end(); it++ )
    {
        cout << it->first  // string (key)
            << " "
            << it->second.times_called
            << endl;

        // ONLY INVOKED IF THE seq ARG IS PASSED
        if(is_seq)
        {
            printMap(it->second.prior_calls, it->first);
        }
    }
}

string parseSystemCall(string str)
{
    string system_call;

    for (int i = 0; i < str.length(); i++)
    {
        if(str[i] == '(')
        {
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
    string str;
    string cur_call;
    string prev_call = "first iteration";
    bool is_seq = 0;

    string last_param = argv[argc-1];
    if(last_param == "seq")
    {
        is_seq = true;
    }

    int individual_calls = 0;
    int unique_calls = 0;

    map<string, call_info> calls;

    while(getline(cin,str))
    {
        cur_call = parseSystemCall(str);
        if(cur_call != "N/A")
        {

            individual_calls++;

            // Check to see if the key already exists
            if(calls.count(cur_call))
            {
                // Add 1 to the number of times it has appeared
                calls[cur_call].times_called = calls[cur_call].times_called + 1;
            }
            else
            {
                unique_calls++;
                calls[cur_call].times_called = 1;
            }

            // Append the current call to the previous call's call info
            // This is how we will store subsequent calls
            if (prev_call != "first iteration")
            {
                // Check to see if the key already exists
                if(calls[prev_call].prior_calls.count(cur_call))
                {
                    // Add 1 to the number of times it has appeared
                    calls[prev_call].prior_calls[cur_call] = calls[prev_call].prior_calls[cur_call] + 1;
                }
                else
                {
                    calls[prev_call].prior_calls[cur_call] = 1;
                }
            }
        }

        prev_call = cur_call;
    }

    cout << "AAA: " << individual_calls << " invoked system call instances from " << unique_calls << " unique system calls" << endl;

    printNestedMap(calls, is_seq);

    return 0;
}
