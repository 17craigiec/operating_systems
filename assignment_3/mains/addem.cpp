#include <vector>
#include <sstream>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>

#include "../public/Msg.h"

using namespace std;

void *RunParent(void *);
void *RunChild(void *);

int num_threads;
int range_val_lower;
int range_val_upper;
vector<pthread_t> thread_ids;

int inputConverter(char const *in);

int main(int argc, char const *argv[])
{
    //Setup input variables
    if(argc == 3)
    {
      //Using just an upper bound, assuming lower bound is 1
      //  EX input: ./addem 10 100 (10 threads sum count 1 to 100)
      range_val_lower = 1;
      range_val_upper = inputConverter(argv[2]);
      num_threads = inputConverter(argv[1]);

    }else if(argc == 4)
    {
      //Using both an upper bound and lower bound
      //  EX input: ./addem 10 50 100 (10 threads sum count 50 to 100)
      range_val_lower = inputConverter(argv[2]);
      range_val_upper = inputConverter(argv[3]);
      num_threads = inputConverter(argv[1]);

    }else
    {
        cerr << "ERROR: Invalid Function Input" << endl;
        return -1;
    }

    cout << "++INPUTS++   Number of threads: " << num_threads << "   Lower Range: " << range_val_lower << "   Upper Range: " << range_val_upper << endl;

    //Collect a list of thread IDs
    for(int i = 0; i < num_threads+1; i++)
    {
        pthread_t t_id;
        thread_ids.push_back(t_id);
    }

    pthread_create(&thread_ids[0], NULL, RunParent, (void *)0);

    for(int i = 0; i < thread_ids.size()-1 ; i++)
    {
        cout << "Terminating " << thread_ids[i] << endl;
        (void)pthread_join(thread_ids[i], NULL);
    }

    return 0;
}


void *RunParent(void *arg)
{
    Msg ToChildren(0, RANGE, range_val_lower, range_val_upper);
    ToChildren.assignEmptyBox(0);
    ToChildren.initGate();

    for(int i = 1; i < num_threads+1; i++)
    {
        ToChildren.assignEmptyBox(i);
        ToChildren.send(i);

        pthread_create(&thread_ids[i], NULL, RunChild, (void *)i);
    }

    Msg FromChildren;
    int count = 1;
    while(count < num_threads+1)
    {
        // wait for a message from child
        FromChildren.recv(0);
        count++;
        //cout << "Count: " << count << endl;
    }
    FromChildren.destroyGate();
}


void *RunChild(void *arg)
{
    int thread_id;
    thread_id = (long)arg;

    Msg ParentMessage;
    ParentMessage.recv(thread_id);

    // This value is recieved by the Msg class
    int sum = 0;
    if(ParentMessage.getType() == RANGE)
    {
        for(int i = ParentMessage.getVal1(); i <= ParentMessage.getVal2(); i++)
        {
            sum += i;
        }
    }

    Msg ToParent(thread_id, ALLDONE, sum, 0);
    ToParent.send(0);

}


int inputConverter(char const *in)
{
    stringstream strValue;
    strValue << in;

    int intValue;
    strValue >> intValue;

    return intValue;
}
