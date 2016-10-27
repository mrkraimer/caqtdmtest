< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase("dbd/exampleDatabase.dbd")
exampleDatabase_registerRecordDeviceDriver(pdbbase)

## Load record instances
dbLoadRecords("db/ai.db","name=caqtdmDBRai,type=ai")
dbLoadRecords("db/ao.db","name=caqtdmDBRao,type=ao")
dbLoadRecords("db/longin.db","name=caqtdmDBRlongin,type=longin")
dbLoadRecords("db/longout.db","name=caqtdmDBRlongout,type=longout")
dbLoadRecords("db/stringin.db","name=caqtdmDBRstringin,type=stringin")
dbLoadRecords("db/stringout.db","name=caqtdmDBRstringout,type=stringout")
dbLoadRecords("db/mbbi.db","name=caqtdmDBRmbbi,type=mbbi")
dbLoadRecords("db/mbbo.db","name=caqtdmDBRmbbo,type=mbbo")
dbLoadRecords("db/counter.db","name=caqtdmDBRcounter")

dbLoadRecords("db/array.db","name=caqtdmDBRbyteArray,type=CHAR")
dbLoadRecords("db/array.db","name=caqtdmDBRubyteArray,type=UCHAR")
dbLoadRecords("db/array.db","name=caqtdmDBRshortArray,type=SHORT")
dbLoadRecords("db/array.db","name=caqtdmDBRushortArray,type=USHORT")
dbLoadRecords("db/array.db","name=caqtdmDBRlongArray,type=ULONG")
dbLoadRecords("db/array.db","name=caqtdmDBRulongArray,type=ULONG")
dbLoadRecords("db/array.db","name=caqtdmDBRfloatArray,type=FLOAT")
dbLoadRecords("db/array.db","name=caqtdmDBRdoubleArray,type=DOUBLE")
dbLoadRecords("db/stringArray.db","name=caqtdmDBRstringArray")


cd ${TOP}/iocBoot/${IOC}
iocInit()
exampleDatabase
startPVAServer
