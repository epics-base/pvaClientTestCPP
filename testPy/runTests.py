#!/usr/bin/env python
'''Controls the database'''
import subprocess
import time
import os
#import pvatests

class DatabaseControl:
    databaseProcess = None
    DEVNULL = None

    def startDatabase(self):
        '''Starts the Database'''
        print os.getcwd()
        curdir = os.getcwd()
        os.chdir("../database/iocBoot/exampleDatabase")
        print os.getcwd()
        database_executable = "../../bin/{0}/exampleDatabase".format(os.environ['EPICS_HOST_ARCH'])
        print database_executable
        database_commands = [database_executable]
        database_commands.extend(["st.cmd"])
        self.databaseProcess = subprocess.Popen(database_commands, stdout=self.DEVNULL, stderr=subprocess.STDOUT)

    def stopDatabase(self):
        '''Stops the Database'''
        self.databaseProcess.terminate()
        if self.DEVNULL:
            self.DEVNULL.close()


if __name__ == "__main__":
    curdir = os.getcwd()
    databaseControl = DatabaseControl()
    databaseControl.startDatabase()
    time.sleep(2)
    os.chdir(curdir + "/../test")
    print "curdir",os.getcwd()
    print "calling make runtests"
    os.system("make runtests")
    print "after make"
    databaseControl.stopDatabase()
