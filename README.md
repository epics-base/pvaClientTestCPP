pvaClientTestCPP
============

pvaClientTestCPP provides examples for pvaClient and pvDatabase.
There are two reasons for a separate repository:

* The examples caused circural dependices between repositories.
* The examples required a running pvAccess server

pvaClientTestCPP provides the following:

* An EPICS IOC that has both EPICS V3 DBRecords, V4 PVRecords, and pvaSrv for accessing the DBRecords.
* examples for pvaClient
* examples for pvDatabase
* make runtests


Building
--------

If a proper RELEASE.local file exists one directory level above pvaClientTestCPP
then just type:

    make

If RELEASE.local does not exist then look at <b>configure/RELEASE</b>
for directions for how to build.

To run the test
---------------

make runtests

To start the database
------------

    mrk> pwd
    /home/epicsv4/pvaClientTestCPP/database/iocBoot/exampleDatabase
    mrk> ../../bin/linux-x86_64/exampleDatabase st.cmd 

Status
------

* Readyfor release 4.5.0


