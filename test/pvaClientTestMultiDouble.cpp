/*pvaClientTestMultiDouble.cpp */
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

#include <pv/pvaClientMultiDouble.h>
#include <epicsUnitTest.h>
#include <testMain.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;


static void testGood(PvaClientPtr const &pvaClient)
{
    testDiag("== testGood ==");
    try {
        size_t num = 5;
        shared_vector<string> channelNames(num);
        channelNames[0] = "exampleDouble01";
        channelNames[1] = "exampleDouble02";
        channelNames[2] = "exampleDouble03";
        channelNames[3] = "exampleDouble04";
        channelNames[4] = "exampleDouble05";

        PVStringArrayPtr pvNames =
            getPVDataCreate()->createPVScalarArray<PVStringArray>();
        pvNames->replace(freeze(channelNames));

        PvaClientMultiDoublePtr multiDouble(PvaClientMultiDouble::create(pvaClient,pvNames));
        shared_vector<double> data = multiDouble->get();
        cout << "# initial " << data << endl;

        for (size_t i=0; i<num; ++i)
            data[i] = data[i] + 1.1;
        multiDouble->put(data);
        data =  multiDouble->get();
        cout << "# final " << data << endl;

        testPass("native channels");
    } catch (std::runtime_error e) {
        testFail("testGood exception '%s'", e.what());
    }
}

static void testGoodMixed(PvaClientPtr const &pvaClient)
{
    testDiag("== testGoodMixed ==");
    try {
        size_t num = 5;
        shared_vector<string> channelNames(num);
        channelNames[0] = "exampleByte";
        channelNames[1] = "exampleShort";
        channelNames[2] = "exampleInt";
        channelNames[3] = "exampleFloat";
        channelNames[4] = "exampleDouble";

        PVStringArrayPtr pvNames =
            getPVDataCreate()->createPVScalarArray<PVStringArray>();
        pvNames->replace(freeze(channelNames));

        PvaClientMultiDoublePtr multiDouble(PvaClientMultiDouble::create(pvaClient,pvNames));
        shared_vector<double> data = multiDouble->get();
        cout << "# initial " << data << endl;

        for (size_t i=0; i<num; ++i)
            data[i] += 1.1;
        multiDouble->put(data);
        data =  multiDouble->get();
        cout << "# final " << data << endl;

        testPass("mixed channels");
    } catch (std::runtime_error e) {
        testFail("testGoodMixed exception '%s'", e.what());
    }
}

static void testChannelNotExist(PvaClientPtr const &pvaClient)
{
    testDiag("== testChannelNotExist ==");
    try {
        size_t num = 5;
        shared_vector<string> channelNames(num);
        channelNames[0] = "exampleDouble01";
        channelNames[1] = "exampleDouble02";
        channelNames[2] = "exampleDouble03";
        channelNames[3] = "NonExistChannel";
        channelNames[4] = "exampleDouble05";

        PVStringArrayPtr pvNames =
            getPVDataCreate()->createPVScalarArray<PVStringArray>();
        pvNames->replace(freeze(channelNames));

        PvaClientMultiDoublePtr multiDouble(PvaClientMultiDouble::create(pvaClient,pvNames));
        shared_vector<double> data = multiDouble->get();
        cout << "# initial " << data << endl;

        for (size_t i=0; i<num; ++i)
            data[i] += 1.1;
        multiDouble->put(data);
        data =  multiDouble->get();
        cout << "# final " << data << endl;

        testFail("non-existent channel found!");
    } catch (std::runtime_error e) {
        testPass("testChannelNotExist exception '%s'", e.what());
    }
}

static void testNonScalar(PvaClientPtr const &pvaClient)
{
    testDiag("== testNonScalar ==");
    try {
        size_t num = 5;
        shared_vector<string> channelNames(num);
        channelNames[0] = "exampleDouble01";
        channelNames[1] = "exampleDouble02";
        channelNames[2] = "exampleDouble03";
        channelNames[3] = "exampleDouble04";
        channelNames[4] = "exampleDouble05Array";

        PVStringArrayPtr pvNames =
            getPVDataCreate()->createPVScalarArray<PVStringArray>();
        pvNames->replace(freeze(channelNames));

        PvaClientMultiDoublePtr multiDouble(PvaClientMultiDouble::create(pvaClient,pvNames));
        shared_vector<double> data = multiDouble->get();
        cout << "# initial " << data << endl;

        for (size_t i=0; i<num; ++i)
            data[i] += 1.1;
        multiDouble->put(data);
        data =  multiDouble->get();
        cout << "# final " << data << endl;

        testFail("non-scalar channel allowed!");
    } catch (std::runtime_error e) {
        testPass("testNonScalar exception '%s'", e.what());
    }
}

MAIN(pvaClientTestMultiDouble)
{
    testPlan(4);
    testDiag("=== pvaClientTestMultiDouble ===");

    PvaClientPtr pvaClient = PvaClient::create();
    testGood(pvaClient);
    testGoodMixed(pvaClient);
    testChannelNotExist(pvaClient);
    testNonScalar(pvaClient);

    testDone();
    return 0;
}
