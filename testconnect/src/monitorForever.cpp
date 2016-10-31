/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <queue>
#include <epicsThread.h>
#include <pv/event.h>

#include <iostream>

#include <pv/pvaClient.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

class ClientMonitor;
typedef std::tr1::shared_ptr<ClientMonitor> ClientMonitorPtr;

class CallbackThread;
typedef std::tr1::shared_ptr<CallbackThread> CallbackThreadPtr;

class epicsShareClass  CallbackThread :
    public epicsThreadRunable
{
    std::queue<ClientMonitorPtr> monitorQueue;
    std::tr1::shared_ptr<epicsThread> thread;
    Mutex mutex;
    Event runStop;
    Event runReturn;
public:
    POINTER_DEFINITIONS(CallbackThread);
    virtual void run();
    void startThread()
    {
         thread =  std::tr1::shared_ptr<epicsThread>(new epicsThread(
            *this,
            "callbackThread",
            epicsThreadGetStackSize(epicsThreadStackSmall),
            epicsThreadPriorityLow));
         thread->start();
    }
    void queueRequest(ClientMonitorPtr const & clientMonitor)
    {
        Lock xx(mutex);
        monitorQueue.push(clientMonitor);
    }
    static CallbackThreadPtr create()
    {
         CallbackThreadPtr t(new CallbackThread());
         t->startThread();
         return t;
    } 
    void stop()
    {
        runStop.signal();
        runReturn.wait();
    }
private:
    CallbackThread()
    {}
};



class ClientMonitor :
    public PvaClientChannelStateChangeRequester,
    public PvaClientMonitorRequester,
    public std::tr1::enable_shared_from_this<ClientMonitor>
{
    PvaClientPtr pvaClient;
    CallbackThreadPtr callbackThread;
    PvaClientChannelPtr channel;
    bool gotFirstConnect;
    bool unlistenCalled;
    PvaClientMonitorPtr monitor;

    ClientMonitor(
        PvaClientPtr const & pvaClient,
        CallbackThreadPtr const & callbackThread,
        PvaClientChannelPtr const & channel)
    : pvaClient(pvaClient),
      callbackThread(callbackThread),
      channel(channel),
      gotFirstConnect(false),
      unlistenCalled(false)
      
    {
        
    }
public:
    POINTER_DEFINITIONS(ClientMonitor);
    static ClientMonitorPtr create(
        PvaClientPtr const & pvaClient,
        CallbackThreadPtr const & callbackThread,
        string const &channelName,
        string const & providerName)
    {
        PvaClientChannelPtr channel = pvaClient->createChannel(channelName,providerName);
        ClientMonitorPtr cm(new ClientMonitor(pvaClient,callbackThread,channel));
        channel->setStateChangeRequester(cm);
        channel->issueConnect();
        return cm;
         
    } 

    virtual void channelStateChange(PvaClientChannelPtr const & channel, bool isConnected)
    {
         cout << "ClientMonitor::channelStateChange"
              << " channelName " << channel->getChannelName()
              << " isConnected " << isConnected
              << endl;
        if(!isConnected || gotFirstConnect) return;
        callbackThread->queueRequest(shared_from_this());
    }
    virtual void event(PvaClientMonitorPtr const & monitor)
    {
        while(monitor->poll()) {
            PvaClientMonitorDataPtr monitorData = monitor->getData();
            cout << "monitor " << channel->getChannelName() << endl;
            cout << "changed\n";
            monitorData->showChanged(cout);
            cout << "overrun\n";
            monitorData->showOverrun(cout);
            cout << endl;
            monitor->releaseEvent();
        }
    }
    virtual void unlisten()
    {
         cout << "ClientMonitorRequester::unlisten\n";
         unlistenCalled = true;
    }
    void firstContact()
    {
        try {
            monitor = channel->createMonitor("value,alarm,timeStamp");
            monitor->connect();
            monitor->setRequester(shared_from_this());
            monitor->start();
            gotFirstConnect = true;
            
        } catch (std::runtime_error e) {
            cerr << "exception " << e.what() << endl;
            return;
        }
    }
    bool isUnlisten() {return unlistenCalled;}
};

void CallbackThread::run()
{
    ClientMonitorPtr clientMonitor;
    while(true) 
    {
        epicsThreadSleep(.1);
        if(runStop.tryWait()) {
             runReturn.signal();
             return;
        }    
        {
              Lock xx(mutex);
              if(!monitorQueue.empty()) cout << "monitorQueue.size " << monitorQueue.size() << endl;
              clientMonitor = monitorQueue.front();
              if(clientMonitor) monitorQueue.pop();
        }
        while(clientMonitor) {
             clientMonitor->firstContact();
             Lock xx(mutex);
             clientMonitor = monitorQueue.front();
             if(clientMonitor) monitorQueue.pop();
        }
    }
}


int main(int argc,char *argv[])
{
    
    cout << "_____monitorForever starting_______\n";
    try {
        PvaClientPtr pvaClient = PvaClient::get("pva ca");
//PvaClient::setDebug(true);
        CallbackThreadPtr callbackThread = CallbackThread::create();
        ClientMonitorPtr caqtdmPVRdouble(
            ClientMonitor::create(pvaClient,callbackThread,"caqtdmPVRdouble","pva"));
        ClientMonitorPtr caqtdmDBRcounterPVA(
            ClientMonitor::create(pvaClient,callbackThread,"caqtdmDBRcounter","pva"));
        ClientMonitorPtr caqtdmDBRaoPVA(
            ClientMonitor::create(pvaClient,callbackThread,"caqtdmDBRao","pva"));
        ClientMonitorPtr caqtdmDBRaoCA(
            ClientMonitor::create(pvaClient,callbackThread,"caqtdmDBRao","ca"));
        while(true) {
            cout << "Type exit to stop: \n";
            string str;
            getline(cin,str);
            if(str.compare("exit")==0) {
                callbackThread->stop();
                break;
            }
        }
    } catch (std::runtime_error e) {
            cerr << "exception " << e.what() << endl;
            return 1;
    }
    return 0;
}
