#include <string.h>
#include <iostream>
#include <semaphore.h>

#define EMPTY -1
#define RANGE 1
#define ALLDONE 2

using namespace std;

class Msg
{
private:
    // Given Fields
    int m_iSender;
    int m_type;
    int m_value1;
    int m_value2;

public:
    // Constructor & Destructor
    Msg();
    Msg(int iSender, int type, int val1, int val2);
    ~Msg();

    // Sending Functionality
    int send(int iTo);
    // Reciviing Functionality
    int recv(int iFrom);

    void initGate();
    void destroyGate();

    // Getters
    int getType();
    int getVal1();
    int getVal2();
    bool isEmpty();

    void setVals(int iSender, int type, int val1, int val2);
    void assignEmptyBox(int i);
};
