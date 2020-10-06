
#include "../public/Msg.h"

void beginRegion();
void endRegion();

Msg mailbox_list[500];
pthread_mutex_t mutex;
sem_t gate;

Msg::Msg(int iSender, int type, int val1, int val2)
{
    m_iSender = iSender;
    m_type = type;
    m_value1 = val1;
    m_value2 = val2;
}

Msg::Msg()
{
    m_iSender = EMPTY;
    m_type = EMPTY;
    m_value1 = EMPTY;
    m_value2 = EMPTY;
}

Msg::~Msg()
{

}

int Msg::send(int iTo)
{
    int count = 0;
    while(!mailbox_list[iTo].isEmpty())
    {
        count++;
        if(count > 100000) {
            cout << "Sender: " << mailbox_list[iTo].m_iSender << "   Type: " << mailbox_list[iTo].m_type << endl;
        }
    }

    beginRegion();
    mailbox_list[iTo].setVals(m_iSender, m_type, m_value1, m_value2);
    cout << "Sending to " << iTo << "   From: " << mailbox_list[iTo].m_iSender << "   Message Type: " << mailbox_list[iTo].m_type << endl;
    endRegion();

    return 1;
}

int Msg::recv(int iFrom)
{
    int count = 0;
    while(mailbox_list[iFrom].isEmpty())
    {
        // count++;
        // if(count > 100000) {
        //     //cout << "Thread " << iFrom << " is not reciving a message" << endl;
        // }
    }

    beginRegion();

    // Update Fields
    m_iSender = mailbox_list[iFrom].m_iSender;
    m_type = mailbox_list[iFrom].m_type;
    m_value1 = mailbox_list[iFrom].m_value1;
    m_value2 = mailbox_list[iFrom].m_value2;

    cout << endl << "Message Recieved   From: " << iFrom << "   Mailbox Emptied" << endl << endl;
    endRegion();

    // Empty Mailbox
    Msg empty;
    mailbox_list[iFrom] = empty;

    return 1;
}

void Msg::initGate()
{
    sem_init(&gate, 0, 1);
}

void Msg::destroyGate()
{
  sem_destroy(&gate);
}

int Msg::getType()
{
    return m_type;
}

int Msg::getVal1()
{
    return m_value1;
}

int Msg::getVal2()
{
    return m_value2;
}

bool Msg::isEmpty()
{
    return (m_type == EMPTY);
}

void Msg::setVals(int iSender, int type, int val1, int val2)
{
    m_iSender = iSender;
    m_type = type;
    m_value1 = val1;
    m_value2 = val2;
}

void Msg::assignEmptyBox(int i)
{
  Msg empty;
  mailbox_list[i] = empty;
}

// Semaphore Control Functions
void beginRegion()
{
    sem_wait(&gate);
    //pthread_mutex_lock(&mutex);
}

void endRegion()
{
    sem_post(&gate);
    //pthread_mutex_unlock(&mutex);
}
