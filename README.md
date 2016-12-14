# caqtdmtest

Provides tests for caqtdm.
It has two subdirectories:

* database is an ioc database that can be accessed via channel access or via the EPICS V4 privider pva.
* caqtdm provides a way to run qtdesigner and caqtcm


## Building and starting database

* Copy the file EXAMPLERELEASE.local to a file named RELEASE.local.
* Edit RELEASE.local so that it has correct locations for all components.
* cd to directory database
* make
* cd to directory database/iocBoot/exampleDatabase
* ../../bin/linux-x86_64/exampleDatabase st.cmd 

## Examples

* Copy the file EXAMPLEsetEnv to setEnv
* Edit setEnv so that it locates caqtdm
* cd to directory caqtdm


To initialize the array records in the database

    ./initArrays


To start caqtdm execute:

      ./startDM

To start qtdesigner execute:


       ./qtdesigner

Directory caqtdm has .ui files that access records from the database.

## testconnect

This is a test for connecting to and monitoring multiple channels.


The test monitors three channels:

* caqtdmPVRdouble  - only available via provider pva
* caqtdmDBRcounter - via provider pva
* caqtdmDBRao      - via provider pva
* caqtdmDBRao      - via provider ca

To build and start the example:

* cd to directory testconnect
* make
* bin/linux-x86_64/monitorForever

To test the example first

* cd to directory database/iocBoot/exampleDatabase
* ../../bin/linux-x86_64/exampleDatabase st.cmd 

While this is running you can issue commands like

    caput caqtdmDBRao 5
    pvput caqtdmPVRdouble 5
    pvput caqtdmDBRao 4

Then stop the exampleDatabase by entering exit.

Now 

* cd to directory database
* bin/linux-x86_64/exampleDatabaseMain

Now the only channel that is connected is caqtdmPVRdouble.
Thus only commands like the following will work:

    pvput caqtdmPVRdouble 5



