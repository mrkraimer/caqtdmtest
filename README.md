# caqtdmtest

Provides tests for caqtdm.
It has two subdirectories:

* database is an ioc database that can be accessed via channel access or via the EPICS V4 privider pva.
* caqtdm provides a way to run qtdesigner and caqtcm


## Building and starting database

* Copy the file EXAMPLERELEASE.local to a file named RELEASE.local.
* Edit RELEASE.local so that it has correct locations for all components to references.
* cd to directory database
* make
* cd to directory database/iocBoot/exampleDatabase
* ../../bin/linux-x86_64/exampleDatabase st.cmd 

## Examples

* Copy the file EXAMPLEsetEnv to setEnv
* Edit setEnv so that it locates caqtdm
* cd to directory caqtdm

To start caqtdm execute:

      ./startDM

To start qtdesigner execute:


       ./qtdesigner

Directory caqtdm has .ui files that access records from the database.

