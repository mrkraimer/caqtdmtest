/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
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
#include <pv/ntscalar.h>
#include <pv/ntscalarArray.h>
#include <pv/ntenum.h>

#define epicsExportSharedSymbols
#include <pv/exampleDatabase.h>
#include <pv/exampleHelloRecord.h>
#include <pv/exampleHelloRPCRecord.h>
#include <pv/traceRecord.h>
#include <pv/removeRecord.h>

using namespace std;
using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::nt;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::exampleCPP::database;

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

static void createRestrictedUnionRecord(
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

static void createRestrictedUnionArrayRecord(
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
    string const &recordNamePrefix)
{
    string recordName = recordNamePrefix;
    NTScalarBuilderPtr ntScalarBuilder = NTScalar::createBuilder();
    PVStructurePtr pvStructure = ntScalarBuilder->
        value(scalarType)->
        addAlarm()->
        addTimeStamp()->
        createPVStructure();
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
    recordName += "Array";
    NTScalarArrayBuilderPtr ntScalarArrayBuilder = NTScalarArray::createBuilder();
    pvStructure = ntScalarArrayBuilder->
        value(scalarType)->
        addAlarm()->
        addTimeStamp()->
        createPVStructure();
    pvRecord = PVRecord::create(recordName,pvStructure);
    result = master->addRecord(pvRecord);
}

void ExampleDatabase::create()
{
    PVDatabasePtr master = PVDatabase::getMaster();
    bool result(false);
    createRecords(master,pvBoolean,"caqtdmPVRboolean");
    createRecords(master,pvByte,"caqtdmPVRbyte");
    createRecords(master,pvShort,"caqtdmPVRshort");
    createRecords(master,pvInt,"caqtdmPVRint");
    createRecords(master,pvLong,"caqtdmPVRlong");
    createRecords(master,pvUByte,"caqtdmPVRubyte");
    createRecords(master,pvUInt,"caqtdmPVRuint");
    createRecords(master,pvUShort,"caqtdmPVRushort");
    createRecords(master,pvULong,"caqtdmPVRulong");
    createRecords(master,pvFloat,"caqtdmPVRfloat");
    createRecords(master,pvDouble,"caqtdmPVRdouble");
    createRecords(master,pvString,"caqtdmPVRstring");

    createRecords(master,pvDouble,"caqtdmPVRdouble01");
    createRecords(master,pvDouble,"caqtdmPVRdouble02");
    createRecords(master,pvDouble,"caqtdmPVRdouble03");
    createRecords(master,pvDouble,"caqtdmPVRdouble04");
    createRecords(master,pvDouble,"caqtdmPVRdouble05");
    result = master->addRecord(TraceRecord::create("caqtdmPVRtraceRecord"));
    if(!result) cout<< "record caqtdmPVRtraceRecord not added\n";
    result = master->addRecord(RemoveRecord::create("caqtdmPVRremoveRecord"));
    if(!result) cout<< "record caqtdmPVRremoveRecord not added\n";

    NTEnumBuilderPtr ntEnumBuilder = NTEnum::createBuilder();
    PVStructurePtr pvStructure = ntEnumBuilder->
        addAlarm()->
        addTimeStamp()->
        createPVStructure();
    shared_vector<string> choices(2);
    choices[0] = "zero";
    choices[1] = "one";
    PVStringArrayPtr pvChoices = pvStructure->getSubField<PVStringArray>("value.choices");
    pvChoices->replace(freeze(choices));
    result = master->addRecord(PVRecord::create("caqtdmPVRenum",pvStructure));
    if(!result) cout<< "record caqtdmPVRenum not added\n";

    createStructureArrayRecord(master,"caqtdmPVRstructureArray");
    createRestrictedUnionRecord(master,"caqtdmPVRrestrictedUnion");
    createVariantUnionRecord(master,"caqtdmPVRvariantUnion");
    createRestrictedUnionArrayRecord(master,"caqtdmPVRrestrictedUnionArray");
    createVariantUnionArrayRecord(master,"caqtdmPVRvariantUnionArray");
    createDumbPowerSupplyRecord(master,"caqtdmPVRdumbPowerSupply");
    result = master->addRecord(ExampleHelloRecord::create("caqtdmPVRhelloPutGet"));
    if(!result) cout<< "record caqtdmPVRhelloPutGet not added\n";
    result = master->addRecord(ExampleHelloRPCRecord::create("caqtdmPVRhelloRPC"));
    if(!result) cout<< "record caqtdmPVRhelloRPC not added\n";
}

