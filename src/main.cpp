#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cmdBuffer.h>

using std::cout;
using std::endl;

#define NUM_BYTES_SERIAL_BT_BUFF    1024
#define MAX_LOG_MSG                 1024
#define NUM_TEMP_INTS               3 
#define NUM_TESTS                   1

#define TEST_ONE_CMD "P0;"

// Main command buffer for testing
CmdBuffer* cmdBuf;

// Various scratch pad variables
int lenTestCmdArr = 0;
char * testCmdArr = new char[1024]();
int * tmpInts = new int[NUM_TEMP_INTS]();
char * testTempArr = new char[1024]();

// Stores test results
bool * testResults = new bool[NUM_TESTS]();


int main()
{
 
    cmdBuf = new CmdBuffer(NUM_BYTES_SERIAL_BT_BUFF);
 
    /*
     * TEST CASE 1: Basic Functionality
     */
    testResults[0] = true;
    strcpy(testCmdArr, TEST_ONE_CMD);
    lenTestCmdArr = sizeof(TEST_ONE_CMD);
    
    // Check that the newly created object has the correct initial state
    // The new buffer was created in the setUp()
    if(cmdBuf->getNumCmds() != 0 ||
        cmdBuf->getFree() != 1024 || 
        cmdBuf->getOccupied() != 0)
    {
        cout << "getNumCmds(), getFree(), or getOccupied() incorrectly initialized";
        testResults[0] = false;
    }
    
    // Add a command to the buffer
    tmpInts[0] = cmdBuf->write(testCmdArr, lenTestCmdArr);
    if(tmpInts[0] != lenTestCmdArr) 
    { 
        cout << "cmdBuf->write() returned " << tmpInts[0] << ", expected " << lenTestCmdArr; 
        testResults[0] = false;
    }
    
    tmpInts[0] = cmdBuf->getNumCmds();
    if(tmpInts[0] != 1)
    { 
        cout << "cmdBuf->getNumCmds() returned " << tmpInts[0] << ", expected " << 1;
        testResults[0] = false;
    }
    
    // Read the command from the buffer
    cmdBuf->readCmd(testTempArr);
    cout << "Read back: (" << testTempArr << ") after having written (" << ")" << endl;

}