/*exampleDatabase.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2013.07.24
 */

/* Author: Marty Kraimer */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <memory>
#include <vector>
#include <iostream>

#include <pv/standardField.h>
#include <pv/standardPVField.h>
#include <pv/channelProviderLocal.h>
#include <pv/serverContext.h>
#include <pv/traceRecord.h>

#include <powerSupply.h>

#define epicsExportSharedSymbols
#include <pv/exampleDatabase.h>
#include <pv/exampleHello.h>
#include <pv/exampleHelloRPC.h>

using namespace std;
using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::exampleHello;
using namespace epics::exampleHelloRPC;

static FieldCreatePtr fieldCreate = getFieldCreate();
static StandardFieldPtr standardField = getStandardField();
static PVDataCreatePtr pvDataCreate = getPVDataCreate();
static StandardPVFieldPtr standardPVField = getStandardPVField();

static void createStructureArrayRecord(
    PVDatabasePtr const &master,
    string const &recordName)
{
    StructureConstPtr top = fieldCreate->createFieldBuilder()->
         addNestedStructureArray("value")->
             add("name",pvString)->
             add("value",pvString)->
             endNested()->
         createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord); 
    if(!result) cout<< "record " << recordName << " not added" << endl;
}

static void createRegularUnionRecord(
    PVDatabasePtr const &master,
    string const &recordName)
{
    StructureConstPtr top = fieldCreate->createFieldBuilder()->
         addNestedUnion("value")->
             add("string",pvString)->
             addArray("stringArray",pvString)->
             endNested()->
         createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
}

static void createVariantUnionRecord(
    PVDatabasePtr const &master,
    string const &recordName)
{
    StructureConstPtr top = fieldCreate->createFieldBuilder()->
         add("value",fieldCreate->createVariantUnion())->
         createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
}

static void createRegularUnionArrayRecord(
    PVDatabasePtr const &master,
    string const &recordName)
{
    StructureConstPtr top = fieldCreate->createFieldBuilder()->
         addNestedUnionArray("value")->
             add("string",pvString)->
             addArray("stringArray",pvString)->
             endNested()->
         createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
}

static void createVariantUnionArrayRecord(
    PVDatabasePtr const &master,
    string const &recordName)
{
    StructureConstPtr top = fieldCreate->createFieldBuilder()->
         addArray("value",fieldCreate->createVariantUnion())->
         createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
}

static void createDumbPowerSupplyRecord(
    PVDatabasePtr const &master,
    string const &recordName)
{
     StructureConstPtr top = fieldCreate->createFieldBuilder()->
         add("alarm",standardField->alarm()) ->
            add("timeStamp",standardField->timeStamp()) ->
            addNestedStructure("power") ->
               add("value",pvDouble) ->
               add("alarm",standardField->alarm()) ->
               endNested()->
            addNestedStructure("voltage") ->
               add("value",pvDouble) ->
               add("alarm",standardField->alarm()) ->
               endNested()->
            addNestedStructure("current") ->
               add("value",pvDouble) ->
               add("alarm",standardField->alarm()) ->
               endNested()->
            createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
}


static void createRecords(
    PVDatabasePtr const &master,
    ScalarType scalarType,
    string const &recordNamePrefix,
    string const &properties)
{
    string recordName = recordNamePrefix;
    PVStructurePtr pvStructure = standardPVField->scalar(scalarType,properties);
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
    recordName += "Array";
    pvStructure = standardPVField->scalarArray(scalarType,properties);
    pvRecord = PVRecord::create(recordName,pvStructure);
    result = master->addRecord(pvRecord);
}

void ExampleDatabase::create()
{
    PVDatabasePtr master = PVDatabase::getMaster();
    PVRecordPtr pvRecord;
    string recordName;
    bool result(false);
    recordName = "traceRecordPGRPC";
    pvRecord = TraceRecord::create(recordName);
    result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
    string properties;
    properties = "alarm,timeStamp";
    createRecords(master,pvByte,"byte01",properties);
    createRecords(master,pvShort,"short01",properties);
    createRecords(master,pvLong,"long01",properties);
    createRecords(master,pvUByte,"ubyte01",properties);
    createRecords(master,pvUInt,"uint01",properties);
    createRecords(master,pvUShort,"ushort01",properties);
    createRecords(master,pvULong,"ulong01",properties);
    createRecords(master,pvFloat,"float01",properties);

    StringArray choices(2);
    choices[0] = "zero";
    choices[1] = "one";
    master->addRecord(PVRecord::create(
         "exampleEnum",standardPVField->enumerated(choices,properties)));

    createRecords(master,pvBoolean,"exampleBoolean",properties);
    createRecords(master,pvByte,"exampleByte",properties);
    createRecords(master,pvShort,"exampleShort",properties);
    createRecords(master,pvInt,"exampleInt",properties);
    createRecords(master,pvLong,"exampleLong",properties);
    createRecords(master,pvFloat,"exampleFloat",properties);
    createRecords(master,pvDouble,"exampleDouble",properties);
    createRecords(master,pvDouble,"exampleDouble01",properties);
    createRecords(master,pvDouble,"exampleDouble02",properties);
    createRecords(master,pvDouble,"exampleDouble03",properties);
    createRecords(master,pvDouble,"exampleDouble04",properties);
    createRecords(master,pvDouble,"exampleDouble05",properties);
    createRecords(master,pvString,"exampleString",properties);
    createStructureArrayRecord(master,"exampleStructureArray");
    createRegularUnionRecord(master,"exampleRegularUnion");
    createVariantUnionRecord(master,"exampleVariantUnion");
    createRegularUnionArrayRecord(master,"exampleRegularUnionArray");
    createVariantUnionArrayRecord(master,"exampleVariantUnionArray");
    createDumbPowerSupplyRecord(master,"exampleDumbPowerSupply");
    recordName = "examplePowerSupply";
    PVStructurePtr pvStructure = createPowerSupply();
    PowerSupplyPtr psr = PowerSupply::create(recordName,pvStructure);
    if(!psr) {
          cout << "PowerSupply::create failed" << endl;
    } else {
        result = master->addRecord(psr);
        if(!result) cout<< "record " << recordName << " not added" << endl;
    }
    recordName = "exampleHello";
    result = master->addRecord(ExampleHello::create(recordName));
    if(!result) cout<< "record " << recordName << " not added" << endl;
    RPCServer::shared_pointer server(new RPCServer());
    server->registerService("exampleHelloRPC",ExampleHelloRPC::create());
}

