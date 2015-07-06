/*pvaClientTest.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <iostream>

#include <pv/pvaClient.h>
#include <epicsUnitTest.h>
#include <testMain.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

class RequesterImpl : public Requester
{
public:
    RequesterImpl(){}
    string getRequesterName()
    {
         static string name("RequesterImpl");
         return name;
    }
    void message(
        string const & message,
        MessageType messageType)
    {
        cout << "RequesterImpl::message " << message << endl;
    }
    void destroy() {}
};


static void example(PvaClientPtr const &pva)
{
    cout << "\nstarting channel example\n";
    RequesterPtr requester(new RequesterImpl());
    pva->setRequester(requester);
    pva->message("this is a test",infoMessage);
    testOk(pva->getRequesterName()=="RequesterImpl","getRequesterName");
    pva->clearRequester();
    pva->message("this is a test",infoMessage);
    testOk(pva->getRequesterName()=="pvaClient","getRequesterName");
    double value = 0.0;
    try {
        value =  pva->channel("double01")->get()->getData()->getDouble();
        cout << "value " << value << endl;
        cout << "after default provider\n";
        pva->showCache();
        testOk(pva->cacheSize()==1,"cacheSize should be 1");
        value =  pva->channel("double01","ca",2.0)->get()->getData()->getDouble();
        cout << "after ca provider\n";
        pva->showCache();
        testOk(pva->cacheSize()==2,"cacheSize should be 2");
        value =  pva->channel("double01")->get()->getData()->getDouble();
        value =  pva->channel("double01","ca",2.0)->get()->getData()->getDouble();
        cout << "after two more requests\n";
        pva->showCache();
        testOk(pva->cacheSize()==2,"cacheSize should still be 2");
        PvaClientChannelPtr pvaChannel = pva->createChannel("string01");
        pvaChannel->connect();
        PvaClientGetPtr pvaGet = pvaChannel->createGet();
        PvaClientPutPtr pvaPut = pvaChannel->createPut();
        pvaGet->connect();
        pvaPut->connect();
        PvaClientPutDataPtr pvaPutData = pvaPut->getData();
        pvaPutData->putString("test");
        pvaPut->put();
        pvaGet->get();
        cout << pvaGet->getData()->getString() << endl;
        testOk(pva->cacheSize()==2,"cacheSize should be 2 after pvaGet and pvaPut");
    } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
    }
}


MAIN(pvaClientTest)
{
    cout << "\nstarting pvaClientTest\n";
    testPlan(6);
    PvaClientPtr pvaClient = PvaClient::create();
    example(pvaClient);
    cout << "done\n";
    return 0;
}
