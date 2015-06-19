< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase("dbd/exampleDatabase.dbd")
exampleDatabase_registerRecordDeviceDriver(pdbbase)

## Load record instances
dbLoadRecords("db/dbStringArray.db","name=stringArray01")
dbLoadRecords("db/dbEnum.db","name=enum01")
dbLoadRecords("db/dbCounter.db","name=counter01")
dbLoadRecords("db/dbArray.db","name=doubleArray,type=DOUBLE")
dbLoadRecords("db/test.db")


cd ${TOP}/iocBoot/${IOC}
iocInit()
epicsThreadSleep(2.0)
exampleDatabase
startPVAServer
