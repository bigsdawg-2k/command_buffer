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
int testCmdArrLen = 0;
char * testCmdArr = new char[1024]();
int * tmpInts = new int[NUM_TEMP_INTS]();
char * testTempArr = new char[1024]();

// Stores test results
bool * testResults = new bool[NUM_TESTS]();

int main(void) 
{
 
    cmdBuf = new CmdBuffer(NUM_BYTES_SERIAL_BT_BUFF);
 
    /*
     * TEST CASE 1: Basic Functionality
     * Create a cmdBuffer
     * Add a command
     * Check the buffer state
     * Get the command
     * Check the buffer state
     */
    testResults[0] = true;
    strcpy(testCmdArr, TEST_ONE_CMD);
    testCmdArrLen = sizeof(TEST_ONE_CMD) - 1; // Don't count the null terminator
    
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
    tmpInts[0] = cmdBuf->write(testCmdArr, testCmdArrLen);
    if(tmpInts[0] != testCmdArrLen) 
    { 
        cout << "cmdBuf->write() returned " << tmpInts[0] << ", expected " << testCmdArrLen; 
        testResults[0] = false;
    }
    
    tmpInts[0] = cmdBuf->getNumCmds();
    if(tmpInts[0] != 1)
    { 
        cout << "cmdBuf->getNumCmds() returned " << tmpInts[0] << ", expected " << 1;
        testResults[0] = false;
    }
    
    // Read the command from the buffer
    tmpInts[0] = cmdBuf->readCmd(testTempArr);
    if(tmpInts[0] != testCmdArrLen ||
        strcmp(testTempArr, testCmdArr) != 0 ||
        cmdBuf->getNumCmds() != 0)
    {
        cout << "cmdBuf->readCmd() returned " << tmpInts[0] << " bytes, expected " << testCmdArrLen << endl;
        cout << "cmdBuf->readCmd() returned " << testTempArr << ", expected " << testCmdArr << endl;
        testResults[0] = false;
    }
    
    // Log test results
    for(int i = 0; i < NUM_TESTS; i++)
    {
        if(testResults[i] == false)
        {
            cout << "Test #" << i+1 << " failed" << endl;
        }
        else
        {
            cout << "Test #" << i+1 << " passed" << endl;
        }
    }

}